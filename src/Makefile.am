## -*- mode: Makefile.am -*-
##
## Copyright (c) 2008 The University of Utah
## All rights reserved.
##
## This file is part of `randprog', a random generator of C programs.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##
##   * Redistributions of source code must retain the above copyright notice,
##     this list of conditions and the following disclaimer.
##
##   * Redistributions in binary form must reproduce the above copyright
##     notice, this list of conditions and the following disclaimer in the
##     documentation and/or other materials provided with the distribution.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
## LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
## CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
## SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.

###############################################################################

bin_PROGRAMS = randprog

# XXX --- yuck!
randprog_SOURCES = \
	Block.cpp \
	Block.h \
	CGContext.cpp \
	CGContext.h \
	CGOptions.cpp \
	CGOptions.h \
	Common.h \
	Constant.cpp \
	Constant.h \
	Effect.cpp \
	Effect.h \
	Expression.cpp \
	Expression.h \
	ExpressionFuncall.cpp \
	ExpressionFuncall.h \
	ExpressionVariable.cpp \
	ExpressionVariable.h \
	Function.cpp \
	Function.h \
	FunctionInvocation.cpp \
	FunctionInvocation.h \
	RandomProgramGenerator.cpp \
	Statement.cpp \
	Statement.h \
	StatementAssign.cpp \
	StatementAssign.h \
	StatementExpr.cpp \
	StatementExpr.h \
	StatementFor.cpp \
	StatementFor.h \
	StatementIf.cpp \
	StatementIf.h \
	StatementReturn.cpp \
	StatementReturn.h \
	Type.cpp \
	Type.h \
	Variable.cpp \
	Variable.h \
	platform.cpp \
	platform.h \
	random.cpp \
	random.h \
	util.cpp \
	util.h

randprog_CPPFLAGS = $(BOOST_CPPFLAGS)

randprog_LDFLAGS = $(BOOST_LDFLAGS)
randprog_LDADD   = $(BOOST_PROGRAM_OPTIONS_LIB)

randprog_includedir = \
	$(includedir)/randprog-$(VERSION)
randprog_include_HEADERS = \
	platform_avr.h \
	platform_generic.h \
	random_runtime.h \
	volatile_runtime.c \
	volatile_runtime.h

## XXX should we put `volatile_runtime.c' in _DATA?

###############################################################################

## End of file.
