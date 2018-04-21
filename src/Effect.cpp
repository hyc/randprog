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

#include "Effect.h"
#include "Variable.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/*
 * The empty effect.
 */
const Effect Effect::empty_effect;

///////////////////////////////////////////////////////////////////////////////

/*
 * Stuff that so badly wants to be rewritten using decent STL containers and
 * algorithms.
 */
static bool
non_empty_intersection(const pool_vector<const Variable *> &va,
					   const pool_vector<const Variable *> &vb)
{
	pool_vector<const Variable *>::size_type va_len = va.size();
	pool_vector<const Variable *>::size_type vb_len = vb.size();
	pool_vector<const Variable *>::size_type i;
	pool_vector<const Variable *>::size_type j;
	
	for (i = 0; i < va_len; ++i) {
		for (j = 0; j < vb_len; ++j) {
			if (va[i] == vb[j]) {
				return true;
			}
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////

/*
 * 
 */
Effect::Effect(void) :
	read_vars(0),
	write_vars(0),
	pure(true),
	side_effect_free(true)
{
	// Nothing else to do.
}

/*
 * 
 */
Effect::Effect(const Effect &e) :
	read_vars(e.read_vars),
	write_vars(e.write_vars),
	pure(e.pure),
	side_effect_free(e.side_effect_free)
{
	// Nothing else to do.
}

/*
 * 
 */
Effect::~Effect(void)
{
	// Nothing else to do.
}

/*
 * 
 */
Effect &
Effect::operator=(const Effect &e)
{
	if (this == &e) {
		return *this;
	}

	read_vars = e.read_vars;
	write_vars = e.write_vars;
	pure = e.pure;
	side_effect_free = e.side_effect_free;

	return *this;
}

/*
 *
 */
void
Effect::read_var(const Variable *v)
{
	if (!is_read(v)) {
		read_vars.push_back(v);
	}
	pure &= (v->isConst && !v->isVolatile);
	side_effect_free &= !v->isVolatile;
}

/*
 *
 */
void
Effect::write_var(const Variable *v)
{
	if (!is_written(v)) {
		write_vars.push_back(v);
	}
	// pure = pure;
	// TODO: not quite correct below ---
	// --- but I think we want "side_effect_free" to mean "volatile_free".
	side_effect_free &= !v->isVolatile;
	// side_effect_free = false;
}

/*
 * 
 */
void
Effect::add_effect(const Effect &e)
{
	if (this == &e) {
		return;
	}

	// Stuff that so badly wants to be rewritten using decent STL containers
	// and algorithms... compute the union effect.

	pool_vector<const Variable *>::size_type len;
	pool_vector<const Variable *>::size_type i;

	len = e.read_vars.size();
	for (i = 0; i < len; ++i) {
		// this->read_var(e.read_vars[i]);
		if (!is_read(e.read_vars[i])) {
			this->read_vars.push_back(e.read_vars[i]);
		}
	}
	len = e.write_vars.size();
	for (i = 0; i < len; ++i) {
		// this->write_var(e.write_vars[i]);
		if (!is_written(e.write_vars[i])) {
			this->write_vars.push_back(e.write_vars[i]);
		}
	}

	pure &= e.pure;
	side_effect_free &= e.side_effect_free;
}

/*
 * 
 */
void
Effect::add_external_effect(const Effect &e)
{
	if (this == &e) {
		return;
	}
	
	pool_vector<Variable *>::size_type len;
	pool_vector<Variable *>::size_type i;

	len = e.read_vars.size();
	for (i = 0; i < len; ++i) {
		if (e.read_vars[i]->is_global()) {
			this->read_var(e.read_vars[i]);
		}
	}

	len = e.write_vars.size();
	for (i = 0; i < len; ++i) {
		if (e.write_vars[i]->is_global()) {
			this->write_var(e.write_vars[i]);
		}
	}
}

/*
 * TODO: tried doing this with `find', but I couldn't get all the `const's
 * right.  Gave up; wrote it by hand!
 */
bool
Effect::is_read(const Variable *v) const
{
	pool_vector<Variable *>::size_type len = read_vars.size();
	pool_vector<Variable *>::size_type i;

	for (i = 0; i < len; ++i) {
		if (read_vars[i] == v) {
			return true;
		}
	}
	return false;
}

/*
 * 
 */
bool
Effect::is_written(const Variable *v) const
{
	pool_vector<Variable *>::size_type len = write_vars.size();
	pool_vector<Variable *>::size_type i;

	for (i = 0; i < len; ++i) {
		if (write_vars[i] == v) {
			return true;
		}
	}
	return false;
}

/*
 * 
 */
bool
Effect::has_race_with(const Effect &e) const
{
	return (   non_empty_intersection(this->read_vars,  e.write_vars)
			|| non_empty_intersection(this->write_vars, e.read_vars)
			|| non_empty_intersection(this->write_vars, e.write_vars));
}

///////////////////////////////////////////////////////////////////////////////

/*
 *
 */
void
Effect::Output(std::ostream &out) const
{
	pool_vector<Variable *>::size_type len;
	pool_vector<Variable *>::size_type i;

	out << "/*" << endl;

	out << " * reads :";
	len = read_vars.size();
	for (i = 0; i < len; ++i) {
		out << " ";
		read_vars[i]->OutputForComment(out);
	}
	out << endl;

	out << " * writes:";
	len = write_vars.size();
	for (i = 0; i < len; ++i) {
		out << " ";
		write_vars[i]->OutputForComment(out);
	}
	out << endl;

	out << " */" << endl;
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
