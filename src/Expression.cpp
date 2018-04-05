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

#include "CGContext.h"
#include "CGOptions.h"
#include "Effect.h"
#include "Expression.h"
#include "Function.h"

#include "Constant.h" // temporary; don't want to depend on subclass!
#include "ExpressionFuncall.h" // temporary; don't want to depend on subclass!
#include "ExpressionVariable.h" // temporary; don't want to depend on subclass!

#include "random.h"

#include <cassert>

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
static eTermType
ExpressionTypeProbability(const CGContext &cg_context)
{
	if (cg_context.stmt_depth > CGOptions::max_stmt_depth())
		return eVariable;

	int i = rnd_upto(100);
#if 0
	if (i < 30)
		return eBinaryExpr;
#endif
	if (i < 65)
		return eFunction;
	if (i < 70)
		return eConstant;
	return eVariable;
}

/*
 *
 */
Expression *
Expression::make_random(CGContext &cg_context)
{
	Expression *e = 0;
	eTermType tt = ExpressionTypeProbability(cg_context);
	
	switch (tt) {
	case eConstant:
		e = Constant::make_random(cg_context);
		break;
	case eVariable:
		e = ExpressionVariable::make_random(cg_context);
		break;
#if 0
	case eBinaryExpr:
		e = ExpressionBinary::make_random(cg_context);
		break;
#endif // 0
	case eFunction:
		e = ExpressionFuncall::make_random(cg_context);
		break;
	}
#if 0
	if (!cg_context.get_effect_context().is_side_effect_free()) {
		assert(e->effect.is_side_effect_free());
	}
#endif
	return e;
}

/*
 *
 */
Expression::Expression(void)
{
	// Nothing to do.
}

/*
 *
 */
Expression::~Expression(void)
{
	// Nothing to do.
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
