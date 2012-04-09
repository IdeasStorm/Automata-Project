#include <QtGui/QApplication>
#include "mainwindow.h"
#include "node.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Node* node = new Node("q0");
    node->link('a');
    //QList<Node*> n = node->nextNode('a');
    Node* n = new Node("q1");
    n->setFinite();
    node->link('b', n);
    QList<Node*> in = node->nextNode('b');

    return a.exec();
}
