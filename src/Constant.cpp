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
#include <ostream>
#include <string>
#include <iostream>
#include <sstream>

#include "CGContext.h"
#include "Constant.h"
#include "Type.h"

#include "random.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 * TODO: make well-known constants
 */

///////////////////////////////////////////////////////////////////////////////

/*
 * 
 */
Constant::Constant(const Type &t, const string &v)
	: type(t),
	  value(v)
{
	// Nothing else to do.
}

/*
 * 
 */
Constant::Constant(const Constant &c)
	: Expression(),
	  type(c.type),
	  value(c.value)
{
	// Nothing else to do.
}

/*
 * 
 */
Constant::~Constant(void)
{
	// Nothing else to do.
}

///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------
static string
GenerateRandomBoolConstant(void)
{
	return rnd_flipcoin(50) ? "true" : "false";
}

// --------------------------------------------------------------
static string
GenerateRandomIntConstant(void)
{
	// Int constant
	string val = "0x" + RandomHexDigits( 8 );
	return val;
}

// --------------------------------------------------------------
static string
GenerateRandomFloatConstant(void)
{
	// Generate a random floating point value with up to 10 digits of precision. (should look up precision of float/double.. 23 bits for IEEE-32?)
	string val = RandomDigits(5) + "." + RandomDigits(5);
	if (val.c_str()[0] == '0')
		val = "1" + val;
	return val;
}

// --------------------------------------------------------------
/*
 * Sometimes we need to generate constants outside of any current function,
 * e.g., initializers for global variables
 */
Constant *
Constant::make_random(CGContext &cg_context,
					  eSimpleType eType)
{
	assert(eType != eVoid);
	assert((eType >= 0) && (eType <= MAX_SIMPLE_TYPES));
	
	++cg_context.stmt_depth;

	// If `eType' is the MAX, choose a (non-void) type at random.
	if (eType == MAX_SIMPLE_TYPES) {
		do {
			eType = (eSimpleType) rnd_upto(MAX_SIMPLE_TYPES);
		} while (eType == eVoid);
	}

	const Type &t = Type::get_simple_type(eType);
	string v;

	if ((eType != eVoid) && (eType != eBool) && rnd_flipcoin(50)) {
		int num;
		if (rnd_flipcoin(50)) {
			num = rnd_upto(3)-1;
		} else {
			num = rnd_upto(20)-10;
		}
		ostringstream oss;
		oss << num;
		v = oss.str();
	} else {
		switch (eType) {
		case eVoid:     v = "/* void */";						break;
		case eBool:     v = GenerateRandomBoolConstant();		break;
		case eInt:      v = GenerateRandomIntConstant();		break;
	    case eFloat:    v = GenerateRandomFloatConstant();		break;
		}
	}

	--cg_context.stmt_depth;

	return new Constant(t, v);
}

/*
 * Return a `Constant' representing the integer value `v'.  Note that the
 * return value may not be "fresh."
 */
Constant *
Constant::make_int(int v)
{
	static const int cache_size = 256;
	static bool cache_inited = false;
	static Constant *cache_constants[cache_size];

	const Type &int_type = Type::get_simple_type(eInt);

	// Initialize our cache of small-number constants.
	if (!cache_inited) {
		cache_inited = true;

		ostringstream oss;
		for (int i = 0; i < cache_size; ++i) {
			oss.str("");
			oss << i;
			cache_constants[i] = new Constant(int_type, oss.str());
		}
	}

	// Get small numbers out of the cache.
	if ((v >= 0) && (v < cache_size)) {
		return cache_constants[v];
	}

	// Create fresh constants for values outside of our cache limit.
	ostringstream oss;
	oss << v;
	return new Constant(int_type, oss.str());
}

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
const Type &
Constant::get_type(void) const
{
	return type;
}

/*
 *
 */
void
Constant::Output(std::ostream &out) const
{
	out << value;
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
