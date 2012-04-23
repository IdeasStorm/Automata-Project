#include "NFA.h"
#include "nodedfa.h"
#include <stack>
#include"QStack"

QList<char> NFA::Alphabetic;

NFA::NFA(QList<QString>KeyWords)
{
    //NFA();
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

    LoadNFA(KeyWords);
}

//GET
NodeNFA * NFA::getStartState()
{ return StartState; }

NodeNFA * NFA::getFinit_WordsState()
{ return Finit_wordsState; }

QList<char> NFA::getAlphabetic()
{ return Alphabetic; }

QList<char> NFA::getSeparate_wordsAlphabetic()
{ return Separate_wordsAlphabetic; }

QSet<NodeNFA*> NFA::getFinitStates()
{ return FinitStates; }

QSet<NodeNFA *> NFA::getAllStates()
{ return AllStates; }

QSet<NodeNFA*> NFA::getNonFinitStates()
{ return AllStates.subtract(FinitStates); }

//Set
void NFA::setStartState(NodeNFA *state)
{ StartState = state; }

void NFA::setAlphabetic(QList<char> alphabetic)
{ Alphabetic = alphabetic; }

void NFA::setFinit_WordsState(NodeNFA *state)
{ Finit_wordsState = state; }

void NFA::addToFinitState(NodeNFA* state)
{ FinitStates.insert(state); }

void NFA::addToState(NodeNFA* state)
{ AllStates.insert(state); }

void NFA::setSeparate_wordsAlphabetic(QList<char> alphabetic)
{ Separate_wordsAlphabetic = alphabetic; }


void NFA::LoadNFA(QList<QString>KeyWords)
{
    NodeNFA *CurrentState = StartState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeDFA) q0,1,2,3....

    //for (int i=0;i<numberWords;i++)
    foreach (QString s ,KeyWords)
    {
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

void NFA::addToList(NodeNFA* node)
{
    if (!temp.contains(node))
       temp.append(node);
}

QString* NFA::setToString(QSet<NodeNFA*> set)
{
    QString* str = new QString();
    foreach (NodeNFA* node, set)
        str->append(node->getName());
    return str;
}

DFA* NFA::convertToDFA()
{
    NodeDFA* DFANode;
    DFA* dfa = new DFA();
    QMap<QPair<QString, char>, NodeDFA*> *Helper =
            new QMap<QPair<QString, char>, NodeDFA*>();
    QList< QSet<NodeNFA*> > groups;
    QHash<QString, NodeDFA*>* nodes
            = new QHash<QString, NodeDFA*>();
    QSet<NodeNFA*> set;
    set.insert(StartState);
    DFANode = new NodeDFA(0);
    nodes->insert(StartState->getName(), DFANode);
    dfa->setStartState(DFANode);
    dfa->addToState(DFANode);
    Helper->insert(QPair<QString, char>("q0", ' '), DFANode);
    groups.append(set);
    int i=0, nodeNum=0, AllNodes = 1;
    QString toSet;
    bool finite = false;
    while (i != groups.count()){
        foreach (char symbol, Alphabetic)
        {
            nodeNum = 0;
            foreach (NodeNFA* node, groups.at(i))
            {
                nodeNum++;
                QList<NodeNFA*> list = node->getNextNode(symbol);
                if (list.length() > 0)
                {
                    for(int k=0;k<list.length();k++)
                    {
                        addToList(list.at(k));
                        toSet.append(list.at(k)->getName());
                        if (list.at(k)->isFiniteState())
                            finite = true;
                    }
                }
                else
                {
                    Helper->insert(QPair<QString, char>(toSet, '?'), dfa->getSeparate_wordsState());
                    QString str = *setToString(groups.at(i));
                    nodes->value(str)->link(symbol, dfa->getSeparate_wordsState());
                }
                set = list.toSet();
                if (temp.count()>0 && nodeNum == groups.at(i).count())
                {
                    if (!groups.contains(temp.toSet()))
                    {

                        if (finite)
                        {
                            QString str = *setToString(groups.at(i));
                            nodes->value(str)->link(symbol, dfa->getFinit_WordsState());
                        }
                        else
                        {
                            groups.append(temp.toSet());
                            NodeDFA* Dfa = new NodeDFA(AllNodes);
                            AllNodes++;
                            Helper->insert(QPair<QString, char>(toSet, symbol), Dfa);
                            if (nodes->count()-1 < 1)
                                DFANode->link(symbol, Dfa);
                            else
                            {
                                QString str = *setToString(groups.at(i));
                                nodes->value(str)->link(symbol, Dfa);
                            }
                            QString str = *setToString(groups.last());
                            nodes->insert(str, Dfa);
                            dfa->addToState(Dfa);
                        }
                    }
                    /*
                    else if (symbol != ' ')
                    {
                        QString str = *setToString(groups.at(i));
                        nodes->value(str)->link(symbol, dfa->getSeparate_wordsState());
                    }*/
                    else
                    {
                        QString str = *setToString(groups.at(i));
                        QString str2 = *setToString(temp.toSet());
                        nodes->value(str)->link(symbol, nodes->value(str2));
                    }
                    temp.clear();
                    finite = false;
                    toSet.clear();
                }
            }
            temp.clear();
        }
        i++;
    }
    return dfa;
}
/*
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
    NodeNFA* start=nfa->getStartState();    //get start state .
    s.push(start);
    NodeNFA* temp;
    while(! s.empty())
    {
        temp=s.pop();
        QMultiHash<char, NodeNFA*>* nodes=temp->getNextNodes();

        foreach (NodeNFA* t , *nodes)
            s.push(t);
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
    }//end while loop
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

NFA::~NFA()
{
    foreach (NodeNFA* state , AllStates)
    {
        delete state ;
    }
}
*/
