/* -*- mode: C -*-
 *
 * Copyright (c) 2007, 2008 The University of Utah
 * All rights reserved.
 *
 * This file is part of `randprog', a random generator of C programs.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RANDOM_RUNTIME_H
#define RANDOM_RUNTIME_H

/*****************************************************************************/

//#include <limits.h>
#define CHAR_BIT 8
#define INT_MAX 2147483647
#define UINT_MAX	4294967295U

/*
 * Our piddly runtime!
 */

#ifndef DEPUTY
#define COUNT(n)
#define TC
#define SAFE
#endif

/*****************************************************************************/

static uint16_t
crcByte(uint16_t crc, uint8_t b)
{
	uint8_t i;
  
	crc = crc ^ b << 8;
	i = 8;
	do {
		if (crc & 0x8000)
			crc = crc << 1 ^ 0x1021;
		else
			crc = crc << 1;
	} while (--i);

	return crc;
}

static void
crcBytes(uint32_t val)
{
	uint8_t *tmp = (uint8_t *COUNT(4))TC(&val);
	int len;
	for (len=4; len > 0; len--) {
		context = crcByte(context, *tmp++);
	}
}

/*****************************************************************************/

static inline int
lshift_s_s(int left, int right)
{
	if ((left < 0)
		|| (right < 0)
		|| (right >= sizeof(int)*CHAR_BIT)
		|| (left > (INT_MAX >> right))) {
		/* Avoid undefined behavior. */
		return left;
	}
	return left << right;
}

static inline int
lshift_s_u(int left, unsigned int right)
{
	if ((left < 0)
		|| (right >= sizeof(int)*CHAR_BIT)
		|| (left > (INT_MAX >> right))) {
		/* Avoid undefined behavior. */
		return left;
	}
	return left << right;
}

static inline unsigned int
lshift_u_s(unsigned int left, int right)
{
	if ((right < 0)
		|| (right >= sizeof(unsigned int)*CHAR_BIT)
		|| (left > (UINT_MAX >> right))) {
		/* Avoid undefined behavior. */
		return left;
	}
	return left << right;
}

static inline unsigned int
lshift_u_u(unsigned int left, unsigned int right)
{
	if ((right >= sizeof(unsigned int)*CHAR_BIT)
		|| (left > (UINT_MAX >> right))) {
		/* Avoid undefined behavior. */
		return left;
	}
	return left << right;
}

static inline int
rshift_s_s(int left, int right)
{
	if ((left < 0)
		|| (right < 0)
		|| (right >= sizeof(int)*CHAR_BIT)) {
		/* Avoid implementation-defined and undefined behavior. */
		return left;
	}
	return left >> right;
}

static inline int
rshift_s_u(int left, unsigned int right)
{
	if ((left < 0)
		|| (right >= sizeof(int)*CHAR_BIT)) {
		/* Avoid implementation-defined and undefined behavior. */
		return left;
	}
	return left >> right;
}

static inline unsigned int
rshift_u_s(unsigned int left, int right)
{
	if ((right < 0)
		|| (right >= sizeof(unsigned int)*CHAR_BIT)) {
		/* Avoid undefined behavior. */
		return left;
	}
	return left >> right;
}

static inline unsigned int
rshift_u_u(unsigned int left, unsigned int right)
{
	if (right >= sizeof(unsigned int)*CHAR_BIT) {
		/* Avoid undefined behavior. */
		return left;
	}
	return left >> right;
}

/*****************************************************************************/

static inline unsigned long int
mod_rhs(const long int rhs)
{
    if (rhs == 0) return 1;
    return rhs;
}

static inline unsigned long int
div_rhs(const long int rhs)
{
    if (rhs == 0) return 1;
    return rhs;
}

/*****************************************************************************/

#endif /* RANDOM_RUNTIME_H */

/*
 * Local Variables:
 * c-basic-offset: 4
 * tab-width: 4
 * End:
 */

/* End of file. */
