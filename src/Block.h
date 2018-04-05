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

#ifndef BLOCK_H
#define BLOCK_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <vector>

#include "Statement.h"

class CGContext;
class Statement;
class Variable;

/*
 *
 */
class Block : public Statement
{
public:
	// Factory method.
	static Block *make_random(CGContext &cg_context);

	Block(void);
	Block(const Block &b);
	virtual ~Block(void);

	//
	virtual bool set_depth_protect(bool b) { depth_protect = b; return b; }
	virtual bool get_depth_protect(void) { return depth_protect; }

	virtual void Output(std::ostream &out) const;

	// These are currently accessed directly.
	std::vector<Statement *> stm;
	std::vector<Variable *> local_vars;

private:
	bool depth_protect;

	//

	Block &operator=(const Block &b); // unimplemented
};

///////////////////////////////////////////////////////////////////////////////

#endif // BLOCK_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
