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

#include "Common.h"

#include "Block.h"
#include "CGContext.h"
#include "CGOptions.h"
#include "Constant.h"
#include "Effect.h"
#include "Function.h"
#include "Statement.h"
#include "Type.h"
#include "Variable.h"

#include "random.h"
#include "util.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

static vector<Function*> FuncList;		// List of all functions in the program
static long cur_func_idx;				// Index into FuncList that we are currently working on
static bool param_first=true;					// Flag to track output of commas

/*
 *
 */
// unsigned
long
FuncListSize(void)
{
	return FuncList.size();
}

/*
 *
 */
Function *
GetFirstFunction(void)
{
	return FuncList[0];
}

/*
 *
 */
static string
RandomFunctionName(void)
{
	return gensym("func_");
}

/*
 *
 */
static Type
RandomReturnType(void)
{
	// TODO: Select a random type that is valid as a return type.
	Type t(eInt);
	return t;
}

/*
 * Choose a function from `funcs' to invoke.
 * Return null if no suitable function can be found.
 */
static Function *
choose_func(vector<Function *> funcs,
			const Effect &effect_context)
{
	vector<Function *> ok_funcs;
	vector<Function *>::iterator i;

	for (i = funcs.begin(); i != funcs.end(); ++i) {
		// We cannot call a function that has an as-yet unknown effect.
		// TODO: in practice, this means we never call an as-yet unbuilt func.
		if ((*i)->is_effect_known() == false) {
			continue;
		}
		// We cannot call a function with a side-effect if the current context
		// already has a side-effect.
		// TODO: is this too strong for what we want?
		if (!((*i)->get_effect().is_side_effect_free())
			&& !effect_context.is_side_effect_free()) {
			continue;
		}
		// We cannot call a function that has a race with the current context.
		if ((*i)->get_effect().has_race_with(effect_context)) {
			continue;
		}
		// Otherwise, this is an acceptable choice.
		ok_funcs.push_back(*i);
	}
	
	vector<Function *>::size_type ok_size = ok_funcs.size();
	
	if (ok_size == 0) {
		return 0;
	}
	if (ok_size == 1) {
		return ok_funcs[0];
	}
	return ok_funcs[rnd_upto(ok_size)];
}

/*
 * WARNING: this function may return null!
 */
Function *
SelectFunction(Function &curFunc, bool &isBackLink,
			   const Effect &effect_context)
{
	// Select between creating a new function, or calling an old function.
	if ((FuncListSize() < CGOptions::max_funcs())
	    && (FuncList.empty() || rnd_flipcoin(50))) {
		isBackLink = false;
		return Function::make_random(curFunc, effect_context);
		
	} else {
		// Try to link to a previously defined function.
		// (This may cause cycles in the call graph!)
		Function *callee = choose_func(FuncList, effect_context);
		if (!callee) {
			// TODO: Maybe should generate a new function?
			return 0;
		}
		// unsigned long idx = rnd_upto(FuncList.size());
		// isBackLink = (idx <= cur_func_idx);
		isBackLink = true; // XXX
		return callee;
	}
}

/*
 *
 */
static bool
ParamListProbability(vector<Variable*> &List)
{
	if (List.empty()) {
		return true;
	}
	return rnd_flipcoin(50);		// 50% probability
}

/*
 *
 */
static void
GenerateParameterList(Function &curFunc, Function &parent)
{
	while (ParamListProbability(curFunc.param)) {
		// With some probability, choose a new random variable, or one from
		// parentParams, parentLocals, or the globals list.
		//
		// Also, build the parent's link structure for this invocation and push
		// it onto the back.
		//
		GenerateParameterVariable(curFunc, parent);
	}
}

/*
 *
 */
Function::Function(const string &name, const Type &return_type)
	: name(name),
	  return_type(return_type),
	  body(0),
	  build_state(UNBUILT)
{
	FuncList.push_back(this);			// Add to global list of functions.
}

/*
 *
 */
Function *
Function::make_random(Function &parent, const Effect& effect_context)
{
	Function *f = new Function(RandomFunctionName(), RandomReturnType());

	// Select a set of vars to operate on.
	GenerateParameterList(*f, parent);
	f->GenerateBody(effect_context);

	return f;
}

/*
 *
 */
Function *
Function::make_first(void)
{
	Function *f = new Function(RandomFunctionName(), RandomReturnType());

	// No Parameter List
	f->GenerateBody(Effect::get_empty_effect());

	return f;
}

