#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->append("<p><font color=\"red\">Этот игрок: </font>" + ui->plainTextEdit->toPlainText() + "</p>");
    ui->plainTextEdit->setPlainText("");
}
