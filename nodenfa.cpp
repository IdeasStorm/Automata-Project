#include "nodenfa.h"


NodeNFA::NodeNFA(QString name)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
    this->name = name;
}

NodeNFA::NodeNFA(int num)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
    this->name = "q" + num;
}

void NodeNFA::link(char value, NodeNFA *second)
{
    nextNodes->insert(value, second);
}

void NodeNFA::link(char value)
{
    nextNodes->insert(value, this);
}

QList<NodeNFA*> NodeNFA::getNextNode(char value)
{
    return nextNodes->values(value);
}

QMultiHash<char, NodeNFA*>* NodeNFA::getNextNodes()
{
    return nextNodes ;
}

void NodeNFA::setNextNodes(QMultiHash<char, NodeNFA*>* nextNodes)
{
    this->nextNodes = nextNodes;
}

QString NodeNFA::getName()
{ return name; }

void NodeNFA::setFinite()
{ finite = true; }

void NodeNFA::setNotFinite()
{ finite = false; }

bool NodeNFA::isFiniteState()
{ return finite; }