/*
 *
 */
static void
OutputFormalParam(Variable *var, std::ostream *pOut)
{
	std::ostream &out = *pOut;
	if ( !param_first ) out << ", ";
	param_first = false;
	out << " " << var->name;
}

/*
 *
 */
void
Function::OutputFormalParamList(std::ostream &out)
{
	if (param.size() > 0) {
		param_first = true;
		for_each(param.begin(),
				 param.end(),
				 std::bind2nd(std::ptr_fun(OutputFormalParam), &out));
	}
}

/*
 *
 */
void
Function::OutputHeader(std::ostream &out)
{
	out << "function " << name << "(";
	OutputFormalParamList( out );
	out << ")";
}

/*
 *
 */
void
Function::OutputForwardDecl(std::ostream &out)
{
	OutputHeader(out);
	out << ";" << endl;
}

/*
 *
 */
void
Function::Output(std::ostream &out)
{
	out << "/* ------------------------------------------ */" << endl;
	effect.Output(out);
	OutputHeader(out);
	out << endl;
	out << "{" << endl;

	if (CGOptions::depth_protect()) {
		out << "if (DEPTH >= MAX_DEPTH) " << endl;
		// TODO: Needs to be fixed when return types are no longer simple
		// types.

		// XXX ick: gin up a `CGContext' just so we can make a number!
		Effect ec, ea;
		CGContext tmp(this, 0, 0, ec, &ea);
		Constant *c = Constant::make_random(tmp, return_type.simple_type);

		out << "    return ";
		c->Output(out);
		delete c;
		out << ";" << endl;
		out << "DEPTH++;" << endl;
	}

	body->Output(out);

	if (CGOptions::depth_protect()) {
		out << "DEPTH--;" << endl;
	}

	out << "}" << endl;
	out << endl << endl;
}

/*
 *
 */
void
Function::GenerateBody(const Effect &effect_context)
{
	if (build_state != UNBUILT) {
		cerr << "warning: ignoring attempt to regenerate func" << endl;
		return;
	}

	build_state = BUILDING;
	
	Effect effect_accum;
	CGContext cg_context(this, 0, 0, effect_context, &effect_accum);

	// Fill in the Function body.
	body = Block::make_random(cg_context);
	body->set_depth_protect(true);
	
	// TODO: Re-enable this when the Statement class is separated into
	// appropriate sub-classes.
	// Make sure there's an appropriate return statement (if required)..
	if (return_type.eType != eSimple
		|| return_type.simple_type != eVoid) {
		stack.push_back(body);
		body->stm.push_back(Statement::make_random(cg_context, eReturn));
		stack.pop_back();
	}
	
	// Compute the function's externally visible effect.  Currently, this
	// is just the effect on globals.
	effect.add_external_effect(*cg_context.get_effect_accum());
	
	// Mark this function as built.
	build_state = BUILT;
}

/*
 *
 */
void
GenerateFunctions(void)
{
	// -----------------
	// Create a basic first function, then generate a random graph from there.
	/* Function *first = */ Function::make_first();
	
	// -----------------
	// Create body of each function, continue until no new functions are created.
	for (cur_func_idx = 0; cur_func_idx < FuncListSize(); cur_func_idx++) {
		// Dynamically adds new functions to the end of the list..
		if (FuncList[cur_func_idx]->is_built() == false) {
			FuncList[cur_func_idx]->GenerateBody(Effect::get_empty_effect());
		}
	}
}

/*
 *
 */
static void
OutputForwardDecl(Function *func, std::ostream *pOut)
{
	func->OutputForwardDecl(*pOut);
}

/*
 *
 */
static void
OutputFunction(Function *func, std::ostream *pOut)
{
	func->Output(*pOut);
}

/*
 *
 */
void
OutputForwardDeclarations(std::ostream &out)
{
	out << endl << endl << "/* --- FORWARD DECLARATIONS --- */" << endl;
	for_each(FuncList.begin(), FuncList.end(),
			 std::bind2nd(std::ptr_fun(OutputForwardDecl), &out));
}

/*
 *
 */
void
OutputFunctions(std::ostream &out)
{
	out << endl << endl << "/* --- FUNCTIONS --- */" << endl;
	for_each(FuncList.begin(), FuncList.end(),
			 std::bind2nd(std::ptr_fun(OutputFunction), &out));
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
