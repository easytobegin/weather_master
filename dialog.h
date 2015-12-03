#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "citymanager.h"

namespace Ui {
class Dialog;
}

class QNetworkAccessManager;
class QNetworkReply;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void onProvinces(const QStringList &strList);
    void onIndex(const int &x,const QString &city);
    void onCitys(const QStringList &strList);
    void onFirstCitys(const QStringList &strList);
    void onWeather(const QStringList &strList);


private slots:
    void on_provinces_currentIndexChanged(const QString &provinceName);
    void on_queryButton_clicked();
    void showClear();

    void on_citys_currentIndexChanged(const QString &arg1);

private:
    Ui::Dialog *ui;
    CityManager cityManager_;
    int index[2];
    QString city;
};

#endif // DIALOG_H
