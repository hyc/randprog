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

#include "Block.h"
#include "CGContext.h"
#include "CGOptions.h"
#include "Function.h"
#include "Statement.h"
#include "Type.h"
#include "Variable.h"

#include "random.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
static bool
BlockProbability(Block &block)
{
	// Always have at least one statement in the block.
	if (block.stm.empty()) {
		return true;
	}
	if (FuncListSize() < 10) {
		return true;
	}
	// 50% probability for each additional statement.  Should upgrade this to
	// generate a uniform distribution around a selected block size (or
	// function size?)
	return rnd_flipcoin(50);
}

/*
 *
 */
Block *
Block::make_random(CGContext &cg_context)
{
	Function *curr_func = cg_context.get_current_func();
	assert(curr_func);
	
	Block *b = new Block();
	
	++cg_context.stmt_depth;
	// Push this block onto the variable scope stack.
	curr_func->stack.push_back(b);
	
	while (BlockProbability(*b)) {
		Statement *s = Statement::make_random(cg_context);
		b->stm.push_back(s);
		if (s->get_type() == eReturn)
			break;
	}

	curr_func->stack.pop_back();
	--cg_context.stmt_depth;

	return b;
}

/*
 *
 */
Block::Block(void)
	: Statement(eBlock),
	  depth_protect(false)
{
	// Nothing else to do.
}

/*
 *
 */
Block::Block(const Block &b)
	: Statement(eBlock),
	  stm(b.stm),
	  local_vars(b.local_vars),
	  depth_protect(b.depth_protect)
{
	// Nothing else to do.
}

/*
 *
 */
Block::~Block(void)
{
	// Nothing to do.
}

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
static void
OutputStatement(Statement *stm, std::ostream *pOut)
{
	stm->Output(*pOut);
}

/*
 *
 */
static void
OutputStatementList(const vector<Statement*> &stm, std::ostream &out)
{
	for_each(stm.begin(), stm.end(),
			 std::bind2nd(std::ptr_fun(OutputStatement), &out));
}

/*
 *
 */
void
Block::Output(std::ostream &out) const
{
	out << "{" << endl;
	
	if (CGOptions::depth_protect()) {
		out << "DEPTH++;" << endl;
	}

	OutputVariableList(local_vars, out);
	OutputStatementList(stm, out);
	
	if (CGOptions::depth_protect()) {
		out << "DEPTH--;" << endl;
	}

	out << "}" << endl;
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
