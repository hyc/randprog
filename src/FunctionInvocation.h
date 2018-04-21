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

#ifndef FUNCTION_EXPRESSION_H
#define FUNCTION_EXPRESSION_H

///////////////////////////////////////////////////////////////////////////////

#include "Pool.h"

class CGContext;
class Function;
class Expression;
class Type;

#include "Effect.h"

#if 0 // to be implemented
enum eUnaryOps
{
	ePreInc,
	ePreDec,
	ePostInc,
	ePostDec
};
#define MAX_UNARY_OP ((eUnaryOps) (ePostDec+1))
#endif // 0

enum eBinaryOps
{
	eAdd,
	eSub,
	eMul,
	eDiv,
	eMod,
	eCmpGt,
	eCmpLt,
	eCmpGe,
	eCmpLe,
	eCmpEq,
	eCmpNe,
	eAnd,
	eOr,
	eBitXor,
	eBitAnd,
	eBitOr,
	eRShift,
	eLShift
};
#define MAX_BINARY_OP ((eBinaryOps) (eLShift+1))

class FunctionInvocation : public pool_base
{
public:
	static FunctionInvocation *make_random(bool,
										   CGContext &cg_context);
	static FunctionInvocation *make_random(Function *target,
										   CGContext &cg_context);

	static FunctionInvocation *make_binary(eBinaryOps op,
										   Expression *lhs,
										   Expression *rhs);

	//

	const Type &get_type(void) const;

	void Output(std::ostream &) const;

private:	
	eBinaryOps eFunc;
	Function *func;
	pool_vector<Expression*> param_value;
	bool isBackLink;
	bool isStandardFunc;

private:
	FunctionInvocation(eBinaryOps op);
	FunctionInvocation(Function *target, bool isBackLink);

	void build_invocation(Function *target,
						  CGContext &cg_context);
};

///////////////////////////////////////////////////////////////////////////////

#endif // FUNCTION_EXPRESSION_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
