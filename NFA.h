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
    ~NFA();
    NFA(QList<QString>KeyWords);
    //Build NFA
    void LoadNFA(QList<QString>KeyWords);

    // Get
    NodeNFA* getStartState();
    NodeNFA* getFinit_WordsState();
    QMultiMap<QString, QPair<QString, char> >* getConvertTable();
    //NodeNFA * getSeparate_wordsState();
  
    void addState(NodeNFA*);
    void addFinitState(NodeNFA*);
    QSet<NodeNFA*> getFinitStates();
    QSet<NodeNFA*> getAllStates();
    QSet<NodeNFA*> getNonFinitStates();
	
    static QList<char> getAlphabetic();
    QList<char> getSeparate_wordsAlphabetic();
    void addToFinitState(NodeNFA* state);
	
    void addToState(NodeNFA* state);
    //Set
    void setStartState(NodeNFA *state);
    void setFinit_WordsState(NodeNFA *state);
	//void setSeparate_wordsState(NodeNFA *state);

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
    //NodeNFA *Separate_wordsState;
    NodeNFA *Finit_wordsState;

    static QList<char> Alphabetic ;
    QList<char> Separate_wordsAlphabetic ;
    QSet<NodeNFA*> FinitStates;
    QSet<NodeNFA*> AllStates;

    QSet<QString>* usedState;
    QList<NodeNFA*> temp;

    void addToList(NodeNFA* node);
    QString* setToString(QSet<NodeNFA*> set);
    QString* listToString(QList<NodeNFA*> list);
    QMultiMap<QString, QPair<QString, char> > *convertTable;
};

#endif // NFA_H
