#include "citymanager.h"
#include <QtNetwork>
#include <QTextCodec>
#include <QXmlStreamReader>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>
#include<QJsonObject>
#include <QtDebug>


CityManager::CityManager(QObject *parent)
    : QObject(parent)
{
    net_ = new QNetworkAccessManager(this);
    net2_ = new QNetworkAccessManager(this);
    //建立信号和槽的连接


    QObject::connect(net2_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyGetIndex(QNetworkReply*)));
    QObject::connect(net_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}
/*
 * 功能：获取支持省份
 * @para1：解析完XML后将要回调的函数
 * */
void CityManager::getSupportProvince(const OnMessage &msg)
{
    onMessage_ = msg;
    QUrl url("http://www.webxml.com.cn/WebServices/WeatherWebService.asmx/getSupportProvince?");
    QNetworkRequest request(url);
    net_->get(request);

}
void CityManager::getIndex(const OnMessage2 &msg){
    onMessage2_ = msg;
    QUrl url("http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=js");
    QNetworkRequest request(url);
    net2_->get(request);



}
/*获取获取省份的城市列表
 * @para1：省份名
 * @para2：解析完XML后将要回调的函数
 * */
void CityManager::getSupportCity(const QString &provinceName, const OnMessage &msg)
{
    qDebug() << "getSupportCity is called" << provinceName;
    this->onMessage_ = msg;
    QString str("http://www.webxml.com.cn/WebServices/WeatherWebService.asmx/getSupportCity?byProvinceName=");
    str.append(provinceName);
    QUrl url(str);
    QNetworkRequest request(url);
    net_->get(request);

}
/*功能：获取天气信息
 * @para1:城市名
 * @para2：回调函数
 *
 * */
void CityManager::getWeatherbyCityName(const QString &cityName, const OnMessage &msg)
{
    onMessage_ = msg;
    QString str("http://www.webxml.com.cn/WebServices/WeatherWebService.asmx/getWeatherbyCityName?theCityName=");
    str.append(cityName);
    QUrl url(str);
    QNetworkRequest request(url);
    net_->get(request);

}


/*响应信号SIGNAL(finished(QNetworkReply*))的槽函数
 * 作用是将reply中的XML数据解析为List
 * 并将List数据通过绑定的OnMessage回调函数发送出去
 *
 */
void CityManager::replyFinished(QNetworkReply *reply)
{

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString all = codec->toUnicode(reply->readAll());
    //qDebug() <<"Fuck" << all ;
    QStringList list;
    QXmlStreamReader reader(all);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if (reader.name() == "string") {
                list.append(reader.readElementText());
            }
        }
    }

    if (onMessage_) {
        onMessage_(list);
    }


    reply->deleteLater();
}




void CityManager::replyGetIndex(QNetworkReply *reply){
    QString province;
    QString city;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString json = codec->toUnicode(reply->readAll());
    //QString json=reply->readAll();
    json=json.right(146);
    json=json.left(145);
    qDebug() << json ;
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();
            province=result["province"].toString();
            city=result["city"].toString();

        }
    } else {
        qDebug() << "解析JSon出错";
        //qFatal(error.errorString().toUtf8().constData());
        //exit(1);
    }
    qDebug() << province;
    qDebug() << city;
    int x=1;

    if(province=="福建")
        x= 20;
    if(province=="江西")
        x= 19;
    if(province=="浙江")
        x= 18;
    if(province=="湖南")
        x= 18;
    if(province=="湖北")
        x= 16;
    if(province=="青海")
        x= 15;
    if(province=="甘肃")
        x= 14;
    if(province=="宁夏")
        x= 13;
    if(province=="陕西")
        x= 12;
    if (onMessage2_) {
        onMessage2_(x,city);
    }



    reply->deleteLater();
}

