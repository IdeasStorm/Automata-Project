#include "node.h"
#include <QString>

Node::Node(QString name)
{
    this->name = name;
    setNotFinite();
}

Node::Node(int num)
{
    //QString name = "q" + num;
    //Node(name);
}

QString Node::getName()
{ return name; }

void Node::setFinite()
{ finite = true; }

void Node::setNotFinite()
{ finite = false; }

bool Node::isFiniteState()
{ return finite; }
