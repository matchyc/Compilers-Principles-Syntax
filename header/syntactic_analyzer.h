//
// Created by 15126 on 2021/5/15.
//

#ifndef SYNTACTIC_ANALYSIS_SYNTACTIC_ANALYZER_H
#define SYNTACTIC_ANALYSIS_SYNTACTIC_ANALYZER_H
#include "utils.h"
#include <unordered_map>

class syntactic_analyzer {
public:
    struct Grammar grammar_;    //文法
//    struct Item item_;      //项目
//    struct ItemSet item_set_;   //项目集
    struct ItemSetDFA dfa_;     //项目集规范族dfa
    std::unordered_map<std::string, std::set<std::string>> first_set; //FIRST集
    std::unordered_map<std::string, std::set<std::string>> follow_set;  //FOLLOW集

    std::pair<std::string, int> action_table[20][20];  //action子表
    std::vector<std::vector<int>> goto_table[20][20];   //goto子表

    //function
    syntactic_analyzer();   //构造函数
    void do_work();
    void init_grammar();    //从文件读入文法
    void calc_first_set();  //计算first集
    void calc_follow_set(); //计算follow集
    void calc_dfa();
private:


};


#endif //SYNTACTIC_ANALYSIS_SYNTACTIC_ANALYZER_H
