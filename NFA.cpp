#include "NFA.h"
#include "nodedfa.h"
#include <stack>
#include"QStack"


//GET
NodeNFA * NFA::getStartState()
{
    return StartState ;
}

NodeNFA * NFA::getFinit_WordsState()
{
    return Finit_wordsState ;
}

NodeNFA * NFA::getSeparate_wordsState()
{
    return Separate_wordsState ;
}

QList<char> NFA::getAlphabetic()
{
    return Alphabetic ;
}
QList<char> NFA::getSeparate_wordsAlphabetic()
{
    return Separate_wordsAlphabetic;
}

QSet<NodeNFA*> NFA::getFinitStates()
{
    return FinitStates;
}

QSet<NodeNFA *> NFA::getAllStates()
{
    return AllStates;
}
QSet<NodeNFA*> NFA::getNonFinitStates()
{
    return AllStates.subtract(FinitStates);
}


//Set
void NFA::setStartState(NodeNFA *state)
{
    StartState = state;
}

void NFA::setAlphabetic(QList<char> alphabetic)
{
    Alphabetic = alphabetic ;
}

void NFA::setFinit_WordsState(NodeNFA *state)
{
    Finit_wordsState = state ;
}

void NFA::setSeparate_wordsState(NodeNFA *state)
{
    Separate_wordsState = state ;
}

void NFA::addToFinitState(NodeNFA* state)
{
    FinitStates.insert(state);
}

void NFA::addToState(NodeNFA* state)
{
    AllStates.insert(state);
}

void NFA::setSeparate_wordsAlphabetic(QList<char> alphabetic)
{
    Separate_wordsAlphabetic = alphabetic;
}



/*
NFA::NFA()
{
    StartState = new NodeNFA('0') ;
    StartState->link(' ');
    //
        int i = 0 ;
        for (char ch = 'a';ch<'Z';ch++)
        {
            Alphabetic.insert(i++,ch);
        }
        Alphabetic.insert(i++,' ');

}
*/
NFA::NFA(QString *KeyWords,int numberWords)
{
    //NFA();
    StartState = new NodeNFA('0') ;
    AllStates.insert(StartState);
    //Separate_wordsState = new NodeNFA ('<'); // | ==> Loop Dead State
    //Separate_wordsState->link(' ',StartState);
    //AllStates.insert(Separate_wordsState);

    if (numberWords > 0)
    {
        Finit_wordsState = new NodeNFA('>');
        Finit_wordsState->setFinite();
        FinitStates.insert(Finit_wordsState);
        AllStates.insert(Finit_wordsState);
        Finit_wordsState->link(' ',StartState);
    }
    //
    int i = 0 ;
    for (char ch = 'a';ch<'z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    for (char ch = 'A';ch<'Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');

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

void NFA::addToSet(QString name, NodeDFA* dfa, char ch)
{
    usedState->insert(name);
    if (dfa == NULL)
        dfa = new NodeDFA(nodeNum);
    else
        dfa->link(ch, new NodeDFA(nodeNum));
    nodeNum++;
}

void NFA::addToList(NodeNFA* node)
{
    if (!temp.contains(node))
       temp.append(node);
}

QList<NodeNFA*>* NFA::getValueNodes(NodeNFA* node, NodeDFA* dfa)
{
    QList<char> keys = node->getNextNodes()->uniqueKeys();
    for(int i=0;i<keys.length();i++)
    {
        QList<NodeNFA*> states = node->getNextNode(keys.at(i));
        QString* toSet = new QString();
        for(int j=0;j<states.length();j++)
        {
            toSet->append(states.at(i)->getName());
            addToList(states.at(i));
        }
        addToSet(*toSet, dfa, keys.at(i));
    }
}

NodeDFA* NFA::convertToDFA()
{

    NodeDFA* DFANode;
    usedState = new QSet<QString>();
    nodeNum = 0;
    addToSet(StartState->getName(), DFANode, ' ');
}


QList<NodeNFA *>*  NFA::getClosure(NodeNFA * state)
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



void NFA::toPureNFA(NFA * nfa)
{

QStack<NodeNFA*> s;
NodeNFA* start=nfa->getStartState();
s.push(start);
NodeNFA* temp;
while(! s.empty())
{
    temp=s.pop();
    QMultiHash<char, NodeNFA*>* nodes=temp->getNextNodes();// after

    foreach (NodeNFA* t , *nodes)
    {
        s.push(t);

    }

  QList<char> keys=nodes->uniqueKeys();

QList<NodeNFA*>* temp1=getClosure(temp);
QList<NodeNFA*>* temp2=new QList<NodeNFA*>();

temp->setNextNodes(new QMultiHash<char,NodeNFA*>());
  foreach(char c,keys)
  {
      if(c!='\0')
      {

              foreach(NodeNFA* t,*temp1)
              {
                  if(t->isFiniteState())
                      temp->setFinite();
                  if(t->getNextNode(c).size()==0)
                      temp2->append(t);

              }


      QList<NodeNFA*>* temp3=func(temp1,temp2);
      QList<NodeNFA*>* temp4=new QList<NodeNFA*>();
      foreach(NodeNFA* b,*temp3)
      {


        temp4->append(*getClosure(b));

      }

          foreach(NodeNFA* nfa ,*temp4)
          {
              temp->link(c,nfa);

          }

       }

  }
temp1->clear();
temp2->clear();



}

}



QList<NodeNFA*> * NFA::func(QList<NodeNFA*>* a,QList<NodeNFA*>*b)
{
    QList<NodeNFA*> *list=new QList<NodeNFA*>();

    foreach (NodeNFA* t,*a)
    {
        if(! b->contains(t))
          list->append(t);


    }

return list;
}
