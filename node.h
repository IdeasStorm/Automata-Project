#ifndef NODE_H
#define NODE_H

#include <QMap>

class Node
{
public:
    Node();
    void link(char, Node*);         //link with another node
    void link(char);                //link with itself
    Node* nextNode(char);           //return the next state if this char enter
                                            //null if there is no next state
    void setFinite();               //call this if this state is finite
    void setNotFinite();            //call this if this state is not finite
    bool isFiniteState();           //return true if is finite otherwise return false
    static int q;                   //state number
private:
    QMap<char, Node*>* nextNodes;
    bool finite;
};

#endif // NODE_H
