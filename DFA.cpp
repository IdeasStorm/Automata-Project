#include "DFA.h"
#include <QSet>
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
typename QSet<NodeDFA*> NodeGroup;
// simplifies the DFA Automata
void DFA::simplify()
{
    NodeGroup non_finit = getNonFinitNodes();
    NodeGroup finit = getFinitNodes();
    QSet<NodeGroup> groups;
    groups.insert(non_finit);
    groups.insert(finit);

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
