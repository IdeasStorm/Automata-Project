#ifndef NODE_H
#define NODE_H

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
private:
    QString name;
    bool finite;
};

#endif // NODE_H

