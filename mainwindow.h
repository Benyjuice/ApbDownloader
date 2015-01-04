#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QtPrintSupport/QPrinter>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QList>
#include "url.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onClickConvert();
    void onClickDownload();
    void netReply(QNetworkReply *);
private:
    Ui::MainWindow *ui;
    QPdfWriter *pdf_writer;
    QPainter *p;
    QNetworkAccessManager *manager;
    QList<QString> urlList;
    int index;
    bool firstpage;
    Url c_url;
};

#endif // MAINWINDOW_H
