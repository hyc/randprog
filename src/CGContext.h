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

#ifndef CGCONTEXT_H
#define CGCONTEXT_H

///////////////////////////////////////////////////////////////////////////////

// Flags..
#define NO_RETURN (1)
#define IN_LOOP (2)
#define DO_HASH (4)

class Effect;
class Function;
class Variable;

/*
 *
 */
class CGContext
{
public:
	// CGContext(void);
	CGContext(Function *current_func,
			  int stmt_depth,
			  unsigned int flags,
			  const Effect &eff_context,
			  Effect *eff_accum);
	CGContext(const CGContext &cgc);
	~CGContext(void);

	Function *get_current_func(void) const			{ return current_func; }

	const Effect &get_effect_context(void) const	{ return effect_context; }
	Effect *get_effect_accum(void) const			{ return effect_accum; }

	void read_var(const Variable *v);
	void write_var(const Variable *v);
	void add_effect(const Effect &e);
	void add_external_effect(const Effect &e);

private:
	Function * const current_func; // may be null.

public: // XXX
	int stmt_depth;
	unsigned int flags;
	
private:
	const Effect &effect_context;
	Effect *effect_accum; // may be null!
	// TODO: move `Function::flags' to here.
	// TODO: move `Function::...' to here?

private:
	CGContext &operator=(const CGContext &cgc);	// unimplementable

	void sanity_check(void);
};

///////////////////////////////////////////////////////////////////////////////

#endif // CGCONTEXT_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
