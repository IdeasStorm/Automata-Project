#include "NFA.h"
#include "nodedfa.h"

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

DFA* NFA::convertToDFA()
{
    NodeDFA* DFANode;
    DFA* dfa = new DFA();
    QMap<QPair<QString, char>, NodeDFA*> *Helper =
            new QMap<QPair<QString, char>, NodeDFA*>();
    //usedState = new QSet<QString>();
    //nodeNum = 0;
    //addToSet(StartState->getName(), DFANode, ' ');
    QList< QSet<NodeNFA*> > groups;
    QSet<NodeNFA*> set;
    set.insert(StartState);
    DFANode = new NodeDFA(0);
    dfa->setStartState(DFANode);
    Helper->insert(QPair<QString, char>("q0", ' '), DFANode);
    //Helper = DFANode;
    groups.append(set);
    //bool no_more_groups = false;
    int i=0, j=1, nodeNum=0;
    QString toSet;
    bool finite = false;
    while (j != groups.length() ){
        //no_more_groups = true;
        for (char symbol = 'a'; symbol < 'z'; ++symbol) // all symbols
        {
            foreach (NodeNFA* node, groups.at(i)) // all nodes in group e.g. [A B C]=>each of A,B,C
            {
                QList<NodeNFA*> list = node->getNextNode(symbol);
                if (list.length() > 0)
                {
                    for(int k=0;k<list.length();k++)
                    {
                        addToList(list.at(k));
                        //QString* str =new QString("q" + nodeNum);
                        toSet.append("q" + nodeNum);
                        if (list.at(k)->isFiniteState())
                            finite = true;
                    }
                }
                else
                {
                    Helper->insert(QPair<QString, char>(toSet, '?'), dfa->DeadState);
                }
                set = list.toSet();
                if (set == groups.at(i))
                {
                    if (!groups.contains(temp.toSet()))
                    {
                        groups.append(temp.toSet());
                        NodeDFA* Dfa = new NodeDFA(j);
                        if (finite)
                            Dfa->setFinite();
                        Helper->insert(QPair<QString, char>(toSet, symbol), Dfa);
                        nodeNum++;
                        //no_more_groups = false;
                        j++;
                    }
                    else
                    {
                        NodeDFA* Dfa = Helper->value(QPair<QString, char>(toSet, symbol));
                        Dfa->link(symbol, Dfa);
                    }
                    i++;
                    temp.clear();
                    finite = false;
                    toSet.clear();
                }
            }
            temp.clear();
         }

        ////////////////////////////////////////////////
        char symbol =' ';
        foreach (NodeNFA* node, groups.at(i)) // all nodes in group e.g. [A B C]=>each of A,B,C
        {
            QList<NodeNFA*> list = node->getNextNode(symbol);
            if (list.length() > 0)
            {
                for(int k=0;k<list.length();k++)
                {
                    addToList(list.at(k));
                    //QString* str =new QString("q" + nodeNum);
                    toSet.append("q" + nodeNum);
                    if (list.at(k)->isFiniteState())
                        finite = true;
                }
            }
            set = list.toSet();
            if (set == groups.at(i))
            {
                if (!groups.contains(temp.toSet()))
                {
                    groups.append(temp.toSet());
                    NodeDFA* Dfa = new NodeDFA(j);
                    if (finite)
                        Dfa->setFinite();
                    Helper->insert(QPair<QString, char>(toSet, symbol), Dfa);
                    nodeNum++;
                    //no_more_groups = false;
                    j++;
                }
                else
                {
                    NodeDFA* Dfa = Helper->value(QPair<QString, char>(toSet, symbol));
                    Dfa->link(symbol, Dfa);
                }
                i++;
                temp.clear();
                finite = false;
                toSet.clear();
            }
        }
        temp.clear();
        //////////////////////////////////
    }
    return dfa;
}
