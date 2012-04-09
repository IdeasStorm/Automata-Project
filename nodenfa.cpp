#include "nodenfa.h"
#include "node.h"

NodeNFA::NodeNFA(QString name) : Node(name)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
}

NodeNFA::NodeNFA(int num) : Node(num)
{
    nextNodes = new QMultiHash<char, NodeNFA*>();
}

void NodeNFA::link(char value, Node *second)
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
