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

    LoadE_NFA(KeyWords,numberWords);
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
        NextState = new NodeNFA("Epsilon");
        AllStates.insert(NextState);
        CurrentState->link('\0',NextState);
        CurrentState = NextState ;
        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {
            QString num;
            num.setNum(CounterState++);
            NextState = new NodeNFA(num);
            AllStates.insert(NextState);
            CurrentState->link(s[j].cell(),NextState);
            if (j!=s.length()-1)
                NextState->link(' ',StartState);
            CurrentState = NextState ;
        }
        //For set Finit of the last Node after node of ' '
        CurrentState->link(' ',Finit_wordsState);

    }//For number Words

    Finit_wordsState->link('\0',StartState);
}


QSet<NodeNFA *> e_NFA::getClosure(NodeNFA * state)
{
    QStack <NodeNFA*> s;
    QSet<NodeNFA*> set;
    set.insert(state);
    s.push(state);
    NodeNFA* q;
    QList<NodeNFA*> r;
    while(  ! s.empty() )
    {
        q=s.pop();
        r=q->getNextNode('\0');

        foreach (NodeNFA* n,r)
        {
            if (!set.contains(n))
            {
              set.insert(n);
              s.push(n);
            }
        }

    }
    return set;
}


//TODO Mazen
NFA* e_NFA::convertToNFA()
{
    NFA* result = new NFA();
     NodeNFA* start1=getStartState();
    NodeNFA* start2=result->getStartState();
    QStack<NodeNFA*> s;
    s.push(start1);
    QHash<QString,NodeNFA*> hash;
    hash.insert(start1->getName(),start2);
    QList<char> keys;
    QSet<NodeNFA*> temp1;
    QSet<NodeNFA*> temp2;
    QSet<NodeNFA*> temp3;
    QSet<NodeNFA*> done;
    while(!s.empty())
    {
       start1=s.pop();          done.insert(start1);
       start2=*(hash.find(start1->getName()));
       //QMultiHash<char,NodeNFA*>* nodes=start1->getNextNodes();
       keys=start1->getNextNodes()->uniqueKeys();
       temp1=getClosure(start1);
       foreach(char c ,keys)
       {
           if (c!='\0')
           {
               foreach(NodeNFA* node,temp1)
               {
                   if(node->getNextNode(c).size()!=0)
                       temp2.insert(node);
               }
               temp3=temp1.intersect(temp2);
               foreach(NodeNFA* node,temp3)
               {
                   if(!hash.values().contains(node))
                   {
                       NodeNFA* temp=new NodeNFA(node->getName());
                       result->getAllStates().insert(temp);
                       start2->link(c,temp);
                       hash.insert(node->getName(),node);
                       if(node->isFiniteState())
                       {
                         temp->setFinite();
                         result->getFinitStates().insert(temp);
                       }
                   }
                   else
                   {
                       NodeNFA* temp=*(hash.find(node->getName()));
                       start2->link(c,temp);
                   }
               }
               foreach(NodeNFA* node,*(start1->getNextNodes()))
               {
                   if(!done.contains(node))
                        s.push_back(node);
               }
               temp1.clear();
               temp2.clear();
               temp3.clear();
           }

       }
    }

    return result;
}
//TODO Mazen

e_NFA::~e_NFA()
{
    foreach (NodeNFA* state , AllStates)
    {
        delete state ;
    }
}


