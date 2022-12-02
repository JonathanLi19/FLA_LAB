#include "turing_machine.h"
const char *ERR = "==================== ERR ====================";
const char *END = "==================== END ====================";

void TuringMachine::print_error(vector<string> message, int err_num)
{
    switch(err_num)
    {
        case FILE_OPEN_ERROR:
        {
            cerr << "file open error" << endl;
            if (verbose)
            {
                cerr<< ERR << endl << "error: " << filename << "cannot open." << endl;
            }
        }
        case ILLEGAL_INPUT:
        {
            cerr << "illegal input" << endl;
            if(verbose)
            {
                cerr << ERR << endl << "error: '" << message[0] << "' was not declared in the set of input symbols" << endl << "Input: " << message[1] << endl << message[2] << endl;
            }
        }
    }
}

//去掉分号之后的内容
string TuringMachine::clear_annotation(string s)
{
    int pos = s.find(";");
    if(pos != s.npos)
    {
        s = s.erase(pos, (s.length() - 1) - pos + 1);
    }
    return s;
}

TuringMachine::TuringMachine(string file_name, bool v)
{
    filename = file_name;
    verbose = v;
    ifstream f;
    f.open(file_name);
    if(!f.is_open())
    {
        print_error({}, FILE_OPEN_ERROR);
        exit(FILE_OPEN_ERROR);
    }
    else
    {
        string s;
        while(getline(f, s))
        {
            s = clear_annotation(s);
            if(s == "")
                continue;
            else if(s[0] == '#') // definition
            {
                switch(s[1])
                {
                    case 'Q':
                    {
                        //第一个状态从s[6]这个字符开始
                        int cur = 6;
                        for(int i=6; i<s.length();i++)
                        {
                            if(s[i] == ',')
                            {
                                //从cur--(i-1)是state
                                string state = s.substr(cur, (i-1) - cur + 1);
                                states.push_back(state);
                                cur = i + 1;
                            }
                            if(i == s.length() - 1)
                            {
                                assert(s[i] == '}');
                                //从cur--(i-1)是state
                                string state = s.substr(cur, (i-1) - cur + 1);
                                states.push_back(state);
                            }
                        }
                    }
                    case 'S':
                    {
                        //第一个状态从s[6]这个字符开始
                        for(int i=6; i<s.length() - 1;i++)
                        {
                            if(s[i] == ',')
                            {
                                // do nothing
                            }
                            else
                            {
                                input_char.push_back(s[i]);
                            }
                        }
                    }
                    case 'G':
                    {
                        //第一个状态从s[6]这个字符开始
                        for(int i=6; i<s.length() - 1;i++)
                        {
                            if(s[i] == ',')
                            {
                                // do nothing
                            }
                            else
                            {
                                tape_char.push_back(s[i]);
                            }
                        }
                    }
                    case 'q':
                    {
                        //从6--strlen()-1
                        start_state = s.substr(6, (s.length() - 1) - 6 + 1);
                    }
                    case 'B':
                    {
                        blank = s[5];
                        assert(blank == '_');
                    }
                    case 'F':
                    {
                        //第一个状态从s[6]这个字符开始
                        int cur = 6;
                        for(int i=6; i<s.length();i++)
                        {
                            if(s[i] == ',')
                            {
                                //从cur--(i-1)是state
                                string state = s.substr(cur, (i-1) - cur + 1);
                                final_states.push_back(state);
                                cur = i + 1;
                            }
                            if(i == s.length() - 1)
                            {
                                assert(s[i] == '}');
                                //从cur--(i-1)是state
                                string state = s.substr(cur, (i-1) - cur + 1);
                                final_states.push_back(state);
                            }
                        }
                    }
                    case 'N':
                    {
                        //数字从第五个字符开始
                        nTape = stoi(s.substr(5, (s.length() - 1) - 5 + 1).c_str());
                    }
                }
            }
            else //transition
            {
                input a; output b;
                int cnt = 0; //当前记录到哪一个信息了
                int cur = 0;
                for(int i=0; i < s.length(); i++)
                {
                    if(i == ' ')
                    {
                        //从cur--(i-1)是我们需要的
                        cnt++;
                        switch(cnt)
                        {
                            case 1:
                            {
                                a.state = s.substr(cur, (i-1) - cur + 1);
                            }
                            case 2:
                            {
                                a.symbols = s.substr(cur, (i-1) - cur + 1);
                            }
                            case 3:
                            {
                                b.symbols = s.substr(cur, (i-1) - cur + 1);
                            }
                            case 4:
                            {
                                b.directions = s.substr(cur, (i-1) - cur + 1);
                            }
                        }
                        cur = i + 1;
                    }
                    if(i == s.length() - 1)
                    {
                        b.state = s.substr(cur, (s.length() - 1) - cur + 1);
                    }
                }
                transition[a] = b;
            }
        }
    }
    f.close();
}

void TuringMachine::check_input(string input)
{
    for(int i=0; i < input.length(); i++)
    {
        if (find(input_char.begin(), input_char.end(), input[i]) != input_char.end()) //找到了
        {
            // do nothing
        }
        else
        {
            // input非法
            string s = input.substr(i, 1);
            string head_pos = "       ";
            for (int j = 0; j < i; j++)
                head_pos = head_pos + ' ';
            head_pos = head_pos + '^';
            print_error({s,input,head_pos}, ILLEGAL_INPUT);
            exit(ILLEGAL_INPUT);
        }
    }
}

string TuringMachine::run(string input)
{
    if(verbose)
        cout<<"Input: "<<input<<endl;
    //所有tape都先初始化成只有一个'_' , 如果输入是空串第一个tape也是只有'_'
    for (int i = 0; i < nTape;i++)
    {
        list<char> tape(1, '_');
        tapes.push_back(tape);
        heads.push_back(tapes[i].begin());
    }
    if(input != "")
    {
        tapes[0].clear();
        //先检查input合法性
        check_input(input);
        //把input放到tapes[0]上去
        //
    }
}