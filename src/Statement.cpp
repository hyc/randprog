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
#include "CGOptions.h"
#include "Function.h"
#include "Statement.h"

#include "Block.h" // temporary; don't want to depend on subclases!
#include "StatementAssign.h" // temporary; don't want to depend on subclases!
#include "StatementExpr.h" // temporary; don't want to depend on subclases!
#include "StatementFor.h" // temporary; don't want to depend on subclases!
#include "StatementIf.h" // temporary; don't want to depend on subclases!
#include "StatementReturn.h" // temporary; don't want to depend on subclases!

#include "random.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
//
// TODO: Build probability tables for each decision point
//       IE: Return statements less common than assignment statements, etc.
//       Allow MIN, MAX and pluggable density functions (uniform, normal, guassian, ...)
//
// Like this:
/*
unsigned int probabilityStatement[eStatementType] =
{
	100, 	//	eAssign,
	75, 	//	eIfElse,
	30,		//	eFor,		// Make this a generic loop construct (while/for/do)
	30,		//	eInvoke,
	10,		//	eReturn,
}

probability probStatement
{
	2, // Min
	0, // Max
	probabilityStatement // Pluggable Function (custom probability table, in this case)
}
*/

static eStatementType
StatementProbability(const CGContext &cg_context)
{
	eStatementType t;

	while (1) {
		t = (eStatementType) rnd_upto(MAX_STATEMENT_TYPE);

//		if ( t == eHash ) continue;

		if (t == eBlock) {
			continue;
		}
		if ((t == eReturn) && (cg_context.flags & NO_RETURN)) {
			continue;
		}
// TODO: Re-enable continue/break when LLVM bug is fixed..
//		if ( (t == eBreak || t == eContinue )
//              && !(cg_context.flags & IN_LOOP) )
//			continue;

		// Limit Function complexity (depth of nested control structures and
		// expressions)
		if (cg_context.stmt_depth > CGOptions::max_stmt_depth()) {
			if (t == eAssign || t == eReturn) {
				return t;
			}
		} else if (FuncListSize() > CGOptions::max_funcs()) { // Limit # of functions..
			if (t != eInvoke) {
				return t;
			}
		} else {
            return t;
		}
	}
}

/*
 *
 */
Statement *
Statement::make_random(CGContext &cg_context,
					   eStatementType t)
{
	// Add more statements:
	// for
	// while
	// library call (malloc, free, str*, mem*, etc..)?
	// switch?
	// ..?
	if (t == MAX_STATEMENT_TYPE) {
		t = StatementProbability(cg_context);
	}

	// XXX: interim ickiness
	Statement *s = 0;
	switch (t) {
	default:
		assert(!"unknown Statement type");
		break;
	case eAssign:
		s = StatementAssign::make_random(cg_context);
		break;
	case eBlock:
		s = Block::make_random(cg_context);
		break;
	case eFor:
		s = StatementFor::make_random(cg_context);
		break;
	case eIfElse:
		s = StatementIf::make_random(cg_context);
		break;
	case eInvoke:
		s = StatementExpr::make_random(cg_context);
		break;
	case eReturn:
		s = StatementReturn::make_random(cg_context);
		break;
	}
	return s;
}

/*
 *
 */
Statement::Statement(eStatementType st)
	: eType(st)
{
	// Nothing else to do.
}

/*
 *
 */
Statement::~Statement(void)
{
	// Nothing to do.
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
