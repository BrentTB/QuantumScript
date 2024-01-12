#include "QuantumScript.h"

using namespace std;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <filename.qts>" << endl;
        return 1;
    }

    const string filename(argv[1]);
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: File '" << filename << "' not found." << endl;
        return 1;
    }

    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    vector<ll> argvNew;
    for (int i = 2; i < argc; i++)
        argvNew.push_back(stoll(argv[i]));

    try
    {
        QuantumScript QM(code, argvNew);
        QM.run();
    }
    catch (exception &e)
    {
        pn("Invalid Code");
        pn(e.what());
    }

    return 0;
}