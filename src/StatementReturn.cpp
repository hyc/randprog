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

#include "CGContext.h"
#include "CGOptions.h"
#include "Function.h"
#include "StatementReturn.h"
#include "Variable.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
StatementReturn *
StatementReturn::make_random(CGContext &cg_context)
{
	Function *curr_func = cg_context.get_current_func();
	assert(curr_func);

	Variable *var = 0;

	if ((curr_func->return_type.eType != eSimple) ||
		(curr_func->return_type.simple_type != eVoid))	{
		// SelectVariableOfType(curFunc.return_type);
		var = SelectVariable(curr_func,
							 Effect::READ,
							 cg_context.get_effect_context());
		cg_context.read_var(var);
	}

	// XXX
	assert(var != 0);

	return new StatementReturn(*var);
}

/*
 *
 */
StatementReturn::StatementReturn(const Variable &v)
	: Statement(eReturn),
	  var(v)
{
	// Nothing else to do.
}

/*
 *
 */
StatementReturn::StatementReturn(const StatementReturn &sr)
	: Statement(sr.get_type()),
	  var(sr.var)
{
	// Nothing else to do.
}

/*
 *
 */
StatementReturn::~StatementReturn(void)
{
	// Nothing to do.
}

/*
 *
 */
void
StatementReturn::Output(std::ostream &out) const
{
	// XXX --- Fix this.  Outputting two stmts instead of one is bad mojo.
	if (CGOptions::depth_protect()) {
		out << "DEPTH--;" << endl;
	}
	out << "return ";
	var.Output(out);
	out << ";" << endl;
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
