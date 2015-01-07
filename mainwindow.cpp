#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include "QPixmap"
#include "url.h"
#include "QFileDialog"
#include "QDir"
#include "QMessageBox"
#include "about.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    ui->progressBar->hide();
    urlList.clear();
    connect(ui->btnConvert,SIGNAL(clicked()),this,SLOT(onClickConvert()));
    connect(ui->btnDownload,SIGNAL(clicked()),this,SLOT(onClickDownload()));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(netReply(QNetworkReply*)));
    connect(ui->menuAbout,SIGNAL(triggered(QAction*)),this,SLOT(showAbout()));
}

MainWindow::~MainWindow()
{
    if(!manager)
        delete manager;
    if(!ui)
        delete ui;
    if(!pdf_writer)
        delete pdf_writer;
    if(!p)
        delete p;
    if(!about)
        delete about;
}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}

void MainWindow::onClickConvert()
{
    int pageStart=ui->pageStart->text().toInt();
    int pageLen=ui->pageLen->text().toInt();
    QString url=ui->urlLine->text();

    if(pageStart <=0 || pageLen <=0) {
        ui->textBrowser->setText("Page set error.");
        return;
    }

    if(url.isEmpty()) {
        ui->textBrowser->setText("URL read faild.");
        return;
    }


    urlList.clear();
    ui->textBrowser->clear();


    //set Quality
    int i=ui->comboBox->currentIndex();
    c_url.setHeigth((Quality)i);

    if(!(c_url.load(url))) {
        ui->textBrowser->setText("Faild to load url");
        return;
    }

    ui->statusBar->showMessage("Converting...");
    for(int i=pageStart;i<=pageStart+pageLen-1;i++) {
        urlList.append(c_url.getUrl()+QString("&pageid=%1").arg(i));
    }

    ui->statusBar->clearMessage();

    index=0;
    ui->textBrowser->append("Ready...");
}

void MainWindow::onClickDownload()
{
    if(urlList.isEmpty()) {
       ui->textBrowser->setText(tr("No url converted."));
        return;
    }

    QString dir=QDir::homePath();
    QString fileName =  QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     dir+("/Untitled.pdf"),
                                                     tr("pdf (*.pdf)"));

    if (fileName.isEmpty()) {
        ui->textBrowser->setText("No file selected.");
        return;
    }
    pdf_writer=new QPdfWriter(fileName);
    pdf_writer->setPageSize(QPagedPaintDevice::A4);
    pdf_writer->setResolution(c_url.getPPI());
    p=new QPainter(pdf_writer);
    firstpage = true;
    ui->progressBar->show();
    ui->progressBar->setValue(0);
    manager->get(QNetworkRequest(QUrl(urlList.at(0))));
}

void MainWindow::netReply(QNetworkReply *reply)
{
    ui->btnConvert->setDisabled(true);
    QPixmap pix;
    pix.loadFromData(reply->readAll());
    if (firstpage) {
        qreal scale=(qreal)p->viewport().width()/pix.width();
        p->scale(scale,scale);
        firstpage = false;
    }
    index ++;
    if(index >= urlList.length()) {
        p->drawPixmap(0,0,pix);
        delete p;
        delete pdf_writer;
        ui->textBrowser->setText(QString("Page:%1 download finish.").arg(index));
        ui->textBrowser->append("Download finished");
        ui->progressBar->hide();
        ui->statusBar->clearMessage();
        ui->btnConvert->setDisabled(false);
        index = 0;
        return;
    } else {
        manager->get(QNetworkRequest(QUrl(urlList.at(index))));
        ui->statusBar->showMessage(QString("Downloading...%%1").arg(index*100/urlList.length()));
        ui->progressBar->setValue(index*100/urlList.length());
        p->drawPixmap(0,0,pix);
        pdf_writer->newPage();
        ui->textBrowser->setText(QString("Page:%1 download finish.").arg(index));
    }
}

void MainWindow::showAbout()
{

    about = new About(0);
    about->show();
}
