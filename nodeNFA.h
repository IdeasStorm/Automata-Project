#ifndef NODENFA_H
#define NODENFA_H

#include <QMultiHash>

class NodeNFA
{
public:
    NodeNFA(QString);
    void link(char, NodeNFA*);         //link with another node
    void link(char);                //link with itself
    QList<NodeNFA*> nextNode(char);           //return the next state if this char enter
                                            //null if there is no next state
    void setFinite();               //call this if this state is finite
    void setNotFinite();            //call this if this state is not finite
    bool isFiniteState();           //return true if is finite otherwise return false
    static int q;                   //state number
private:
    QString name;
    QMultiHash<char, NodeNFA*>* nextNodes;
    bool finite;
};

#endif // NODENFA_H
