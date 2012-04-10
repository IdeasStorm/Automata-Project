#ifndef NODEDFA_H
#define NODEDFA_H

#include <QMap>
#include <QString>

class NodeDFA
{
public:
    NodeDFA(QString);
    NodeDFA(int);
    void link(char, NodeDFA*);         //link with another node
    void link(char);                //link with itself
    NodeDFA* nextNode(char);
                                            //null if there is no next state
    void setFinite();
    void setNotFinite();
    bool isFiniteState();
private:
    QMap<char, NodeDFA*>* nextNodes;
    QString name;
    bool finite;
};

#endif // NODEDFA_H
