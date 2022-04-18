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
    std::vector<int> state_stack_;  //状态栈
    std::vector<char> symbol_statck_;   //符号栈
    struct Grammar grammar_;    //文法
    struct ItemSetDFA dfa_;     //项目集规范族dfa
    std::unordered_map<std::string, std::set<std::string>> first_set; //FIRST集
    std::unordered_map<std::string, std::set<std::string>> follow_set;  //FOLLOW集

    std::string action_table[50][50];  //action子表
    int goto_table[50][50];   //goto子表
    std::string input;      //输入串符号串
    int total_state = 0;    //总状态
    int step_ = 0;          //实例分析时的步骤记录

    //function
    void do_work();         //开始工作！
    syntactic_analyzer();   //构造函数
    void init_grammar();    //从文件读入文法
    void calc_first_set();  //计算first集
    void calc_dfa();        //自动构造DFA
    void calc_item_set_closure(struct ItemSet& item_set); //对每一个LR(1)项目 计算闭包
    void is_in_dfa(struct ItemSet&, int& in);             //判断项目集是否在DFA中
    std::set<std::string> get_suffix_first(std::string);    //求LR(1)转移时的向前搜索符
    void get_analyze_table();       //构造分析表
    void display_analyze_table();   //打印分析表
    void actually_analyze();        //分析输入串

private:


};


#endif //SYNTACTIC_ANALYSIS_SYNTACTIC_ANALYZER_H
