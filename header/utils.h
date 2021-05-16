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
#include <fstream>
#include <iostream>
#include <list>

//����ʽ������
//����һ���������ߵ������ս�����ұ�Ϊ��
struct ProductionRule{
    std::string left;
    std::string right;
    bool operator == (ProductionRule& r) {
        if(this->left != r.left) {
            return false;
        }
        return this->right == r.right;
    }
    ProductionRule() {};
    ProductionRule(const ProductionRule& r) {
        this->left = r.left;
        this->right = r.right;
    }
};

//Grammar �ķ�
struct Grammar{
    std::set<std::string> terminal;    //�ս��
    std::set<std::string> non_terminal;    //���ս��
    std::vector<ProductionRule> rules;   //����ʽ����
};


//LR(1)��Ŀ
struct Item{
    ProductionRule r;
    std::string lookahead; // ��ǰ���ַ�
    int point_loc;  // ���λ����Ϣ
    Item();
    Item(ProductionRule r, std::string lookahead, int loc):
    r(r), lookahead(lookahead), point_loc(loc){};
    friend std::ostream& operator << (std::ostream& os, const struct Item& r) {
        struct ProductionRule temp;
        temp.right = r.r.right;
        temp.right.insert(r.point_loc, "��");
        std::cout << r.r.left << "->" << temp.right << " " << r.lookahead << std::endl;
        return os;
    };
    bool operator == (const struct Item& r) {
        if(point_loc != r.point_loc)
            return false;
        if(lookahead != r.lookahead)
            return false;
        if(this->r.left != r.r.left)
            return false;
        if(this->r.right != r.r.right)
            return false;
        return true;
    }
};

//LR(1)��Ŀ��
struct ItemSet{
    int no;
    std::vector<Item> items;
};

//LR(1)��Ŀ���淶��
struct ItemSetDFA{
    std::vector<ItemSet> family;   //���е���Ŀ��
    std::vector<std::unordered_map<std::string,int>> strategy; // ת�Ʒ�ʽ
};

int char_to_num(char ch);

#endif //SYNTACTIC_ANALYSIS_UTILS_H
