#include "NFA.h"

NFA::NFA(QString *KeyWord,int numberWord) //not for use in Work Case
{
    startState = 0 ; //q0
    int currentState =startState ;
    for (int i=0;i<numberWord;i++)
    {
        QString s = KeyWord[i];
        //transitions.insert(qMakePair(startState,s[0]),++currentState);
        //transitions.insert(qMakePair(startState,' '),startState);
        for (int j=1;j<s.length();j++)
        {
            //transitions.insert(qMakePair(currentState,' '),startState);
            //transitions.insert(qMakePair(currentState,s[j]),++currentState);
        }
        finalStates.insert(currentState);
    }
}

DFA ConvertToDFA()
{
    //To Jopory ....
}
