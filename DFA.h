#ifndef DFA_H
#define DFA_H

#include<QString>
#include "node.h"
#include "nodedfa.h"

class DFA {

private:
    Node *StartState ;

public:
    //Default Constructer of DFA
    DFA();
    // Default Constructer of DFA and build DFA
    DFA(QString *KeyWord,int numberWord);

    //Build DFA
    void LoadDFA(QString *KeyWord,int numberWord);

    // Get StartState
    Node * getStartState();
    //Set StartState
    void setStartState(Node *state);
    // simulate DFA (Run DFA)
    bool SimulateDFA(QString input); //Simulate DFA and return True or false

};

#endif // DFA_H
