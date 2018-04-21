OBJS = Block.o CGContext.o CGOptions.o Constant.o Effect.o ExpressionFuncall.o Expression.o \
	ExpressionVariable.o FunctionInvocation.o Function.o random.o RandomProgramGenerator.o \
	StatementAssign.o StatementExpr.o StatementFor.o StatementIf.o Statement.o StatementReturn.o \
	Type.o util.o Variable.o SFMT/SFMT.o RunJS.o Hash.o Pool.o \
	crypto/c_blake256.o crypto/c_groestl.o crypto/c_jh.o crypto/c_keccak.o crypto/c_skein.o

PROG = randprog

V8DIR=/home/software/v8
V8BIN=$(V8DIR)/out.gn/x64.release

OPT=-g
CFLAGS=$(OPT)
CXXFLAGS=$(OPT) -std=c++11

all: $(PROG)

randprog:	$(OBJS)
	$(CXX) -o $@ $(OBJS) $(V8BIN)/libv8.so $(V8BIN)/libv8_libplatform.so $(V8BIN)/libicuuc.so

crypto/c_blake256.o: crypto/c_blake256.c crypto/c_blake256.h
crypto/c_groestl.o: crypto/c_groestl.c crypto/c_groestl.h
crypto/c_jh.o: crypto/c_jh.c crypto/c_jh.h
crypto/c_keccak.o: crypto/c_keccak.c crypto/c_keccak.h
crypto/c_skein.o: crypto/c_skein.c crypto/c_skein.h

SFMT/SFMT.o: SFMT/SFMT.c

RunJS.o: RunJS.cpp
	$(CXX) $(CXXFLAGS) -I$(V8DIR)/include -c -o $@ $<

clean:
	-rm $(OBJS) $(PROG)
