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

#ifndef TYPE_H
#define TYPE_H

///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <ostream>

/*
 *
 */
enum eTypeDesc
{
	eSimple
//	ePointer,
//	eArray,
//	eUnion,
//	eStruct,
};
#define MAX_TYPE_DESC ((eTypeDesc) (eSimple+1))

/*
 *
 */
enum eSimpleType
{
	eVoid,
	eBool,
	eInt,
	eFloat
};
#define MAX_SIMPLE_TYPES ((eSimpleType) (eFloat+1))

/*
 *
 */
class Type
{
public:
	// Pseudo-factory method.  This is `choose_random()' rather than
	// `make_random()' because the returned object is not fresh.
	static const Type *choose_random(void);

	Type(eSimpleType simple_type);
	Type(const Type &t);
	~Type(void);
	
	Type &operator=(const Type &t);

	static const Type &get_simple_type(eSimpleType simple_type);
	
	void Output(std::ostream &) const;

// private:	
	eTypeDesc eType;
	// Type *ptr_type;
	eSimpleType simple_type;
	// vector<int> arrayDim;
	// vector<Type*> aggregateType;

private:
	// Factory method.  Public should (?) use `choose_random()' instead.
	static Type *make_random(void);
	
	static const Type *simple_types[MAX_SIMPLE_TYPES];
	
	// Package init.
	friend void GenerateAllTypes(void);
};

void GenerateAllTypes(void);

///////////////////////////////////////////////////////////////////////////////

#endif // TYPE_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
