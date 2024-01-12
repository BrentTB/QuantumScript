#include "QuantumScript.h"

using namespace std;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename.qts>" << std::endl;
        return 1;
    }

    const std::string filename(argv[1]);
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // SimpleLanguageInterpreter interpreter;
    // interpreter.execute(code);

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