#ifndef NODEDFA_H
#define NODEDFA_H

#include <QMap>
#include <QString>

class NodeDFA
{
public:
    NodeDFA(char);
    NodeDFA(int);
    void link(char, NodeDFA*);
    void link(char);
    NodeDFA* nextNode(char);
    void setFinite();
    void setNotFinite();
    bool isFiniteState();
    void setnextNodes( QMap<char, NodeDFA*>*);
    QMap<char, NodeDFA*>* getnextNodes();

private:
    char name;
    QMap<char, NodeDFA*>* nextNodes;
    bool finite;
};

#endif // NODEDFA_H
