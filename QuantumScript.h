#ifndef QUANTUMSCRIPT
#define QUANTUMSCRIPT

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#define p(x) cout << (x);
#define pn(x) cout << x << endl;
#define del(x) cout << x << endl;
#define ll long long
#define fo(i, a, b) for (int i = a; i < (int)b; i++)
#define rfo(i, a, b) for (int i = a - 1; i >= b; i--)

using namespace std;

// fabi.crdo1.+cab-dio=r>dfbcd.r
class QuantumScript
{

public:
    /**
     * @brief Construct a new Quantum Script object
     *
     * @param code the string representing the user's input
     * @param argv the command line arguments given when running QuantumScript
     */
    QuantumScript(string code, vector<ll> &argv);

    /**
     * @brief runs the code and produces the expected output
     *
     */
    void run();

private:
    /**
     * @brief Set the function names and load the functions
     *
     * @param code the string representing the user's input
     */
    void setUpFunctions(string code);

    /**
     * @brief Set up the variables in the main loop
     *
     * @param argv the command line values used in
     */
    void setUpVariablesMain(vector<ll> &argv);

    /**
     * @brief Set the input and declared variables for a line of code, called everytime that function is called
     *
     * @param index the index of the function being called
     * @param functionCarry the values passed into the function
     */
    void setUpVariables(string function, vector<ll> &functionCarry);

    /**
     * @brief Set the Declared Variables of a function
     *
     * @param index the index of the function
     * @param line the string of the function
     */
    void setDeclaredVariables(ll index);

    ll mainLoop(ll fnIdx);

    // returns the value for a variable, or the outcome for running a function
    // the first ll is the value, the second is the next char that wasnt used in this
    pair<ll, ll> getValue(ll fnIdx, ll lnIdx);

    // returns the next variable or throws an error
    string getVariable(ll fnIdx, ll lnIdx);

    pair<ll, ll> commandValue(char command, ll fnIdx, ll lnIdx);

    pair<ll, ll> callFunction(string newFn, ll fnIdx, ll lnIdx);

    // helper function used to find what a number is and where it ends in a string (it points to the char after the number)
    // returns a pair of val,index
    // should be called after every variable declaration, as it works if no numbers are given
    pair<ll, ll> findNum(ll functionIndex, ll stringIndex);

    // vector<ll> argv_;
    vector<string> functions_;              // index 0 is the main loop
    map<string, pair<ll, ll>> functionMap_; // returns the index of the function, as well as how many inputs it has
    vector<map<string, ll>> valueMap_;      // stores each variable at a layer of functions
    ll varSize_;                            // how many chars each variable name is
    ll numFunctions_;                       // how many functions there are, including the main line
    ll stackDepth_;                         // how deep in the stack the program is currently
    int checkMode_;                         // when >0, stops any printing from happening
};

#endif

/* ./quantumscript test.qts
g++ -std=c++17 *.cpp -o quantumscript
g++ -std=c++17 *.cpp -o quantumscript && ./quantumscript test.qts 1 2 3
*/

