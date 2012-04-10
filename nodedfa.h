#ifndef NODEDFA_H
#define NODEDFA_H
#include <QMap>
#include "node.h"

class NodeDFA : public Node
{
public:
    NodeDFA(QString);
    NodeDFA(int);
    void link(char, NodeDFA*);         //link with another node
    void link(char);                //link with itself
    NodeDFA* nextNode(char);           //return the next state if this char enter
                                            //null if there is no next state
private:
    QMap<char, NodeDFA*>* nextNodes;
};

#endif // NODEDFA_H
