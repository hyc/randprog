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

#ifndef CGOPTIONS_H
#define CGOPTIONS_H

///////////////////////////////////////////////////////////////////////////////

/*
 * XXX --- Collect all the default values here.
 *
 * "static const int" still needs declared storage, so use #define's instead.
 */
#define CGOPTIONS_DEFAULT_MAX_FUNCS			(20)
#define CGOPTIONS_DEFAULT_MAX_STMT_DEPTH	(8)

/*
 *
 */
class CGOptions {
public:
	static bool compute_hash(void);
	static bool compute_hash(bool p);

	static bool print_hash(void);
	static bool print_hash(bool p);

	static bool depth_protect(void);
	static bool depth_protect(bool p);

	static int max_funcs(void);
	static int max_funcs(int p);

	static int max_stmt_depth(void);
	static int max_stmt_depth(int p);

	static bool wrap_volatiles(void);
	static bool wrap_volatiles(bool p);

	static bool allow_const_volatile(void);
	static bool allow_const_volatile(bool p);

private:
	// Until I do this right, just make them all static.
	static bool	compute_hash_;
	static bool	print_hash_;
	static bool	depth_protect_;
	static int	max_funcs_;
	static int	max_stmt_depth_;
	static bool	wrap_volatiles_;
	static bool	allow_const_volatile_;

private:
	CGOptions(void);
	CGOptions(CGOptions &cgo);
};

///////////////////////////////////////////////////////////////////////////////

#endif // CGOPTIONS_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
