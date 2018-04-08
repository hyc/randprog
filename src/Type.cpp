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

#include "Common.h"
#include "Type.h"

#include "random.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
const Type *Type::simple_types[MAX_SIMPLE_TYPES];

// ---------------------------------------------------------------------
// List of all types used in the program
static vector<const Type *> AllTypes;

///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------
Type::Type(eSimpleType simple_type) :
	eType(eSimple),
	simple_type(simple_type)
{
	// Nothing else to do.
}

// --------------------------------------------------------------
Type::Type(const Type &t) :
	eType(t.eType),
	simple_type(t.simple_type)
{
	// Nothing else to do.
}

// --------------------------------------------------------------
Type::~Type(void)
{
	// Nothing to do.
}

// --------------------------------------------------------------
Type &
Type::operator=(const Type& t)
{
	if (this == &t) {
		return *this;
	}

	eType = t.eType;
	simple_type = t.simple_type;
	
	return *this;
}

// ---------------------------------------------------------------------
const Type &
Type::get_simple_type(eSimpleType st)
{
	static bool inited = false;

	if (!inited) {
		for (int i = 0; i < MAX_SIMPLE_TYPES; ++i) {
			Type::simple_types[i] = 0;
		}
		inited = true;
	}
	
	if (Type::simple_types[st] == 0) {
		Type::simple_types[st] = new Type(st);
	}
	return *Type::simple_types[st];
}

// ---------------------------------------------------------------------
static bool
AllTypesProbability(void)
{
	// Always have at least 10 types in the program.
	if (AllTypes.size() < 10)
		return true;
	// 50% probability for each additional type.
	return rnd_flipcoin(50);
}

// ---------------------------------------------------------------------
Type *
Type::make_random(void)
{
	// TODO: Test other type creation.
	eSimpleType simple_type;
	do {
		simple_type = (eSimpleType) rnd_upto(MAX_SIMPLE_TYPES);
	} while (simple_type == eVoid);
	
	return new Type(simple_type);
}

// ---------------------------------------------------------------------
void
GenerateAllTypes(void)
{
	while (AllTypesProbability()) {
		AllTypes.push_back(Type::make_random());
	}
}

// ---------------------------------------------------------------------
const Type *
Type::choose_random(void)
{
	return AllTypes[rnd_upto(AllTypes.size())];
}

// ---------------------------------------------------------------------
void
Type::Output(std::ostream &out) const
{
	switch (eType) {
	case eSimple:
		switch (simple_type) {
		case eVoid:		out << "void"; break;
		case eBool:		out << "bool"; break;
		case eInt:		out << "int"; break;
		case eFloat:	out << "float"; break;
		}
		break;
//	case ePointer:  ptr_type->OutputType( out ); out << "*"; break;
//	case eUnion:
//	case eStruct:
//	case eArray:	ptr_type->OutputType( out> ); out << "[]"; break;	// TODO: Output correct array subscripts..
	}
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
