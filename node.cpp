#include "node.h"

//int Node::q = -1;

Node::Node()
{
    //Node::q++;
    setNotFinite();
    nextNodes = new QMap<char, Node*>();
}

void Node::link(char value, Node *second)
{
    nextNodes->insert(value, second);
}

void Node::link(char value)
{
    nextNodes->insert(value, this);
}

void Node::setFinite()
{ finite = true; }

void Node::setNotFinite()
{ finite = false; }

bool Node::isFiniteState()
{ return finite; }

Node* Node::nextNode(char value)
{
    return nextNodes->value(value);
}
