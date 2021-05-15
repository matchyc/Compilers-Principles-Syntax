//
// Created by 15126 on 2021/5/15.
//

#include "../header/syntactic_analyzer.h"
#include <fstream>
#include <iostream>
//using namespace std;

syntactic_analyzer::syntactic_analyzer() {
    this->init_grammar();
}

void syntactic_analyzer::do_work() {
    calc_first_set();
}

void syntactic_analyzer::init_grammar() {
    std::fstream f;
    f.open("grammar.txt", std::ios::in);
    if(!f.is_open()) {
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
    for(int i = 0; i < n; ++i) {
        f >> input;
        int l_loc = input.find_first_of('-');
        rule.left = input.substr(0, l_loc);
        rule.right = input.substr(l_loc + 2, 10);
        this->grammar_.rules.push_back(rule);
    }
    f >> input;
    while (input != "$") {
        this->grammar_.non_terminal.insert(input);
        f >> input;
    }
    f >> input;
    while(input != "$") {
        this->grammar_.terminal.insert(input);
        f >> input;
    }
    printf("Production rules:\n");
    for(auto& k:grammar_.rules) {
        std::cout << k.left << "->" << k.right << std::endl;
    }
    std::cout << "Terminals:" << std::endl;
    for(auto& k:grammar_.terminal) {
        std::cout << k << ' ';
    }
    std::cout << std::endl;
    printf("Non-terminals:\n");
    for(auto& k:grammar_.non_terminal) {
        std::cout << k << ' ';
    }
    printf("\n");
    grammar_.terminal.insert("$");
}

void syntactic_analyzer::calc_first_set() {
    // @ 代表 空
    for(auto& k: grammar_.terminal) {
        first_set[k].insert(k);
    }
    //不对人工求法进行模拟，而是对产生式右侧可能会需要的每一个非终结符
    //进行求first集再并入的操作，每当任意一个终结符有新的first集元素
    //都重新进行该算法，确保不遗漏。
    bool flag = true;
    for(;flag;) {   //每当任意一个终结符有新的first集元素继续算法
        flag = false;
        for(auto r: grammar_.rules) {
            std::string c; //右侧第一个文法符号
            if(grammar_.terminal.find(r.right) == grammar_.terminal.end()) {
                c = r.right.substr(0,1);
            } else {
                c = r.right;
            }
            //若是终结符
            if(grammar_.terminal.find(c) != grammar_.terminal.end() || c == "@"){
                auto ret = first_set[r.left].insert(c);
                if(ret.second) {
                    flag = true;
                }
            } else {    //若不是终结符
                bool isnull = true;
                int loc = 0;
                //有可能是空，需要继续往后找，有可能不是空
                while(isnull && loc<r.right.size()) {
                    std::string now_n;
                    if(grammar_.terminal.find(r.right) == grammar_.terminal.end()) {
                        now_n = r.right.substr(loc, 1);
                    } else {
                        now_n = r.right.substr(loc, 1);
                    }
                    //把遇到的非终结符的first集加入到当前所求文法符号的first集
                    for(auto& k:first_set[now_n]) {
                        if(k != "@") {
                            auto ret = first_set[r.left].insert(k);
                            if(ret.second) {
                                flag = true;
                            }
                        }
                    }
                    //如果是空，还要继续看下一个文法符号的first集。
                    if(first_set[now_n].find("@") != first_set[now_n].end()) {
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
    for(auto& i:first_set) {
        if(grammar_.non_terminal.find(i.first) != grammar_.non_terminal.end()) {
            std::cout << i.first << " | ";
            for(auto& j: i.second) {
                std::cout << j << ' ';
            }
            std::cout << std::endl;
        }
    }
}

void syntactic_analyzer::calc_follow_set() {

}

void syntactic_analyzer::calc_dfa() {
    //I0
    struct Item I0(grammar_.rules[0], "$", 0);




}