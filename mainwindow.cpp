#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DFA.h"
#include "NFA.h"
#include "node.h"
#include "edge.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text() != "") //&&(!getAllKeywords()->contains(ui->lineEdit->text())
    {
        ui->listWidget->addItem(new QListWidgetItem(ui->lineEdit->text()));
        ui->lineEdit->setText("");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QListWidgetItem *current_item = ui->listWidget->currentItem();
    ui->listWidget->removeItemWidget(current_item);
    delete current_item;
}

QString *MainWindow::getAllKeywords()
{
    QList<QListWidgetItem*> all_items = ui->listWidget->findItems("*", Qt::MatchWrap | Qt::MatchWildcard);
    int n = all_items.count();
    QString *all_words = new QString[n];
    int i = 0;
    foreach (QListWidgetItem* item, all_items) {
        all_words[i++] = item->text();
    }
    return all_words;
}

void MainWindow::ViewGraphOfDFA(DFA* dfa)
{
    GraphWidget *graph = new GraphWidget();
    QPointF p(-80,-70);
    QSet<NodeDFA*> visited ;
    QHash<NodeDFA*,Node*> hash ;
    fillFromDFANode(dfa->getStartState(),dfa,graph,p,visited,hash);
}
void MainWindow::fillFromDFANode(NodeDFA* currentstate , DFA* dfa,GraphWidget *graph,QPointF p,QSet<NodeDFA*>& visited,QHash<NodeDFA*,Node*>& nodeOfState)
{
    if (visited.contains(currentstate))
        return ;
    else{
        visited.insert(currentstate);
        Node *node = graph->createNode(currentstate);
        node->setText(QString(currentstate->getName()));
        nodeOfState.insert(currentstate,node);
        if (currentstate==dfa->getSeparate_wordsState())
            node->setPos(-100,150);
        else if (currentstate==dfa->getStartState())
            node->setPos(-150,-70) ;
        else if (currentstate==dfa->getFinit_WordsState())
            node->setPos(150,100) ;
        else
            node->setPos(p) ;

        ui->graphicsView->currentScene->addItem(node);
        foreach(char ch, dfa->getAlphabetic())
        {

            if (currentstate->nextNode(ch)!=dfa->getSeparate_wordsState())
            {
                NodeDFA *nextstate = currentstate->nextNode(ch);
                p += QPointF(50,20);
                fillFromDFANode(nextstate,dfa,graph,p,visited,nodeOfState);
                Edge *edge = new Edge(node,nodeOfState.value(nextstate));
                (ch==' ')?edge->setSymbol('|'):edge->setSymbol(ch);
                ui->graphicsView->currentScene->addItem(edge);
            }
            else
            {
                if (!visited.contains(dfa->getSeparate_wordsState()))
                {
                    NodeDFA *nextstate = currentstate->nextNode(ch);
                    fillFromDFANode(nextstate,dfa,graph,p,visited,nodeOfState);
                }
            }
        }
        Edge *edgeToSeperatedWords = new Edge(node,nodeOfState.value(dfa->Separate_wordsState));
        edgeToSeperatedWords->setSymbol('?');
        ui->graphicsView->currentScene->addItem(edgeToSeperatedWords);
    }//from else
}

void MainWindow::on_pushButton_3_clicked()
{
    NFA *mytt = new NFA(getAllKeywords(),getWordCount());
    DFA *myt = mytt->convertToDFA();
    ui->graphicsView->currentScene->clear();
 //   buildDFA(getAllKeywords(),getWordCount());
    //ui->OutPut->clear();
    //NFA *myt = new NFA(getAllKeywords(),getWordCount());

    ui->OutPut->clear();
//    DFA *myt = new DFA(getAllKeywords(),getWordCount());
    //ui->graphicsView->loadFromDFA(myt);
    //return;


    QHash<QString,int> reshash = myt->SimulateDFA(ui->plainTextEdit->toPlainText());

    QList<QString> res = reshash.keys();

    foreach(QString key,res)
    {
        int num = reshash.value(key);
        QString number;
        number.setNum(num);
        QString* str = new QString(key + number);
        ui->OutPut->addItem(new QListWidgetItem(*str));
    }
    ViewGraphOfDFA(myt);
}

int MainWindow::getWordCount()
{
    return ui->listWidget->count();
}

