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
private:
    NodeNFA *StartState ;
    QSet<NodeNFA *> FinitStates;
};

#endif // NFA_H
