#ifndef DFA_H
#define DFA_H

#include<QString>
#include <QSet>
#include "nodedfa.h"
#include<QMap>


class DFA {

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
    QHash<QString,int>  SimulateDFA(QString input); //Simulate DFA and return True or false

private:
    NodeDFA *StartState ;
    NodeDFA *DeadState;
    QSet<NodeDFA*> FinitStates;
};

#endif // DFA_H
