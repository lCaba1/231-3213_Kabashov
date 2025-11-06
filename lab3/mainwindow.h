#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include "clientinterface.h"

#include <QPair>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void displayRequestsResult(QMap<int, int> &executionOrder, int totalRequests);
private slots:
    void on_getBtn_clicked();

    void on_listBtn_clicked();

    void on_deleteBtn_clicked();

    void on_createBtn_clicked();

    void on_updateBtn_clicked();

private:
    Ui::MainWindow *ui;
    ClientInterface* clientInt = nullptr;
};
#endif // MAINWINDOW_H
