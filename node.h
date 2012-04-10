#ifndef NODE_H
#define NODE_H
#include <QString>

class Node
{
public:
    Node(QString);
    Node(int);
    virtual void link(char, Node*)=0;
    virtual void link(char)=0;
    void setFinite();
    void setNotFinite();
    bool isFiniteState();
    QString getName();

    //virtual Node * nextNode(char);           //return the next state if this char enter

private:
    QString name;
    bool finite;
};

#endif // NODE_H

