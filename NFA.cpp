#include "NFA.h"
#include "nodedfa.h"
#include <stack>
#include"QStack"

QList<char> NFA::Alphabetic;
NFA::NFA()
{
    StartState = new NodeNFA("q0") ;
    AllStates.insert(StartState);
    StartState->link(' ',StartState);

    Finit_wordsState = new NodeNFA(">");
    Finit_wordsState->setFinite();
    FinitStates.insert(Finit_wordsState);
    AllStates.insert(Finit_wordsState);

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

NFA::NFA(QList<QString>KeyWords)
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
    QMap<QPair<QString, char>, QString> *Helper =
            new QMap<QPair<QString, char>, QString>();
    QList< QSet<NodeNFA*> > groups;
    QHash<QString, NodeDFA*>* nodes
            = new QHash<QString, NodeDFA*>();
    QSet<NodeNFA*> set;
    set.insert(StartState);
    DFANode = new NodeDFA(0);
    nodes->insert(StartState->getName(), DFANode);
    dfa->setStartState(DFANode);
    dfa->addToState(DFANode);
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
                            Helper->insert(QPair<QString, char>(str, symbol),
                                           *listToString(temp));
                        }
                        else
                        {
                            groups.append(temp.toSet());
                            NodeDFA* Dfa = new NodeDFA(AllNodes);
                            AllNodes++;
                            if (nodes->count()-1 < 1)
                            {
                                DFANode->link(symbol, Dfa);
                                Helper->insert(QPair<QString, char>("q0", symbol),
                                               *listToString(temp));
                            }
                            else
                            {
                                QString str = *setToString(groups.at(i));
                                nodes->value(str)->link(symbol, Dfa);
                                Helper->insert(QPair<QString, char>(str, symbol),
                                               *listToString(temp));
                            }
                            QString str = *setToString(groups.last());
                            nodes->insert(str, Dfa);
                            dfa->addToState(Dfa);
                        }
                    }
                    else
                    {
                        QString str = *setToString(groups.at(i));
                        QString str2 = *setToString(temp.toSet());
                        nodes->value(str)->link(symbol, nodes->value(str2));
                        Helper->insert(QPair<QString, char>(str, symbol), str2);
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

NFA::~NFA()
{
    foreach (NodeNFA* state , AllStates)
    {
        delete state ;
    }
}

