// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008 The University of Utah
// All rights reserved.
//
// This file is part of `randprog', a random generator of C programs.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

//
// This file was derived from a random program generator written by Bryan
// Turner.  The attributions in that file was:
//
// Random Program Generator
// Bryan Turner (bryan.turner@pobox.com)
// July, 2005
//

#include <cassert>

#include "Common.h"

#include "CGContext.h"
#include "Effect.h"
#include "Expression.h"
#include "Function.h"
#include "FunctionInvocation.h"
#include "Type.h"

#include "random.h"

using namespace std;

static vector<bool> needcomma;  // Flag to track output of commas

static bool IsOrderedStandardFunc(eBinaryOps);

///////////////////////////////////////////////////////////////////////////////

/*
 * XXX: replace with a useful constructor.
 */
FunctionInvocation::FunctionInvocation(eBinaryOps op)
	: eFunc(op),
	  func(0),
	  isBackLink(false),
	  isStandardFunc(true)
{
	// Nothing else to do.  Caller must build useful params.
}

/*
 * XXX: replace with a useful constructor.
 */
FunctionInvocation::FunctionInvocation(Function *target, bool isBackLink)
	: eFunc(eAdd), // unused
	  func(target),
	  isBackLink(isBackLink),
	  isStandardFunc(false)
{
	// Nothing else to do.  Caller must build useful params.
}

/*
 *
 */
FunctionInvocation *
FunctionInvocation::make_random(bool bStandardFunc,
								CGContext &cg_context)
{
	Function *curr_func = cg_context.get_current_func();
	assert(curr_func);
	
	FunctionInvocation *fi = 0;
	bool is_std_func = bStandardFunc;
	bool is_back_link = false;
	
	// If we are looking for a program-defined function, try to find one.
	// If we cannot find one, fall back to calling a "standard" function.
	Function *callee = 0;
	if (!is_std_func) {
		callee = SelectFunction(*curr_func,
								is_back_link,
								cg_context.get_effect_context());
		if (!callee) {
			is_std_func = true;
		}
	}
	
	if (is_std_func) {
		eBinaryOps op = (eBinaryOps)(rnd_upto(MAX_BINARY_OP));
		fi = new FunctionInvocation(op);
		
		Effect lhs_eff_accum;
		CGContext lhs_cg_context(cg_context.get_current_func(),
								 cg_context.stmt_depth,
								 cg_context.flags,
								 cg_context.get_effect_context(),
								 &lhs_eff_accum);
		
		// TODO: Generate an expression with the correct type...
		Expression *lhs = Expression::make_random(lhs_cg_context);
		Expression *rhs = 0;

		cg_context.add_effect(lhs_eff_accum);

		// If we are guaranteed that the LHS will be evaluated before the RHS,
		// or if the LHS is pure (not merely side-effect-free),
		// then we can generate the RHS under the original effect context.
		if (IsOrderedStandardFunc(op) || lhs_eff_accum.is_pure()) {
			rhs = Expression::make_random(cg_context);
		} else {
			// Otherwise, the RHS must be generated under the combined effect
			// of the original effect and the LHS effect.
			Effect rhs_eff_context(cg_context.get_effect_context());
			rhs_eff_context.add_effect(lhs_eff_accum);

			CGContext rhs_cg_context(cg_context.get_current_func(),
									 cg_context.stmt_depth,
									 cg_context.flags,
									 rhs_eff_context,
									 cg_context.get_effect_accum());
			rhs = Expression::make_random(rhs_cg_context);
		}
		
		// TODO: fix `rhs' for eLShift and eRShift
		// Current, the "fix" is handled in `FunctionInvocation::Output'.
		fi->param_value.push_back(lhs);
		fi->param_value.push_back(rhs);
		
		// TODO: we don't need to do this; we already accumulated the effect.
#if 0
		fi->effect = lhs->get_effect();
		fi->effect.add_effect(rhs->get_effect());
#endif
	} else {
		fi = new FunctionInvocation(callee, is_back_link);
		fi->build_invocation(callee, cg_context);
	}
	
	assert(fi != 0);
	return fi;
}

/*
 * TODO: FIX!  This is a bogus constructor, used only by the `OutputMain'.
 * The problem is, there is no representation for the calling function,
 * so we "just use" `*target'.  Bah!
 */
FunctionInvocation *
FunctionInvocation::make_random(Function *target,
								CGContext &cg_context)
{
	FunctionInvocation *fi = new FunctionInvocation(target, true);
	fi->build_invocation(target, cg_context);
	return fi;
}

/*
 * Build an "invocation" of a binary operation.
 */
FunctionInvocation *
FunctionInvocation::make_binary(eBinaryOps op,
								Expression *lhs, Expression *rhs)
{
	FunctionInvocation *fi = new FunctionInvocation(op);
	fi->param_value.push_back(lhs);
	fi->param_value.push_back(rhs);

	return fi;
}

/*
 * Internal helper function.
 */
