#include "nodedfa.h"
#include "node.h"

NodeDFA::NodeDFA(QString name) : Node(name)
{
    nextNodes = new QMap<char, Node *>();
}

NodeDFA::NodeDFA(int num) : Node(num)
{
    nextNodes = new QMap<char, Node *>();
}

void NodeDFA::link(char value, Node *second)
{
    nextNodes->insert(value, second);
}

void NodeDFA::link(char value)
{
    nextNodes->insert(value, this);
}

Node * NodeDFA::nextNode(char value)
{
    return nextNodes->value(value);
}

