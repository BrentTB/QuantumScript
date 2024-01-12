#include "QuantumScript.h"

QuantumScript::QuantumScript(string code, vector<ll> &argv) : stackDepth_{0}
{
    istringstream iss(code);

    string line;
    while (std::getline(iss, line))
    {
        functions_.push_back(line);
    }
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

    fo(i, line.find_last_of('.') + 1, line.length())
    {
        return valueMap_[stackDepth_][line.substr(i, varSize_)];
    }
    return 0;
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
    case '*':
    case '+':
    case '/':
    case '-':
    case '%':
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
        case '*':
            val = val1.first * val2.first;
            break;
        case '+':
            val = val1.first + val2.first;
            break;
        case '/':
            val = val1.first / val2.first;
            break;
        case '-':
            val = val1.first - val2.first;
            break;
        case '%':
            val = val1.first % val2.first;
            break;
        }

        valueMap_[stackDepth_][target] = val;

        return {val, lnIdx};
    }
    case '@':
    {
        auto val = getValue(fnIdx, lnIdx);
        p(val.first);
        return {val.first, val.second};
    }
    case '$':
    {
        auto val = getValue(fnIdx, lnIdx);
        p((char)val.first);
        return {val.first, val.second};
    }
    case '>':
    case '<':
    {
        auto val1 = getValue(fnIdx, lnIdx);
        lnIdx = val1.second;

        ll endIf = -1;
        ll hasElse = -1;
        fo(i, lnIdx, functions_[fnIdx].find_last_of('.'))
        {
            char character = functions_[fnIdx][i];
            if (character == '>' || character == '<')
                break;
            if (character == '|')
            {
                if (endIf == -1)
                {
                    endIf = i + 1;
                }
                else if (hasElse == -1)
                {
                    hasElse = i + 1;
                }
                else
                {
                    /** THROW ERROR */
                    cout << "ERROR: Too many '|' provided in if statement";
                    return {};
                }
            }
        }

        if ((command == '>' && val1.first > 0) || (command == '<' && val1.first <= 0))
        {

            auto val2 = getValue(fnIdx, lnIdx);
            if (hasElse != -1)
            {
                return {val2.first, hasElse}; // if it has an else, we want the index to be past that else
            }
            else
                return {val2.first, endIf};
        }
        else
        {
            if (hasElse != -1)
            {
                auto elseVal = getValue(fnIdx, endIf);
                return {elseVal.first, hasElse};
            }
            else
                return {0, endIf}; // if no else, return a zero at the index after the var ends
        }
    }

    break;
    case '=':
    {
        string target = getVariable(fnIdx, lnIdx);
        lnIdx += varSize_;
        auto val = getValue(fnIdx, lnIdx);
        valueMap_[stackDepth_][target] = val.first;

        return val;
    }
    case '|': // this is handled by the if statements, so it shouldn't appear on its own
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
    while (true)
    {
        char on = functions_[functionIndex][stringIndex];
        if (!isnumber(on))
            break;
        val = 10 * val + on - '0';
        stringIndex++;
    }
    return {val, stringIndex};
}

/*
g++ -std=c++17 *.cpp -o quantumscript && ./quantumscript test.qts 1 2 3
*/