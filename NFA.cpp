#include "NFA.h"

NodeNFA * NFA::getStartState()
{
    return StartState ;
}



NFA::NFA()
{
    StartState = new NodeNFA("q0") ;
    StartState->link(' ');
}

NFA::NFA(QString *KeyWords,int numberWords)
{
    StartState = new NodeNFA("q0") ;
    StartState->link(' ');
    LoadNFA(KeyWords,numberWords);
}

void NFA::LoadNFA(QString *KeyWords,int numberWords)
{
    NodeNFA *CurrentState = StartState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeDFA) q0,1,2,3....

    for (int i=0;i<numberWords;i++)
    {
        QString s = KeyWords[i];
        CurrentState = StartState;
        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {
            NextState = new NodeNFA(CounterState++);
            CurrentState->link(s[j].cell(),NextState);
            NextState->link(' ',StartState);
            CurrentState = NextState ;
        }

        //For set Finit of the last Node after node of ' '
        NextState = new NodeNFA(CounterState++);
        CurrentState->link(' ',NextState);
        NextState->setFinite();
        FinitStates.insert(NextState);

    }//For number Words

    //for copy of Start State map into Finite States
    foreach (NodeNFA *state , FinitStates)
    {
        for (char ch='a';ch<'z';ch++)
        {
            QList<NodeNFA*> nodes = StartState->nextNode(ch);
            foreach(NodeNFA* temp , nodes)
            {
                state->link(ch,temp);
            }
        }
    }
}

QSet<NodeNFA *> NFA::getFinitStates()
{
    return FinitStates;
}
