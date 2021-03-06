# ADscript

ADscript was created with having a direct connection to the host program in mind.  
The main feature of this language is the ability to use C/C++ functions and variables inside of it.  
Functions and variables on the host process can be called by ADscript once registered.  

## Documentation

The syntax for ADscript is simple and has little diversity.  
Every new line signifies a new function.  
Every space signifies a new argument for that function.  

All lines of the script can be abstracted as:  
`FunctionName arg0 arg1 ... argN`  

The language is "compiled" into a program structure.  
Compiling simply creates an array of instructions.  
Each of these instructions includes a function ptr and all the args for the function.  
The compiler will only check if the argument count is the same as what is expected, type safety is not guarenteed.  
All standard functions assume that everything is of type int.  

A comment can be made by starting a line with ';'  
The compiler will ignore that line (; MUST BE THE FIRST CHARACTER ON HE LINE!!!! NO SPACES BEFORE!!!)

### Registering

Function Registering must be done prior to compiling the program.  
You must first make a function before you can register it.  

All functions must be of type `void(*)(program*, arg*)`
(returns void, takes in the host program and array of byte arrays as args)  

`ADscript::registerFunction` can then be called with the following parameters:  
`std::string` ID (what will be used in the script)  
`unsigned int` ArgCount (expected amount of args)  
function pointer (what will actually happen)  

Variable Registering must be done prior to running the program.  
However, the script can be "compiled" prior to registering.  

`ADscript::registerVariable` can be called with the following parameters:  
`std::string` ID (what will be used in the script)  
`char*` address (the address of the variable as a `char*`)  

#### Optimization Registering

Optimization Registering allows for the user to specify optimizations by instruction.  
Doing so requires a function that takes in and modifies an instruction pointer.  
Optimizations will only replace existing instructions with more fitting ones.  
For example `ADD 0 foo foo` would be replaced with `NONE`.  
Optimizations should not change program execution but doing so is legal.  
Optimizations do not take into account previous or subsequent instructions. It works one at a time.

The full optimizer will remove NONE instructions and shorten the following:  
MARKers that have no associated JUMPs (removes)  
VARs that are actually constants (removes and reruns peephole)  
adjacent EQUALS and NEQUALS (all but last is removed)  
JUMPs that skip instructions (all skipped instructions and the JUMP is removed)  
instructions that don't do anything by themselves at the end are removed. (VAR, NONE, EQUAL, NEQUAL, END)

### Using Registered Stuff

Registered functions are accessed exactly like any other function.  

Registered variables can be accessed by preceeding the variable ID with $  
ex: $foo  

### Standard Functions

**VAR**  
Creates a new named variable on the stack  
Takes 2 arguments,  
variable id, initial value  

**DELETE**
Removes a named variable from the stack
Takes 1 argument,
name of variable
Do note that attemping to delete things that don't exist on the stack will do nothing (except waste time).

**END**  
Ends the program early  
Takes 0 arguments

**PRINT**  
Prints out a value   
Takes 1 argument,  
value

**SET**  
Sets a variable to a value  
Takes 2 arguments,  
location to store, value  
Do note that it is possible to store the result into a non-named value.  
Although the usage of this is unclear.

**ADD**  
Adds two values together  
Takes 3 arguments,  
value, value, location to store result  
Do note that it is possible to store the result into a non-named value.  
Although the usage of this is unclear.  

**SUB**  
Subtracts two values  
Takes 3 arguments,  
value, value, location to store result  
Do note that it is possible to store the result into a non-named value.  
Although the usage of this is unclear.

**MULT**  
Multiplies two values together  
Takes 3 arguments,  
value, value, location to store result  
Do note that it is possible to store the result into a non-named value.  
Although the usage of this is unclear.

**DIV**  
Divides two values  
Takes 3 arguments,  
value, value, location to store result  
Do note that it is possible to store the result into a non-named value.  
Although the usage of this is unclear.

**EQUAL**  
Compares two values for equality  
Takes 2 values,  
value, value  
The result is stored inside the programs comparisonRet bool.  
comparisonRet is used by other functions such as CJUMP.

**NEQUAL**  
Compares two values for non-equality  
Takes 2 values,  
value, value  
The result is stored inside the programs comparisonRet bool.  
comparisonRet is used by other functions such as CJUMP.

**LESS**  
Compares two values using less than  
Takes 2 values,  
value, value  
The result is stored inside the programs comparisonRet bool.  
comparisonRet is used by other functions such as CJUMP.

**GREAT**  
Compares two values using greater than  
Takes 2 values,  
value, value  
The result is stored inside the programs comparisonRet bool.  
comparisonRet is used by other functions such as CJUMP.

**JUMP**  
Jumps to a marker.  
Takes 1 value,  
instruction number

**CJUMP**  
Conditionally jumps to a marker.  
Takes 1 value,  
instruction number

**MARK**  
Marks positions to jump to.
Takes 1 value,
a marker name

**NONE**  
Does nothing.  
Takes 0 values,  
mainly used for optimizations