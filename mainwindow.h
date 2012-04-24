#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphwidget.h"
#include "NFA.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QList<QString>getAllKeywords();
    int getWordCount();

    void buildDFA(QString *KeyWords,int numberWords);
    void ViewGraphOfDFA(DFA* dfa, GraphWidget *graph);
    QString filterText(QString);
    void fillFromDFANode(NodeDFA* currentstate , DFA* dfa,GraphWidget *graph,QPointF p,QSet<NodeDFA*>&,QHash<NodeDFA*,Node*>&);
    void createTable(NFA* nfa);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
