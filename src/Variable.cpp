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

#include <cassert>

#include "Common.h"

#include "Block.h"
#include "CGContext.h"
#include "CGOptions.h"
#include "Constant.h"
#include "Effect.h"
#include "Function.h"
#include "Type.h"
#include "Variable.h"

#include "random.h"
#include "util.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------
// Globals.

// All globals, including volatiles.
static vector<Variable*> GlobalList;

// All the non-volatile globals.
static vector<Variable*> GlobalNonvolatilesList;

// --------------------------------------------------------------
static string
RandomGlobalName(void)
{
	return gensym("g_");
}

// --------------------------------------------------------------
static string
RandomLocalName(void)
{
	return gensym("l_");
}

// --------------------------------------------------------------
static string
RandomParamName(void)
{
	return gensym("p_");
}

// --------------------------------------------------------------
/*
 * Choose a variable from `vars' to read or write.
 * Return null if no suitable variable can be found.
 */
static Variable *
choose_var(vector<Variable *> vars,
		   Effect::Access access,
		   const Effect &effect_context)
{
	vector<Variable *> ok_vars;
	vector<Variable *>::iterator i;

	for (i = vars.begin(); i != vars.end(); ++i) {
		// We cannot read or write a volatile if the current context already
		// has a side-effect.
		// TODO: is this too strong for what we want?
		if ((*i)->isVolatile && !effect_context.is_side_effect_free()) {
			continue;
		}
		// We can neither read nor write a variable that is being written in
		// the current context.
		if (((access == Effect::READ) || (access == Effect::WRITE))
			&& effect_context.is_written(*i)) {
			continue;
		}
		// We cannot write a variable that is being read in the current
		// context.
		if ((access == Effect::WRITE) && effect_context.is_read(*i)) {
			continue;
		}
		// We cannot write `const' variables.
		if ((access == Effect::WRITE) && (*i)->isConst) {
			continue;
		}
		// Otherwise, this is an acceptable choice.
		ok_vars.push_back(*i);
	}

	vector<Variable *>::size_type ok_size = ok_vars.size();

	if (ok_size == 0) {
		return 0;
	}
	if (ok_size == 1) {
		return ok_vars[0];
	}
	return ok_vars[rnd_upto(ok_size)];
}

// --------------------------------------------------------------
static Variable *
GenerateNewGlobal(Effect::Access access, const Effect &effect_context)
{
	bool isVolatile = effect_context.is_side_effect_free() && rnd_flipcoin(30);
	bool isConst = (access != Effect::WRITE) && rnd_flipcoin(10);
	// bool isConst = false;
	
	if (isVolatile && isConst && !CGOptions::allow_const_volatile()) {
		isConst = false;
	}
	
	Variable *var = new Variable(RandomGlobalName(), Type::choose_random(),
								 isConst, isVolatile,
								 false, false, false);
	GlobalList.push_back(var);
	if (!isVolatile) {
		GlobalNonvolatilesList.push_back(var);
	}
	return var;
}

// --------------------------------------------------------------
// Select a random global variable.
static Variable *
SelectGlobal(Effect::Access access, const Effect &effect_context)
{
	if (effect_context.is_side_effect_free()) {
		if (GlobalList.empty())	{
			return GenerateNewGlobal(access, effect_context);
		}
	} else {
		if (GlobalNonvolatilesList.empty()) {
			return GenerateNewGlobal(access, effect_context);
		}
	}
	return choose_var(GlobalList, access, effect_context);
}

// --------------------------------------------------------------
static Variable *
GenerateNewParentLocal(Block &block,
					   Effect::Access /*access*/,
					   const Effect &/*effect_context*/)
{
	// We never generate const locals, so `access' is unneeded.
	// We never generate volatile locals, so `effect_context' is unneeded.
	Variable *var = new Variable(RandomLocalName(), Type::choose_random());
	block.local_vars.push_back(var);
	return var;
}

// --------------------------------------------------------------
// XXX --- this seems like a mess.  We potentially generate a variable, and
// then just use its type?
//
// TODO: fix this so we don't need an effect.
void
GenerateParameterVariable(Function &curFunc, Function &parent)
{
	Variable *var = SelectVariable(parent,
								   Effect::READ, Effect::get_empty_effect());

	// Add this type to our parameter list.
	Variable *param = new Variable(RandomParamName(), var->type);
	curFunc.param.push_back(param);
}

