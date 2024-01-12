# QuantumScript

QuantumScript is an interpreted programming language created in c++. It allows for the use of variables, math, control statements, and recursion.
While there is no dedicated looping structure, loops can be created through recursion.

# Running QuantumScript (.qts) files [MAC]

- Create the interpreter with c++, such as with

```bash
g++ -std=c++17 *.cpp -o quantumscript
```

- Run the .qts file with

```bash
./quantumscript file.qts 1 2 3
```

- or run

```bash
g++ -std=c++17 *.cpp -o quantumscript && ./quantumscript file.qts 1 2 3
```

_in the two examples above, three command line arguments have been passed in_

## Language Structure

The first line of the code is its main line, and every other line is a function.
Every line has four components, separated by periods '.'

### Main Line Structure

The main line starts with a number, which tells the interpreter how many characters each variable name is, called the 'Variable Size'
The characters until the first period are any variables used to hold command line values, which default to zero if not supplied with a value at run time. if too many values are supplied, the excess are ignored

After the first period, the rest of the variables are defined. Variables can have numbers after them, which is the value they hold as the program starts, and is set to zero if no value is supplied

After the second period, the code to be run is written. This includes any operator use and function calls.

After the last period, the return value is given. This can be left empty or can have a variable. For the main line, a return value is used to represent an error, with 0 meaning there is no error, and any other value indicating an error.

_Note: A non-zero value can be returned even if the progra works as expected, if the user desires, it only adds as a checking mechanism_

```bash
2abcc.qw4er.@qw.ab
```

- This line means that variables are 2 characters long. 2 command line values are defined, 'ab' and 'cc'.
- Two more variables are defined, 'qw' which has a value of 4, and 'er' which has a value of zero
- This program will only print the value of qw, ie: 4
- the program returns ab, and so if a non-zero value is supplied through the command line for 'ab', it will return an error

### Function Structure

Like the main line, functions have four components.

The first part lists the function name, with the same length as the number given to the main line. It then defines variables until the first period.
These variables are the function's inputs. ie: for a function that has three variables, the function must also be called with three variables.(a function can have 0 or more inputs)

The second and third component are the same as the main line, representing variables declared in the function and the code to be run.

The last component is the value to be returned when the function call ends. This value is returned to where the fuction call takes place.

Functions can be defined in any order, and thus you can call a function that appears above or below the currrent location in the input text.

## Variables

Variables and functions can be defined with any lowercase or uppercase letters. Variables and functions must both be the same length as the variable size supplied to the main line.
Variables only have the scope they are defined in, and thus variables can have the same name if they are defined in different functions.
However, as functions are available anywhere, a variable cannot share a name with a function.

```
1.abc..
dab.c..
e.z..
```

- this is valid naming, as no variable or function shares a scope with another variable or function of the same name

_the main line defined three variables 'a', 'b' and 'c'. The function named 'd' also defined three variables, with the same names. The function 'e' defined a variable called 'z'_

```
1abc.cde..
z.de..
oy.o..
zfv.h..
```

this is an invalid naming scheme, as:

- two variables in the same function (same scope) have the same name, 'c'
- two functions have the same name, 'z'
- a function and a variable have the same name, 'o'

## Operators

Operators are written first, and then the values they work on are listed after.

All operators return the value they represent

```
+za+abc
```

For a variable size of 1, The line above means the same as z = a + (a=b+c).
As operators run from left to right, this program ends with z having the value of z = a+b+c, and not z = (b+c)+(b+c).
As a consequency for this, the second 'a' could be any variable and it would not change the value z recieves.

However, if the line were '+za+zbc', this would mean z = a + (z=b+c), resulting in z being defined twice in a row.
However, as the second addition must be completed for the first addition to complete, this will first give z the value of b+c, and then give z the value of a+b+c

### Math Operators

- \* Multiplication
- \+ Addition
- / Division
- \- Subtraction
- % Modulo

Math operators take three variables, one to assign to and two for the values.

```
+abc
```

- For a variable size of 1, this line means a = b+c

```
/abcdefghi
```

- For a variable size of 3, this line means abc = def/ghi

### Output Operators

- @ Output variable value
- $ Output ascii character of the value

Output operators only take one variable and output its value, either the number it holds or its ascii value

```
1.n66.@66$66.   # This outputs '66B', as the variable n is set to 66 and B has the ascii value of 66
```

### Control Structure

- \> if input \> 0
- \< if input \<= 0
- | used to separate statements and to provide an else

Control statements take two variables by default, but can take three if a second '|' is used as an else

```
>ab|    # This line means if (a > 0) then call (b)
<cd|e|  # This line means if (c <= 0) then call (d) else call (e)
```

An if statement must have at least one '|' operator to be valid.
Note: Only one function or assignment can be called in an if statement (right now)

This means that

```
1.a5.>aba|.
bz..@z.
```

and

```
1.a5.>ababa|.
bz..@z.
```

