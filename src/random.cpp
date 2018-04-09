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

#define SFMT_MEXP	19937
#include "SFMT/SFMT.h"

#include "crypto/c_keccak.h"

using namespace std;

static sfmt_t sfmt_state;

int hex2bin( char *in, char *out0, int len )
{
	int i;
	char *out = out0;
	for (i=0; i<len; i+=2) {
		char nyb1, nyb2;
		nyb1 = *in++;
		nyb2 = *in++;
		nyb1 &= ~0x20;
		nyb2 &= ~0x20;
		if (nyb1 & 0x40)
			nyb1 -= 55;
		else
			nyb1 &= 0x0f;
		if (nyb2 & 0x40)
			nyb2 -= 55;
		else
			nyb2 &= 0x0f;
		*out++ = nyb1 << 4 | nyb2;
	}
	return out - out0;
}

void seedrand( char *seed )
{
	uint64_t state[25];
	int len = strlen(seed);
	char *binseed = (char *)alloca(len/2);
	len = hex2bin(seed, binseed, len);
	keccak((const uint8_t *)binseed, len, (uint8_t *)state, sizeof(state));
	sfmt_init_by_array(&sfmt_state, (uint32_t *)state, sizeof(state)/sizeof(uint32_t));
}

unsigned long genrand(void)
{
	return sfmt_genrand_uint32(&sfmt_state);
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
