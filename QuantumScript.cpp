#include "QuantumScript.h"

const char MULTIPLICATION = '*';
const char ADDITION = '+';
const char DIVISION = '/';
const char SUBTRACTION = '-';
const char MODULUS = '%';
const char IFGREATER = '>';
const char IFLESS = '<';
const char ELSE = ':';
const char ENDIF = '|';
const char PRINTVALUE = '@';
const char PRINTCHAR = '$';
const char ASSIGNMENT = '=';
const char COMMENT = '#';
const char WHITESPACE = ' ';

QuantumScript::QuantumScript(string code, vector<ll> &argv) : stackDepth_{0}
{
    istringstream iss(code);

    string line;
    while (std::getline(iss, line))
    {
        functions_.push_back(line);
    }
    removeCharacters();
    setUpVariablesMain(argv);
    setUpFunctions(code);
}
void QuantumScript::run()
{
    auto error = mainLoop(0);

    if (error != 0)
    {
        pn("Error returned with value " << error);
    }
}

ll QuantumScript::mainLoop(ll fnIdx)
{

    string line = functions_[fnIdx];
    ll lnIdx = functions_[fnIdx].find(" ") + 1;

    // each loop we first have to see if a command is being used *+-... or a function being called
    while (lnIdx < line.find_last_of('.'))
    {
        auto val = getValue(fnIdx, lnIdx);
        lnIdx = val.second;
    }

    ll val = 0;
    fo(i, line.find_last_of('.') + 1, line.length())
    {
        val = valueMap_[stackDepth_][line.substr(i, varSize_)];
        break;
    }
    valueMap_.pop_back();
    return val;
}

pair<ll, ll> QuantumScript::getValue(ll fnIdx, ll lnIdx)
{
    string line = functions_[fnIdx];

    if (isalpha(line[lnIdx])) // it must be a function/value
    {
        string str = line.substr(lnIdx, varSize_);
        if (functionMap_.count(str) == 0 && valueMap_[stackDepth_].count(str) != 0)
        {
            return {valueMap_[stackDepth_][str], lnIdx + varSize_};
        }
        else if (functionMap_.count(str) != 0 && valueMap_[stackDepth_].count(str) == 0)
        {
            return callFunction(str, fnIdx, lnIdx + varSize_);
        }
        else
        {

            /** THROW ERROR */
            cout << "ERROR: the given string, '" + str + "' is not a function nor a variable";
            return {};
        }
    }
    else // it must be a command symbol
    {
        char command = line[lnIdx];
        return operatorvalue(line[lnIdx], fnIdx, lnIdx + 1);
    }
}

string QuantumScript::getVariable(ll fnIdx, ll lnIdx)
{
    string line = functions_[fnIdx];

    if (isalpha(line[lnIdx])) // it must be a value
    {
        string str = line.substr(lnIdx, varSize_);
        if (functionMap_.count(str) == 0 && valueMap_[stackDepth_].count(str) != 0)
        {
            return str;
        }
    }

    /** THROW ERROR */
    cout << "ERROR: the given string, is not a variable";
    return {};
}

pair<ll, ll> QuantumScript::operatorvalue(char command, ll fnIdx, ll lnIdx)
{
    switch (command)
    {
    case MULTIPLICATION:
    case ADDITION:
    case DIVISION:
    case SUBTRACTION:
    case MODULUS:
    {
        string target = getVariable(fnIdx, lnIdx);
        lnIdx += varSize_;
        auto val1 = getValue(fnIdx, lnIdx);
        lnIdx = val1.second;
        auto val2 = getValue(fnIdx, lnIdx);
        lnIdx = val2.second;

        ll val = 0;

        switch (command)
        {
        case MULTIPLICATION:
            val = val1.first * val2.first;
            break;
        case ADDITION:
            val = val1.first + val2.first;
            break;
        case DIVISION:
            val = val1.first / val2.first;
            break;
        case SUBTRACTION:
            val = val1.first - val2.first;
            break;
        case MODULUS:
            val = val1.first % val2.first;
            break;
        }

        valueMap_[stackDepth_][target] = val;

        return {val, lnIdx};
    }
    case PRINTVALUE:
    {
        auto val = getValue(fnIdx, lnIdx);
        p(val.first);
        return {val.first, val.second};
    }
    case PRINTCHAR:
    {
        auto val = getValue(fnIdx, lnIdx);
        p((char)val.first);
        return {val.first, val.second};
    }
    case IFGREATER:
    case IFLESS:
    {
        auto val = getValue(fnIdx, lnIdx);
        lnIdx = val.second;

        ll endIf = -1;
        ll hasElse = -1;

        ll startCount = 0;                 // for tracking if we are in a sub-if statement, so we go to the correct end
        ll subIfStart = -1, subIfEnd = -1; // where the sub-if statements start and end
        fo(i, lnIdx, functions_[fnIdx].find_last_of('.'))
        {
            char character = functions_[fnIdx][i];
            if (character == IFGREATER || character == IFLESS)
            {
                startCount++;
                if (subIfStart == -1)
                    subIfStart = i;
            }
            else if (character == ENDIF)
            {
                if (startCount > 0)
                {
                    startCount--;
                    if (subIfEnd == -1)
                        subIfEnd = i + 1;
                }
                else
                {
                    endIf = i + 1;
                    break;
                }
            }
            else if (character == ELSE)
            {
                if (startCount == 0) // we are in the original if statement
                {
                    if (hasElse != -1) // there are 2 else operators in one if statement
                    {
                        /** THROW ERROR */
                        cout << "ERROR: Too many : symbols in one if statement";
                        return {};
                    }
                    hasElse = i + 1;
                }
            }
        }
        if (startCount != 0 || endIf == -1)
        {
            /** THROW ERROR */
            cout << (startCount >= 0 ? ("ERROR: If statement without a corresponding | symbol") : ("ERROR: Too many | symbols without corresponding if statements"));
            return {};
        }

        ll on = 0, end = 0;
        if ((command == IFGREATER && val.first > 0) || (command == IFLESS && val.first <= 0))
        {
            on = lnIdx;
            end = (hasElse != -1 ? hasElse - 1 : endIf - 1);
        }
        else
        {
            on = (hasElse != -1 ? hasElse : endIf);
            end = endIf - 1;
        }

        val = {0, 0};
        while (on < end)
        {
            val = getValue(fnIdx, on);
            on = val.second;
        }
        if (on != end)
        {
            /** THROW ERROR */
            cout << "ERROR: If statement ended in an impossible location";
            return {};
        }

        return {val.first, endIf};
    }

    break;
    case ASSIGNMENT:
    {
        string target = getVariable(fnIdx, lnIdx);
        lnIdx += varSize_;
        auto val = getValue(fnIdx, lnIdx);
        valueMap_[stackDepth_][target] = val.first;

        return val;
    }
    case ENDIF: // this is handled by the if statements, so it shouldn't appear on its own
    case ELSE:  // this is handled by the if statements, so it shouldn't appear on its own
    default:
        /** THROW ERROR */
        pn("ERROR: Unknown command");
        exit(1);
        return {};

        break;
    }
}

