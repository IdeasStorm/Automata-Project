#ifndef NFA_H
#define NFA_H

#include"DFA.h"
#include "nodenfa.h"
#include<QString>
#include <QSet>

class NFA {

private:
    NodeNFA *StartState ;
    QSet<NodeNFA *> FinitStates;

public:
    //Default Constructer of NFA
    NFA();
    // Default Constructer of NFA and build NFA
    NFA(QString *KeyWords,int numberWord);

    //Build NFA
    void LoadNFA(QString *KeyWords,int numberWord);

    // Get
    NodeNFA * getStartState();
    QSet<NodeNFA *> getFinitStates();
    //Set
    void setStartState(NodeNFA *state);

};

#endif // NFA_H
