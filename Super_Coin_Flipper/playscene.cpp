#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QSound>
#include <QMessageBox>
#include "chooselevelscene.h"

//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}

PlayScene::PlayScene(int index) : levalIndex_(index)
{
    // 翻金币场景基本设置
    this->setFixedSize(390, 570);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("Breakthrough begins!");

    // 菜单栏设置
    // 创建菜单栏
    QMenuBar *startBar = this->menuBar();
    this->setMenuBar(startBar);
    QMenu *startMenu = startBar->addMenu("Game");
    QAction *quitAction = startMenu->addAction("Quit");


    // 创建帮助栏
    QMenuBar *helpBar = this->menuBar();
    this->setMenuBar(helpBar);
    QMenu *helpMenu = helpBar->addMenu("Help");
    QAction *instructionsAction = helpMenu->addAction("Game Instructions");
    QAction *authorAction = helpMenu->addAction("About");

    // 点击【Help】-【Instructions】显示游戏说明
    connect(instructionsAction, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, "Instructions", QString("There are 20 levels in this game, "
                                                        "and each level has a 4*4 grid with gold and silver coins distributed inside. "
                                                        "Click on the coin to turn the surrounding coins with it. "
                                                        "When only gold coins are left in the grid, "
                                                        "the level will be successfully broken."));
    });

    // 点击【Help】-【About】显示关于信息
    connect(authorAction, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, "About", QString("<font size=\"5\" face=\"arial\" color=\"black\">Super Coin Flipper<br><br></font>"
                                                  "<font size=\"4\" face=\"arial\" color=\"black\">Based on Qt 5.12.12 (MinGW 7.3.0 64-bit)<br><br>"
                                                  "Built on Jul 26<br><br>"
                                                  "Copyright 2023 <font color=\"blue\">Kaixiang Wu.</font> All rights reserved.<br><br></font>"
                                                  "<font size=\"3\" face=\"arial\" color=\"black\">As a Qt beginner, I referred to many senior blogs and tutorials in the process of completing this project, "
                                                  "thank you all of you here! "
                                                  "Welcome to communicate with me or provide some opinions: kxwu2002@gmail.com</font>"));
    });


    //点击【Game】-【Quit】 退出游戏
    connect(quitAction, &QAction::triggered,this, [=]() {this->close();});

    // 创建返回按钮
    MyPushButton *closeBtn =
            new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width()-closeBtn->width(), this->height()-closeBtn->height());

    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this); // 返回按钮音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);    // 翻金币音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);    // 胜利按钮音效

    // 点击返回按钮触发自定义信号，关闭自身，该信号写到 signals下做声明
    connect(closeBtn, &MyPushButton::clicked, this, [=]() {
        backSound->play();
        QTimer::singleShot(500, this, [=]() {
            this->hide();
            emit this->chooseSceneBack();
        });
    });

    //当前关卡标题
    QLabel *label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(15);
    label->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    label->setPalette(pe);
    QString str = QString("Leavel: %1").arg(this->levalIndex_);
    label->setText(str);
    label->setGeometry(QRect(30, this->height()-50, 120, 50)); //设置大小和位置

    // 准备胜利图片
    QLabel *winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0, 0, tmpPix.width(), tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width())*0.5, -tmpPix.height());

    // 初始化各个关卡
    dataConfig config;
    for (int i = 0 ; i < 4;i++) {
        for (int j = 0; j < 4; j++) {
            gameArray_[i][j] = config.mData[this->levalIndex_][i][j];
        }
    }

    //创建金币的背景图片
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            QLabel* label = new QLabel;
            QPixmap pix;
            pix.load(":/res/BoardNode.png");
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(95 + i*50, 200 + j*50);

            // 初始化金币对象
            QString img;
            if (gameArray_[i][j] == 1)
              img = ":/res/Coin0001.png";
            else
              img = ":/res/Coin0008.png";

            MyCoin * coin = new MyCoin(img);
            coin->setParent(this);
            coin->move(97 + i*50, 204+j*50);
            coin->posX_ = i; //记录x坐标
            coin->posY_ = j; //记录y坐标
            coin->flag_ = gameArray_[i][j]; //记录正反标志
            coinBtn_[i][j] = coin;

            // 监听每个按钮的点击效果，并翻转金币
            connect(coin, &MyCoin::clicked, this, [=]() {
                flipSound->play();
                for(int i = 0; i < 4;i++) {
                    for(int j = 0; j < 4; j++) {
                        coinBtn_[i][j]->isWin_ = true;
                    }
                }

                coin->ChangeFlag();
                gameArray_[i][j] = (gameArray_[i][j] == 0 ? 1 : 0);


                // 翻转周围硬币
                QTimer::singleShot(300, this,[=](){
                    if ((coin->posX_ + 1) <= 3) {
                        coinBtn_[coin->posX_ + 1][coin->posY_]->ChangeFlag();
                        gameArray_[coin->posX_ + 1][coin->posY_] =
                                (gameArray_[coin->posX_ + 1][coin->posY_] == 0 ? 1 : 0);
                    }
                    if ((coin->posX_ - 1) >= 0) {
                        coinBtn_[coin->posX_ - 1][coin->posY_]->ChangeFlag();
                        gameArray_[coin->posX_ - 1][coin->posY_] =
                                (gameArray_[coin->posX_ - 1][coin->posY_] == 0 ? 1 : 0);
                    }
                    if ((coin->posY_ + 1) <= 3) {
                        coinBtn_[coin->posX_ ][coin->posY_ + 1]->ChangeFlag();
                        gameArray_[coin->posX_][coin->posY_ + 1] =
                                (gameArray_[coin->posX_][coin->posY_ + 1] == 0 ? 1 : 0);
                    }
                    if ((coin->posY_ - 1) >= 0) {
                        coinBtn_[coin->posX_ ][coin->posY_ - 1]->ChangeFlag();
                        gameArray_[coin->posX_][coin->posY_ - 1] =
                                (gameArray_[coin->posX_][coin->posY_ - 1] == 0 ? 1 : 0);
                    }

                    for(int i = 0; i < 4;i++) {
                        for(int j = 0; j < 4; j++) {
                            coinBtn_[i][j]->isWin_ = false;
                        }
                    }

                    // 胜利检测
                    isWin_ = true;
                    for (int i = 0 ; i < 4;i++) {
                        for (int j = 0 ; j < 4; j++) {
                            //qDebug() << coinBtn[i][j]->flag ;
                            if (coinBtn_[i][j]->flag_ == false)
                            {
                                isWin_ = false;
                                break;
                            }
                        }
                    }
                    if (isWin_) {
                        //qDebug() << "胜利";
                        winSound->play();
                        for(int i = 0; i < 4;i++) {
                            for(int j = 0; j < 4; j++) {
                                coinBtn_[i][j]->isWin_ = true;
                            }
                        }

                        QPropertyAnimation * animation =  new QPropertyAnimation(winLabel, "geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                        animation->setEndValue(QRect(winLabel->x(), winLabel->y()+114, winLabel->width(), winLabel->height()));
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }
                });
             });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixBackground, pixTitle;

    // 设置背景图片和标题图片
    pixBackground.load(":/res/PlayLevelSceneBg.png");
    pixTitle.load(":/res/Title.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pixBackground);
    painter.drawPixmap((this->width() - pixTitle.width())*0.5, 30,
                       pixTitle.width(), pixTitle.height(), pixTitle);
}


