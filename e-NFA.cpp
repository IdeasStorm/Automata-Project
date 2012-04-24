#include "e-NFA.h"

QList<char>e_NFA::Alphabetic;


e_NFA::e_NFA()
{
    StartState = new NodeNFA("q0") ;
    AllStates.insert(StartState);
    StartState->link(' ',StartState);

    Finit_wordsState = new NodeNFA(">");
    Finit_wordsState->setFinite();
    FinitStates.insert(Finit_wordsState);
    AllStates.insert(Finit_wordsState);
    //
    int i = 0 ;
    for (char ch = 'a';ch<='z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    for (char ch = 'A';ch<='Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');
}

e_NFA::e_NFA (QString str)
{
    StartState = new NodeNFA("q0") ;
    AllStates.insert(StartState);
    StartState->link(' ',StartState);

    //
    int i = 0 ;
    for (char ch = 'a';ch<='z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    for (char ch = 'A';ch<='Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');

    NodeNFA* current  = StartState, *Next ;
    int count = 0 ;
    for (int j=0;j<str.length();j++)
    {
        QString num;
        num.setNum(count++);
        Next = new NodeNFA(num);
        AllStates.insert(Next);
        current->link(str[j].cell(),Next);
        if (j!=str.length()-1)
            Next->link(' ',StartState);
        current = Next ;
    }
    current->setFinite();
    FinitStates.insert(current);
    current->link('\0',StartState);
    setFinit_WordsState(current);
}

e_NFA* e_NFA::Regex(QString expression)
{
    QList<QString> tokens = getTokens(expression);
    //StartState = new NodeNFA("q0");
    e_NFA* e_nfa = new e_NFA();
    e_nfa->setStartState(new NodeNFA("q1"));
    NodeNFA* node = e_nfa->getStartState();//StartState;
    int nodesNum = 2;
    foreach (QString token, tokens)
    {
        if(token == "*")
        {
            for(char c='a';c<='z';c++)
                node->link(c);
            for(char c='A';c<='Z';c++)
                node->link(c);
        }
        else
        {
            //create NFA
            e_NFA* nfa = new e_NFA(token);
            //connect node with start state of e_nfa by eps
            node->link('\0', nfa->getStartState());
            //node is the last state
            node = nfa->getFinit_WordsState();
            //node is not finite state now
            node->setNotFinite();
            //remove it from FiniteState set
            //nfa->getFinitStates().remove(node);
            //add node to nodesNum
            nodesNum += nfa->getAllStates().count();
            e_nfa->getFinit_WordsState()->setNotFinite();
            //e_nfa->setFinitStats(e_nfa->getFinitStates().remove(e_nfa->getFinit_WordsState()));
            //add nodes to AllState set
            e_nfa->addToState(nfa->getAllStates());
            //create new node
            NodeNFA* NFAnode = new NodeNFA(nodesNum);
            nodesNum++;
            //link the last node of nfa with new node by eps
            node->link('\0', NFAnode);
            //make it the node
            node = NFAnode;
        }
    }
    //make the node finite
    //node->setFinite();
    //add it to finite set
    //addToFinitState(node);
    //make it the finit words State
    //Finit_wordsState = node;
    return e_nfa;
}

e_NFA::e_NFA(QList<QString>KeyWords)
{
    StartState = new NodeNFA("q0") ;
    AllStates.insert(StartState);
    StartState->link(' ',StartState);
    if (KeyWords.length() > 0)
    {
        Finit_wordsState = new NodeNFA(">");
        Finit_wordsState->setFinite();
        FinitStates.insert(Finit_wordsState);
        AllStates.insert(Finit_wordsState);
    }
    //
    int i = 0 ;
    for (char ch = 'a';ch<='z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    for (char ch = 'A';ch<='Z';ch++)
    {
        Alphabetic.insert(i++,ch);
    }
    Alphabetic.insert(i++,' ');

    LoadE_NFA(KeyWords);
}
void e_NFA::addState(NodeNFA* state)
{
    AllStates.insert(state);
}
void e_NFA::addFinitState(NodeNFA *state)
{

    FinitStates.insert(state);
}

//GET
NodeNFA* e_NFA::getStartState()
{ return StartState; }

NodeNFA* e_NFA::getFinit_WordsState()
{ return Finit_wordsState; }

QList<char>e_NFA::getAlphabetic()
{ return Alphabetic; }

QList<char>e_NFA::getSeparate_wordsAlphabetic()
{ return Separate_wordsAlphabetic; }

QSet<NodeNFA*>e_NFA::getFinitStates()
{ return FinitStates; }

QSet<NodeNFA *>e_NFA::getAllStates()
{ return AllStates; }

QSet<NodeNFA*>e_NFA::getNonFinitStates()
{ return AllStates.subtract(FinitStates); }

//Set
void e_NFA::setStartState(NodeNFA *state)
{ StartState = state; }

void e_NFA::setAlphabetic(QList<char> alphabetic)
{ Alphabetic = alphabetic; }

void e_NFA::setFinit_WordsState(NodeNFA *state)
{ Finit_wordsState = state; }

void e_NFA::addToFinitState(NodeNFA* state)
{ FinitStates.insert(state); }

void e_NFA::addToState(QSet<NodeNFA*> states)
{
    foreach(NodeNFA* node, states)
        AllStates.insert(node);
}

void e_NFA::addToState(NodeNFA* state)
{ AllStates.insert(state); }

void e_NFA::setSeparate_wordsAlphabetic(QList<char> alphabetic)
{ Separate_wordsAlphabetic = alphabetic; }

void e_NFA::LoadE_NFA(QList<QString>KeyWords)
{
    NodeNFA *CurrentState = StartState,*NextState = StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeDFA) q0,1,2,3....

    //for (int i=0;i<numberWords;i++)
    foreach (QString s ,KeyWords)
    {
        CurrentState = StartState ;
        NextState = new NodeNFA("Epsilon");
        AllStates.insert(NextState);
        CurrentState->link('\0',NextState);
        CurrentState = NextState ;
        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {
            if (s[j].cell() == '*')
            {
                for(char c='a';c<='z';c++)
                    CurrentState->link(c);
                for(char c='A';c<='Z';c++)
                    CurrentState->link(c);
                if (j!=s.length()-1)
                    NextState->link(' ', StartState);
                continue;
            }

            QString num;
            num.setNum(CounterState++);
            NextState = new NodeNFA(num);
            AllStates.insert(NextState);
            CurrentState->link(s[j].cell(),NextState);
            if (j!=s.length()-1)
                NextState->link(' ',StartState);
            CurrentState = NextState ;
        }
        //For set Finit of the last Node after node of ' '
        CurrentState->link(' ',Finit_wordsState);

    }//For number Words

    Finit_wordsState->link('\0',StartState);
}


QSet<NodeNFA *> e_NFA::getClosure(NodeNFA * state)
{
    QStack <NodeNFA*> s;
    QSet<NodeNFA*> set;
    set.insert(state);
    s.push(state);
    NodeNFA* q;
    QList<NodeNFA*> r;
    while(  ! s.empty() )
    {
        q=s.pop();
        r=q->getNextNode('\0');

        foreach (NodeNFA* n,r)
        {
            if (!(set.contains(n)))
            {
              set.insert(n);
              s.push(n);
            }
        }
    }
    return set;
}


NFA* e_NFA::convertToNFA()
{
    NFA* result = new NFA();
    NodeNFA* start1=getStartState();
    NodeNFA* start2=result->getStartState();
    QStack<NodeNFA*> s;
    s.push(start1);
    QHash<QString,NodeNFA*> hash;
    hash.insert(start2->getName(),start2);
    QSet<NodeNFA*> temp1;
    QSet<NodeNFA*> temp2;
    QSet<NodeNFA*> temp3;
    QSet<NodeNFA*> done;
    bool added = false ;
    while(!s.empty())
    {
       start1=s.pop();
       done.insert(start1);
       start2=hash.value(start1->getName());
       temp1=getClosure(start1);
       foreach(char c ,getAlphabetic())
       {
           if (c!='\0')
           {
                temp2.clear();
                temp3.clear();
               foreach(NodeNFA* node,temp1)
               {
                   foreach(NodeNFA* n,node->getNextNode(c))
                       temp2.insert(n);
               }
               foreach(NodeNFA* n,temp2)
               {
                   QSet<NodeNFA*> myset=getClosure(n);
                   foreach(NodeNFA* node,myset)
                   {
                       if(!temp3.contains(node))
                          temp3.insert(node);
                   }
               }
               foreach(NodeNFA* node,temp3)
               {
                   if(!(hash.keys().contains(node->getName())))
                   {
                       NodeNFA* temp=new NodeNFA(node->getName());
                       start2->link(c,temp);
                       result->addState(temp);
                       hash.insert(temp->getName(),temp);
                       if(node->isFiniteState())
                       {
                         temp->setFinite();
                         result->addFinitState(temp);
                       }
                   }
                   else
                   {
                       NodeNFA* temp=hash.value(node->getName());
                       start2->link(c,temp);
                   }
               }

               foreach(NodeNFA* node,temp3)
               {
                   if ( (!done.contains(node)) && (node->getName()!="Epsilon"))
                   {
                        s.push_back(node);
                   }
                   else if ( (node->getName()=="Epsilon") && (!added) )
                   {
                       added = true ;
                       s.push_back(node);
                   }
                   if (!(done.contains(node)) && !(s.contains(node)))
                        s.push(node);
               }
           }
       }
        temp1.clear();
    }
    return result;
}

e_NFA::~e_NFA()
{
    foreach (NodeNFA* state , AllStates)
    {
        delete state ;
    }
}

QList<QString> e_NFA::getTokens(QString str)
{
    QList<QString> result;
    QString buffer;
    foreach (QChar c, str) {
        if (c == '*') {
            if (buffer != "")
                result.append(buffer);
            result.append("*");
            buffer.clear();
        } else {
            buffer.append(c);
        }    }
    if (!buffer.isEmpty())
        result.append(buffer);
    return result;
}
