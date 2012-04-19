#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphwidget.h"
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString *getAllKeywords();
    int getWordCount();
void buildDFA(QString *KeyWords,int numberWords);
void ViewGraphOfDFA(DFA* dfa);
void fillFromDFANode(NodeDFA* currentstate , DFA* dfa,GraphWidget *graph,QPointF p,QSet<NodeDFA*>&,QHash<NodeDFA*,Node*>&);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
