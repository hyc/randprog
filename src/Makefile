OBJS = Block.o CGContext.o CGOptions.o Constant.o Effect.o ExpressionFuncall.o Expression.o \
	ExpressionVariable.o FunctionInvocation.o Function.o random.o RandomProgramGenerator.o \
	StatementAssign.o StatementExpr.o StatementFor.o StatementIf.o Statement.o StatementReturn.o \
	Type.o util.o Variable.o crypto/c_keccak.o SFMT/SFMT.o

PROG = randprog

all: $(PROG)

randprog:	$(OBJS)
	$(CXX) -o $@ $(OBJS)

crypto/c_keccak.o: crypto/c_keccak.c crypto/c_keccak.h

SFMT/SFMT.o: SFMT/SFMT.c

clean:
	-rm $(OBJS) $(PROG)
