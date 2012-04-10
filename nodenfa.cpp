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

QList<NodeNFA*> NodeNFA::nextNode(char value)
{
    return nextNodes->values(value);
}

QMultiHash<char, NodeNFA*>* NodeNFA::getnextNodes()
{
    return nextNodes ;
}

void NodeNFA::setnextNodes(QMultiHash<char, NodeNFA*>* nextnodes)
{
    nextNodes = nextnodes;
}

QString NodeNFA::getName()
{ return name; }

void NodeNFA::setFinite()
{ finite = true; }

void NodeNFA::setNotFinite()
{ finite = false; }

bool NodeNFA::isFiniteState()
{ return finite; }
