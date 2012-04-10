#ifndef NODENFA_H
#define NODENFA_H

#include <QMultiHash>
#include "node.h"

class NodeNFA : public Node
{
public:
    NodeNFA(QString);
    NodeNFA(int);
    void link(char, Node*);         //link with another node
//NOTE this method can't override link(char,Node*)
    void link(char);                //link with itself
    QList<Node*> nextNode(char);           //return the next state if this char enter
                                            //null if there is no next state
private:
    QMultiHash<char, Node*>* nextNodes;
};

#endif // NODENFA_H
