#include "DFA.h"
#include <QLinkedList>
#include "nodedfa.h"

NodeDFA * DFA::getStartState()
{
    return StartState ;
}

void DFA::setStartState(NodeDFA *state)
{
    StartState = state;
}
QList<char> DFA::getAlphabetic()
{
    return Alphabetic ;
}

DFA::DFA()
{
    StartState = new NodeDFA('0') ;
    Separate_words = new NodeDFA ('|'); // | ==> Loop Dead State
    StartState->link('?',Separate_words);
    StartState->link(' ');
    Separate_words->link(' ',StartState);
    Separate_words->link('?'); // ? ==> a-->z
//
    int i = 0 ;
    for (char ch = 'a';ch<'Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');
}

DFA::DFA(QString *KeyWords,int numberWords)
{
    //DFA();
    StartState = new NodeDFA('0') ;
    AllStates.insert(StartState);
    Separate_words = new NodeDFA ('|'); // | ==> Loop Dead State
    StartState->link('?',Separate_words);
    StartState->link(' ');
    Separate_words->link(' ',StartState);
    Separate_words->link('?'); // ? ==> a-->z
    //
    int i = 0 ;
    for (char ch = 'a';ch<'Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');

    LoadDFA(KeyWords,numberWords);
}

void DFA::LoadDFA(QString *KeyWords,int numberWords)
{
    NodeDFA *CurrentState = StartState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeDFA) q0,1,2,3....

    for (int i=0;i<numberWords;i++)
    {
        QString s = KeyWords[i];
        CurrentState = StartState;
        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {
            if (CurrentState->nextNode(s[j].cell()) == CurrentState->nextNode('?'))
            {
                NextState = new NodeDFA(CounterState++);
                AllStates.insert(NextState);
                NextState->link('?',Separate_words); //  Link to Dead State
                CurrentState->link(s[j].cell(),NextState);
                if (CurrentState->nextNode(' ')==CurrentState->nextNode('?'))
                    CurrentState->link(' ',StartState);
                CurrentState = NextState ;
            }
            else
                CurrentState = CurrentState->nextNode(s[j].cell()) ;
        }
        //For set Finit of the last Node after node of ' '
        NextState = new NodeDFA(CounterState++);
        AllStates.insert(NextState);
        CurrentState->link(' ',NextState);
        NextState->setFinite();
        FinitStates.insert(NextState);

    }//For number Words

    //for copy of Start State map into Finite States
    foreach (NodeDFA *state , FinitStates)
    {
        for (char ch='a';ch<'z';ch++)
            state->link(ch,StartState->nextNode(ch));
    //    state->setnextNodes(StartState->getnextNodes());
    }
}


QHash<QString,int> DFA::SimulateDFA(QString input)
{
    QHash<QString,int> resault ;
    QString str ;
    NodeDFA *CurrentState = StartState ;
    for (int i=0;i<input.length();i++)
    {
        str +=input[i];
        CurrentState = CurrentState->nextNode(input[i].cell());

         // to caculate the last word
        if ((i ==input.length()-1)&&(input[i]!=' '))
        {
            CurrentState = CurrentState->nextNode(' ');
            str+=" ";
        }
        // To add Key Words to Resault Hash
        if (CurrentState->isFiniteState())
        {
            if(resault.contains(str))
            {
                int value = resault.value(str);
                resault.insert(str,++value);
            }
            else
                resault.insert(str,1);
            str = "" ;
        }
        if (input[i]==' ')
            str = "";
    }
    return resault ;
}

// simplifies the DFA Automata
void DFA::simplify()
{
    QSet<NodeDFA*> non_finit ;//= get
    QSet<NodeDFA*> finit = getFinitStates();
    QList<QSet<NodeDFA*> > groups;
    groups.append(finit);
    groups.append(non_finit);
    bool no_more_groups = false;
    QList<QSet<NodeDFA*> >::iterator group_iter = groups.begin();
    while (!no_more_groups){
        QSet<NodeDFA*> group = *group_iter;
        // just giving a default value
        no_more_groups = true;
        if (group.count() <=1 ) continue;
        else {
            for (char symbol = 'a'; symbol < 'Z'; ++symbol) { // all symbols
                // initialising a group to add out-going nodes to it
                QSet<NodeDFA*> new_group;

                foreach (NodeDFA* node, group) { // all nodes in group e.g. [A B C]=>each of A,B,C
                    // the next state delta(node,symbol)
                    NodeDFA *next_node = node->nextNode(symbol);

                    if (!group.contains(next_node)) { //same group => divide
                        // dividing and making a new group
                        no_more_groups = false;
                        // adding new out-going node to the temp new_group
                        new_group.insert(node);
                    }
                }
                // checking whether there is any out-going nodes
                if (!no_more_groups) {
                    // extracting out-going nodes from the old group
                    group.subtract(new_group);
                    // adding the new group
                    groups.append(new_group);
                    break; // not reliable
                }
            }
        }
        if (group_iter != groups.end())
            group_iter++;
        else
            no_more_groups = true;
    }
    // searching for groups including more than one node
    foreach (QSet<NodeDFA*> group, groups) {
        if (group.count() > 1) {
            // merge nodes
            NodeDFA* new_node = new NodeDFA(1); //TODO must change name
            foreach (NodeDFA* node, group) {
                for (char symbol = 'a'; symbol < 'Z'; ++symbol) { // all symbols
                    new_node->link(symbol, node->nextNode(symbol));
                }
            }
            foreach (NodeDFA*node, getAllStates()) {
                for (char symbol = 'a'; symbol < 'Z'; ++symbol) { // all symbols
                    if (group.contains(node->nextNode(symbol)))
                        node->link(symbol, new_node);
                }
            }
        }
    }

}

QSet<NodeDFA*> DFA::getFinitStates()
{
    return FinitStates;
}

QSet<NodeDFA *> DFA::getAllStates()
{
    return AllStates;
}
QSet<NodeDFA*> DFA::getNonFinitStates()
{
    return AllStates.subtract(FinitStates);
}

