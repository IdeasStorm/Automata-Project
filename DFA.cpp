#include "DFA.h"
#include <QLinkedList>
#include "nodedfa.h"


//GET
NodeDFA * DFA::getStartState()
{
    return StartState ;
}

NodeDFA * DFA::getFinit_WordsState()
{
    return Finit_wordsState ;
}

NodeDFA * DFA::getSeparate_wordsState()
{
    return Separate_wordsState ;
}

QList<char> DFA::getAlphabetic()
{
    return Alphabetic ;
}
QList<char> DFA::getSeparate_wordsAlphabetic()
{
    return Separate_wordsAlphabetic;
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

//Set
void DFA::setStartState(NodeDFA *state)
{
    StartState = state;
}

void DFA::setAlphabetic(QList<char> alphabetic)
{
    Alphabetic = alphabetic ;
}

void DFA::setFinit_WordsState(NodeDFA *state)
{
    Finit_wordsState = state ;
}

void DFA::setSeparate_wordsState(NodeDFA *state)
{
    Separate_wordsState = state ;
}

void DFA::addToFinitState(NodeDFA* state)
{
    FinitStates.insert(state);
}

void DFA::addToState(NodeDFA* state)
{
    AllStates.insert(state);
}

void DFA::setSeparate_wordsAlphabetic(QList<char> alphabetic)
{
    Separate_wordsAlphabetic = alphabetic;
}

/*
DFA::DFA()
{
    StartState = new NodeDFA('0') ;
    Separate_wordsState = new NodeDFA ('|'); // | ==> Loop Dead State
    StartState->link('?',Separate_wordsState);
    StartState->link(' ');
    Separate_wordsState->link(' ',StartState);
    Separate_wordsState->link('?'); // ? ==> a-->z
//
    int i = 0 ;
    for (char ch = 'a';ch<'Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');
}
*/
DFA::DFA(QString *KeyWords,int numberWords)
{
    //DFA();
    StartState = new NodeDFA('0') ;
    AllStates.insert(StartState);
    Separate_wordsState = new NodeDFA ('<'); // | ==> Loop Dead State
    AllStates.insert(Separate_wordsState);

    if (numberWords > 0)
    {
        Finit_wordsState = new NodeDFA('>');
        Finit_wordsState->setFinite();
        FinitStates.insert(Finit_wordsState);
        AllStates.insert(Finit_wordsState);
    }

    //StartState->link('?',Separate_words);
    //StartState->link(' ');
//    Separate_wordsState->link(' ',StartState);
    //Separate_words->link('?'); // ? ==> a-->z
    //
    int i = 0 ;
    for (char ch = 'a';ch <= 'z';ch++)
        Alphabetic.insert(i++,ch);
    for (char ch = 'A';ch <= 'Z';ch++)
        Alphabetic.insert(i++,ch);
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
//            if (CurrentState->nextNode(s[j].cell()) == CurrentState->nextNode('?'))
            if (CurrentState->nextNode(s[j].cell()) == NULL )
            {
                NextState = new NodeDFA(CounterState++);
                AllStates.insert(NextState);
  //              NextState->link('?',Separate_words); //  Link to Dead State
                CurrentState->link(s[j].cell(),NextState);
                //if (CurrentState->nextNode(' ')==CurrentState->nextNode('?'))
//                    CurrentState->link(' ',StartState);
                CurrentState = NextState ;
            }
            else
                CurrentState = CurrentState->nextNode(s[j].cell()) ;
        }
        //For set Finit of the last Node after node of ' '
        //NextState = new NodeDFA(CounterState++);
        //AllStates.insert(NextState);
        CurrentState->link(' ',Finit_wordsState);
        //NextState->setFinite();
        //FinitStates.insert(NextState);

    }//For number Words


    foreach (char ch,Alphabetic)
    {
        if (StartState->nextNode(ch)==NULL)
        {
            if (ch ==' ')
                StartState->link(ch,StartState);
            else
                StartState->link(ch,Separate_wordsState);
        }
    }

    foreach(NodeDFA* state,AllStates)
    {
        //add all link of a-->Z to all node
        foreach (char ch,Alphabetic)
        {
            if (state->nextNode(ch)==NULL)
            {
                if (ch ==' ')
                    state->link(ch,StartState);
                else
                {
                    if (FinitStates.contains(state))
                        state->link(ch,StartState->nextNode(ch));
                    else
                        state->link(ch,Separate_wordsState);
                }
            }
        }
    } // for a --> Z
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
    QSet<NodeDFA*> non_finit = getNonFinitStates();
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

        if (group.count() >1 ) {
            foreach (char symbol, Alphabetic) { // all symbols
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
                    //break; // not reliable
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
            bool at_first_node = false;
            bool at_last_node = false;
            // merge nodes
            NodeDFA* new_node = new NodeDFA(1); //TODO must change name
            foreach (NodeDFA* node, group) {
                foreach (char symbol, Alphabetic) { // all symbols
                    new_node->link(symbol, node->nextNode(symbol));
                }
                if (node == this->StartState)
                    at_first_node = true;
                if (node->isFiniteState())
                    at_last_node = true;
            }
            if (at_first_node)  this->StartState = new_node;
            if (at_last_node) {
                this->FinitStates.insert(new_node);
                new_node->setFinite();
            }
            this->AllStates.insert(new_node);
            foreach (NodeDFA*node, getAllStates()) {
                foreach (char symbol, Alphabetic) { // all symbols
                    if (group.contains(node->nextNode(symbol)))
                        node->link(symbol, new_node);
                }
            }
            foreach (NodeDFA* node, group){
                FinitStates.remove(node);
                AllStates.remove(node);
                delete node;
            }
        }
    }

}

DFA::~DFA()
{
    foreach (NodeDFA* state , AllStates)
    {
    }
}

