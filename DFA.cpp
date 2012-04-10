#include "DFA.h"

NodeDFA * DFA::getStartState()
{
    return StartState ;
}

void DFA::setStartState(NodeDFA *state)
{
    StartState = state;
}


DFA::DFA()
{
    StartState = new NodeDFA("q0") ;
    //For link Start State with self if the input a-->z
    for (char ch='a';ch<'z';ch++)
    {
        StartState->link(ch);
    }
}

DFA::DFA(QString *KeyWord,int numberWord)
{

    DFA();
    LoadDFA(KeyWord,numberWord);
}

void DFA::LoadDFA(QString *KeyWord,int numberWord)
{
    NodeDFA *CurrentState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(node)

    for (int i=0;i<numberWord;i++)
    {
        QString s = KeyWord[i];

        CurrentState = new NodeDFA(CounterState++);

        //For link each node with another node Based input
        for (int j=0;j<s.length();j++)
        {
            NextState = new NodeDFA(CounterState++);
            CurrentState->link(s[j].cell(),NextState);
            CurrentState->link(' ',StartState);
            CurrentState = NextState ;
        }
        //For set Finit of the last node in each word ..
        NextState = new NodeDFA(CounterState++);
        CurrentState->link(' ',NextState);
        NextState->setFinite();
    }//For number Word
}


bool DFA::SimulateDFA(QString input)
{
    NodeDFA* currentState = StartState ;
    for(int i=0;i<input.length();i++)
    {
        //currentState = transitions[qMakePair(currentState,input[i])];
        currentState = StartState->nextNode(input[i].cell());
    }
    return currentState->isFiniteState();
}


QString NodeDFA::getName()
{ return name; }

void NodeDFA::setFinite()
{ finite = true; }

void NodeDFA::setNotFinite()
{ finite = false; }

bool NodeDFA::isFiniteState()
{ return finite; }
