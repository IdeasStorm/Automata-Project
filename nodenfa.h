#ifndef NODENFA_H
#define NODENFA_H

#include <QMultiHash>

class NodeNFA
{
public:
    NodeNFA(QString);
    NodeNFA(int);
    void link(char, NodeNFA*);
    void link(char);
    QList<NodeNFA*> nextNode(char);
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
