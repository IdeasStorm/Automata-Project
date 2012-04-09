#include "nodedfa.h"
#include "node.h"

NodeDFA::NodeDFA(QString name) : Node(name)
{
    nextNodes = new QMap<char, NodeDFA*>();
}

NodeDFA::NodeDFA(int num) : Node(num)
{
    nextNodes = new QMap<char, NodeDFA*>();
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

