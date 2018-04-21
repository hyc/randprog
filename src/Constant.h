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

#ifndef CONSTANT_H
#define CONSTANT_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "Pool.h"
#include "Expression.h"
#include "Type.h"

class CGContext;

/*
 *
 */
class Constant : public Expression
{
public:
	// Factory method.
	static Constant *make_random(CGContext &cg_context,
								 eSimpleType eType = MAX_SIMPLE_TYPES);

	// Factory method.
	static Constant *make_int(int v);
	
	Constant(const Type &t, const pool_string &v);
	Constant(const Constant &c);
	virtual ~Constant(void);

	//

	virtual const Type &get_type(void) const;
	// Unused:
	// const std::string &get_value(void) const { return value; }
	
	virtual void Output(std::ostream &) const;

private:	
	const Type &type;
	const pool_string value;
};

void ConstantReset(void);

///////////////////////////////////////////////////////////////////////////////

#endif // CONSTANT_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
