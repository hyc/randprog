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
// Contributions and bug fixes by:
// Jan-2007 : Mat Hostetter - Explicit "return 0" for main()
//

/*
Random C/C++ Program Generator
------------------------------
1) Create a set of random types to be used throughout the program
2) Create the main func.
3) Generate a random block of statements with maximum control & expression nesting depths.
4) If new functions were defined in #3, then loop back to fill in its body.
5) When a maximum number of functions is reached, stop generating new functions and finish off the bodies of the remaining funcs.
6) Output generated program.

GOALS:
- Locate basic errors in compiler front-ends (crashes, etc).
- Ferret out correctness errors in optimization passes.
- Support the design of tools to search for improved optimization paths (partial exection, etc).

TODO:
- Protect back links with a global DEPTH, don't call if DEPTH is too high (avoid infinite recursion)
- Main function generates hash of global state as output of program - use to locate optimization errors.
	- Compile in Debug mode vs. Optimized mode, compare hash value at program termination.
- Improve hash function; use stronger hashing (ARCFOUR) and perform hashing at random points in the graph.
	- Output only after successful program termination.

FUTURE:
- Complex types
- Type-correct expressions
- Some substitutions allowed
	- int, char, short, long, float, double - all interchangeable to some degree (use appropriate casts)
	- array <--> pointer
	- pointer promotion (ie: passing the pointer to a local var, or droping the pointer to pass by value)
- Memory Allocation & Manipulation?
	- Choose random functions to perform allocations
	- Choose random children/ancestors to perform deallocations
	- Work from leaves to root
	- If node uses pointer or array, it is potential heap store allocated. 
*/

#include <ostream>
#include <sstream>

#include "Common.h"

#include "Block.h"
#include "CGContext.h"
#include "CGOptions.h"
#include "Effect.h"
#include "Expression.h"
#include "Function.h"
#include "FunctionInvocation.h"
#include "Statement.h"
#include "Type.h"
#include "Variable.h"

#include "platform.h"
#include "random.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Globals

// Program seed - allow user to regenerate the same program on different
// platforms.
static char seed0[] = "0707ccedaad605aa7606f9b0525c3ccc5983cfb8c5c46225aa25d74c11bb09f6762c890beeabea000000005a3e53034a380f1a9c7ad748e050fb58248bb6375dbf8352807da02ed9756a8104";
static char *g_Seed = seed0;

// ----------------------------------------------------------------------------
static void
OutputHeader(ostream &out, int argc, char *argv[])
{
	out << "/*" << endl;
	out << " * This is a RANDOMLY GENERATED PROGRAM." << endl;
	out << " *" << endl;
	out << " * Generator: " << PACKAGE_STRING << endl;
	out << " * Options:  ";
	if (argc <= 1) {
		out << " (none)";
	} else {
		for (int i = 1; i < argc; ++i) {
			out << " " << argv[i];
		}
	}
	out << endl;
	out << " * Seed:      " << g_Seed << endl;
	out << " */" << endl;
	out << endl;

    out << "function nonzero(n) {" << endl;
	out << "  if (n == 0) return 1;" << endl;
	out << "  return n; }" <<endl;
	out << endl;

	if (CGOptions::depth_protect()) {
		out << "var MAX_DEPTH = 5;" << endl;
		out << "var DEPTH = 0;" << endl;
		out << endl;
	}
}

// ----------------------------------------------------------------------------
static void
OutputMain(ostream &out)
{
	CGContext cg_context(GetFirstFunction() /* BOGUS -- not in first func. */,
						 0,
						 0,
						 Effect::get_empty_effect(),
						 0);
	
	FunctionInvocation *invoke =
		FunctionInvocation::make_random(GetFirstFunction(), cg_context);
	out << endl << endl
		<< "/* ---------------------------------------- */" << endl;
	out << "    /* Call the first function */" << endl;
	out << "    ";
	invoke->Output(out);
	out << ";" << endl;
	if (CGOptions::compute_hash()) {
		HashGlobalVariables(out);
	}
	if (CGOptions::print_hash()) {
		out << "    printf(\"%d\\n\", context);" << endl;
	}
}

// ----------------------------------------------------------------------------
int
main(int argc, char **argv)
{
	if (argc == 2)
		g_Seed = argv[1];
	seedrand(g_Seed);

	CGOptions::compute_hash(true);
	CGOptions::print_hash(true);
	CGOptions::depth_protect(true);
	CGOptions::max_funcs(CGOPTIONS_DEFAULT_MAX_FUNCS);
	CGOptions::max_stmt_depth(CGOPTIONS_DEFAULT_MAX_STMT_DEPTH);

	if (CGOptions::print_hash()) {
		CGOptions::compute_hash(true);
	}
	if (CGOptions::max_funcs() < 1) {
		cerr << argv[0]
			 << ": error: max-funcs must be at least 1" << endl;
		return 1;
	}
	if (CGOptions::max_stmt_depth() < 1) {
		cerr << argv[0]
			 << ": error: max-stmt-depth must be at least 1" << endl;
		return 1;
	}

	////

	// Create a list of types that will be used by this program
	GenerateAllTypes();
	GenerateFunctions();

	// Output the random program..
//	ofstream file;			// Output file
//	file.open("junk.cpp");
//	ostream &out = file;

	std::ostringstream outbuf;
	ostream &out = outbuf; // cout;
	OutputHeader(out, argc, argv);
	OutputGlobalVariables(out);
	OutputFunctions(out);
	OutputMain(out);
	cout << outbuf.str();
	
//	file.close();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
