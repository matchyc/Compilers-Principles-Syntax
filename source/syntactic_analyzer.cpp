//
// Created by 15126 on 2021/5/15.
//

#include "../header/syntactic_analyzer.h"

//using namespace std;

syntactic_analyzer::syntactic_analyzer() {
    this->init_grammar();
    dfa_.strategy.resize(30);
}

void syntactic_analyzer::do_work() {
    input = "1+1$";
    calc_first_set();
    calc_dfa();
    total_state = dfa_.family.size();
    get_analyze_table();
    display_analyze_table();
    state_stack_.push_back(0);
    actually_analyze();
}

void syntactic_analyzer::actually_analyze() {
    using namespace std;
//    printf("step\t\tsymbol_statck_\t\tstate\t\tinput\t\taction\n");
    cout << "step" << setw(20) << "symbol_stack" << setw(20)
         << setw(20) << setw(20) << "state_stack" << setw(20) << "input" <<
         setw(20) << setw(20)<< "action" << endl;
    while (true)
    {
        if (isdigit(input[0])) //处理数字
        {
            while (isdigit(input[0]) || input[0] == '.') //弹出数字及浮点
                input.erase(input.begin());
            input.insert(input.begin(), 'n'); //压入数字代表的终结符n
        }
        std::string action = action_table[state_stack_.back()][char_to_num(input[0])];
        std::string tip;
        if (action[0] == 'S') //移进动作
        {
            symbol_statck_.push_back(input[0]);         //将a压入符号栈
            input.erase(input.begin());         //将原符号从输入串弹出
            state_stack_.push_back(stoi(action.substr(1))); //将S压入状态栈顶
            //输出 移进动作提示
            tip = "shift " + action.substr(1);
        }
        else if (action[0] == 'R') //规约动作
        {
            int i = stoi(action.substr(1));
            int popNumber = (int)grammar_.rules[i].right.size(); //x->共3个字符多余
            //输出 规约动作提示
            tip = "reduce by " + grammar_.rules[i].left + "->" + grammar_.rules[i].right;
            //弹出规约符号
            while (popNumber-- > 0)
            {
                state_stack_.pop_back();
                symbol_statck_.pop_back();
            }
            //压入新符号
            symbol_statck_.push_back(grammar_.rules[i].left[0]);
            state_stack_.push_back(goto_table[state_stack_.back()][char_to_num(grammar_.rules[i].left[0])]);
        }
        else if (action == "ACC")
        {
            printf("Accept!");
            break;
        }
        else {
            std::cout << "error" << std::endl;
            exit(-2);
        }
        //分析结束输出
        using namespace std;

        cout << setw(2) << step_++ << '|';
        for (int i = 0; i < symbol_statck_.size(); i++) //输出栈的状态

            printf("%c", symbol_statck_[i]);
        printf("\t\t");
        for (int i = 0; i < state_stack_.size(); i++)
            printf("%d", state_stack_[i]);
        printf("\t\t");
        for (int i = 0; i < input.size(); i++)
            printf("%c", input[i]);
        printf("\t\t");
        printf("%s", tip.c_str());
        printf("\n");
    }
}

void syntactic_analyzer::display_analyze_table() {
    using namespace std;
    string terminal[10] = {"num", "+", "-", "*", "/", "(", ")", "$"};
    string non_terminal[10] = {"E", "T", "F"};
    std::cout << "state" << "|" << std::setw(30) << "action" << std::setw(30) << "|"
    << std::setw(16)<< "goto" << std::endl;
    cout << setw(6) << "|";
    for(auto i = 0; i < 8; ++i) {
        cout << setw(7) << terminal[i];
    }
    cout << setw(4) << '|' << " ";
    for(auto j = 0; j < 3; ++j) {
        cout << setw(7) << non_terminal[j];
    }
    cout << endl;
    std::cout << std::setfill('-') << std::setw(100) << "-" << std::endl;
    for(int i = 0; i < total_state; ++i) {
        std::cout << std::setfill(' ') << std::setw(4) << i << " |";
        for(int j = 0; j < grammar_.terminal.size(); ++j) {
            std::cout << setw(7) << action_table[i][j];
        }
        cout << setw(4) << '|';
        for(int j = 0; j < grammar_.non_terminal.size(); ++j) {
            std::cout << setw(8) << goto_table[i][j];
        }
        std::cout << std::endl;
    }
}