void
FunctionInvocation::build_invocation(Function *target,
									 CGContext &cg_context)
{
	isStandardFunc = false;	// XXX: unnecessary; done by constructor
	func = target;			// XXX: unnecessary; done by constructor
	
	Effect running_eff_context(cg_context.get_effect_context());
	
	for (unsigned int i = 0; i < func->param.size(); i++) {
		// TODO: Generate an expression with the correct type...
		Effect param_eff_accum;
		CGContext param_cg_context(cg_context.get_current_func(),
								   cg_context.stmt_depth,
								   cg_context.flags,
								   running_eff_context,
								   &param_eff_accum);
		Expression *p = Expression::make_random(param_cg_context);
		param_value.push_back(p);
		// Update the "running effect context": the context that we must use
		// when we generate subsequent parameters within this invocation.
		running_eff_context.add_effect(param_eff_accum);
		// Update the total effect of this invocation, too.
		cg_context.add_effect(param_eff_accum);
	}
	cg_context.add_external_effect(func->get_effect());
}

///////////////////////////////////////////////////////////////////////////////

/*
 * XXX --- we should memoize the types of "standard functions."
 */
const Type &
FunctionInvocation::get_type(void) const
{
	if (isStandardFunc) {
		switch (eFunc) {
		case eAdd:
		case eSub:
		case eMul:
		case eDiv:
		case eMod:
		case eBitXor:
		case eBitAnd:
		case eBitOr:
			{
				const Type &l_type = param_value[0]->get_type();
				const Type &r_type = param_value[1]->get_type();
				// XXX --- not really right!
				if (!(l_type.is_signed()) && !(r_type.is_signed())) {
					return Type::get_simple_type(eInt);
				} else {
					return Type::get_simple_type(eUInt);
				}
			}
			break;

		case eCmpGt:
		case eCmpLt:
		case eCmpGe:
		case eCmpLe:
		case eCmpEq:
		case eCmpNe:
		case eAnd:
		case eOr:
			return Type::get_simple_type(eInt);
			break;

		case eRShift:
		case eLShift:
			{
				const Type &l_type = param_value[0]->get_type();
				// XXX --- not really right!
				if (!l_type.is_signed()) {
					return Type::get_simple_type(eInt);
				} else {
					return Type::get_simple_type(eUInt);
				}
			}
			break;
		}
	}

	return func->return_type;
}

///////////////////////////////////////////////////////////////////////////////

/*
 * Return true if `eFunc' defines and order for the evaluation of its
 * arguments.
 */
static bool
IsOrderedStandardFunc(eBinaryOps eFunc)
{
	return ((eFunc == eAnd) || (eFunc == eOr));
}

/*
 *
 */
static void
OutputStandardFuncName(eBinaryOps eFunc, std::ostream &out)
{
	switch (eFunc) {
		// Math Ops
	case eAdd:		out << "+";		break;
	case eSub:		out << "-";		break;
	case eMul:		out << "*";		break;
	case eDiv:		out << "/";		break;
	case eMod:		out << "%";		break;
		
		// Logical Ops
	case eAnd:		out << "&&";	break;
	case eOr:		out << "||";	break;
	case eCmpEq:	out << "==";	break;
	case eCmpNe:	out << "!=";	break;
	case eCmpGt:	out << ">";		break;
	case eCmpLt:	out << "<";		break;
	case eCmpLe:	out << "<=";	break;
	case eCmpGe:	out << ">=";	break;
		
		// Bitwise Ops
	case eBitAnd:	out << "&";		break;
	case eBitOr:	out << "|";		break;
	case eBitXor:	out << "^";		break;
	case eLShift:	out << "<<";	break;
	case eRShift:	out << ">>";	break;
	}
}

/*
 *
 */
static void
OutputActualParamExpression(Expression *expr, std::ostream *pOut)
{
	std::ostream &out = *pOut;
	if (needcomma.back()) {
		out << ", ";
	}
	needcomma.back() = true;
	expr->Output(out);
}

/*
 *
 */
static void
OutputExpressionVector(const vector<Expression*> &var, std::ostream &out)
{
	needcomma.push_back(false);
	for_each(var.begin(), var.end(),
			 std::bind2nd(std::ptr_fun(OutputActualParamExpression), &out));
	needcomma.pop_back();
}

/*
 *
 */
void
FunctionInvocation::Output(std::ostream &out) const
{
	if (isStandardFunc) {
		// One of the Binary Ops
		out << "(";
		switch (eFunc) {
		case eLShift:
		case eRShift:
			// We handle shifts specially in order to avoid undefined behavior.
			if (eFunc == eLShift) {
				out << "lshift_";
			} else {
				out << "rshift_";
			}
			if (param_value[0]->get_type().is_signed()) {
				out << "s_";
			} else {
				out << "u_";
			}
			if (param_value[1]->get_type().is_signed()) {
				out << "s";
			} else {
				out << "u";
			}
			out << "(";
			param_value[0]->Output(out);
			out << ", ";
			param_value[1]->Output(out);
			out << ")";
			break;
			
		default:
			param_value[0]->Output(out);
			out << " ";
			OutputStandardFuncName(eFunc, out);
			out << " ";
			// XXX --- more code to restrict range of rhs for certain ops
			bool need_range_restriction = false;
			switch (eFunc) {
			default:
				break;
			case eMod:
				out << "mod_rhs(";
				need_range_restriction = true;
				break;
			case eDiv:
				out << "div_rhs(";
				need_range_restriction = true;
				break;
			}
			param_value[1]->Output(out);
			if (need_range_restriction) {
				out << ")";
			}
			break;
		}
	} else {
		out << func->name << "(";
		OutputExpressionVector(param_value, out);
	}
	out << ")";
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
