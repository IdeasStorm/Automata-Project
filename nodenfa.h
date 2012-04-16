#ifndef NODENFA_H
#define NODENFA_H

#include <QMultiHash>

class NodeNFA
{
public:
    NodeNFA(QString);
    NodeNFA(int);
    NodeNFA(const NodeNFA&);
    void link(char, NodeNFA*);
    void link(char);
    QList<NodeNFA*> getNextNode(char);
    QMultiHash<char, NodeNFA*>* getNextNodes();
    void setNextNodes(QMultiHash<char, NodeNFA*>*);
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