void syntactic_analyzer::init_grammar() {

    std::fstream f;
    f.open("grammar.txt", std::ios::in);
    if (!f.is_open()) {
        std::cerr << "Fail to open grammar file\n";
        exit(1);
    }
    std::string input;
    f >> input;
    int n = std::atoi(input.c_str());
//    std::cout << "This grammar has " << n << " production rules" << std::endl;
    ProductionRule rule;
    //首先加入拓广文法
    rule.left = "S";
    rule.right = "E";
    this->grammar_.rules.push_back(rule);
    for (int i = 0; i < n; ++i) {
        f >> input;
        int l_loc = input.find_first_of('-');
        rule.left = input.substr(0, l_loc);
        rule.right = input.substr(l_loc + 2, 10);
//        if(rule.right == "num") {
//            rule.right = "n";
//        }
        this->grammar_.rules.push_back(rule);
    }
    f >> input;
    while (input != "$") {
        this->grammar_.non_terminal.insert(input);
        f >> input;
    }
    f >> input;
    while (input != "$") {
//        if(input == "num") {
//            input = "n";
//        }
        this->grammar_.terminal.insert(input);
        f >> input;
    }
    printf("Production rules:\n");
    for (auto &k:grammar_.rules) {
        std::cout << k.left << "->" << k.right << std::endl;
    }
    std::cout << "Terminals:" << std::endl;
    for (auto &k:grammar_.terminal) {
        std::cout << k << ' ';
    }
    std::cout << std::endl;
    printf("Non-terminals:\n");
    for (auto &k:grammar_.non_terminal) {
        std::cout << k << ' ';
    }
    printf("\n");
    grammar_.terminal.insert("$");
    grammar_.terminal.erase("num");
    grammar_.terminal.insert("n");
    grammar_.rules.back().right = "n";
}

void syntactic_analyzer::calc_first_set() {
    // @ 代表 空
    for (auto &k: grammar_.terminal) {
        first_set[k].insert(k);
    }
    //不对人工求法进行模拟，而是对产生式右侧可能会需要的每一个非终结符
    //进行求first集再并入的操作，每当任意一个终结符有新的first集元素
    //都重新进行该算法，确保不遗漏。
    bool flag = true;
    for (; flag;) {   //每当任意一个终结符有新的first集元素继续算法
        flag = false;
        for (auto r: grammar_.rules) {
            std::string c; //右侧第一个文法符号
            if (grammar_.terminal.find(r.right) == grammar_.terminal.end()) {
                c = r.right.substr(0, 1);
            } else {
                c = r.right;
            }
            //若是终结符
            if (grammar_.terminal.find(c) != grammar_.terminal.end() || c == "@") {
                auto ret = first_set[r.left].insert(c);
                if (ret.second) {
                    flag = true;
                }
            } else {    //若不是终结符
                bool isnull = true;
                int loc = 0;
                //有可能是空，需要继续往后找，有可能不是空
                while (isnull && loc < r.right.size()) {
                    std::string now_n;
                    if (grammar_.terminal.find(r.right) == grammar_.terminal.end()) {
                        now_n = r.right.substr(loc, 1);
                    } else {
                        now_n = r.right.substr(loc, 1);
                    }
                    //把遇到的非终结符的first集加入到当前所求文法符号的first集
                    for (auto &k:first_set[now_n]) {
                        if (k != "@") {
                            auto ret = first_set[r.left].insert(k);
                            if (ret.second) {
                                flag = true;
                            }
                        }
                    }
                    //如果是空，还要继续看下一个文法符号的first集。
                    if (first_set[now_n].find("@") != first_set[now_n].end()) {
                        isnull = true;
                        ++loc;
                    } else {
                        isnull = false;
                    }
                }
            }
        }
    }
    printf("First set:\n");
    for (auto &i:first_set) {
        if (grammar_.non_terminal.find(i.first) != grammar_.non_terminal.end()) {
            std::cout << i.first << " | ";
            for (auto &j: i.second) {
                std::cout << j << ' ';
            }
            std::cout << std::endl;
        }
    }
}

void syntactic_analyzer::calc_follow_set() {

}


