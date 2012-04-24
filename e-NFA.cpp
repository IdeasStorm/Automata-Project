#include "e-NFA.h"

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

e_NFA::e_NFA(QList<QString>KeyWords)
{
    StartState = new NodeNFA("q0") ;
    AllStates.insert(StartState);
    StartState->link(' ',StartState);
    if (KeyWords.length() > 0)
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

    LoadE_NFA(KeyWords);
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

void e_NFA::LoadE_NFA(QList<QString>KeyWords)
{
    NodeNFA *CurrentState = StartState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeDFA) q0,1,2,3....

    //for (int i=0;i<numberWords;i++)
    foreach (QString s ,KeyWords)
    {
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


NFA* e_NFA::convertToNFA()
{
    NFA* result = new NFA();
    NodeNFA* start1=getStartState();
    NodeNFA* start2=result->getStartState();
    QStack<NodeNFA*> s;
    s.push(start1);
    QHash<QString,NodeNFA*> hash;
    hash.insert(start2->getName(),start2);
    QSet<NodeNFA*> temp1;
    QSet<NodeNFA*> temp2;
    QSet<NodeNFA*> temp3;
    QSet<NodeNFA*> done;

    while(!s.empty())
    {
       start1=s.pop();
       done.insert(start1);
       start2=hash.value(start1->getName());
       temp1=getClosure(start1);
       if(temp1.size()==1)
           exit(0);
       foreach(char c ,getAlphabetic())
       {
           if (c!='\0')
           {
                temp2.clear();
                temp3.clear();
               foreach(NodeNFA* node,temp1)
               {
                   foreach(NodeNFA* n,node->getNextNode(c))
                       temp2.insert(n);
               }
               foreach(NodeNFA* n,temp2)
               {
                   QSet<NodeNFA*> myset=getClosure(n);
                   foreach(NodeNFA* node,myset)
                   {
                       if(!temp3.contains(node))
                          temp3.insert(node);
                   }
               }
               foreach(NodeNFA* node,temp3)
               {
                   if(!(hash.keys().contains(node->getName())))
                   {
                       NodeNFA* temp=new NodeNFA(node->getName());
                       start2->link(c,temp);
                       result->getAllStates().insert(temp);
                       hash.insert(temp->getName(),temp);
                       if(node->isFiniteState())
                       {
                         temp->setFinite();
                         result->getFinitStates().insert(temp);
                       }
                   }
                   else
                   {
                       NodeNFA* temp=hash.value(node->getName());
                       start2->link(c,temp);
                     //  exit(0);
                   }
               }
               foreach(NodeNFA* node,temp3)
               {
                   if (!done.contains(node))
                        s.push_back(node);
               }
           }
       }
      // QMultiHash<char,NodeNFA*> *nodes=start1->getNextNodes();

        temp1.clear();
    }
   // if(result->getAllStates().size()==2)
     //   exit(0);
    return result;
}

e_NFA::~e_NFA()
{
    foreach (NodeNFA* state , AllStates)
    {
        delete state ;
    }
}

