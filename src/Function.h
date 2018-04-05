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

#ifndef FUNCTION_H
#define FUNCTION_H

///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include "Effect.h"
#include "Type.h"

class Block;
class Variable;

///////////////////////////////////////////////////////////////////////////////

class Function
{
public:
	friend void GenerateFunctions(void);

	// Factory methods.
	static Function *make_first(void);
	static Function *make_random(Function &, const Effect &effect_context);

	void Output(std::ostream &);
	void OutputForwardDecl(std::ostream &);

	bool is_built(void) const { return (build_state == BUILT); }
	bool is_effect_known(void) const { return (build_state == BUILT); }
	const Effect &get_effect(void) const { return effect; }
	
	std::string name;
	std::vector<Variable*> param;
//	vector<Expression*> param_value;
	Type return_type;
//	bool isBackLink;

	Effect effect;
	
	std::vector<Block*> stack;
	Block *body;
	
private:
	Function(const std::string &name, const Type &return_type);
	void OutputHeader(std::ostream &);
	void OutputFormalParamList(std::ostream &);
	void GenerateBody(const Effect &effect_context);

private:
	enum { UNBUILT, BUILDING, BUILT } build_state;
};

void GenerateFunctions(void);
Function *GetFirstFunction(void);
Function *SelectFunction(Function &curFunc,
						 bool &isBackLink, const Effect &effect_context);
// unsigned
long FuncListSize(void);
void OutputForwardDeclarations(std::ostream &out);
void OutputFunctions(std::ostream &out);

///////////////////////////////////////////////////////////////////////////////

#endif // FUNCTION_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
