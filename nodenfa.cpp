#include "nodenfa.h"
#include "node.h"

NodeNFA::NodeNFA(QString name)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
}

NodeNFA::NodeNFA(int num)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
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