/*
 ** IDEAS:
The language holds two different code at any time, one at even indexes, one at odd
0123456
is equal to
0 2 4 6
 1 3 5

or maybe its done one command at a time, so you could have
ab.cd.de.fd
which is
ab de -> line 1. each of those could be a function or a number or a command
cd fd -> line 2

one side is used for the normal code, other is for variable declarations?
when one side is done, you can put Q to stop, and from that point on all the next chars are part of the other side




 ** CODE:

multiply * (3) -> takes 2 vars, which can be functions if needed, and the last one can only be a value. If the last one is a function, the output becomes its first input
add + (3)
minus - (3)
divide / (3)
modulo % (3)
separation . (u/d)
print char $ (1) -> also returns the value if you want to print and use it. Same with all functions
print number @ (1) -> also returns the value if you want to print and use it. Same with all functions
if > (2)
else | (1)
not if < (2)
assign = (2) -> you can assign an if, so the variable will be equal to the output of the if, or zero. The variable part cannot be a function or a command, only a variable

for some symbol, the first variable after the symbol is where it is returned to, the next one or two are the inputs
=ab means a=b
+abc means a=b+c


Actually, each line is a function, except for the first line
first 2 chars are function name. the next 2 chars will be the input name, until you get to a .
so
abcdef. is a function called ab which has 2 inputs, cd and ef
last 2 chars are always the returned
Then after the first dot we show variables that can be used in the function, we can .. if we dont want any more variables
if a number is put after the variable, it assigns values
After the last . it shows all the variables it returns

The first line also has 3 full stops, a.b.c.d, first is an int that gives how long the var names are. Then it gives the variables given by args, defaulted to zero if not given.
b is used to define any other variables, like a normal function
c is used for code, like a normal function
d is used to return, like normal, however returning a number from the main line is used for error control. no return, or a return of 0 is default, and non zero is an error. If you want to return zero, just end with the full stop

// ** print Hello World!
1.h72e101l108o111s32w87r114d100x33p10.$h$e$l$l$o$s$w$o$r$l$d$x$p.


// ** multiply 4 and 5 and print the result
2.ab4bc5.@aaabbc.           -> ab = 4, bc = 5, print(aa(ab,bc))
aaabcd..*abcdab.ab          -> ab = 4, cd = 5, ab = cd*ab = 20, return 20


// ** fibonacci. It defaults to printing 6 terms, but you can give an argument to change that if you want

1z.a8o1qs32n10.+zzo>z@mqoz|@mqoa|$n.            -> prints fibbonaci sequence, 6 terms by default but you can put it more using argz
mabi.t2.=bfab-tit.b                             -> they are separated by spaces and have a new line character. The smallest input it works with is 2
fabi.crdo1s32.+cab@b$s-dio=r>dfbcd|c|.r         -> uses recursion to replace loops


have a varlength feature in my compiler, allowing for single,double,triple,etc var name length, so you could potentially have infinite possible variables
first line starts with var length and then dot. That variable before the dot gets the value of any argument given to the code (only supports one argument)


Functions arent really needed here, but they are needed when using if statements, as the inside of the if must be a different function

aa
ab
ac
ad


if has the form iiabcd -> which means if (ab>0) {call cd}, if ab is a variable. if its a function, &abcdefgh 0> if(ab(cd,ef)>0) {call gh}. ! means if (x <= 0)

It can actually return multiple variables, if a function returns 2 and that result is loaded into a different function that takes one, it ignores the second result


variable declaration -> a number after the variable. If no number, it defaults to zero. If you are calling a function, however, you need to provide a value, you cant just leave it

spaces aren't used and are deleted as the program begins, so do something with the spaces? maybe can be used as like a signature?

order of functions doesnt matter, you can call a function above or below you

you can't define numbers as having negatove values, but you could take 1, miinus 2, and multiple that by a number to get a negative


>ab| means if(a>0){b}
>ab|c| means if (a>0){b}else{c}


TODO: make arrays?



a = 1, b = 1, rep = 4.
func(a, b, rep).        result:0, dec:0,one:1, add:0.   add = a+b, dec=rep-one, result = if(dec>0){func(b,add,dec)}else{add} return result

fabi.crdo.+cab-dio=c>dfbcd.c


 ** TODO:

add all features
add error messages, with their own error classes
add arrays somehow
remove all spaces from input and then maybe do something with them
change ll to a number type that can grow arbitrarily large
make it so that a function can return multiple values and all of them can be used for a function call. Do this by changing what callFunction returns, and so also changes getValue. The return in mainloop must also thus change
decide what to do about caps, or are caps different from their lowercase? or maybe have a special use?
make it so that you can have lots of code in if statements, and not just one function/symbol(@,+,-...)
make it use loops instead of recursion? c++ has a stack depth limit which limits my program
add comment ability, as after the first space nothing is read? lets you write stuff ithout it affecting the code

n output means it always returns zero

*/