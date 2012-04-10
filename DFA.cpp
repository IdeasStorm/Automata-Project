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
    bool no_more_groups = false;
    //(QSet<NodeDFA*> group, groups)
    while (!no_more_groups){
        // just giving a default value
        no_more_groups = true;
        if (group.count() <=1 ) continue;
        else {
            for (char symbol = 'a'; symbol < 'Z'; ++symbol) { // all symbols
                // initialising a group to add out-going nodes to it
                QSet<NodeDFA*> new_group;

                foreach (NodeDFA* node, group) { // all nodes in group e.g. [A B C]=>each of A,B,C
                    // the next state delta(node,symbol)
                    NodeDFA *next_node = node->nextNode(symbol);

                    if (!group.contains(next_node)) { //same group => divide
                        // dividing and making a new group
                        no_more_groups = false;
                        // adding new out-going node to the temp new_group
                        new_group.insert(node);
                    }
                }
                // checking whether there is any out-going nodes
                if (!no_more_groups) {
                    // extracting out-going nodes from the old group
                    group.subtract(new_group);
                    // adding the new group
                    groups.append(new_group);
                    break; // not reliable
                }
            }
        }
    }


}

QSet<NodeDFA*> DFA::getFinitStates()
{
    return FinitStates;
}