void syntactic_analyzer::calc_dfa() {
    printf("======Begin to produce DFA=======\n");
    //首先要有I0
    struct Item r0(grammar_.rules[0], "$", 0);
    struct ItemSet I0;
    I0.no = 0;
    I0.items.push_back(r0);
    //计算出完整的I0
    calc_item_set_closure(I0);
    //I0加入DFA
    dfa_.family.push_back(I0);
    //待扩展双端队列list
    std::list<struct ItemSet> wait_list;
    wait_list.push_back(I0);
    while (!wait_list.empty()) {
        struct ItemSet one_set = wait_list.front();
        //对每一个项目
        for (auto &ter: grammar_.terminal) {
            struct ItemSet new_set;
            for (auto &item:one_set.items) {
                if(item.point_loc < item.r.right.size()) {
                    std::string next = item.r.right.substr(item.point_loc, 1);
                    if(next == ter) {
                        struct Item new_item(item.r, item.lookahead, item.point_loc + 1);
                        new_set.items.push_back(new_item);
                    }
                }
            }
            calc_item_set_closure(new_set);
            if(!new_set.items.empty()) {
                int in_dfa = -1;
                is_in_dfa(new_set, in_dfa);
                if (in_dfa == -1) {  //若是新项目集
                    new_set.no = dfa_.family.size();
                    dfa_.family.push_back(new_set);
                    wait_list.push_back(new_set);
                    in_dfa = new_set.no;
                } else {
                    //nothing to do
                }
                //自动机添加边
                dfa_.strategy[one_set.no][ter] = in_dfa;
            }

        }
        for(auto &non_ter:grammar_.non_terminal) {
            struct ItemSet new_set;
            for (auto &item:one_set.items) {
                if(item.point_loc < item.r.right.size()) {
                    std::string next = item.r.right.substr(item.point_loc, 1);
                    if(next == non_ter) {
                        struct Item new_item(item.r, item.lookahead, item.point_loc + 1);
                        new_set.items.push_back(new_item);
                    }
                }
            }
            calc_item_set_closure(new_set);
            if(!new_set.items.empty()) {
                int in_dfa = -1;
                is_in_dfa(new_set, in_dfa);
                if (in_dfa == -1) {  //若是新项目集
                    new_set.no = dfa_.family.size();
                    dfa_.family.push_back(new_set);
                    wait_list.push_back(new_set);
                    in_dfa = new_set.no;
                } else {
                    //nothing to do
                }
                //自动机添加边
                dfa_.strategy[one_set.no][non_ter] = in_dfa;
            }
        }
//        for(auto& item:one_set.items) {
//            if(item.point_loc < item.r.right.size()) { //还不是归约项目时
//                std::string next = item.r.right.substr(item.point_loc, 1);
//                if(grammar_.terminal.find(next) != grammar_.terminal.end()
//                    ||
//                    grammar_.non_terminal.find(next) != grammar_.non_terminal.end()) {
//                    //构造新的项目
//                    struct ItemSet new_set;
//                    struct Item new_item(item.r, item.lookahead, item.point_loc + 1);
//
//                    new_set.items.push_back(new_item);
//                    //计算闭包
//                    calc_item_set_closure(new_set);
//                    int in_dfa = 0;
//                    is_in_dfa(new_set, in_dfa);
//                    if(in_dfa == -1) {  //若是新项目集
//                        new_set.no = dfa_.family.size();
//                        dfa_.family.push_back(new_set);
//                        wait_list.push_back(new_set);
//                        in_dfa = new_set.no;
//                    } else {
//                        //nothing to do
//                    }
//                    //自动机添加边
//                    dfa_.strategy[one_set.no][next] = in_dfa;
//                }
//            }
//        }
        wait_list.pop_front();// 出队
    }

    for (int i = 0; i < dfa_.family.size(); ++i) {
        struct ItemSet item_set = dfa_.family[i];
        printf("Item set %d \n", i);
        for (auto &k: item_set.items) {
            std::cout << k << std::endl;
        }
    }
    std::cout << "Total item set: " << dfa_.family.size() << std::endl;
}



void syntactic_analyzer::get_analyze_table() {
    for(int i = 0; i < total_state; ++i) {
        struct ItemSet one_set = dfa_.family[i];
        for(auto& item: one_set.items) {
            if(item.point_loc < item.r.right.size()) {
                std::string next = item.r.right.substr(item.point_loc, 1);
                if(grammar_.terminal.find(next) != grammar_.terminal.end()) {
                    action_table[one_set.no][char_to_num(next[0])] = "S" + std::to_string(dfa_.strategy[one_set.no][next]);
                }
            } else {
                //归约项目处理
                if(item.r.right == "E" && item.lookahead == "$") {
                    action_table[one_set.no][char_to_num('$')] = "ACC";
                } else {
                    for(int j = 0; j <grammar_.rules.size(); ++j) {
                        if(item.r == grammar_.rules[j]) {
                            action_table[one_set.no][char_to_num(item.lookahead[0])] = "R"+std::to_string(j);
                        }
                    }
                }
            }
        }
        for(auto it = dfa_.strategy[i].begin(); it != dfa_.strategy[i].end(); ++it) {
            if(grammar_.non_terminal.find(it->first) != grammar_.non_terminal.end()) {
                //是非终结符
                goto_table[i][char_to_num(it->first[0])] = it->second;
            }
        }
    }

}

