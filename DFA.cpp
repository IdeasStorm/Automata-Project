#include "DFA.h"

NodeDFA * DFA::getStartState()
{
    return StartState ;
}

void DFA::setStartState(NodeDFA *state)
{
    StartState = state;
}

DFA::DFA()
{
    StartState = new NodeDFA('0') ;
    DeadState = new NodeDFA ('|'); // | ==> Loop Dead State
    StartState->link('?',DeadState);
    StartState->link(' ');
    DeadState->link(' ',StartState);
    DeadState->link('?'); // ? ==> a-->z
}

DFA::DFA(QString *KeyWords,int numberWords)
{
    //DFA();
    StartState = new NodeDFA('0') ;
    DeadState = new NodeDFA ('|'); // | ==> Loop Dead State
    StartState->link('?',DeadState);
    StartState->link(' ');
    DeadState->link(' ',StartState);
    DeadState->link('?'); // ? ==> a-->z
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
                NextState->link('?',DeadState); //  Link to Dead State
                CurrentState->link(s[j].cell(),NextState);
                CurrentState->link(' ',StartState);
                CurrentState = NextState ;
            }
            else
                CurrentState = CurrentState->nextNode(s[j].cell()) ;
        }
        //For set Finit of the last Node after node of ' '
        NextState = new NodeDFA(CounterState++);
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

QSet<NodeDFA*> DFA::getFinitStates()
{
    return FinitStates;
}

