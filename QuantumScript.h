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

    /**
     * @brief runs the code for a specific line/function and returns the value defined by the function
     *
     * @param fnIdx the index of the function being used
     * @return ll
     */
    ll mainLoop(ll fnIdx);

    // returns the next variable or throws an error
    /**
     * @brief Get the next variable in a function at some point in its code
     *
     * @param fnIdx the index of the function being used
     * @param lnIdx the index in the function where the next variable should be found
     * @return string
     */
    string getVariable(ll fnIdx, ll lnIdx);

    // returns the value for a variable, or the outcome for running a function
    // the first ll is the value, the second is the next char that wasnt used in this

    /**
     * @brief Gets the value of the next variable/function/operator. Returns the value as well as the next unused index in the function
     *
     * @param fnIdx the index of the function being used
     * @param lnIdx the index in the function where the next value should be found
     * @return pair<ll, ll> - <value, line index>
     */
    pair<ll, ll> getValue(ll fnIdx, ll lnIdx);

    /**
     * @brief Gets the value of the next function. Returns the value as well as the next unused index in the function
     *
     * @param fnIdx the index of the function being used
     * @param lnIdx the index in the function where the next value should be found
     * @return pair<ll, ll> - <value, line index>
     */
    pair<ll, ll> callFunction(string newFn, ll fnIdx, ll lnIdx);

    /**
     * @brief Gets the value of the next operator. Returns the value as well as the next unused index in the function
     *
     * @param fnIdx the index of the function being used
     * @param lnIdx the index in the function where the next value should be found
     * @return pair<ll, ll> - <value, line index>
     */
    pair<ll, ll> operatorvalue(char command, ll fnIdx, ll lnIdx);

    // helper function used to find what a number is and where it ends in a string (it points to the char after the number)
    // returns a pair of val,index
    // should be called after every variable declaration, as it works if no numbers are given

    /**
     * @brief Gets the value of a number and the index where the number ends
     *
     * @param fnIdx the index of the function being used
     * @param lnIdx the index in the function where the next value should be found
     * @return pair<ll, ll> - <value, line index>
     */
    pair<ll, ll> findNum(ll functionIndex, ll stringIndex);

    vector<string> functions_;              // index 0 is the main loop
    map<string, pair<ll, ll>> functionMap_; // returns the index of the function, as well as how many inputs it has
    vector<map<string, ll>> valueMap_;      // stores each variable at a layer of functions
    ll varSize_;                            // how many chars each variable name is
    ll stackDepth_;                         // how deep in the stack the program is currently
};

#endif
