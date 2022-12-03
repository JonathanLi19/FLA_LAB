#include "common.h"
struct input_
{
    string state; //输入的状态
    string symbols; //在当前n个磁带上的symbol
    bool operator < (const input_ x) const {  
            return (state < x.state || (state == x.state && symbols < x.symbols));
        }
    bool operator==(const input_ b) const  
    {  
        return this->state == b.state && this->symbols == b.symbols;  
    } 
};

struct output_
{
    string state; //输出的状态
    string symbols; //输出的n个symbol
    string directions; //输出的n个head的移动方向
    bool operator < (const output_ x) const {  
            return (state < x.state || (state == x.state && symbols < x.symbols) || (state == x.state && symbols == x.symbols && directions < x.directions));
        }
    bool operator==(const output_ b) const  
    {  
        return this->state == b.state && this->symbols == b.symbols && this->directions == b.directions;  
    } 
};

struct Cell
{
    int index;
    char symbol;
    Cell(int i, char s)
    {
        index = i;
        symbol = s;
    }
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

        vector<list<Cell>> tapes;
        vector<list<Cell>::iterator> heads;
        map<input_, output_> transition;
        //argc
        bool verbose;
        string filename;
    public:
        TuringMachine (string file_name, bool verbose);
        string run(string input);
        void print_error(vector<string> message, int err_num);
        string clear_annotation(string s);
        void check_input(string input);
        void print_step(string cur_state, int step);
        string Get_result();
        void Write_Move(string old_symbols, string symbols, string directions);
};