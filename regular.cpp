#include"regular.h"
#include "regular.h"
#include"QRegularExpression"
#include"QString"
#include"QFile"

QStringList regular::search(QString regulardata,QString data)
{
    QStringList searchdata;
    QRegularExpression regularExpression(regulardata);
        int index = 0;
        QRegularExpressionMatch match;
        do {
            match = regularExpression.match(data, index);
            if(match.hasMatch()) {
                index = match.capturedEnd();
                searchdata.append(match.captured(0));
            } else {
                break;
            }
        } while(index < data.length());
        return searchdata;
}
QStringList regular::searchimg(QString data)
{
    QString regulardata=QString("<a class=\"thumb_image\" href=\"(.*)\.(png|jpeg)");
     QStringList searchdata=search(regulardata,data);
    return searchdata;
}
QStringList regular::searchnext(QString data)
{
    QString regulardata=QString("<li class=\"next\"><a href=\"(.*)\"");
    QStringList searchdata=search(regulardata,data);
    return searchdata;
}
