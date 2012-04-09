#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ui->listWidget->addItem(new QListWidgetItem(ui->lineEdit->text()));
    ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_2_clicked()
{
    QListWidgetItem *current_item = ui->listWidget->currentItem();
    ui->listWidget->removeItemWidget(current_item);
    delete current_item;
}
