#include "node.h"

//int Node::q = -1;

Node::Node(QString name)
{
    //Node::q++;
    this->name = name;
    setNotFinite();
    nextNodes = new QMultiHash<char, Node*>();
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

QList<Node*> Node::nextNode(char value)
{
    return nextNodes->values(value);
}
