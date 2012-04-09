#include "DFA.h"

Node * DFA::getStartState()
{
    return StartState ;
}

void DFA::setStartState(Node *state)
{
    StartState = state;
}


DFA::DFA()
{
    StartState = new Node("q0") ;
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
    Node *CurrentState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(node)

    for (int i=0;i<numberWord;i++)
    {
        QString s = KeyWord[i];

//        CurrentState = new Node(CounterState++);
        //For link StartState with self by (e)
        StartState->link('e',CurrentState);

        //For link each node with another node Based input
        for (int j=0;j<s.length();j++)
        {
            //NextState = new Node(CounterState++);
            CurrentState->link(s[j].cell(),NextState);
            CurrentState->link(' ',StartState);
            CurrentState = NextState ;
        }

        //For set Finit of the last node in each word ..
        NextState->setFinite();
    }//For number Word
}
/*
bool DFA::SimulateDFA(QString input)
{
    int currentState = startState ;
    for(int i=0;i<input.length();i++)
    {
        currentState = transitions[qMakePair(currentState,input[i])];
    }
    return finalStates.find(currentState)!= finalStates.end();
}

*/