Will both only print the number '5' once. The second 'ba' in '>ababa|' is never read.
Thus, if you want multiple things to happen in an if statement, they must be put in a separate function and called from the if statement.

_this code means: define var a=5. if a >0, call function b with input a. Function b takes one input, defined z, prints its value, and returns 0_

### Assignment

- = Assignment

Assignment takes two values.

```
=ab   # For a variable size of 1, this line means a = b
```

Assignment is equivalent to adding or subtracting 0 from a number, or multiplying or dividing by 1, and thus variables can be assigned in multiple ways

## Calling Functions

A function is called by putting its name followed by the number of values that must be passed into it

_Note: the values passed into a function can contain another function, and that functions output will become one of the new function's variables_

```
1.a1b2c3d4e5.zaaa z qabc qede qaaa.  # the spaces were added for clarity, the interpreter ignores spaces
zabc..@a@b@c.b
qabc..@c@b@a.b
```

In the code above, the functions 'z' and 'q' take in three inputs and print them forward or backwards, and then both return the middle value.
The function 'z' is called twice in the main line, first with three values defined in the main line.
It is then called, with all three of its values being returned from the 'q' function.

_Note the first line is equaivalent to: z(a,a,a) followed by z(q(a,b,c), q(e,d,e), q(a,a,a))_

## Comments

Anything after a '#' symbol in a line will not be read, and thus can be used after the function to write comments. Spaces are also automatically removed, so as many spaces as desired can be used

## Examples

### Hello World!

```
1.h72e101l108o111s32w87r114d100x33p10.$h$e$l$l$o$s$w$o$r$l$d$x$p.
```

This code stores the ascii values for the letters in 'Hello World!' and then prints them

### Multiply Three Numbers

```
2in.ab4bc5.@aaabbcin.
aaxbcdqw..*xbcdxb*xbxbqw.xb
```

This code multiplies 4, 5, and the value given by the user. if no value is given it outputs zero, as the defaut variable value is 0

### Greatest Common Divisor and Lowest Common Multiple

```
3 ina inb. spc32 end10.@gcd ina inb $spc @lcm ina inb $end.  # spacing has been added for readability
gcd ina inb.. =inb <inb ina|gcd inb %ina ina inb|.inb        # this is equivalent to the following c++ code: int gcd(int a, int b) { return b==0 ? a: gcd(b, a%b);}
lcm ina inb.var. *var ina inb /var var gcd ina inb.var       # this is equivalent to the following c++ code: int lcm(int a, int b) { return a*b /gcd(a,b);}
```

This code takes in two command line variables and prints their greatest common divisor and their lowest common multiple

### Fibonacci

```
1 z.a8 o1 q s32 n10. +zzo >z@mqoz|@mqoa| $n.  # spaces are added for clarity
m abi.t2. =bfab -tit .b
f abi. c r d o1 s32. +cab @b $s -dio =r>dfbcd|c| .r
```

This code prints the fibbonaci sequence with as many terms as the user gives with the command line. If no value is supplied, it defaults to 6 terms.
Recursion is used with a variable that decrements itself each iteration in order to produce the correct amount of terms.
The code separates each term with a space (ascii of 32) and ends with a newline character (ascii of 10).

_This code does not work if the user inputs the number one, and for any value 0 or less, it will print the default 6 terms_

```
1zc.a8o1qs32n10v.+zzo=v>z@mqoz|@mqoa|$n-ccv.c
mabi.t2.=bfab-tit.b
fabi.crdo1s32.+cab@b$s-dio=r>dfbcd|c|.r
```

This code is similar to above, but takes in two values, the number of terms and the correct value after that many terms.
If the program's output is not the same as the calculated value, it will display an error.

### Prime Checker

```
1p..>xpT|F|.                             # a result of 1 means true, zero means false
F.N78o111t116s32.$N$o$t$sT.              # prints the string 'Not' and calls the next print function
T.P80r114i105m109e101n10.$P$r$i$m$e$n.   # prints the string 'Prime'
xp.o1z0g.-gpo=p<gz|qpo|.p                # if p is less than or equal to one, return zero, else call prime helper function, 'q'
ypi.tz0r.%tpi=r<tz|qpi|.r                # the prime function. if p%i != 0, return 0, or else call function 'q' to increment i
qpi.vo1t2as.+aio*vit-spv=v<so|ypa|.v     # if i > p/2, return one, else call the prime function with i incremented by 1
```

The code above takes in a number in the command line and prints 'Prime' or 'Not Prime' if the number is prime.

\_Note: Due to recursion limits, this code can only find primes until 3769

## Future Planned Additions

- add error messages, with their own error classes
- add arrays
- add ability for comments
- removes all whitespaces from text if they exist
- change ll to a number type that can grow arbitrarily large
- give functions the ability to return multiple values and for all of those to be used in a different function call
- make it so that you can have lots of code in if statements, and not just one function/symbol(@,+,-...)
- add support for negative (the code works with them but you can't define variables as negative right now)
- add support for an if defined in another if
