#ifndef DFA_H
#define DFA_H

#include<QString>
#include <QSet>
#include "nodedfa.h"

class DFA {

private:
    NodeDFA *StartState ;
    QSet<NodeDFA*> FinitStates;

public:
    //Default Constructer of DFA
    DFA();
    // Default Constructer of DFA and build DFA
    DFA(QString *KeyWord,int numberWords);

    //Build DFA
    void LoadDFA(QString *KeyWord,int numberWords);

    // Get
    NodeDFA * getStartState();
    QSet<NodeDFA*> getFinitStates();
    //Set
    void setStartState(NodeDFA *state);
    // simulate DFA (Run DFA)
    bool SimulateDFA(QString input); //Simulate DFA and return True or false

};

#endif // DFA_H
