#include "nodedfa.h"
#include <QString>

NodeDFA::NodeDFA(char name)
{
    nextNodes = new QMap<char, NodeDFA *>();
    this->name = name;
    setNotFinite();
}

NodeDFA::NodeDFA(int num)
{
    nextNodes = new QMap<char, NodeDFA *>();
    this->name = num;
    setNotFinite();

}

void NodeDFA::link(char value, NodeDFA *second)
{
    nextNodes->insert(value, second);
}

void NodeDFA::link(char value)
{
    nextNodes->insert(value, this);
}

NodeDFA* NodeDFA::nextNode(char value)
{
    if (nextNodes->contains(value))
        return nextNodes->value(value);
    else
        return nextNodes->value('?'); // return Dead State
}

void NodeDFA::setFinite()
{ finite = true; }

void NodeDFA::setNotFinite()
{ finite = false; }

bool NodeDFA::isFiniteState()
{ return finite; }
void NodeDFA::setnextNodes( QMap<char, NodeDFA*>* nextnodes)
{
    this->nextNodes = nextnodes ;
}

QMap<char, NodeDFA*>* NodeDFA::getnextNodes()
{
    return nextNodes;
}
