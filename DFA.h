#ifndef DFA_H
#define DFA_H
#include<QMap>
#include<QSet>
#include<QString>
#include<QPair>

class DFA {
public: // Private
    int startState;
    QMap<QPair<int, char>, int> transitions; // for save the transition of DFA
    QSet<int> finalStates;
public:
    DFA(QString *KeyWord,int numberWord); //not to use in Search ..! only simulate for convert NFA to DFA and to build NFA
    bool SimulateDFA(QString input); //Simulate DFA and return True or false
};

#endif // DFA_H
