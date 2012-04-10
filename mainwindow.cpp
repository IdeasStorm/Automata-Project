#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DFA.h"
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
    if (ui->lineEdit->text() != ""){
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

void MainWindow::on_pushButton_3_clicked()
{
    DFA *myt = new DFA(getAllKeywords(),getWordCount());
    QString res = (myt->SimulateDFA(ui->plainTextEdit->toPlainText())) ? "true" : "false";
    ui->OutPut->addItem(new QListWidgetItem(res));
}

int MainWindow::getWordCount()
{
    return ui->listWidget->count();
}
