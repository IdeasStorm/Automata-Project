#ifndef NODEDFA_H
#define NODEDFA_H

#include <QMap>
#include <QString>

class NodeDFA
{
public:
    NodeDFA(QString);
    NodeDFA(int);
    void link(char, NodeDFA*);
    void link(char);
    NodeDFA* nextNode(char);
    void setFinite();
    void setNotFinite();
    bool isFiniteState();
private:
    QMap<char, NodeDFA*>* nextNodes;
    QString name;
    bool finite;
};

#endif // NODEDFA_H
