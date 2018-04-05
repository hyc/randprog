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

#ifndef STATEMENT_ASSIGN_H
#define STATEMENT_ASSIGN_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "Statement.h"

class CGContext;
class Expression;
class Variable;

/*
 * "Assignment operators."  This is sort of a dumb model, but it will do for
 * now.
 */
enum eAssignOps
{
	eSimpleAssign,
	eMulAssign,
	eDivAssign,
	eRemAssign,
	eAddAssign,
	eSubAssign,
	eLShiftAssign,
	eRShiftAssign,
	eBitAndAssign,
	eBitXorAssign,
	eBitOrAssign,
	//
	ePreIncr,
	ePreDecr,
	ePostIncr,
	ePostDecr
};
#define MAX_ASSIGN_OP ((eAssignOps) (ePostDecr+1))

/*
 *
 */
class StatementAssign : public Statement
{
public:
	// Factory method.
	static StatementAssign *make_random(CGContext &cg_context);

	StatementAssign(const Variable &v, const Expression &e);
	StatementAssign(const Variable &v, eAssignOps op, const Expression &e);
	StatementAssign(const StatementAssign &sa);
	virtual ~StatementAssign(void);

	//

	virtual void Output(std::ostream &out) const;

	// XXX --- This should go away, once assignments are properly modeled as
	// expressions.
	virtual void OutputAsExpr(std::ostream &out) const;
	
private:
	const eAssignOps op;
	const Variable   &var;
	const Expression &expr;

	//

	StatementAssign &operator=(const StatementAssign &se); // unimplementable
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_ASSIGN_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
