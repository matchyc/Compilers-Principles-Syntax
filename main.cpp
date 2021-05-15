# include "header/syntactic_analyzer.h"

int main(int argc, char** argv) {

    printf("hello world!\n");
    syntactic_analyzer* lr1_parser = new syntactic_analyzer();
    lr1_parser->do_work();
    return 0;
}