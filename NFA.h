#ifndef NFA_H
#define NFA_H

#include"DFA.h"
#include "nodenfa.h"
#include<QString>
#include <QSet>
#include <stack>

class NFA {
public:
    NFA();
    NFA(QString *KeyWords,int numberWord);
    //Build NFA
    void LoadNFA(QString *KeyWords,int numberWord);

    // Get
    NodeNFA * getStartState();
    NodeNFA * getFinit_WordsState();
  //  NodeNFA * getSeparate_wordsState();

    QSet<NodeNFA*> getFinitStates();
    QSet<NodeNFA*> getAllStates();
    QSet<NodeNFA*> getNonFinitStates();

    QList<char> getAlphabetic();
    QList<char> getSeparate_wordsAlphabetic();
    void addToFinitState(NodeNFA* state);

    void addToState(NodeNFA* state);
    //Set
    void setStartState(NodeNFA *state);
    void setFinit_WordsState(NodeNFA *state);
 //   void setSeparate_wordsState(NodeNFA *state);

    void setAlphabetic(QList<char> alphabetic);
    void setSeparate_wordsAlphabetic(QList<char> alphabetic);

    //Convert from NFA to DFA
    DFA* convertToDFA();

    //closure
     QList<NodeNFA *>* getClosure(NodeNFA*);

     //to Pure NFA
     void toPureNFA(NFA*);
     QList<NodeNFA*> * func(QList<NodeNFA*>* a,QList<NodeNFA*>*b);
private:
     NodeNFA *StartState ;
 //    NodeNFA *Separate_wordsState;
     NodeNFA *Finit_wordsState;

     QList<char> Alphabetic ;
     QList<char> Separate_wordsAlphabetic ;
     QSet<NodeNFA*> FinitStates;
     QSet<NodeNFA*> AllStates;

    QSet<QString>* usedState;
    QList<NodeNFA*> temp;

    void addToList(NodeNFA* node);
    QString* setToString(QSet<NodeNFA*> set);
};

#endif // NFA_H
