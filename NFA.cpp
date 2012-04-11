#include "NFA.h"
#include "nodedfa.h"
#include <stack>
#include"QStack"
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
            QList<NodeNFA*> nodes = StartState->getNextNode(ch);
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
