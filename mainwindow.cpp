#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DFA.h"
#include "NFA.h"
#include "e-NFA.h"
#include "node.h"
#include "edge.h"
#include <QString>
#include <QStringList>
#include <QHeaderView>
#include <QMessageBox>

bool graphic;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graphic = true;
    ui->tableWidget->hide();
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
            if ((((NFA::getAlphabetic()).contains(s[j].cell())) && (s[j]!=' ')) ||( (graphic) &&(s[j]=='*')))
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

void MainWindow::createTable(NFA* nfa)
{
    QMultiMap<QString, QPair<QString, char> >* table = nfa->getConvertTable();   
    QHash<QString,int> rowList;
    QHash<QString,int> symbols;
    typedef QPair<QString,char> char_string ;
    int counter = 0;
    foreach (char_string value, table->values()) {
        if (!symbols.contains(QString(value.second)))
            symbols.insert(QString(value.second),counter++);
    }
    counter = 0;
    foreach (QString key, table->keys()) {
        if (!rowList.contains(key))
            rowList.insert(key,counter++);
    }


    ui->tableWidget->setColumnCount(symbols.count());
    ui->tableWidget->setRowCount(rowList.count());
    foreach (QString key, table->keys()) {
        QList<QPair<QString, char> > values = table->values(key);
        foreach (char_string pair, values) {
            QString str = pair.first;
            QTableWidgetItem *item = new QTableWidgetItem(str);
            int row = rowList.value(key);
            int col = symbols.value(QString(pair.second));
            ui->tableWidget->setItem(row,col,item);
        }
    }
    ui->tableWidget->setVerticalHeaderLabels(rowList.keys());
    ui->tableWidget->setHorizontalHeaderLabels(symbols.keys());
}

void MainWindow::fillFromDFANode(NodeDFA* currentstate , DFA* dfa,GraphWidget *graph,QPointF p,QSet<NodeDFA*>& visited,QHash<NodeDFA*,Node*>& nodeOfState)
{
    if (visited.contains(currentstate))
        return ;
    else{
        visited.insert(currentstate);
        Node *node = graph->createNode(currentstate);
        //graph->currentScene->addItem(node);
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

    ui->graphicsView->currentScene->clear();
    ui->OutPut->clear();
    DFA * nodeDFA ;
    NFA *nodeNFA ;
    bool simplify = ui->checkBox->isChecked();
    timerStart();
    if (graphic)
    {
        // Build E-NFA
        e_NFA *nodeENFA = new e_NFA(getAllKeywords());
        nodeNFA = nodeENFA->convertToNFA();
        nodeDFA = nodeNFA->convertToDFA();
        if (simplify)
            nodeDFA->simplify();
    }
    else
    {
        //Build NFA
        nodeNFA = new NFA(getAllKeywords());
        nodeDFA = nodeNFA->convertToDFA();
        //myt->simplify();
    }
    QHash<QString,int> reshash = nodeDFA->SimulateDFA(filterText(ui->plainTextEdit->toPlainText()));
    timerEnd();
    QList<QString> res = reshash.keys();
    foreach(QString key,res)
    {
        int num = reshash.value(key);
        QString number;
        number.setNum(num);
        QString* str = new QString(key + number);
        ui->OutPut->addItem(new QListWidgetItem(*str));
    }
    if (graphic)
    {
        ui->tableWidget->hide();
        ui->graphicsView->show();
        GraphWidget *graph = new GraphWidget();
        ViewGraphOfDFA(nodeDFA,graph);
    }
    else
    {
        ui->graphicsView->hide();
        ui->tableWidget->show();
        createTable(nodeNFA);
    }

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

void MainWindow::on_radioButton_clicked()
{
    ui->tableWidget->hide();
    ui->graphicsView->show();
    graphic = true;
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->listWidget->clear();
    ui->graphicsView->hide();
    ui->tableWidget->show();
    graphic = false;
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    // just for debugging
    //QMessageBox::information(this,"alert",QString("%1 , %2").arg(row).arg(column),0);
}

void MainWindow::timerStart()
{
    time_elapsed = spec_timer.currentTime().msec();
}

int MainWindow::timerEnd()
{
    int delta = spec_timer.currentTime().msec() - time_elapsed;
    ui->label_3->setText(QString("Exec Time : %1 ms").arg(time_elapsed));
    return delta;
}
