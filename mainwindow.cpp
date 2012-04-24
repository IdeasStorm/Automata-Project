#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DFA.h"
#include "NFA.h"
#include "e-NFA.h"
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

    NFA* test  = new NFA();
    if ((ui->lineEdit->text() != "") && (!getAllKeywords().contains(ui->lineEdit->text()) ))
    {
        QString s = ui->lineEdit->text();
        QString res ;
        for (int j=0;j<s.length();j++)
        {
            if ((((NFA::getAlphabetic()).contains(s[j].cell())) && (s[j]!=' ')) ||(s[j]=='*'))
                res +=s[j];
        }

        ui->listWidget->addItem(new QListWidgetItem(res));
        ui->lineEdit->setText("");
    }
    else
        ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_2_clicked()
{
    QListWidgetItem *current_item = ui->listWidget->currentItem();
    ui->listWidget->removeItemWidget(current_item);
    delete current_item;
}

QList<QString> MainWindow::getAllKeywords()
{
    QList<QListWidgetItem*> all_items = ui->listWidget->findItems("*", Qt::MatchWrap | Qt::MatchWildcard);
    QList<QString> all_words ;
    int i = 0;
    foreach (QListWidgetItem* item, all_items)
    {
        all_words.insert(i++,item->text());
    }
    return all_words;
}

void MainWindow::ViewGraphOfDFA(DFA* dfa ,GraphWidget * graph)
{
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
        graph->currentScene->addItem(node);
        node->setText(QString(currentstate->getName()));
        nodeOfState.insert(currentstate,node);
        if (currentstate==dfa->getSeparate_wordsState())
            node->setPos(-100,150);
        else if (currentstate==dfa->getStartState())
            node->setPos(-150,-70) ;
        else if (currentstate==dfa->getFinit_WordsState())
            node->setPos(160,30) ;
        else
            node->setPos(p) ;

        ui->graphicsView->currentScene->addItem(node);
        bool One = false ;
        QPointF t = QPointF(0,0);
        foreach(char ch, dfa->getAlphabetic())
        {
            if (currentstate->nextNode(ch)!=dfa->getSeparate_wordsState())
            {
                NodeDFA *nextstate = currentstate->nextNode(ch);
                if (!One)
                {
                    fillFromDFANode(nextstate,dfa,graph,p+QPointF(50,0),visited,nodeOfState);
                    One = true ;
                }
                else
                {
                    if ((t.x() >= -50) && (t.y() <= 100) )
                        t += QPointF(-50,+30);
                    fillFromDFANode(nextstate,dfa,graph,p+t,visited,nodeOfState);
                }

                Edge *edge = new Edge(node,nodeOfState.value(nextstate));
                (ch==' ')?edge->setSymbol('_'):edge->setSymbol(ch);
                graph->currentScene->addItem(edge);
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
        //Edge *edgeToSeperatedWords = new Edge(node,nodeOfState.value(dfa->Separate_wordsState));
        //edgeToSeperatedWords->setSymbol('?');
        //graph->currentScene->addItem(edgeToSeperatedWords);
        //ui->graphicsView->currentScene->addItem(edgeToSeperatedWords);
    }//from else
}

void MainWindow::on_pushButton_3_clicked()
{   

        // Build DFA
         //DFA *myt = new DFA(getAllKeywords());
        // Build E-NFA
        e_NFA *myt3 = new e_NFA(getAllKeywords());
        NFA *myt2 = myt3->convertToNFA();
        DFA *myt = myt2->convertToDFA();
        myt->simplify();

    //Build NFA
/*    NFA *myt2 = new NFA(getAllKeywords());
    DFA *myt = myt2->convertToDFA();
    myt->simplify();
*/
        ui->graphicsView->currentScene->clear();
        ui->OutPut->clear();
        QHash<QString,int> reshash = myt->SimulateDFA(filterText(ui->plainTextEdit->toPlainText()));
        QList<QString> res = reshash.keys();
        foreach(QString key,res)
        {
            int num = reshash.value(key);
            QString number;
            number.setNum(num);
            QString* str = new QString(key + number);
            ui->OutPut->addItem(new QListWidgetItem(*str));
        }
        GraphWidget *graph = new GraphWidget();
        ViewGraphOfDFA(myt,graph);
}

QString MainWindow::filterText(QString s)
{
    QString res ;
    for (int i=0 ; i<s.length();i++)
    {
        if ((NFA::getAlphabetic()).contains(s[i].cell()) )
            res +=s[i];
        else
            res +=' ';
    }
    return res ;
}

int MainWindow::getWordCount()
{
    return ui->listWidget->count();
}
