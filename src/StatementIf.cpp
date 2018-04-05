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

#include "Block.h"
#include "Expression.h"
#include "StatementIf.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
StatementIf *
StatementIf::make_random(CGContext &cg_context)
{
	Expression *expr = Expression::make_random(cg_context);

	Block *if_true = Block::make_random(cg_context);
	Block *if_false = Block::make_random(cg_context);

	return new StatementIf(*expr, *if_true, *if_false);
}

/*
 *
 */
StatementIf::StatementIf(const Expression &test,
						 const Block &if_true, const Block &if_false)
	: Statement(eIfElse),
	  test(test),
	  if_true(if_true),
	  if_false(if_false)
{
	// Nothing else to do.
}

/*
 *
 */
StatementIf::StatementIf(const StatementIf &si)
	: Statement(si.get_type()),
	  test(si.test),
	  if_true(si.if_true),
	  if_false(si.if_false)
{
	// Nothing else to do.
}

/*
 *
 */
StatementIf::~StatementIf(void)
{
	// Nothing to do.
}

/*
 *
 */
void
StatementIf::Output(std::ostream &out) const
{
	out << "if (";
	test.Output(out);
	out << ")" << endl;
	if_true.Output(out);
	out << "else" << endl;
	if_false.Output(out);
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
