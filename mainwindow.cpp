#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include "QPixmap"
#include "url.h"
#include "QFileDialog"
#include "QDir"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    urlList.clear();
    connect(ui->btnConvert,SIGNAL(clicked()),this,SLOT(onClickConvert()));
    connect(ui->btnDownload,SIGNAL(clicked()),this,SLOT(onClickDownload()));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(netReply(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
//    if(!(url.contains("pageid="))) {
//        ui->textBrowser->append(tr("Unrecogenized url.\nPlease find and Copy thr url use Chrome.\n"));
//        return;
//    }

//    page_start=ui->pageStart->text().toInt();
//    page_end=ui->pageLen->text().toInt()+page_start-1;
//    if(page_start <= 0 || page_end <= 0) {
//        ui->textBrowser->append("Page varient error!");
//        return;
//    }

//    int pre_pos,cut_pos;
//    pre_pos=url.indexOf("pageid");
//    pre_pos+=7;
//    cut_pos=pre_pos;

//    for(int i=1;i<=10;i++) {
//       temp=url.mid(pre_pos++,1);
//       if(temp=="&")
//           break;
//       if(i==10) {
//           ui->textBrowser->append("pageid is to long");
//           return;
//       }
//    }
//    url.replace(cut_pos,pre_pos-cut_pos-1,"%1");
//    for(int i=page_start;i<=page_end;i++) {
//        ui->statusBar->showMessage(QString("Conveting..."));
//        urlList.append(url.arg(i));
//    }
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

    QString dir=QDir::homePath()+"/%1";
    QString fileName =  QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     dir.arg("Untitled.pdf"),
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
        ui->statusBar->clearMessage();
        ui->btnConvert->setDisabled(false);
        index = 0;
        return;
    } else {
        manager->get(QNetworkRequest(QUrl(urlList.at(index))));
        ui->statusBar->showMessage(QString("Downloading...%%1").arg(index*100/urlList.length()));
        p->drawPixmap(0,0,pix);
        pdf_writer->newPage();
        ui->textBrowser->setText(QString("Page:%1 download finish.").arg(index));
    }
}
