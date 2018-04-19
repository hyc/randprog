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

#include <cassert>

#include "StatementAssign.h"

#include "CGContext.h"
#include "Expression.h"
#include "Variable.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
StatementAssign *
StatementAssign::make_random(CGContext &cg_context)
{
	Function *curr_func = cg_context.get_current_func();
	assert(curr_func);

	Variable *v;
	do { v = SelectLValue(*curr_func,
							   cg_context.get_effect_context());
	} while (v->isLooper);
	Expression *e = Expression::make_random(cg_context);

	cg_context.write_var(v);

	return new StatementAssign(*v, *e);
}

/*
 *
 */
StatementAssign::StatementAssign(const Variable &v,
								 const Expression &e)
	: Statement(eAssign),
	  op(eSimpleAssign),
	  var(v),
	  expr(e)
{
	// Nothing else to do.
}

/*
 *
 */
StatementAssign::StatementAssign(const Variable &v,
								 eAssignOps op,
								 const Expression &e)
	: Statement(eAssign),
	  op(op),
	  var(v),
	  expr(e)
{
	// XXX --- Sanity check: if op is ++/--, check that expr is 1.
	// Nothing else to do.
}

/*
 *
 */
StatementAssign::StatementAssign(const StatementAssign &sa)
	: Statement(sa.get_type()),
	  op(sa.op),
	  var(sa.var),
	  expr(sa.expr)
{
	// Nothing else to do.
}

/*
 *
 */
StatementAssign::~StatementAssign(void)
{
	// Nothing to do.
}

/*
 *
 */
static void
OutputAssignOp(eAssignOps op, std::ostream &out)
{
	switch (op) {
	case eSimpleAssign: out << "="; break;
	case eMulAssign:	out << "*="; break;
	case eDivAssign:	out << "/="; break;
	case eRemAssign:	out << "%="; break;
	case eAddAssign:	out << "+="; break;
	case eSubAssign:	out << "-="; break;
	case eLShiftAssign:	out << "<<="; break;
	case eRShiftAssign:	out << ">>="; break;
	case eBitAndAssign:	out << "&="; break;
	case eBitXorAssign:	out << "^="; break;
	case eBitOrAssign:	out << "|="; break;

	case ePreIncr:		out << "++"; break;
	case ePreDecr:		out << "--"; break;
	case ePostIncr:		out << "++"; break;
	case ePostDecr:		out << "--"; break;
	}
}

/*
 *
 */
void
StatementAssign::Output(std::ostream &out) const
{
	OutputAsExpr(out);
	out << ";" << endl;
}

/*
 *
 */
void
StatementAssign::OutputAsExpr(std::ostream &out) const
{
	switch (op) {
	default:
		var.OutputAsLHS(out);
		out << " ";
		OutputAssignOp(op, out);
		out << " ";
		expr.Output(out);
		break;
		
	case ePreIncr:
	case ePreDecr:
		OutputAssignOp(op, out);
		var.OutputAsLHS(out);
		break;

	case ePostIncr:
	case ePostDecr:
		var.OutputAsLHS(out);
		OutputAssignOp(op, out);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
