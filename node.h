#ifndef NODE_H
#define NODE_H

class Node
{
public:
    Node();
    void link(char, Node*);     //linke with another node
    Node* nextState(char);          //return the next state if this char enter
                                    //      null if there is no next state
};

#endif // NODE_H
