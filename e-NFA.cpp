#include "e-NFA.h"
#include "nodenfa.h"
#include <stack>
#include "QStack"

QList<char>e_NFA::Alphabetic;

e_NFA::e_NFA()
{
    StartState = new NodeNFA("q0") ;
    AllStates.insert(StartState);
    StartState->link(' ',StartState);

    Finit_wordsState = new NodeNFA(">");
    Finit_wordsState->setFinite();
    FinitStates.insert(Finit_wordsState);
    AllStates.insert(Finit_wordsState);
    //
    int i = 0 ;
    for (char ch = 'a';ch<='z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    for (char ch = 'A';ch<='Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');
}

e_NFA::e_NFA(QString *KeyWords,int numberWords)
{
    StartState = new NodeNFA("q0") ;
    AllStates.insert(StartState);
    StartState->link(' ',StartState);
    if (numberWords > 0)
    {
        Finit_wordsState = new NodeNFA(">");
        Finit_wordsState->setFinite();
        FinitStates.insert(Finit_wordsState);
        AllStates.insert(Finit_wordsState);
    }
    //
    int i = 0 ;
    for (char ch = 'a';ch<='z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    for (char ch = 'A';ch<='Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');

}

//GET
NodeNFA* e_NFA::getStartState()
{ return StartState; }

NodeNFA* e_NFA::getFinit_WordsState()
{ return Finit_wordsState; }

QList<char>e_NFA::getAlphabetic()
{ return Alphabetic; }

QList<char>e_NFA::getSeparate_wordsAlphabetic()
{ return Separate_wordsAlphabetic; }

QSet<NodeNFA*>e_NFA::getFinitStates()
{ return FinitStates; }

QSet<NodeNFA *>e_NFA::getAllStates()
{ return AllStates; }

QSet<NodeNFA*>e_NFA::getNonFinitStates()
{ return AllStates.subtract(FinitStates); }

//Set
void e_NFA::setStartState(NodeNFA *state)
{ StartState = state; }

void e_NFA::setAlphabetic(QList<char> alphabetic)
{ Alphabetic = alphabetic; }

void e_NFA::setFinit_WordsState(NodeNFA *state)
{ Finit_wordsState = state; }

void e_NFA::addToFinitState(NodeNFA* state)
{ FinitStates.insert(state); }

void e_NFA::addToState(NodeNFA* state)
{ AllStates.insert(state); }

void e_NFA::setSeparate_wordsAlphabetic(QList<char> alphabetic)
{ Separate_wordsAlphabetic = alphabetic; }

void e_NFA::LoadE_NFA(QString *KeyWords,int numberWords)
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
            AllStates.insert(NextState);
            CurrentState->link(s[j].cell(),NextState);
            if (j!=s.length()-1)
                NextState->link(' ',StartState);
            CurrentState = NextState ;
        }
        //For set Finit of the last Node after node of ' '
        CurrentState->link(' ',Finit_wordsState);

    }//For number Words

    //for copy of Start State map into Finite States
    foreach (NodeNFA *state , FinitStates)
    {
        foreach (char ch ,Alphabetic)
        {
            QList<NodeNFA*> nodes = StartState->getNextNode(ch);
            foreach(NodeNFA* temp , nodes)
            {
                state->link(ch,temp);
            }
        }
    }
}



/*
//TODO Mazen
NFA* e_NFA::convertToNFA()
{
    NFA* resault = new NFA();

    return resault;
}
//TODO Mazen

QList<NodeNFA *>* e_NFA::getClosure(NodeNFA * state)
{
    QStack <NodeNFA*> s;
    QList<NodeNFA*>* list;         // the final set
    list->push_front(state);
    s.push(state);
    NodeNFA* q;
    QList<NodeNFA*> r;
    while(  ! s.empty() )
    {

        q=s.pop();
        r=q->getNextNode('\0');

        foreach (NodeNFA* n,r)
        {
            if (!list->contains(n))
            {
              list->push_front(n);
              s.push(n);
            }
        }
    }
    return list;

}
*/
e_NFA::~e_NFA()
{
    foreach (NodeNFA* state , AllStates)
    {
        delete state ;
    }
}

