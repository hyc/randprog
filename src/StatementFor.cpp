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

#include "Common.h"

#include "Block.h"
#include "CGContext.h"
#include "Constant.h"
#include "ExpressionFuncall.h"
#include "ExpressionVariable.h"
#include "Function.h"
// `FunctionInvocation' for `eBinaryOps'.  Yuck.
#include "FunctionInvocation.h"
#include "StatementFor.h"
#include "Variable.h"

#include "random.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 * Randomly determine the parameters for a counting loop: initial value, limit
 * value, increment value, test operator, and increment operator.
 */
static void
make_random_loop_control(int &init, int &limit, int &incr,
						 eBinaryOps &test_op,
						 eAssignOps &incr_op)
{
	init  = rnd_flipcoin(50) ? 0 : (rnd_upto(60)-30);
	limit = rnd_flipcoin(50) ? 0 : (rnd_upto(60)-30);
	int pos;

	/* make sure limit is different from init */
	while (limit == init)
		limit = rnd_upto(60)-30;

	/* make sure loop will terminate */
	if (limit > init) {
		eBinaryOps t_ops[] = { eCmpLt, eCmpLe, eCmpNe };
		test_op = t_ops[rnd_upto(sizeof(t_ops)/sizeof(*t_ops))];
		pos = 1;
	} else {
		eBinaryOps t_ops[] = { eCmpGt, eCmpGe, eCmpNe };
		test_op = t_ops[rnd_upto(sizeof(t_ops)/sizeof(*t_ops))];
		pos = 0;
	}

	if (rnd_flipcoin(50)) {
		// Do `+=' or `-=' by an increment between 1 and 9 inclusive.
		incr_op = pos ? eAddAssign : eSubAssign;
		incr = rnd_upto(9)+1;
		// Don't use != if we could incr past it.
		if (test_op == eCmpNe)
			test_op = pos ? eCmpLe : eCmpGe;
	} else {
		// Do `++' or `--', pre- or post-.
		eAssignOps i_ops[] = { ePreDecr, ePostDecr, ePreIncr, ePostIncr };
		incr_op = i_ops[rnd_upto(2)+pos*2];
		incr = 1;
	}
}

/*
 *
 */
StatementFor *
StatementFor::make_random(CGContext &cg_context)
{
	// Generate a somewhat-trivial `for' loop.

	Function *curr_func = cg_context.get_current_func();
	assert(curr_func);

	// Select the loop control variable.
	Variable *var;
	do { var = SelectLValue(curr_func, cg_context.get_effect_context()); }
	while (var->isLooper || var->is_global());

	cg_context.write_var(var);
	cg_context.read_var(var);
	var->isLooper = true;

	// Select the loop parameters: init, limit, increment, etc.
	int        init_n, limit_n, incr_n;
	eBinaryOps test_op;
	eAssignOps incr_op;
	
	make_random_loop_control(init_n, limit_n, incr_n, test_op, incr_op);

	// Build the IR for the subparts of the loop.

	unsigned int flags_save = cg_context.flags;
	
	StatementAssign *init =
		new StatementAssign(*var, *Constant::make_int(init_n));
	cg_context.flags |= IN_LOOP;
	Expression *test =
		new ExpressionFuncall(
			*FunctionInvocation::make_binary(test_op,
											 new ExpressionVariable(*var),
											 Constant::make_int(limit_n)));
	StatementAssign *incr =
			new StatementAssign(*var, incr_op, *Constant::make_int(incr_n));
	// TODO: jigger with the effect_context so that we don't write to the loop
	// control variable?
	Block *body =
			Block::make_random(cg_context);
	cg_context.flags = flags_save;
	var->isLooper = false;

	return new StatementFor(*init, *test, *incr, *body);
}

/*
 *
 */
StatementFor::StatementFor(const StatementAssign &init,
						   const Expression &test,
						   const StatementAssign &incr,
						   const Block &body)
	: Statement(eFor),
	  init(init),
	  test(test),
	  incr(incr),
	  body(body)
{
	// Nothing else to do.
}

/*
 *
 */
StatementFor::StatementFor(const StatementFor &sf)
	: Statement(sf.get_type()),
	  init(sf.init),
	  test(sf.test),
	  incr(sf.incr),
	  body(sf.body)
{
	// Nothing else to do.
}

/*
 *
 */
StatementFor::~StatementFor(void)
{
	// Nothing to do.
}

/*
 *
 */
void
StatementFor::Output(std::ostream &out) const
{
	out << "for (";
	init.OutputAsExpr(out);
	out << "; ";
	test.Output(out);
	out << "; ";
	incr.OutputAsExpr(out);
	out << ")" << endl;
	body.Output(out);
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
