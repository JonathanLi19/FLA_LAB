#include "turing_machine.h"
const char *ERR = "==================== ERR ====================";
const char *RUN = "==================== RUN ====================";
const char* SYNTAX_ERROR = "syntax error";
const char *DIVIDING_LINE = "---------------------------------------------";
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
            break;
        }
        case ILLEGAL_INPUT:
        {
            cerr << "illegal input" << endl;
            if(verbose)
            {
                cerr << ERR << endl << "error: '" << message[0] << "' was not declared in the set of input symbols" << endl << "Input: " << message[1] << endl << message[2] << endl;
            }
            break;
        }
        case WRONG_STATE:
        {
            cerr << "Wrong state in tm file" << endl;
            if(verbose)
            {
                cerr << ERR << endl << "error: '" << message[0] <<  "' was not declared in the set of states" << endl;
            }
            break;
        }
        case ERROR_DIRECTION:
        {
            cerr << SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << ERR << endl << "error: tape can be only moved to l,r,*"<< endl;
            }
            break;
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
        exit(0);
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
                        break;
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
                        break;
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
                        break;
                    }
                    case 'q':
                    {
                        //从6--strlen()-1
                        start_state = s.substr(6, (s.length() - 1) - 6 + 1);
                        break;
                    }
                    case 'B':
                    {
                        blank = s[5];
                        assert(blank == '_');
                        break;
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
                        break;
                    }
                    case 'N':
                    {
                        //数字从第五个字符开始
                        nTape = stoi(s.substr(5, (s.length() - 1) - 5 + 1).c_str());
                        break;
                    }
                }
            }
            else //transition
            {
                input_ a; output_ b;
                int cnt = 0; //当前记录到哪一个信息了
                int cur = 0;
                for(int i=0; i < s.length(); i++)
                {
                    if(s[i] == ' ')
                    {
                        //从cur--(i-1)是我们需要的
                        cnt++;
                        switch(cnt)
                        {
                            case 1:
                            {
                                a.state = s.substr(cur, (i-1) - cur + 1);
                                break;
                            }
                            case 2:
                            {
                                a.symbols = s.substr(cur, (i-1) - cur + 1);
                                break;
                            }
                            case 3:
                            {
                                b.symbols = s.substr(cur, (i-1) - cur + 1);
                                break;
                            }
                            case 4:
                            {
                                b.directions = s.substr(cur, (i-1) - cur + 1);
                                break;
                            }
                            case 5: //如果最后那个状态后面有个空格
                            {
                                b.state = s.substr(cur, (i - 1) - cur + 1);
                                break;
                            }
                        }
                        cur = i + 1;
                    }
                    if(i == s.length() - 1)
                    {
                        if(cnt == 4)
                        {
                            b.state = s.substr(cur, (s.length() - 1) - cur + 1);
                        }
                    }
                }
                auto iter = transition.find(a);
                if(iter != transition.end())
                {
                    cout << a.state << endl << a.symbols << endl;
                    cout << transition[a].state << endl << transition[a].symbols << endl << transition[a].directions << endl;
                    assert(0);
                }   
	            transition[a] = b;
                assert(transition.count(a) == 1);
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
            exit(0);
        }
    }
}

void TuringMachine::print_step(string cur_state, int step)
{
    cout<<"Step\t: "<<step<<endl;
    cout << "State\t: " << cur_state << endl;
    for(int i=0; i< nTape;i++)
    {
        auto left = tapes[i].begin();
        auto right = --tapes[i].end();
        int l_index = 0;
        int r_index = 0;
        while(left != tapes[i].end())
        {
            if((*left).symbol != blank || (*left).index == (*heads[i]).index) //要么不是blank，如果是blank的话如果head正好指向它也算进去
            {
                l_index = (*left).index;
                break;
            }
            else
                left++;
        }
        while(right != tapes[i].end())
        {
            if((*right).symbol != blank || (*right).index == (*heads[i]).index)
            {
                r_index = (*right).index;
                break;
            }
            else
                right--;
        }

        string index_str = "";
        string tape_str = "";
        string head_str = "";
        int output_num = r_index - l_index + 1;
        auto p = tapes[i].begin();
        bool start = false;
        while(output_num > 0 && p != tapes[i].end())
        {
            if((*p).index == l_index)
                start = true;
            if(start)
            {
                int index = (*p).index;
                if(index < 0)
                    index = -index;
                string s_index = to_string(index);
                index_str = index_str + s_index + ' ';
                tape_str = tape_str + (*p).symbol + string(s_index.length(), ' ');
                if((*p).index == (*heads[i]).index)
                    head_str = head_str + '^';
                head_str = head_str + string(s_index.length()+1, ' ');
                output_num--;
            }
            p++;
        }
        index_str.erase(index_str.find_last_not_of(' ') + 1 , (index_str.length() - 1) - (index_str.find_last_not_of(' ') + 1) + 1);
        tape_str.erase(tape_str.find_last_not_of(' ') + 1, (tape_str.length() - 1) - (tape_str.find_last_not_of(' ') + 1) + 1);
        head_str.erase(head_str.find_last_not_of(' ') + 1, (head_str.length() - 1) - (head_str.find_last_not_of(' ') + 1) + 1);
        cout << "Index" << i << "\t: " << index_str << endl
             << "Tape" << i << "\t: " << tape_str << endl
             << "Head" << i << "\t: " << head_str << endl;
    }
    cout << DIVIDING_LINE << endl;
}

