#ifndef NFA_H
#define NFA_H

#include"DFA.h"
#include "nodenfa.h"

#include<QString>
#include "node.h"

class NFA {

private:
    Node *StartState ;

public:
    //Default Constructer of NFA
    NFA();
    // Default Constructer of NFA and build NFA
    NFA(QString *KeyWord,int numberWord);

    //Build DFA
    void LoadNFA(QString *KeyWord,int numberWord);

    // Get StartState
    Node * getStartState();
    //Set StartState
    void setStartState(Node *state);

};

#endif // NFA_H
