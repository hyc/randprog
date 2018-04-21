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

#ifndef STATEMENT_H
#define STATEMENT_H

///////////////////////////////////////////////////////////////////////////////

#include "Pool.h"

class CGContext;

enum eStatementType
{
	eAssign,
	eBlock,
	eFor,		// Make this a generic loop construct (while/for/do)
	eIfElse,
	eInvoke,
	eReturn

// TODO: Re-enable continue/break when LLVM bug is fixed..
//	eBreak,		// Only inside of loops
//	eContinue,	// Only inside of loops

//  ..more?  try, catch, throw, goto

//	eHash,
};
#define MAX_STATEMENT_TYPE ((eStatementType) (eReturn+1))

/*
 *
 */
class Statement : public pool_base
{
public:
	// Factory method.
	static Statement *make_random(CGContext &cg_context,
								  eStatementType t = MAX_STATEMENT_TYPE);

	virtual ~Statement(void);

	eStatementType get_type(void) const { return eType; }

	virtual void Output(std::ostream &out) const = 0;

protected:
	const eStatementType eType;

protected:
	Statement(eStatementType st);

private:
	Statement &operator=(const Statement &s); // unimplementable
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
