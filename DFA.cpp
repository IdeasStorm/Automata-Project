#include "DFA.h"
#include <QLinkedList>
#include "nodedfa.h"

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

DFA::DFA(QString *KeyWords,int numberWords)
{

    DFA();
    LoadDFA(KeyWords,numberWords);
}

void DFA::LoadDFA(QString *KeyWords,int numberWords)
{
    NodeDFA *CurrentState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeDFA) q0,1,2,3....

    for (int i=0;i<numberWords;i++)
    {
        QString s = KeyWords[i];
        CurrentState = StartState;

        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {
            if (CurrentState->nextNode(s[j].cell()) == NULL)
            {
                NextState = new NodeDFA(CounterState++);
                CurrentState->link(s[j].cell(),NextState);
                CurrentState->link(' ',StartState);
                CurrentState = NextState ;
            }
            else
            {
                CurrentState = CurrentState->nextNode(s[j].cell()) ;
            }
        }

        //For set Finit of the last Node after node of ' '
        NextState = new NodeDFA(CounterState++);
        CurrentState->link(' ',NextState);
//        NextState->setFinite();
        FinitStates.insert(NextState);

    }//For number Words

    //for copy of Start State map into Finite States
    foreach (NodeDFA *state , FinitStates)
    {
        //state->setnextNodes(StartState->getnextNodes());
    }
}


bool DFA::SimulateDFA(QString input)
{/*
    NodeDFA* currentState = StartState ;
    for(int i=0;i<input.length();i++)
    {
        //currentState = transitions[qMakePair(currentState,input[i])];
        currentState = StartState->nextNode(input[i].cell());
    }
    return currentState->isFiniteState();
    */
}

// simplifies the DFA Automata
void DFA::simplify()
{
    QSet<NodeDFA*> non_finit ;//= get
    QSet<NodeDFA*> finit = getFinitStates();
    QList<QSet<NodeDFA*> > groups;
    groups.append(finit);
    groups.append(non_finit);

    foreach (QSet<NodeDFA*> group, groups) {
        if (group.count() <=1 ) continue;
        else {
            for (char symbol = 'a'; symbol < 'Z'; ++symbol) { // all symbols
                foreach (NodeDFA* node, group) { // all nodes in group e.g. [A B C]=>each of A,B,C
                    NodeDFA *next_node = node->nextNode(symbol);
                    if (group.contains(next_node)) { //same group => don't divide
                        // do nothing
                    }else{ // divide
                        // divide and make a new group
                    }
                }
            }
        }
    }


}

QSet<NodeDFA*> DFA::getFinitStates()
{
    return FinitStates;
}

