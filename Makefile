CC=g++
mypyton: lex.yy.c python.tab.c
	$(CC) -std=c++11 -g lex.yy.c python.tab.c  ProgramTree.cpp EvalMachine.cpp main.cpp -o mypython
lex.yy.c: python.tab.h
	flex python.l
python.tab.c python.tab.h:
	bison -d python.y
clean:
	rm lex.yy.cc python.tab.cc python.tab.hh interp stack.hh lex.yy.c python.tab.c python.tab.h mypython