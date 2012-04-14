#ifndef DFA_H
#define DFA_H

#include<QString>
#include <QSet>
#include "nodedfa.h"
#include<QMap>


class DFA {

public:
    //Default Constructer of DFA
    DFA();
    ~DFA();
    // Default Constructer of DFA and build DFA
    DFA(QString *KeyWord,int numberWords);

    //Build DFA
    void LoadDFA(QString *KeyWord,int numberWords);

    // Get
    NodeDFA * getStartState();
    NodeDFA * getFinit_WordsState();
    NodeDFA * getSeparate_wordsState();
    QSet<NodeDFA*> getFinitStates();
    QSet<NodeDFA*> getAllStates();
    QSet<NodeDFA*> getNonFinitStates();
    QList<char> getAlphabetic();
    QList<char> getSeparate_wordsAlphabetic();
    void addToFinitState(NodeDFA* state);
    void addToState(NodeDFA* state);
    //Set
    void setStartState(NodeDFA *state);
    void setFinit_WordsState(NodeDFA *state);
    void setSeparate_wordsState(NodeDFA *state);

    void setAlphabetic(QList<char> alphabetic);
    void setSeparate_wordsAlphabetic(QList<char> alphabetic);
    // simulate DFA (Run DFA)
    QHash<QString,int>  SimulateDFA(QString input); //Simulate DFA and return True or false

    void simplify();

private:
    NodeDFA *StartState ;
    NodeDFA *Separate_wordsState;
    NodeDFA *Finit_wordsState;
    QList<char> Alphabetic ;
    QList<char> Separate_wordsAlphabetic ;
    QSet<NodeDFA*> FinitStates;
    QSet<NodeDFA*> AllStates;

};

#endif // DFA_H
