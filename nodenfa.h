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
    bool operator ==(NodeNFA second) {
        if(this->name == second.name &&
                this->finite == this->finite &&
                this->nextNodes == second.nextNodes)
            return true;

        return false;
    }

private:
    QString name;
    bool finite;
    QMultiHash<char, NodeNFA*>* nextNodes;
};

#endif // NODENFA_H
