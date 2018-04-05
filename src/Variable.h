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

#ifndef VARIABLE_H
#define VARIABLE_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <string>
#include <vector>

#include "Effect.h"

class Expression;
class Function;
class Type;

enum eVariableScope
{
	eGlobal,
	eParentLocal,
	eParentParam,
	eNewValue
};
#define MAX_VAR_SCOPE ((eVariableScope) (eNewValue+1))

class Variable
{
public:
	Variable(const std::string &name, const Type *type,
			 bool isConst, bool isVolatile,
			 bool isAuto, bool isStatic, bool isRegister);
	Variable(const std::string &name, const Type *type);
	~Variable(void);

	bool is_global(void) const;

	///////////////////////////////////////////////////////////////////////
	
	void Output(std::ostream &) const;
	void OutputAsLHS(std::ostream &) const;
	void OutputAddrOf(std::ostream &) const;
	void OutputForComment(std::ostream &) const;
	void OutputDef(std::ostream &) const;

	const std::string name;
	const Type *type;
	const Expression *init;

	// Type qualifiers.
	const bool isConst;
	const bool isVolatile;

	// Storage-class specifiers.
	const bool isAuto;
	// bool isExtern;
	const bool isStatic;
	const bool isRegister;
};

// Factories.
Variable *SelectExistingVariable(Function &func,
								 Effect::Access access,
								 const Effect &effect_context);
Variable *SelectVariable(Function &func,
						 Effect::Access access,
						 const Effect &effect_context);
Variable *SelectLValue(Function &func,
					   const Effect &effect_context);
void GenerateParameterVariable(Function &curFunc, Function &parent);
void OutputVariableList(const std::vector<Variable*> &var, std::ostream &out);
void OutputGlobalVariables(std::ostream &);
void HashGlobalVariables(std::ostream &);

///////////////////////////////////////////////////////////////////////////////

#endif // VARIABLE_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
