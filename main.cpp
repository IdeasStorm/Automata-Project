#include <QtGui/QApplication>
#include "mainwindow.h"
#include "nodenfa.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    NodeNFA* node = new NodeNFA("q0");
    node->link('a');
    //QList<Node*> n = node->nextNode('a');
    NodeNFA* n = new NodeNFA("q1");
    n->setFinite();
    node->link('b', n);
    QList<NodeNFA*> in = node->nextNode('b');

    return a.exec();
}
