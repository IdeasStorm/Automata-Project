#ifndef E_NFA_H
#define E_NFA_H


#include "NFA.h"
#include "QStack"
#include "nodenfa.h"
#include <QString>
#include <QSet>
#include <stack>

class e_NFA {
public:
    e_NFA();
    ~e_NFA();

    e_NFA(QList<QString>);
    e_NFA* Regex(QString);
    //Build e_NFA
    void LoadE_NFA(QList<QString>KeyWords);
    // Get
    NodeNFA* getStartState();
    NodeNFA* getFinit_WordsState();

    QSet<NodeNFA*> getFinitStates();
    QSet<NodeNFA*> getAllStates();
    QSet<NodeNFA*> getNonFinitStates();

    static QList<char> getAlphabetic();
    QList<char> getSeparate_wordsAlphabetic();

    void addToFinitState(NodeNFA* state);
    void addToState(NodeNFA* state);
    void addToState(QSet<NodeNFA*> states);
    //Set
    void setStartState(NodeNFA *state);
    void setFinit_WordsState(NodeNFA *state);

    void setAlphabetic(QList<char> alphabetic);
    void setSeparate_wordsAlphabetic(QList<char> alphabetic);

    // Parses a string and returns a list of tokens
    QList<QString> getTokens(QString str);

    //Convert from e_NFA to NFA
    NFA* convertToNFA();

    //closure
    QSet<NodeNFA *> getClosure(NodeNFA*);

private:
    NodeNFA *StartState ;
    NodeNFA *Finit_wordsState;

    static QList<char> Alphabetic ;
    QList<char> Separate_wordsAlphabetic ;
    QSet<NodeNFA*> FinitStates;
    QSet<NodeNFA*> AllStates;

};

#endif // E_NFA_H

