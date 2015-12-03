#ifndef CITYMANAGER_H
#define CITYMANAGER_H
#endif // CITYMANAGER_H
#include <QObject>
#include <functional>

class QNetworkAccessManager;
class QNetworkReply;

typedef std::function<void (const QStringList &strList)> OnMessage;
typedef std::function<void (const int &z,const QString &str)> OnMessage2;
class CityManager : public QObject
{
    Q_OBJECT

public:
    CityManager(QObject *parent = 0);

    void getSupportProvince(const OnMessage &msg);
    void getIndex(const OnMessage2 &msg);
    void getSupportCity(const QString &provinceName, const OnMessage &msg);
    void getWeatherbyCityName(const QString &cityName, const OnMessage &msg);
    int getIndexByProvince(QString str);
    QString unicodeToString(QString str);


private slots:
    void replyGetIndex(QNetworkReply *);
    void replyFinished(QNetworkReply *);

private:
    QNetworkAccessManager *net_;
    QNetworkAccessManager *net2_;
    OnMessage onMessage_;
    OnMessage2 onMessage2_;



};


