#ifndef TEST_H
#define TEST_H
#include "DFA.h"
#include "nodedfa.h"

class test
{
public:
    test();
    static test1(){
        NodeDFA* a = new NodeDFA('A');
        NodeDFA* b = new NodeDFA('B');
        NodeDFA* c = new NodeDFA('C');
        NodeDFA* d = new NodeDFA('D');

        // linking ...
        a->link('c',b);
        b->link('a',c);
        c->link('t',d);
    }
};

#endif // TEST_H
