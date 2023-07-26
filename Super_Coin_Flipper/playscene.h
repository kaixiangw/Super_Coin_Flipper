#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    // explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int index);
    void paintEvent(QPaintEvent *);

signals:
    void chooseSceneBack();
    void playSceneNew();

private:
     int levalIndex_;
     int gameArray_[4][4];
     bool isWin_ = true;         // 是否胜利
     MyCoin * coinBtn_[4][4];   //金币按钮数组
};

#endif // PLAYSCENE_H
