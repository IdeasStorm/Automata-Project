#ifndef NODENFA_H
#define NODENFA_H

#include <QMultiHash>
#include "node.h"

class NodeNFA : public Node
{
public:
    NodeNFA(QString);
    NodeNFA(int);
    void link(char, NodeNFA*);         //link with another node
    void link(char);                //link with itself
    QList<NodeNFA*> nextNode(char);           //return the next state if this char enter
                                            //null if there is no next state
private:
    QMultiHash<char, NodeNFA*>* nextNodes;
};

#endif // NODENFA_H
