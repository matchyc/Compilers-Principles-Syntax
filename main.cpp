# include "header/syntactic_analyzer.h"

int main(int argc, char** argv) {

    syntactic_analyzer* lr1_parser = new syntactic_analyzer();//实例化对象
    lr1_parser->do_work();  //调用对象的do_work()方法，开始工作。
    system("pause");
    return 0;
}