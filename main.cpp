# include "header/syntactic_analyzer.h"

int main(int argc, char** argv) {

    syntactic_analyzer* lr1_parser = new syntactic_analyzer();//ʵ��������
    lr1_parser->do_work();  //���ö����do_work()��������ʼ������
    system("pause");
    return 0;
}