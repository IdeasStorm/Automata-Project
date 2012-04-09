#include "nodedfa.h"

NodeDFA::NodeDFA(QString name)
{
    this->name = name;
    setNotFinite();
    nextNodes = new QMap<char, NodeDFA*>();
}

NodeDFA::NodeDFA(int num)
{
    QSrting name = "q" + num;
    this(name);
}

void NodeDFA::link(char value, NodeDFA *second)
{
    nextNodes->insert(value, second);
}

void NodeDFA::link(char value)
{
    nextNodes->insert(value, this);
}

void NodeDFA::setFinite()
{ finite = true; }

void NodeDFA::setNotFinite()
{ finite = false; }

bool NodeDFA::isFiniteState()
{ return finite; }

NodeDFA* NodeDFA::nextNode(char value)
{
    return nextNodes->value(value);
}

