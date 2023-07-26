#include "mypushbutton.h"
#include <QDebug>
#include <QPropertyAnimation>

//MyPushButton::MyPushButton(QWidget *parent) : QWidget(parent)
//{

//}


/**
 * @brief MyPushButton 构造函数
 * @param normalImg: 代表正常显示的图片
 * @param pressImg: 代表按下后显示的图片，默认为空
 */
MyPushButton::MyPushButton(QString normalImg, QString pressImg)
        : normalImg_(normalImg), pressImg_(pressImg)
{
    QPixmap pixmap;

    if (!pixmap.load(normalImg_))
        qDebug() << normalImg << "加载图片失败!";
    this->setFixedSize(pixmap.width(), pixmap.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pixmap);
    this->setIconSize(QSize(pixmap.width(), pixmap.height()));
}

void MyPushButton::ZoomDown()
{
    QPropertyAnimation *animation =
            new QPropertyAnimation(this, "geometry"); //创建动画对象
    animation->setDuration(200);                      //设置时间间隔，单位毫秒
    animation->setStartValue(QRect(this->x(), this->y(),
                                    this->width(), this->height()));    //创建起始位置
    animation->setEndValue(QRect(this->x(), this->y()+10,
                                 this->width(), this->height()));       //创建结束位置
    animation->setEasingCurve(QEasingCurve::OutBounce);                 //设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果
    animation->start(); //开始执行动画
}

void MyPushButton::ZoomUp()
{
    QPropertyAnimation *animation =
            new QPropertyAnimation(this, "geometry"); //创建动画对象
    animation->setDuration(200);                      //设置时间间隔，单位毫秒
    animation->setStartValue(QRect(this->x(), this->y()+10,
                                   this->width(), this->height()));     //创建起始位置
    animation->setEndValue(QRect(this->x(), this->y(),
                                 this->width(), this->height()));       //创建结束位置
    animation->setEasingCurve(QEasingCurve::OutBounce);                 //设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果
    animation->start(); //开始执行动画
}

/**
 * @brief MyPushButton::mousePressEvent
 *        重写鼠标按下事件为改变按钮图片
 * @param e
 */
void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if (pressImg_ != "") {
        QPixmap pixmap;

        if(!pixmap.load(pressImg_))
            qDebug() << pressImg_ << "加载图片失败!";

        this->setFixedSize(pixmap.width(), pixmap.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
    }
    return QPushButton::mousePressEvent(e); //交给父类执行按下事件
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (normalImg_ != "") {
        QPixmap pixmap;

        if (!pixmap.load(normalImg_))
            qDebug() << normalImg_ << "加载图片失败!";

        this->setFixedSize( pixmap.width(), pixmap.height() );
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
    }
    return QPushButton::mouseReleaseEvent(e); //交给父类执行释放事件
}
