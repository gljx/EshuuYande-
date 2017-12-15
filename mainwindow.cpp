#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QtNetwork/QNetworkAccessManager"
#include"QtNetwork/QNetworkReply"
#include"QFileDialog"
#include"QFileInfo"
#include"about.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pathaction,SIGNAL(triggered(bool)),this,SLOT(savepath()));
    connect(ui->actionEshuushuu,SIGNAL(triggered(bool)),this,SLOT(getEshuushuuImage()));
    connect(ui->actionPixiv,SIGNAL(triggered(bool)),this,SLOT(getPixivImage()));
    connect(ui->actionYande,SIGNAL(triggered(bool)),this,SLOT(getYandeImage()));
    connect(ui->actionabout,SIGNAL(triggered(bool)),this,SLOT(aboutus()));
    ui->spinBox->setRange(1,99999);
    ui->spinBox->setValue(1);
    downloadurl="The download url is  :";
    filesavepath="save path is :";
    path="K:";
    ui->textBrowser->setText("默认保存到K盘文件夹目录下\n没有K盘的朋友请先选择保存路径\n本软件仅供测试");
    ui->failreport->setText("爬取失败链接\n");
    nowloading="\0";

}

MainWindow::~MainWindow()
{
    delete ui;
}
QString MainWindow::savepath()
{
    path=QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                   QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
     qDebug()<<path<<endl;
     return path;
}
QString MainWindow::getwebcode(QString temp)
{
    //网页地址
    const QString URLSTR = temp;
    nowloading=temp;
    QUrl url(URLSTR);
    //https
    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(conf);
        request.setUrl(url);
      QNetworkAccessManager manager;
    QEventLoop loop;
    qDebug() << "Reading code form " << URLSTR;
    //发出请求
    QNetworkReply *reply = manager.get(request);
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();

    //将读到的信息写入文件
    code = reply->readAll();
    return code;

}
void MainWindow::saveEshuushuu(QString tempstr)
{
    qDebug()<<"getimg called"<<endl;
    QStringList imglist=regulars.searchimg(tempstr);
    qDebug()<<imglist<<endl;
    qDebug()<<imglist.size()<<endl;
    int i=0;
    QString temps;
    while(!imglist.isEmpty())
   {
       qDebug()<<"imglist"<<i<<"  is    "<<imglist[0]<<endl;
        imglist[0].replace("<a class=\"thumb_image\" href=\"","http://e-shuushuu.net");
        imglist[0].replace("\"","");
        temps=imglist.takeFirst();
        nowloading=temps;
   qDebug()<<temps<<endl;
   qDebug()<<downloadurl<<temps;
   downloadurl=downloadurl+temps+"\n";
   ui->textBrowser->append(downloadurl);
    QUrl url(temps);
    QNetworkRequest requset;
    requset.setUrl(url);
    QNetworkAccessManager manager;
    qDebug() << "Download img form  " << url;
    downloadurl=downloadurl+temps+"\n";
    ui->textBrowser->append(downloadurl);
    //发出请求
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString temp;
    temp=path+"/"+url.fileName();
    qDebug()<<"url.fileName is :   "<<url.fileName()<<endl;
    qDebug()<<"temp path is :   "<<temp<<endl;

    QFile file(temp);
    file.open(QIODevice::Append);
    if(file.isOpen())
    {
    file.write(reply->readAll());
    file.close();
    qDebug()<<"img had download"<<endl;
    }
    else
    {
        ui->failreport->append(nowloading);
    }
    filesavepath=filesavepath+temp+"\n";
    ui->textBrowser->append(filesavepath);
    i++;

   }

}
void MainWindow::getEshuushuuImage()
{
    ui->textBrowser->setText("正在下载");
    QString next;
    QStringList nextlist;
    static int page=ui->spinBox->value();
    int i;
    getwebcode("http://e-shuushuu.net/");
    saveEshuushuu(code);
    qDebug()<<"next page ready"<<endl;
    nextlist=regulars.searchnext(code);
    qDebug()<<nextlist<<endl;
    if(!nextlist.isEmpty())
  {  next=nextlist.takeFirst();
     next.replace("<li class=\"next\"><a href=\"","http://e-shuushuu.net/");
     next.replace("\"","");
    for(i=1;i<=page;i++)
    {
        qDebug()<<next<<endl;
        saveEshuushuu(getwebcode(next));
        nextlist=regulars.searchnext(next);
        if(!nextlist.isEmpty())
        {next=nextlist.takeFirst();
        next.replace("<li class=\"next\"><a href=\"","http://e-shuushuu.net");
        next.replace("\"","");
        }
    }
    qDebug()<<"循环结束"<<endl;


   }
    ui->textBrowser->setText("下载结束");
}
void MainWindow::getPixivImage()
{
    ui->label->hide();
    ui->textBrowser->hide();
    ui->spinBox->hide();
    ui->lineEdit->setText("此功能尚未完成");
}
void MainWindow::getYandeImage()
{
    if(ui->lineEdit->text()=="")
    {
        ui->failreport->setText("请先输入一个Y站图片ID");
    }
    else
    {
    ui->lineEdit->show();
    ui->label->hide();
    ui->textBrowser->setText("正在下载");
    QString next;
    QStringList nextlist;
    static int page=ui->spinBox->value();
     int i;
    QString t("https://yande.re/post");
    t=t+"/show/"+ui->lineEdit->text();
    nowloading=t;
    QString tempcode=getwebcode(t);
   saveYande(tempcode);
   nextlist=regulars.search("<li><a href=\"(.*)\">Previous",tempcode);//* next page
   qDebug()<<"Previous list is :"<<nextlist<<endl;
   if(!nextlist.isEmpty())
   {
       next=nextlist.takeFirst();
       next.replace("<li><a href=\"","https://yande.re");
       next.replace("\">Previous","");
       nowloading=next;
       QString tempstr;
       for(i=1;i<=page;i++)
       {
           qDebug()<<next<<endl;
           tempstr=getwebcode(next);
           saveYande(tempstr);
           nextlist=regulars.search("<li><a href=\"(.*)\">Previous",tempstr);//查找下一页
           if(!next.isEmpty())
           {
                next=nextlist.takeFirst();
                next.replace("<li><a href=\"","https://yande.re");
                next.replace("\">Previous","");
                nowloading=next;
            }
           else
           {
               ui->failreport->append(nowloading);
           }
        }
       qDebug()<<"循环结束"<<endl;
       ui->textBrowser->setText("下载结束");
        }
    }
}
void MainWindow::aboutus()
{
    about aboutus;
    aboutus.exec();
}
void MainWindow::saveYande(QString tempcode)
{
    QUrl urly;
    QString tempstring;
    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(conf);QNetworkAccessManager manager;
    QStringList imglist=regulars.search("<a class=\"original-file-changed\" id=\"highres\" href=\"(.*).(jpg|png)",tempcode);
    qDebug()<<imglist<<endl;
    if(!imglist.isEmpty())

    {
        imglist[0].replace("<a class=\"original-file-changed\" id=\"highres\" href=\"","");
        imglist[0].replace("\"","");
        tempstring=imglist.takeFirst();
        urly=tempstring;
        request.setUrl(urly);
        qDebug() << downloadurl<< urly;
        downloadurl=downloadurl+tempstring+"\n";
        ui->textBrowser->append(downloadurl);
        //发出请求
        QEventLoop loop;
        QNetworkReply *reply = manager.get(request);
      QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QString temp;
        temp=path+"/"+urly.fileName();
        qDebug()<<"url.fileName is :   "<<urly.fileName()<<endl;
        qDebug()<<"temp path is :   "<<temp<<endl;

        QFile file(temp);
        file.open(QIODevice::Append);
        file.write(reply->readAll());
        file.close();
        qDebug()<<"img had download"<<endl;
        filesavepath=filesavepath+temp+"\n";
        ui->textBrowser->append(filesavepath);
   }
    else
       {qDebug()<<"Fail"<<endl;
        ui->failreport->append(nowloading);
         }
}
