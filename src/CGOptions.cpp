// -*- mode: C++ -*-
//
// Copyright (c) 2008 The University of Utah
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

///////////////////////////////////////////////////////////////////////////////

#include "CGOptions.h"

/*
 *
 */
#define DEFINE_GETTER_SETTER(type, init, f)	\
	type CGOptions::f##_ = (init); \
	type \
	CGOptions::f(void) { return f##_; }	\
	type \
	CGOptions::f(type p) { f##_ = p; return p; }

/*
 *
 */
#define DEFINE_GETTER_SETTER_BOOL(f) \
	DEFINE_GETTER_SETTER(bool, false, f)

#define DEFINE_GETTER_SETTER_INT(f) \
	DEFINE_GETTER_SETTER(int, 0, f)

///////////////////////////////////////////////////////////////////////////////

DEFINE_GETTER_SETTER_BOOL(compute_hash)
DEFINE_GETTER_SETTER_BOOL(print_hash)
DEFINE_GETTER_SETTER_BOOL(depth_protect)
DEFINE_GETTER_SETTER_INT (max_funcs)
DEFINE_GETTER_SETTER_INT (max_stmt_depth)
DEFINE_GETTER_SETTER_BOOL(wrap_volatiles)
DEFINE_GETTER_SETTER_BOOL(allow_const_volatile)

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
