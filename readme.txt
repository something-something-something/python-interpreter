1.

Group Members:
Eric Krebs 1335559 cosc2979
Universon Van 1675390 cosc3003

2.
needs flex and bison

To build run make
To build again first run make clean

to run: ./mypython filename.py

3.Limitations:
A. return statments inside of if and else statments won't work

Example:
#This will return 4 instead of 2
def f()
   if 1==1:
      return 2
   return 4



This works fine:
#This returns 2
def f()
   x=4
   if 1==1:
      x=2
   return x


B. ./mypython does not work in interactive  mode

Everything else should work.


4.

Flex and bison are used to parse the python file Bison. Bison produces a vector containing parse/ast trees for each line.
buildParseTree is then called on the vector to produce a single tree that takes ino account indentation and child statments of if/else and functions
A EvalMachine is created which is responsible for evaluating the program and keeping track of variable values and function definitions.

The EvalMachine.evaluate is called on the root of the tree which reccursivily calls evaluate() on the children (if it makes sense for the children to have children)

Evaluate is responsible for printing, variable assignment, function calls if else etc.

Variables are kept track of in the vector variables of EvalMachine. It keeps track of where the name,value and scope of the varaible. Scope is either a pointer to the root of the program tree or the function where it was created. When evaluate comes to a variable assingment if the variable does not exist in variables it gets added to the variables vector(with the apropriate scope). If it does exist the value is updated (if it is in the right scope)

Functions work in a similar way to variables. When a function is defined the functions vector is updated with a pointer to the location of the function in the tree. When a function is called the location of the tree is retrived and evaluate is run on the function statements. 

