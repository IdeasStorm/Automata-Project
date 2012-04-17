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

void MainWindow::buildDFA(QString *KeyWords,int numberWords)
{
    DFA *dfa = new DFA();
    dfa->StartState = new NodeDFA('0') ;
    dfa->addToState(dfa->StartState);
    dfa->Separate_wordsState = new NodeDFA ('<'); // | ==> Loop Dead State
    dfa->addToState(dfa->Separate_wordsState);

    if (numberWords > 0)
    {
        dfa->Finit_wordsState = new NodeDFA('>');
        dfa->Finit_wordsState->setFinite();
        dfa->FinitStates.insert(dfa->Finit_wordsState);
        dfa->AllStates.insert(dfa->Finit_wordsState);
    }

    int i = 0 ;
    for (char ch = 'a';ch <= 'z';ch++)
        dfa->Alphabetic.insert(i++,ch);
    for (char ch = 'A';ch <= 'Z';ch++)
        dfa->Alphabetic.insert(i++,ch);
    dfa->Alphabetic.insert(i++,' ');

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GraphWidget *myt = new GraphWidget();
    QPointF p(-50,-50);
    Node *start = myt->createNode(dfa->StartState);
    Node *sep = myt->createNode(dfa->Separate_wordsState);
    Node *finit = myt->createNode(dfa->Finit_wordsState);
    ui->graphicsView->currentScene->addItem(start);
    start->setPos(50,10);
    ui->graphicsView->currentScene->addItem(sep);
    sep->setPos(100,100);
    ui->graphicsView->currentScene->addItem(finit);
    finit->setPos(100,50);

    p += QPointF(0,10);
    start->setPos(p);
    //Edge *edge = new Edge(start,start);
    p += QPointF(10,0);
    //edge->setSymbol(' ');
    //ui->graphicsView->currentScene->addItem(edge);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    NodeDFA *CurrentState = dfa->StartState,*NextState = dfa->StartState;
    int CounterState = 1 ; //for generate and save name of states(NodeDFA) q0,1,2,3....
    for (int i=0;i<numberWords;i++)
    {
        QString s = KeyWords[i];
        CurrentState = dfa->StartState;
        Node *current = start ;
        //For link each NodeNFA with another NodeNFA Based input
        for (int j=0;j<s.length();j++)
        {
//            if (CurrentState->nextNode(s[j].cell()) == CurrentState->nextNode('?'))
            if (CurrentState->nextNode(s[j].cell()) == NULL )
            {

                NextState = new NodeDFA(CounterState++);
                dfa->AllStates.insert(NextState);
                CurrentState->link(s[j].cell(),NextState);

                Node *next = myt->createNode(NextState);
                ui->graphicsView->currentScene->addItem(next);
                p += QPointF(20,20);
                next->setPos(p);
                //p += QPointF(10,0);
                Edge *edge1 = new Edge(current,next);
                edge1->setSymbol(s[j].cell());
                ui->graphicsView->currentScene->addItem(edge1);
                CurrentState = NextState ;
                current = next ;
                Edge *edge2 = new Edge(next,sep);
                edge2->setSymbol('?');
                ui->graphicsView->currentScene->addItem(edge2);
            }
            else
            {
                CurrentState = CurrentState->nextNode(s[j].cell()) ;
                //this is the big problem o_O ...
           //     current = current->nextnode(s[j]);
            }
        }
        CurrentState->link(' ',dfa->Finit_wordsState);
        Edge *edge3 = new Edge(current,finit);
        edge3->setSymbol(' ');
        ui->graphicsView->currentScene->addItem(edge3);

    }//For number Words

    foreach (char ch,dfa->Alphabetic)
    {
        if (dfa->StartState->nextNode(ch)==NULL)
        {
            if (ch ==' ')
            {
                dfa->StartState->link(ch,dfa->StartState);
            }
            else
            {
                dfa->StartState->link(ch,dfa->Separate_wordsState);
            }
        }
    }
    Edge *startToStart = new Edge(start,start);
    startToStart->setSymbol('~');
    ui->graphicsView->currentScene->addItem(startToStart);

    Edge *startToSep = new Edge(start,sep);
    startToSep->setSymbol('?');
    ui->graphicsView->currentScene->addItem(startToSep);

    Edge *sepTosep = new Edge(sep,sep);
    sepTosep->setSymbol('?');
    ui->graphicsView->currentScene->addItem(sepTosep);

    foreach(NodeDFA* state,dfa->AllStates)
    {
        //add all link of a-->Z to all node
        foreach (char ch,dfa->Alphabetic)
        {
            if (state->nextNode(ch)==NULL)
            {
                if (ch ==' ')
                    state->link(ch,dfa->StartState);
                else
                {
                    if (dfa->FinitStates.contains(state))
                        state->link(ch,dfa->StartState->nextNode(ch));
                    else
                        state->link(ch,dfa->Separate_wordsState);
                }
            }
        }
    } // for a --> Z

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
    //ViewGraphOfDFA(myt);
    // this code is Work 100%....
    //ui->OutPut->clear();
    //DFA *myt = new DFA(getAllKeywords(),getWordCount());
 //   ui->graphicsView->loadFromDFA(myt);
    //return;
    /*
    QHash<QString,int> reshash = myt->SimulateDFA(ui->plainTextEdit->toPlainText());

    QList<QString> res = reshash.keys();

    foreach(QString key,res)
    {
        int num = reshash.value(key);
        QString number;
        number.setNum(num);
        QString* str = new QString(key + number);
        ui->OutPut->addItem(new QListWidgetItem(*str));
    }*/
    ViewGraphOfDFA(myt);
}

int MainWindow::getWordCount()
{
    return ui->listWidget->count();
}

