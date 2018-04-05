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

// random.cpp --- utilities for random choice

#include <cassert>
#include <cstdlib>

#include "random.h"

using namespace std;

void seedrand( unsigned long seed )
{
	srand48 (seed);
}

unsigned long genrand(void)
{
	return lrand48();
}

// ------------------------------------------------------------------------------------------
static const char hex[] = "0123456789ABCDEF";
static const char dec[] = "0123456789";

string RandomHexDigits( int num )
{
	string str;
	while ( num-- )
	{
		str += hex[genrand()%16];
	}

	return str;
}

string RandomDigits( int num )
{
	string str;
	while ( num-- )
	{
		str += dec[genrand()%10];
	}

	return str;
}

///////////////////////////////////////////////////////////////////////////////

/*
 * Return a random number in the range 0..(n-1).
 */
unsigned int
rnd_upto(unsigned int n)
{
	return genrand() % n;
}

/*
 * Return `true' p% of the time.
 */
bool
rnd_flipcoin(unsigned int p)
{
	assert(p < 100);
	return (genrand() % 100) < p;
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