// --------------------------------------------------------------
static Variable *
SelectParentLocal(Function &parent,
				  Effect::Access access,
				  const Effect &effect_context)
{
	// Select from the local variables of the parent OR any of its block stack.

	if (parent.stack.empty()) {
		// We're choosing a local from a function whose body hasn't been built.
		// This should never happen.
		assert(!parent.stack.empty());
		return 0;
	}

	Block *block = parent.stack[rnd_upto(parent.stack.size())];
	
	// Should be "generate new block local"...
	if (block->local_vars.empty()) {
		return GenerateNewParentLocal(*block, access, effect_context);
	}

	return choose_var(block->local_vars, access, effect_context);
}

// --------------------------------------------------------------
static eVariableScope
VariableSelectionProbability(vector<Variable*> & /*List*/)
{
	// Should probably modify choice based on current list of params, parent
	// params, parent locals, # of globals, etc.
	//
	// ...but this is easier.
	return (eVariableScope)(rnd_upto(MAX_VAR_SCOPE));
}

// --------------------------------------------------------------
static eVariableScope
LValueSelectionProbability(vector<Variable*> &List)
{
	eVariableScope eType;
	while (1) {
		eType = VariableSelectionProbability(List);
		if (eType != eNewValue)
			return eType;
	}
}

// --------------------------------------------------------------
static eVariableScope
VariableCreationProbability(Function & /*parent*/)
{
	if (rnd_flipcoin(10))		// 10% chance to create new global var
		return eGlobal;
	else
		return eParentLocal;
}

// --------------------------------------------------------------
static Variable *
SelectParentParam(Function &parent,
				  Effect::Access access,
				  const Effect &effect_context)
{
	if (parent.param.empty())
		return SelectParentLocal(parent, access, effect_context);
	// We never generate volatile params, so `effect_context' is unused below.
	return choose_var(parent.param, access, effect_context);
}

// --------------------------------------------------------------
static Variable *
GenerateNewVariable(Function &func,
					Effect::Access access,
					const Effect &effect_context)
{
	Variable *var = 0;
	switch (VariableCreationProbability(func)) {
	case eGlobal:
		var = GenerateNewGlobal(access, effect_context);
		break;
	case eParentLocal:
		var = GenerateNewParentLocal(
				*(func.stack[rnd_upto(func.stack.size())]),
				access,
				effect_context);
		break;
	default:
		break;
	}
	return var;
}

// --------------------------------------------------------------
Variable *
SelectLValue(Function &func, const Effect &effect_context)
{
	Variable *var = 0;
	// Note that many of the functions that select `var' can return null, if
	// they cannot find a suitable variable.  So we loop.
	while (!var) {
		switch (LValueSelectionProbability(func.param)) {
		case eGlobal:
			var = SelectGlobal(Effect::WRITE, effect_context);
			break;
		case eParentLocal:
			// ...a local var from one of its blocks.
			var = SelectParentLocal(func, Effect::WRITE, effect_context);
			break;
		case eParentParam:
			// ...one of the function's parameters.
			var = SelectParentParam(func, Effect::WRITE, effect_context);
			break;
		default:
			break;
		}
	}
	return var;
}

// --------------------------------------------------------------
// Select or Create a new variable visible to this scope (new var may be
// global, or local to one of the function's blocks)
Variable *
SelectVariable(Function &func,
			   Effect::Access access,
			   const Effect &effect_context)
{
	Variable *var = 0;
	// Note that many of the functions that select `var' can return null, if
	// they cannot find a suitable variable.  So we loop.
	while (!var) {
		switch (VariableSelectionProbability(func.param)) {
		case eGlobal:
			var = SelectGlobal(access, effect_context);
			break;
		case eParentLocal:
			// ...a local var from one of its blocks.
			var = SelectParentLocal(func, access, effect_context);
			break;
		case eParentParam:
			// ...one of the function's parameters.
			var = SelectParentParam(func, access, effect_context);
			break;
		case eNewValue:
			// Must decide where to put the new variable (global or parent
			// local)?
			var = GenerateNewVariable(func, access, effect_context);
			break;
		}
	}
	if (!effect_context.is_side_effect_free()) {
		assert(!var->isVolatile);
	}
	return var;
}

