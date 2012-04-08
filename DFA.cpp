#include "DFA.h"

DFA::DFA(QString *input,int numberWord) //not for use in Work Case
{
    startState = 0 ; //q0
    int currentState =startState ;
    for (int i=0;i<numberWord;i++)
    {
        QString s = input[i];
        //transitions.insert(new QPair<int,char>(startState,s[0]),++currentState);
        for (int j=1;j<s.length();j++)
        {
          //  transitions.insert(new QPair<int,char>(currentState,s[j]),++currentState);
        }
        finalStates.insert(currentState);
    }
}

bool DFA::SimulateDFA(QString input)
{
    int currentState = startState ;
    for(int i=0;i<input.length();i++)
    {
        currentState = transitions[qMakePair(currentState,input[i])];
    }
    return finalStates.find(currentState)!= finalStates.end();
}
