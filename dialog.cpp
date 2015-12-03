#include "dialog.h"
#include "ui_dialog.h"
#include <QtNetwork>
#include <QTextCodec>
#include <QtWidgets>
#include <QDebug>
#include <QTimer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    index[0]=0;
    index[1]=0;
    //UI初始化
    ui->setupUi(this);
    setWindowTitle(tr("天气小助手"));
    setFixedSize(width(), height());
    setWindowIcon(QIcon(QPixmap(":/rc/cloud2.png")));
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    ui->labelCity->setPixmap(QPixmap(":/rc/cloud2.png"));
    showClear();

    cityManager_.getIndex((OnMessage2)std::bind(&Dialog::onIndex, this,std::placeholders::_1,std::placeholders::_2));
    ui->citys->clear();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onIndex(const int &x,const QString &city){
    qDebug() << "onIndex is called";
    this->index[0]=x;
    this->city=city;
    cityManager_.getSupportProvince((OnMessage)std::bind(&Dialog::onProvinces, this, std::placeholders::_1));

}


/*
 * 功能：加载省份列表到下拉框
 * @para:解析省份列表XML数据得到的List
 * 此函数在CityManager::replyFinished中被回调
 */
void Dialog::onProvinces(const QStringList &strList){
    qDebug() << "onProvince is called=======================================";
    int i=0;
    QStringList::const_iterator citer = strList.constBegin();

    for (i=0 ; citer != strList.constEnd(); ++citer) {
        ui->provinces->addItem(*citer);
        i++;

    }
    ui->provinces->setCurrentIndex(index[0]);
    ui->citys->clear();
    qDebug() << "onProvince调用cityManager.getSupportCity====" << ui->provinces->currentText();
    cityManager_.getSupportCity(ui->provinces->currentText(),(OnMessage)std::bind(&Dialog::onFirstCitys, this, std::placeholders::_1));


}



/*
 * 功能：加载城市列表到下拉框
 * @para:解析城市列表XML数据得到的List
 * 此函数在CityManager::replyFinished中被回调
 */
void Dialog::onCitys(const QStringList &strList)
{
    qDebug() << "onCitys is called";
    ui->citys->clear();
    QStringList::const_iterator citer = strList.constBegin();
    for ( ; citer != strList.constEnd(); ++citer) {
        QString str = citer->left(citer->indexOf(QChar(' ')));
        ui->citys->addItem(str);
    }
}
void Dialog::onFirstCitys(const QStringList &strList){
    qDebug() << "onFirstCitys is called";

    ui->citys->clear();
    QStringList::const_iterator citer = strList.constBegin();

    for (int i=0 ; citer != strList.constEnd(); ++citer) {
        QString str = citer->left(citer->indexOf(QChar(' ')));
        if(this->city==str){
            index[1]=i;
        }

        ui->citys->addItem(str);
        i++;
    }


   // cityManager_.getWeatherbyCityName(this->city, (OnMessage)std::bind(&Dialog::onWeather, this, std::placeholders::_1));
    ui->citys->setCurrentIndex(index[1]);

}


/*
 * 功能：设置天气界面
 * @para:解析天气XML数据得到的List
 * 此函数在CityManager::replyFinished中被回调
 */
void Dialog::onWeather(const QStringList &strList)
{
    if (strList.count() < 23) {
        return;
    }

    QString strImage(":/rc/");
    ui->labelImage1->setPixmap(QPixmap(strImage + strList[8]));
    ui->labelImage2->setPixmap(QPixmap(strImage + strList[9]));
    ui->labelTemperature->setText(strList[5]);
    ui->labelWeather->setText(strList[6]);
    ui->labelWind->setText(strList[7]);

    ui->labelImage3->setPixmap(QPixmap(strImage + strList[15]));
    ui->labelImage4->setPixmap(QPixmap(strImage + strList[16]));
    ui->labelTemperature_2->setText(strList[12]);
    ui->labelWeather_2->setText(strList[13]);
    ui->labelWind_2->setText(strList[14]);
    ui->recomand->setText(strList[11]);
    ui->weather_1->setText(strList[10]);
    ui->advice->setText("温馨提示,天气小助手建议如下:");

}

//当Provinces下拉框内容改变
void Dialog::on_provinces_currentIndexChanged(const QString &provinceName)
{
    showClear();
    //获取支持的城市列表，绑定更新UI的回调函数
     qDebug() << "on_provinces_currentIndexChanged 调用cityManager.getSupportCity====" << provinceName;
    cityManager_.getSupportCity(provinceName, (OnMessage)std::bind(&Dialog::onCitys, this, std::placeholders::_1));
}
//当查询按钮被点击
void Dialog::on_queryButton_clicked()
{
    //获取天气，绑定更新UI的回调函数
    cityManager_.getWeatherbyCityName(ui->citys->currentText(), (OnMessage)std::bind(&Dialog::onWeather, this, std::placeholders::_1));
}

/*
 * 清除UI
 * */
void Dialog::showClear()
{
    ui->labelImage1->clear();
    ui->labelImage2->clear();
    ui->labelTemperature->clear();
    ui->labelWeather->clear();
    ui->labelWind->clear();

    ui->labelImage3->clear();
    ui->labelImage4->clear();
    ui->labelTemperature_2->clear();
    ui->labelWeather_2->clear();
    ui->labelWind_2->clear();
    ui->recomand->clear();
    ui->advice->clear();
    ui->weather_1->clear();
}

void Dialog::on_citys_currentIndexChanged(const QString &)
{
    showClear();
}
