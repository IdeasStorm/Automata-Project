#include "NFA.h"

NodeNFA * NFA::getStartState()
{
    return StartState ;
}

void NFA::setStartState(NodeNFA *state)
{
    StartState = state;
}


NFA::NFA()
{
    StartState = new NodeNFA("q0") ;
    StartState->link(' ');
    //For link Start State with self if the input a-->z
    for (char ch='a';ch<'z';ch++)
    {
        StartState->link(ch);
    }
}

NFA::NFA(QString *KeyWords,int numberWords)
{

    NFA();
    LoadNFA(KeyWords,numberWords);
}

void NFA::LoadNFA(QString *KeyWords,int numberWords)
{
    NodeNFA *CurrentState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeNFA) q0,1,2,3....

    /*
    for (int i=0;i<numberWords;i++)
    {
        NextState = new NodeNFA (CounterState++);
        StartState->link(KeyWords[i],NextState);
        NextState->link(' ',StartState);
        if (s.length() ==1 )
        {
            //For set Finit of the last Node after node of ' '
            CurrentState = NextState ;
            NextState = new NodeNFA(CounterState++) ;
            CurrentState->link(' ', NextState);
            NextState->setFinite();
            //for of Start State map to copy it into Finite State
        }
    }*/

    for (int i=0;i<numberWords;i++)
    {
        QString s = KeyWords[i];
        CurrentState = StartState;

        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {         
            NextState = new NodeNFA(CounterState++);
            CurrentState->link(s[j].cell(),NextState);
            CurrentState->link(' ',StartState);
            CurrentState = NextState ;
        }
        //For set Finit of the last Node after node of ' '
        NextState = new NodeNFA(CounterState++);
        CurrentState->link(' ',NextState);
        NextState->setFinite();
        FinitStates.insert(NextState);

    }//For number Word

    //for copy of Start State map to copy into Finite States
    foreach (NodeNFA *state , FinitStates)
    {
        state->setnextNodes(StartState->getnextNodes());
    }

}

QSet<NodeNFA *> NFA::getFinitStates()
{
    return FinitStates;
}
