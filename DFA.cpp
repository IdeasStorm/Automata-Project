#include "DFA.h"
#include <QSet>
#include "nodedfa.h"

DFA::DFA(QString *KeyWord,int numberWord) //not for use in Work Case
{
    startState = 0 ; //q0
    int currentState =startState ;
    for (int i=0;i<numberWord;i++)
    {
        QString s = KeyWord[i];
        //transitions.insert(qMakePair(startState,(QChar)s[0]),++currentState);
        for (int j=1;j<s.length();j++)
        {
            //transitions.insert(qMakePair(currentState,(QChar)s[j]),++currentState);
        }
        finalStates.insert(currentState);
    }
}

bool DFA::SimulateDFA(QString input)
{
    int currentState = startState ;
    for(int i=0;i<input.length();i++)
    {
        //currentState = transitions[qMakePair(currentState,input[i])];
    }
    return finalStates.find(currentState)!= finalStates.end();
}
typename QSet<NodeDFA*> NodeGroup;
// simplifies the DFA Automata
void DFA::simplify()
{
    NodeGroup non_finit = getNonFinitNodes();
    NodeGroup finit = getFinitNodes();
    QSet<NodeGroup> groups;
    groups.insert(non_finit);
    groups.insert(finit);

    foreach (QSet<NodeDFA*> group, groups) {
        if (group.count() <=1 ) continue;
        else {
            for (char symbol = 'a'; symbol < 'Z'; ++symbol) { // all symbols
                foreach (NodeDFA* node, group) { // all nodes in group e.g. [A B C]=>each of A,B,C
                    NodeDFA *next_node = node->nextNode(symbol);
                    if (group.contains(next_node)) { //same group => don't divide
                        // do nothing
                    }else{ // divide
                        // divide and make a new group
                    }
                }
            }
        }
    }


}
