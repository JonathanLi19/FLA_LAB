#include "common.h"
struct input
{
    string state; //输入的状态
    string symbols; //在当前n个磁带上的symbol
};

struct output
{
    string state; //输出的状态
    string symbols; //输出的n个symbol
    string directions; //输出的n个head的移动方向
};

class TuringMachine
{
    private:
        //define
        vector<string> states;//Q
        
        vector<char> input_char;//S
        vector<char> tape_char;//G
        string start_state; //q0
        char blank; //B
        vector<string> final_states;//F
        int nTape; //N

        vector<list<char>> tapes;
        vector<list<char>::iterator> heads;
        map<input, output> transition;
        //argc
        bool verbose;
        string filename;
    public:
        TuringMachine (string file_name, bool verbose);
        string run(string input);
        void print_error(vector<string> message, int err_num);
        string clear_annotation(string s);
        void check_input(string input);
};