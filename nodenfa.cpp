#include "nodenfa.h"
#include "NFA.h"


NodeNFA::NodeNFA(QString name)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
    this->name = name;
    setNotFinite();
}

NodeNFA::NodeNFA(int num)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
    this->name = num;
    setNotFinite();
}
NodeNFA::NodeNFA(const NodeNFA & copy_from_me)
{
    name=copy_from_me.name;
    finite=copy_from_me.finite;
    nextNodes=new QMultiHash<char,NodeNFA*>(*(copy_from_me.nextNodes));
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

NodeNFA::~NodeNFA()
{
    nextNodes->clear();
    nextNodes->~QHash();
}
