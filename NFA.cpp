#include "NFA.h"
#include "nodedfa.h"

NodeNFA * NFA::getStartState()
{
    return StartState ;
}

void NFA::setStartState(NodeNFA *state)
{
    StartState = state;
}


NFA::NFA()
{
    StartState = new NodeNFA("q0") ;
    StartState->link(' ');
    //For link Start State with self if the input a-->z
    for (char ch='a';ch<'z';ch++)
    {
        StartState->link(ch);
    }
}

NFA::NFA(QString *KeyWords,int numberWords)
{

    NFA();
    LoadNFA(KeyWords,numberWords);
}

void NFA::LoadNFA(QString *KeyWords,int numberWords)
{
    NodeNFA *CurrentState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeNFA) q0,1,2,3....

    /*
    for (int i=0;i<numberWords;i++)
    {
        NextState = new NodeNFA (CounterState++);
        StartState->link(KeyWords[i],NextState);
        NextState->link(' ',StartState);
        if (s.length() ==1 )
        {
            //For set Finit of the last Node after node of ' '
            CurrentState = NextState ;
            NextState = new NodeNFA(CounterState++) ;
            CurrentState->link(' ', NextState);
            NextState->setFinite();
            //for of Start State map to copy it into Finite State
        }
    }*/

    for (int i=0;i<numberWords;i++)
    {
        QString s = KeyWords[i];
        CurrentState = StartState;

        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {         
            NextState = new NodeNFA(CounterState++);
            CurrentState->link(s[j].cell(),NextState);
            CurrentState->link(' ',StartState);
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
        state->setNextNodes(StartState->getNextNodes());
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
    QMap<QPair<QString, char>, NodeDFA*> *Helper =
            new QMap<QPair<QString, char>, NodeDFA*>();
    //usedState = new QSet<QString>();
    //nodeNum = 0;
    //addToSet(StartState->getName(), DFANode, ' ');
    QList< QSet<NodeNFA*> > groups;
    QSet<NodeNFA*> set;
    set.insert(StartState);
    DFANode = new NodeDFA(0);
    Helper->insert(new QPair("q0", ' '), DFANode);
    //Helper = DFANode;
    groups.append(set);
    //bool no_more_groups = false;
    int i=0, j=1, nodeNum=0;
    QString toSet = new QString();
    bool finite = false;
    while (j != groups.length() ){
        //no_more_groups = true;
        for (char symbol = 'a'; symbol < 'Z'; ++symbol) // all symbols
        {
            foreach (NodeNFA* node, groups.at(i)) // all nodes in group e.g. [A B C]=>each of A,B,C
            {
                QList<NodeNFA*> list = node->getNextNode(symbol);
                if (list.length() > 0)
                {
                    for(int k=0;k<list.length();k++)
                    {
                        addToList(list.at(k));
                        toSet += "q" + nodeNum;
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
                        Helper->insert(QPair(toSet, symbol), Dfa);
                        nodeNum++;
                        //no_more_groups = false;
                        j++;
                    }
                    else
                    {
                        NodeDFA* Dfa = Helper[QPair(nodeNum, symbol)];
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
    }
    return DFANode;
}
