#include "nodenfa.h"

NodeNFA::NodeNFA(QString name)
{
    this->name = name;
    setNotFinite();
    nextNodes = new QMultiHash<char, NodeNFA*>();
}

NodeNFA::NodeNFA(int num)
{
    QSrting name = "q" + num;
    this(name);
}

void NodeNFA::link(char value, NodeNFA *second)
{
    nextNodes->insert(value, second);
}

void NodeNFA::link(char value)
{
    nextNodes->insert(value, this);
}

void NodeNFA::setFinite()
{ finite = true; }

void NodeNFA::setNotFinite()
{ finite = false; }

bool NodeNFA::isFiniteState()
{ return finite; }

QList<NodeNFA*> NodeNFA::nextNode(char value)
{
    return nextNodes->values(value);
}
