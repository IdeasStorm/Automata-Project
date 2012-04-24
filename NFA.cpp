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


QMultiMap<QString, QPair<QString, char> >* NFA::getConvertTable()
{
    return convertTable;
}

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

QString* NFA::listToString(QList<NodeNFA*> list)
{
    QString* str = new QString();
    foreach (NodeNFA* node, list)
        str->append(node->getName());
    return str;
}

DFA* NFA::convertToDFA()
{
    NodeDFA* DFANode;
    DFA* dfa = new DFA();
    convertTable = new QMultiMap<QString, QPair<QString, char> >();
    //list of new groups of nodes
    QList< QSet<NodeNFA*> > groups;
    //hash take gruops of NFA retrun DFA node
    QHash<QString, NodeDFA*>* nodes
            = new QHash<QString, NodeDFA*>();
    //Helper Set
    QSet<NodeNFA*> set;
    //insert Start State
    set.insert(StartState);
    //make NodeDFA start state of NFA
    DFANode = new NodeDFA(0);
    nodes->insert(StartState->getName(), DFANode);
    dfa->setStartState(DFANode);
    //add the node to states of DFA
    dfa->addToState(DFANode);
    //add first set
    groups.append(set);
    //initialized variables
    int i=0, nodeNum=0, AllNodes = 1;
    //Helper String
    QString toSet;
    //useed it to know if node is finite
    bool finite = false;
    //while we dont see all groups
    while (i != groups.count()){
        //for all symbol in language
        foreach (char symbol, Alphabetic)
        {
            nodeNum = 0;
            //for all nodes in groups
            foreach (NodeNFA* node, groups.at(i))
            {
                nodeNum++;
                //get all nodes if symbol come
                QList<NodeNFA*> list = node->getNextNode(symbol);
                //if there is node if symbol comes
                if (list.length() > 0)
                {
                    //for all nodes
                    for(int k=0;k<list.length();k++)
                    {
                        //add it to temp list to bulid Union
                        addToList(list.at(k));
                        //add its name to string
                        toSet.append(list.at(k)->getName());
                        //if node finite
                        if (list.at(k)->isFiniteState())
                            finite = true;
                    }
                }
                else    //if there is no node if symbol comes
                {
                    //get the nodes name
                    QString str = *setToString(groups.at(i));
                    //get DFA node of this group and link it with separate words
                    nodes->value(str)->link(symbol, dfa->getSeparate_wordsState());
                }
                //convert to set
                set = list.toSet();
                //if there is node and the node is the last one in group
                if (temp.count()>0 && nodeNum == groups.at(i).count())
                {
                    //if node not in group
                    if (!groups.contains(temp.toSet()))
                    {
                        //there is finit node in this groups
                        if (finite)
                        {
                            //get the groups
                            QString str = *setToString(groups.at(i));
                            //link the DFA node of this group with Finit Word
                            nodes->value(str)->link(symbol, dfa->getFinit_WordsState());
                            //add to table
                            convertTable->insert(str, QPair<QString, char>(*listToString(temp), symbol));
                        }
                        else    //if node not finite
                        {
                            //add to groups
                            groups.append(temp.toSet());
                            //Create new DFA node
                            NodeDFA* Dfa = new NodeDFA(AllNodes);
                            AllNodes++;
                            //if this is first add
                            if (nodes->count()-1 < 1)
                            {
                                DFANode->link(symbol, Dfa);
                                convertTable->insert("q0", QPair<QString, char>(*listToString(temp), symbol));
                            }
                            else //if not first add
                            {
                                //get the group and link the DFA node of this group with Finit Word
                                QString str = *setToString(groups.at(i));
                                nodes->value(str)->link(symbol, Dfa);
                                //add to table
                                convertTable->insert(str, QPair<QString, char>(*listToString(temp), symbol));
                            }
                            //get the last one and add it to nodes
                            QString str = *setToString(groups.last());
                            nodes->insert(str, Dfa);
                            dfa->addToState(Dfa);
                        }
                    }
                    else    //if group already exist
                    {
                        //get group and node to connect them
                        QString str = *setToString(groups.at(i));
                        QString str2 = *setToString(temp.toSet());
                        nodes->value(str)->link(symbol, nodes->value(str2));
                        //add to table
                        convertTable->insert(str, QPair<QString, char>(str2, symbol));
                    }
                    temp.clear();
                    finite = false;
                    toSet.clear();
                }
            }
            temp.clear();
        }
        //see the next group
        i++;
    }
    return dfa;
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
