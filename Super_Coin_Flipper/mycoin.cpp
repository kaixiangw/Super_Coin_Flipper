#include "mycoin.h"
#include <QDebug>
#include <QTimer>
#include <QPixmap>

//MyCoin::MyCoin(QWidget *parent) : QWidget(parent)
//{

//}

MyCoin::MyCoin(QString butImg)
{
    QPixmap pixmap;
    if (!pixmap.load(butImg)) {
        qDebug() << butImg << "加载图片失败!";
    }

    this->setFixedSize(pixmap.width(), pixmap.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pixmap);
    this->setIconSize(QSize(pixmap.width(), pixmap.height()));


    timer1_ = new QTimer(this);
    timer2_ = new QTimer(this);
    // 监听正面翻转的信号槽
    connect(timer1_, &QTimer::timeout, this, [=]() {
        QPixmap pixmap;
        QString str = QString(":/res/Coin000%1.png").arg(this->min_++);
        pixmap.load(str);
        this->setFixedSize(pixmap.width(), pixmap.height() );
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
        if(this->min_ > this->max_) { //如果大于最大值，重置最小值，并停止定时器
            this->min_ = 1;
            isAnimation_ = false;
            timer1_->stop();
        }
    });

    // 监听反面翻转的信号槽
    connect(timer2_, &QTimer::timeout, this, [=]() {
        QPixmap pixmap;
        QString str = QString(":/res/Coin000%1.png").arg((this->max_)--);
        pixmap.load(str);
        this->setFixedSize(pixmap.width(), pixmap.height() );
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
        if(this->max_ < this->min_) { //如果小于最小值，重置最大值，并停止定时器
            this->max_ = 8;
            isAnimation_ = false;
            timer2_->stop();
        }
    });


}

void MyCoin::ChangeFlag()
{
    if (this->flag_) { // 正面
        timer1_->start(30);
        isAnimation_ = true;
        this->flag_ = false;
    }
    else //反面
    {
        timer2_->start(30);
        isAnimation_ = true;
        this->flag_ = true;
    }
}

void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if (this->isAnimation_ || isWin_ == true)
        return;
    else
        return QPushButton::mousePressEvent(e);
}
