//
// Created by 15126 on 2021/5/15.
//
// This header file aims to define several data structures for syntatic analysis.

#ifndef SYNTACTIC_ANALYSIS_UTILS_H
#define SYNTACTIC_ANALYSIS_UTILS_H

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

//产生式规则定义
//考虑一般情况，左边单个非终结符，右边为串
struct ProductionRule{
    std::string left;
    std::string right;
};

//Grammar 文法
struct Grammar{
    std::set<std::string> terminal;    //终结符
    std::set<std::string> non_terminal;    //非终结符
    std::vector<ProductionRule> rules;   //产生式规则
};


//LR(1)项目
struct Item{
    ProductionRule r;
    std::string lookahead; // 向前看字符
    int point_loc;  // 点的位置信息
    Item();
    Item(ProductionRule r, std::string lookahead, int loc):
    r(r), lookahead(lookahead), point_loc(loc){};
};

//LR(1)项目集
struct ItemSet{
    std::vector<Item> items;
};

//LR(1)项目集规范族
struct ItemSetDFA{
    std::vector<ItemSet> family;   //所有的项目集
    std::vector<std::unordered_map<std::string,int>> strategy; // 转移方式
};



#endif //SYNTACTIC_ANALYSIS_UTILS_H
