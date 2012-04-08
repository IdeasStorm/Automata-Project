#ifndef NFA_H
#define NFA_H
#include"DFA.h"
#include<QMap>
#include<QSet>
#include<QString>
#include<QPair>

class NFA {
public: // Private
    int startState;
    QMap<QPair<int, QCharRef>, int> transitions; // for save the transition of DFA
    QSet<int> finalStates;
public:
    NFA(QString *input,int numberWord); //not to use in Search ..! only simulate for convert NFA to DFA and to build NFA
    DFA ConvertToDFA();
};

#endif // NFA_H
