#ifndef NODEDFA_H
#define NODEDFA_H

class NodeDFA
{
public:
    NodeDFA(QString);
    NodeDFA(int);
    void link(char, NodeDFA*);         //link with another node
    void link(char);                //link with itself
    QList<NodeDFA*> nextNode(char);           //return the next state if this char enter
                                            //null if there is no next state
    void setFinite();               //call this if this state is finite
    void setNotFinite();            //call this if this state is not finite
    bool isFiniteState();           //return true if is finite otherwise return false
private:
    QString name;
    QMap<char, NodeDFA*>* nextNodes;
    bool finite;
};

#endif // NODEDFA_H
