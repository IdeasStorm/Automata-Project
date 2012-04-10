#ifndef NODEDFA_H
#define NODEDFA_H
#include <QMap>
#include "node.h"

class NodeDFA
{
public:
    NodeDFA(QString);
    NodeDFA(int);
    void link(char, NodeDFA *);         //link with another node
    void link(char);                //link with itself
    NodeDFA * nextNode(char);
                                            //null if there is no next state


private:
    QMap<char, NodeDFA *>* nextNodes;
private:
    QString name;
    bool finite;
};

#endif // NODEDFA_H
