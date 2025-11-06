#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clientInt = ClientInterface::getInstance();
}

MainWindow::~MainWindow()
{
    clientInt->deleteLater();
    delete ui;
}


void MainWindow::on_getBtn_clicked()
{
    int id = ui->orderIdGet->value();
    Order order = clientInt->getOrder(id);
    order.display();
}


void MainWindow::on_listBtn_clicked()
{
    QList<Order> orderList = clientInt->getOrders();
    for(auto it = orderList.begin(); it != orderList.end(); it++)
        (*it).display();
}


void MainWindow::on_deleteBtn_clicked()
{
    int id = ui->orderIdDelete->value();
    clientInt->deleteOrder(id);
}


void MainWindow::on_createBtn_clicked()
{
    Order order(
        -1,
        ui->orderUserCreate->value(),
        ui->orderBaseCreate->currentText(),
        ui->orderQuoteCreate->currentText(),
        ui->orderSideCreate->currentText(),
        ui->orderSizeCreate->value(),
        ui->orderPriceCreate->value(),
        ui->orderIsProcessedCreate->currentText() == "True",
        ui->orderOpenedAtCreate->dateTime()
    );

    Order result = clientInt->createOrder(order);
    result.display();
}


void MainWindow::on_updateBtn_clicked()
{
    Order order(
        ui->orderIdUpdate->value(),
        ui->orderUserUpdate->value(),
        ui->orderBaseUpdate->currentText(),
        ui->orderQuoteUpdate->currentText(),
        ui->orderSideUpdate->currentText(),
        ui->orderSizeUpdate->value(),
        ui->orderPriceUpdate->value(),
        ui->orderIsProcessedUpdate->currentText() == "True",
        ui->orderOpenedAtUpdate->dateTime()
    );

    Order result = clientInt->updateOrder(order);
    result.display();
}
