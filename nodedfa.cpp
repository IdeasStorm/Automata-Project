#include "nodedfa.h"
#include <QString>

NodeDFA::NodeDFA(QString name)
{
    nextNodes = new QMap<char, NodeDFA *>();
    this->name = name;
}

NodeDFA::NodeDFA(int num)
{
    nextNodes = new QMap<char, NodeDFA *>();
    this->name = "q" + num;
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
    return nextNodes->value(value);
}

void NodeDFA::setFinite()
{ finite = true; }

void NodeDFA::setNotFinite()
{ finite = false; }

bool NodeDFA::isFiniteState()
{ return finite; }

