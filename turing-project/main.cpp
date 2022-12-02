#include "common.h"
#include "turing_machine.h"
const char* HELP_OUTPUT = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";
const char* UNKNOWN_OPTION = "unknown option:";
const char* UNKNOWN_COMMAND = "unknown command:";
const char *END = "==================== END ====================";
int main(int argc, char* argv[])
{
    string tm_file = "";
    string input_str = "";
    bool verbose = false;
    if(argc == 1)
    {
        return 1;
    }
    else
    {
        int i = 1;
        while(i < argc)
        {
            string arg = argv[i];
            if(arg[0] == '-') //option
            {
                if(arg == "-h"||arg == "--help")
                {
                    cout << HELP_OUTPUT << endl;
                    break;
                }
                else if(arg == "-v"||arg == "--verbose")
                    verbose = true;
                else
                {
                    cerr << UNKNOWN_OPTION << arg << endl;
                    exit(0);
                }
            }
            else//command
            {
                if(tm_file=="")
                {
                    if(arg.length() > 3 && arg.substr(arg.length()-3,arg.length())==".tm")
                        tm_file = arg;
                    else
                    {
                        cerr << UNKNOWN_COMMAND << arg << endl;
                        exit(0);
                    }
                }
                else if(input_str=="")
                {
                    input_str = arg;
                }
                else
                {
                    cerr << UNKNOWN_COMMAND << arg << endl;
                    exit(0);
                }
            }
            i++;
        }
    }
    
    TuringMachine tm(tm_file, verbose);
    string res = tm.run(input_str);
    if(verbose)
        cout << "Result: ";
    cout << res << endl;
    if(verbose)
        cout << END << endl;
    return 0;
}