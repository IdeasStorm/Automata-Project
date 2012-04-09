#include <QtGui/QApplication>
#include "mainwindow.h"
#include "node.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Node* node = new Node();
    node->link('a');
    Node* n = node->nextNode('a');
    n = new Node();
    n->setFinite();
    node->link('b', n);
    Node* in = node->nextNode('b');

    return a.exec();
}