// --------------------------------------------------------------
Variable *
SelectExistingVariable(Function &func,
					   Effect::Access access,
					   const Effect &effect_context)
{
	Variable *var = 0;
	// Note that many of the functions that select `var' can return null, if
	// they cannot find a suitable variable.  So we loop.
	while (!var) {
		switch (VariableSelectionProbability(func.param)) {
		case eGlobal:
			var = SelectGlobal(access, effect_context);
			break;
		case eParentLocal:
			// ...a local var from one of its blocks.
			var = SelectParentLocal(func, access, effect_context);
			break;
		case eParentParam:
			// ...one of the function's parameters.
			var = SelectParentParam(func, access, effect_context);
			break;
		default:
			break;
		}
	}
	return var;
}

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
Variable::Variable(const std::string &name, const Type *type,
				   bool isConst, bool isVolatile,
				   bool isAuto, bool isStatic, bool isRegister)
	: name(name), type(type),
	  init(0),
	  isConst(isConst), isVolatile(isVolatile),
	  isAuto(isAuto), isStatic(isStatic), isRegister(isRegister)
{
	// XXX caller should generate the init expr more cleverly/explicitly
	CGContext cg_context(0, 0, 0, Effect::get_empty_effect(), 0);
	init = Constant::make_random(cg_context, type->simple_type);
}

/*
 *
 */
Variable::Variable(const std::string &name, const Type *type)
	: name(name), type(type),
	  init(0),
	  isConst(false), isVolatile(false),
	  isAuto(false), isStatic(false), isRegister(false)
{
	// XXX caller should generate the init expr more cleverly/explicitly
	CGContext cg_context(0, 0, 0, Effect::get_empty_effect(), 0);
	init = Constant::make_random(cg_context, type->simple_type);
}

/*
 *
 */
Variable::~Variable(void)
{
	// Nothing to do.
}

// --------------------------------------------------------------
bool
Variable::is_global(void) const
{
	// TODO: A bit of a hack.  Variables should track their own scopes.
	for (vector<Variable *>::iterator i = GlobalList.begin();
		 i != GlobalList.end();
		 ++i) {
		if ((*i) == this) {
			return true;
		}
	}
	return false;
}

// --------------------------------------------------------------
void
Variable::OutputDef(std::ostream &out) const
{
	out << "var " << name << " = ";
	init->Output(out);
	out << ";" << endl;
}

// --------------------------------------------------------------
void
Variable::Output(std::ostream &out) const
{
	if (isVolatile && CGOptions::wrap_volatiles()) {
		out << "VOL_RVAL(" << name << ", ";
		type->Output(out);
		out << ")";
	} else {
		out << name;
	}
}

// --------------------------------------------------------------
void
Variable::OutputAsLHS(std::ostream &out) const
{
	if (isVolatile && CGOptions::wrap_volatiles()) {
		out << "VOL_LVAL(" << name << ", ";
		type->Output(out);
		out << ")";
	} else {
		out << name;
	}
}

// --------------------------------------------------------------
// This function is a bit of hack, because ---
//   &VOL_RVAL(g_4)
// is invalid when VOL_RVAL expands into a function call.
void
Variable::OutputAddrOf(std::ostream &out) const
{
	out << "&" << name;
}

// --------------------------------------------------------------
void
Variable::OutputForComment(std::ostream &out) const
{
	out << name;
}

// --------------------------------------------------------------
static void
MapVariableList(const vector<Variable*> &var, std::ostream &out,
				void (*func)(Variable *var, std::ostream *pOut))
{
	for_each(var.begin(), var.end(), std::bind2nd(std::ptr_fun(func), &out));
}

// --------------------------------------------------------------
static void
OutputVariable(Variable *var, std::ostream *pOut)
{
	var->OutputDef(*pOut);
}

// --------------------------------------------------------------
void
OutputVariableList(const vector<Variable*> &var, std::ostream &out)
{
	MapVariableList(var, out, OutputVariable);
}

// --------------------------------------------------------------
void
OutputGlobalVariables(std::ostream &out)
{
	out << "/* --- GLOBAL VARIABLES --- */" << endl;
	OutputVariableList(GlobalList, out);
}

// --------------------------------------------------------------
static void
HashVariable(Variable *var, std::ostream *pOut)
{
	std::ostream &out = *pOut;
	out << "print(";
	var->Output(out);
	out << ");" << endl;
}

// --------------------------------------------------------------
void
HashGlobalVariables(std::ostream &out)
{
	MapVariableList(GlobalList, out, HashVariable);
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
