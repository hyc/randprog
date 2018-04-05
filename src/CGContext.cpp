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
#include "Effect.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

#if 0
/*
 * 
 */
CGContext::CGContext(void)
	: current_func(0),
	  stmt_depth(0),
	  flags(0),
	  effect_context(),
	  effect_accum(0)
{
	// Nothing else to do.
}
#endif

/*
 *
 */
CGContext::CGContext(Function *current_func,
					 int stmt_depth,
					 unsigned int flags,
					 const Effect &eff_context,
					 Effect *eff_accum)
	: current_func(current_func),
	  stmt_depth(stmt_depth),
	  flags(flags),
	  effect_context(eff_context),
	  effect_accum(eff_accum)
{
	// Nothing else to do.
}

/*
 * 
 */
CGContext::CGContext(const CGContext &cgc)
	: current_func(cgc.current_func),
	  stmt_depth(cgc.stmt_depth),
	  flags(cgc.flags),
	  effect_context(cgc.effect_context),
	  effect_accum(cgc.effect_accum)
{
	// Nothing else to do.
}

/*
 * 
 */
CGContext::~CGContext(void)
{
	// Nothing else to do.
}

/*
 *
 */
void
CGContext::read_var(const Variable *v)
{
	if (effect_accum) {
		effect_accum->read_var(v);
	}
	sanity_check();
}

/*
 *
 */
void
CGContext::write_var(const Variable *v)
{
	if (effect_accum) {
		effect_accum->write_var(v);
	}
	sanity_check();
}

/*
 * 
 */
void
CGContext::add_effect(const Effect &e)
{
	if (effect_accum) {
		effect_accum->add_effect(e);
	}
	sanity_check();
}

/*
 * 
 */
void
CGContext::add_external_effect(const Effect &e)
{
	if (effect_accum) {
		effect_accum->add_external_effect(e);
	}
	sanity_check();
}

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
void
CGContext::sanity_check(void)
{
#if 0
	// XXX: too strong.  See note in `TODO'.
	if (effect_accum && !effect_context.is_side_effect_free()) {
		assert(effect_accum->is_side_effect_free());
	}
#endif // 0
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