int char_to_num(char ch)
{
    switch (ch)
    {
        case 'n':
            return 0;
        case '+':
            return 1;
        case '-':
            return 2;
        case '*':
            return 3;
        case '/':
            return 4;
        case '(':
            return 5;
        case ')':
            return 6;
        case '$':
            return 7;
        case 'E':
            return 0;
        case 'T':
            return 1;
        case 'F':
            return 2;
        default:
            return -1;
    }
}

bool item_in_set(struct Item item, struct ItemSet item_set) {
    for(auto& k:item_set.items) {
        if(k.point_loc == item.point_loc &&
            k.lookahead == item.lookahead &&
            k.r.left == item.r.left &&
            k.r.right == item.r.right) {
            return true;
        }
    }
    return false;
}

void syntactic_analyzer::is_in_dfa(struct ItemSet &now_set, int &in) {
    for (auto &k:dfa_.family) {
        if (k.items.size() == now_set.items.size()) {
            bool issame = false;
            for (int i = 0; i < k.items.size(); ++i) {
                if(item_in_set(now_set.items[i], k)) {
                    issame = true;
                } else {
                    issame = false;
                    break;
                }
            }
            if (issame) {
                in = k.no;
                return;
            }
        }
    }
    in = -1;
}

void syntactic_analyzer::calc_item_set_closure(struct ItemSet &one_set) {
    bool not_done = true; //是否算完的标记
    //只要没算完，就循环算法
    int n_size = one_set.items.size();
    if(n_size <= 0) return;
    while (not_done) {
        not_done = false;
        for (int i = 0; i < one_set.items.size(); ++i) {
//            printf("Now item set %d have %d, scan to %d\n",one_set.no, one_set.items.size(), i);
            struct Item item = one_set.items[i];
            if (item.point_loc < item.r.right.size()) {
                std::string next = item.r.right.substr(item.point_loc, 1);
                if (grammar_.non_terminal.find(next) != grammar_.non_terminal.end()) {
                    //若是非终结符，则考虑加入该项目集
                    std::string suffix = item.r.right.substr(item.point_loc + 1, 10);
                    suffix += item.lookahead;
                    //求suffix的first集
                    std::set<std::string> now_first = get_suffix_first(suffix);
                    //将每一个后缀不同的项目加入项目集
                    for (auto &rule: grammar_.rules) { // 对每一个产生式
//                        std::cout << "Now rule right:" << rule.right << std::endl;
                        if (rule.left == next) {
                            for (auto &t: now_first) {  // 每一个现在first集中的元素
                                struct Item temp_item(rule, t, 0);
                                //看是否重复
                                bool isreplica = false;
                                for (auto &p: one_set.items) {
//                                    printf("Now compare:");
//                                    std::cout << "In set: " << p << "generate: " << temp_item << std::endl;
//                                    p.r.right == temp_item.r.right &&
//                                    p.r.left == temp_item.r.left &&
//                                    p.point_loc == temp_item.point_loc &&
//                                    p.lookahead == temp_item.lookahead
                                    if (p == temp_item) {
                                        isreplica = true;
//                                        std::cout << "result: " << "same!" << std::endl;
                                        break;

                                    } else {
                                        isreplica = false;
                                    }//ifelse
                                }//for
                                if (isreplica) {
                                    not_done = false;
                                } else {
//                                    std::cout << "result: " << "not same! insert!" << std::endl;
                                    not_done = true;
                                    one_set.items.push_back(temp_item);
                                }
                            }
                        }//if
                    }//for
                }
            }
//            std::cout << "Toal Item Set: " << dfa_.family.size() << std::endl;
        }
    }
}

std::set<std::string> syntactic_analyzer::get_suffix_first(std::string suffix) {
    std::set<std::string> suffix_first;
    bool isok = false;  //控制非终结符或推出空继续。
    for (int i = 0; i < suffix.size() && !isok; ++i) {
        isok = true;
        if (grammar_.terminal.find(suffix.substr(i, 1)) != grammar_.terminal.end()) {
            //若是终结符
            suffix_first.insert(suffix.substr(i, 1));
        } else if (suffix[i] == '@') {
            //若不是，则可能是空
            isok = false;
            continue;
        } else {
            std::string s = suffix.substr(i, 1);
            for (auto &k:first_set[s]) {
                suffix_first.insert(k);
                if (k == "@") {  //能推出空，则继续
                    isok = false;
                }
            }
        }
    }
//    if(!isok) {
//        suffix_first.insert("@");
//    }
    return suffix_first;
}