pair<ll, ll> QuantumScript::callFunction(string newFn, ll fnIdx, ll lnIdx)
{
    int numInputs = functionMap_[newFn].second;
    vector<ll> vals;

    while (vals.size() != numInputs)
    {
        auto next = getValue(fnIdx, lnIdx);
        vals.push_back(next.first);
        lnIdx = next.second;
    }

    // ** Find vals based on how many the function needs to take
    stackDepth_++;
    setUpVariables(newFn, vals);
    auto val = mainLoop(functionMap_[newFn].first);
    stackDepth_--;

    return {val, lnIdx};
}

void QuantumScript::removeCharacters()
{
    fo(i, 0, functions_.size())
    {
        fo(ch, 0, functions_[i].length())
        {
            auto character = functions_[i][ch];
            if (character == COMMENT)
            {
                functions_[i] = functions_[i].substr(0, ch);
                continue;
            }
            else if (character == WHITESPACE)
            {
                functions_[i] = functions_[i].substr(0, ch) + functions_[i].substr(ch + 1);
                ch--;
            }
        }
    }
}

void QuantumScript::setUpFunctions(string code)
{
    fo(i, 1, functions_.size())
    {
        auto line = functions_[i];
        string name = line.substr(0, varSize_);
        line = line.substr(varSize_);
        ll numInputs = line.find(".") / varSize_;
        functionMap_[name] = {functionMap_.size() + 1, numInputs};

        ll tmp = line.find(".");
        line[tmp] = ' ';
        line[line.find(".")] = ' ';
        line[tmp] = '.';
        functions_[i] = line;
    }
}

void QuantumScript::setUpVariablesMain(vector<ll> &argv)
{
    auto number = findNum(0, 0);
    varSize_ = number.first;
    functions_[0] = functions_[0].substr(number.second);

    ll argc = functions_[0].find(".") / varSize_;
    argv.resize(argc); // unused values will be set to zero, and any excess inputs will be ignored

    valueMap_.resize(1);
    fo(i, 0, argc)
    {
        valueMap_[0][functions_[0].substr(i * varSize_, varSize_)] = argv[i];
    }
    // truncate the input string so that it only contains from the second full stop onwards. ie:
    auto line = functions_[0];
    ll tmp = line.find(".");
    line[tmp] = ' ';
    line[line.find(".")] = ' ';
    line[tmp] = '.';

    functions_[0] = line;
    setDeclaredVariables(0);
}

void QuantumScript::setUpVariables(string function, vector<ll> &functionCarry)
{
    valueMap_.push_back({});

    auto numVars = functionMap_[function].second;
    auto idx = functionMap_[function].first;
    auto line = functions_[idx];

    fo(i, 0, numVars)
    {
        valueMap_[stackDepth_][line.substr(i * varSize_, varSize_)] = functionCarry[i];
    }

    setDeclaredVariables(idx);
}

void QuantumScript::setDeclaredVariables(ll idx)
{
    string line = functions_[idx];
    int idxOn = line.find('.') + 1;

    while (idxOn < line.find(' '))
    {
        string var = line.substr(idxOn, varSize_);
        auto num = findNum(idx, idxOn + varSize_);
        ll val = num.first;
        idxOn = num.second;
        valueMap_[stackDepth_][var] = val;
    }
}

pair<ll, ll> QuantumScript::findNum(ll functionIndex, ll stringIndex)
{
    ll val = 0;
    bool negative = functions_[functionIndex][stringIndex] == '-';
    if (negative)
        stringIndex++;
    while (true)
    {
        char on = functions_[functionIndex][stringIndex];
        if (!isnumber(on))
            break;
        val = 10 * val + on - '0';
        stringIndex++;
    }
    return {val * (negative ? -1 : 1), stringIndex};
}

/*
g++ -std=c++17 *.cpp -o quantumscript && ./quantumscript file.qts 1 2 3
*/