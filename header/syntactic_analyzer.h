//
// Created by 15126 on 2021/5/15.
//

#ifndef SYNTACTIC_ANALYSIS_SYNTACTIC_ANALYZER_H
#define SYNTACTIC_ANALYSIS_SYNTACTIC_ANALYZER_H
#include "utils.h"
#include <unordered_map>
#include <iostream>
#include <iomanip>

class syntactic_analyzer {
public:
    std::vector<int> state_stack_;
    std::vector<char> symbol_statck_;
    struct Grammar grammar_;    //文法
//    struct Item item_;      //项目
//    struct ItemSet item_set_;   //项目集
    struct ItemSetDFA dfa_;     //项目集规范族dfa
    std::unordered_map<std::string, std::set<std::string>> first_set; //FIRST集
    std::unordered_map<std::string, std::set<std::string>> follow_set;  //FOLLOW集

    std::string action_table[50][50];  //action子表
    int goto_table[50][50];   //goto子表
    std::string input;
    int total_state = 0;
    //function
    syntactic_analyzer();   //构造函数
    void do_work();
    void init_grammar();    //从文件读入文法
    void calc_first_set();  //计算first集
    void calc_follow_set(); //计算follow集
    void calc_dfa();
    void calc_item_set_closure(struct ItemSet& item_set);
    void is_in_dfa(struct ItemSet&, int& in);
    std::set<std::string> get_suffix_first(std::string);
    void get_analyze_table();
    void display_analyze_table();
    void actually_analyze();

private:
    int step_ = 0;

};


#endif //SYNTACTIC_ANALYSIS_SYNTACTIC_ANALYZER_H
