#ifndef NFA_H
#define NFA_H

#include"DFA.h"
#include "nodenfa.h"
#include<QString>
#include <QSet>

class NFA {
public:
    NFA();
    NFA(QString *KeyWords,int numberWord);
    //Build NFA
    void LoadNFA(QString *KeyWords,int numberWord);

    // Get
    NodeNFA * getStartState();
    QSet<NodeNFA *> getFinitStates();
    //Set
    void setStartState(NodeNFA *state);

    //Convert from NFA to DFA
    NodeDFA* convertToDFA();
private:
    NodeNFA *StartState ;
    QSet<NodeNFA *> FinitStates;
    QSet<QString>* usedState;
    QList<NodeNFA*> temp;
    int nodeNum;

    void addToSet(QString, NodeDFA*, char);
    void addToList(NodeNFA* node);
    QList<NodeNFA*>* getValueNodes(NodeNFA* node, NodeDFA* dfa);
};

#endif // NFA_H
