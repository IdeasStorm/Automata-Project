#include "node.h"

Node::Node(QString name)
{
    this->name = name;
    setNotFinite();
}

Node::Node(int num)
{
    QSrting name = "q" + num;
    this(name);
}

QString Node::getName()
{ return name; }

void Node::setFinite()
{ finite = true; }

void Node::setNotFinite()
{ finite = false; }

bool Node::isFiniteState()
{ return finite; }
