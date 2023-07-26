#ifndef MYCOIN_H
#define MYCOIN_H
#include <QTimer>
#include <QPushButton>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyCoin(QWidget *parent = nullptr);
    MyCoin(QString butImg);
    void ChangeFlag();  //改变标志，执行翻转效果
    bool isAnimation_  = false; //做翻转动画的标志
    void mousePressEvent(QMouseEvent *e);

    int posX_, posY_;   // 金币位置
    int min_ = 1;       // 动画帧图片最小标号
    int max_ = 8;       // 动画帧图片最大标号
    bool flag_;         // 金币正反
    bool isWin_ = false; //胜利标志
    QTimer *timer1_;    // 正面翻反面定时器
    QTimer *timer2_;    // 反面翻正面定时器

signals:
};

#endif // MYCOIN_H
