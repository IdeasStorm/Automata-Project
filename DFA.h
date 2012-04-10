#ifndef DFA_H
#define DFA_H

#include<QString>
#include "nodedfa.h"

class DFA {

private:
    NodeDFA *StartState ;

public:
    //Default Constructer of DFA
    DFA();
    // Default Constructer of DFA and build DFA
    DFA(QString *KeyWord,int numberWord);

    //Build DFA
    void LoadDFA(QString *KeyWord,int numberWord);

    // Get StartState
    NodeDFA* getStartState();
    //Set StartState
    void setStartState(NodeDFA *state);
    // simulate DFA (Run DFA)
    bool SimulateDFA(QString input); //Simulate DFA and return True or false

};

#endif // DFA_H