string TuringMachine::Get_result()
{
    string res = "";
    bool start = false; //是否已经过了最左边的空格区域
    for(auto i = tapes[0].begin(); i != tapes[0].end();i++)
    {
        char ch = (*i).symbol;
        if(ch != blank)
        {
            res = res + ch;
            start = true;
        }
        if(start == true && ch == blank) //如果碰到了最右边的空格区域，直接返回
        {
            return res;
        }
    }
    return res;
}

void TuringMachine::Write_Move(string old_symbols, string symbols, string directions) //write on the tape and move the heads
{
    assert(directions.length() == nTape && symbols.length() == nTape);
    for(int i=0;i < nTape; i++)
    {
        if(old_symbols[i] == '*' && symbols[i] != '*')
        {
            (*heads[i]).symbol = symbols[i];
        }
        else if(old_symbols[i] == '*' && symbols[i] == '*')
        {
            //do nothing
        }
        else
            (*heads[i]).symbol = symbols[i];
            
        switch(directions[i])
        {
            case 'l':
                if(heads[i]==tapes[i].begin())
                {
                    tapes[i].push_front(Cell((*heads[i]).index - 1, '_'));
                    heads[i]--;
                }
                else
                    heads[i]--;
                break;
            case 'r':
                if(heads[i]==--tapes[i].end())
                {
                    tapes[i].push_back(Cell((*heads[i]).index + 1, '_'));
                    heads[i]++;
                }
                else
                    heads[i]++;
                break;
            case '*':
                break;
            default:
                print_error({}, ERROR_DIRECTION);
                break;
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
        list<Cell> tape(1, Cell(0, '_'));
        tapes.push_back(tape);
        heads.push_back(tapes[i].begin());
    }
    if(input != "")
    {
        tapes[0].clear();
        //先检查input合法性
        check_input(input);
        //把input放到tapes[0]上去; 更新heads[0]
        for(int i = 0; i < input.length(); i++)
        {
            tapes[0].push_back(Cell(i, input[i]));
        }
        heads[0] = tapes[0].begin();
        //开始transition
        string cur_state = start_state;
        int step = 0;
        if(verbose)
        {
            cout << RUN << endl;
        }
        // 当没有碰到halt的时候循环
        while(true)
        {
            if(verbose)
            {
                print_step(cur_state, step);   
            }
            auto p = find(final_states.begin(), final_states.end(), cur_state);
            if(p != final_states.end()) //到达终止状态
            {
                return Get_result();
            }
            //通过transition获得output
            input_ a;
            a.state = cur_state;
            a.symbols = "";
            for(int i=0;i < nTape; i++)
            {
                a.symbols += (*heads[i]).symbol;
            }
            //TODO: 如果a找不到，可能规则中有*
            if(transition.find(a) == transition.end()) //如果a不存在transition中
            {
                cout << a.state << endl << a.symbols << endl;
                //对每一个规则试试看现在的symbols可以符合他吗，找到可以符合的就把a.symbols更新
                bool find = false;
                for(auto iter = transition.begin(); iter != transition.end(); iter++)
                {
                    input_ key = iter->first;
                    bool fit = true; //是否找到符合的规则
                    if(key.state == cur_state)
                    {
                        for(int i=0;i<nTape;i++)
                        {
                            char c = heads[i]->symbol;
                            
                            if(c != key.symbols[i])
                            {
                                if(c == blank)
                                {
                                    fit = false;
                                    break;
                                }
                                else if(key.symbols[i] == '*')
                                {
                                    //do nothing
                                }
                                else
                                {
                                    fit = false;
                                    break;
                                }
                            }
                        }

                    }
                    else
                    {
                        fit = false;
                    }
                    if(fit)
                    {
                        a.symbols = key.symbols;
                        find = true;
                        break;
                    }
                }
                if(!find)
                {
                    //如果规则中没有匹配的*
                    cout << "Halted. No rule for state: '" << cur_state <<"' and symbol: '"<< a.symbols <<"'"<< endl;
                    return Get_result();
                }
                else
                {
                    //do nothing
                }
            }
            output_ b;
            b = transition[a];
            auto p1 = find(states.begin(), states.end(), b.state);
            if(p1 == states.end())
            {
                print_error({b.state}, WRONG_STATE);
                exit(0);
            }
            //heads write & move then change the state
            Write_Move(a.symbols, b.symbols, b.directions);
            cur_state = b.state;
            step++;
        }
    }
}