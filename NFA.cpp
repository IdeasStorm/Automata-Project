#include "NFA.h"

Node * NFA::getStartState()
{
    return StartState ;
}

void NFA::setStartState(Node *state)
{
    StartState = state;
}


NFA::NFA()
{
    StartState = new NodeNFA("q0") ;
    //For link Start State with self if the input a-->z
    for (char ch='a';ch<'z';ch++)
    {
        StartState->link(ch);
    }
}

NFA::NFA(QString *KeyWord,int numberWord)
{

    NFA();
    LoadNFA(KeyWord,numberWord);
}

void NFA::LoadNFA(QString *KeyWord,int numberWord)
{
    Node *CurrentState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(node) q0,1,2,3....

    for (int i=0;i<numberWord;i++)
    {
        QString s = KeyWord[i];

        CurrentState = new NodeNFA(CounterState++);
        //For link StartState with self by (/0) Obselon
        StartState->link('\0',CurrentState);

        //For link each node with another node Based input
        for (int j=0;j<s.length();j++)
        {
            NextState = new NodeNFA(CounterState++);
            CurrentState->link(s[j].cell(),NextState);
            CurrentState->link(' ',StartState);
            CurrentState = NextState ;
        }

        //For set Finit of the last node in each word ..
        NextState->setFinite();
    }//For number Word
}

