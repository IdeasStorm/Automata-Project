#ifndef NODENFA_H
#define NODENFA_H

#include <QMultiHash>

class NodeNFA
{
public:
    NodeNFA(QString);
    NodeNFA(int);
    void link(char, NodeNFA*);         //link with another node
//NOTE this method can't override link(char,Node*)
    void link(char);                //link with itself
    QList<NodeNFA*> nextNode(char);           //return the next state if this char enter
                                         //null if there is no next state
    QMultiHash<char, NodeNFA*>* getnextNodes();
    void setnextNodes(QMultiHash<char, NodeNFA*>* nextnodes);
    void setFinite();
    void setNotFinite();
    bool isFiniteState();
    QString getName();

private:
    QString name;
    bool finite;
    QMultiHash<char, NodeNFA*>* nextNodes;
};

#endif // NODENFA_H
