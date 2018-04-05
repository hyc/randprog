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

#include "Common.h"

#include "CGContext.h"
#include "CGOptions.h"
#include "ExpressionFuncall.h"
#include "Function.h"
#include "FunctionInvocation.h"

#include "random.h"

#include <cassert>

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
static bool
ExpressionFunctionProbability(const CGContext &/*cg_context*/)
{
	if (FuncListSize() > CGOptions::max_funcs()) {
		return false;
	}
	return rnd_flipcoin(80);
}

/*
 *
 */
ExpressionFuncall *
ExpressionFuncall::make_random(CGContext &cg_context)
{
	++cg_context.stmt_depth;

	FunctionInvocation *fi =
		FunctionInvocation::make_random(
			ExpressionFunctionProbability(cg_context),
			cg_context);
	ExpressionFuncall *e = new ExpressionFuncall(*fi);

	--cg_context.stmt_depth;

	return e;
}

/*
 *
 */
ExpressionFuncall::ExpressionFuncall(const FunctionInvocation &fi)
	: invoke(fi)
{
	// Nothing else to do.
}

/*
 *
 */
ExpressionFuncall::~ExpressionFuncall(void)
{
	// Nothing to do.
}

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
const Type &
ExpressionFuncall::get_type(void) const
{
	return invoke.get_type();
}

/*
 *
 */
void
ExpressionFuncall::Output(std::ostream &out) const
{
	invoke.Output(out);
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
