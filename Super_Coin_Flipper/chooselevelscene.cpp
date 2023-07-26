#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "playscene.h"
#include <QSound>
#include <QMessageBox>

// 返回按钮音效
QSound *backSound = new QSound(":/res/BackButtonSound.wav");

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    // 关卡选择窗口基本设置
    this->setFixedSize(390, 570);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("Select Game Level");

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
    QAction *instructionsAction = helpMenu->addAction("Instructions");
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

    //点击【Game】-【Quit】退出游戏
    connect(quitAction, &QAction::triggered, this, [=](){this->close();});

    // 选择关卡场景游戏界面设置
    // 创建返回按钮
    MyPushButton *closeBtn =
            new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width()-closeBtn->width(), this->height()-closeBtn->height());

    //点击返回按钮触发自定义信号，关闭自身，该信号写到 signals下做声明
    connect(closeBtn, &MyPushButton::clicked, this, [=]() {
        QTimer::singleShot(500, this, [=]() {
            backSound->play();
            this->hide();
            emit this->chooseSceneBack();
        });
    });

    // 创建选择关卡按钮
    for (int i = 0; i < 20; i++) {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(40 + (i%4)*80 , 130 + (i/4)*80);

        //按钮上显示的文字
        QLabel * label = new QLabel;
        QFont font("Arial", 16, QFont::Bold);
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(i + 1));
        label->setFont(font);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //设置居中
        label->move(40 + (i%4)*80 , 130 + (i/4)*80);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);  //鼠标事件穿透

        // 监听选择关卡界面返回按钮的信号槽
        connect(menuBtn, &MyPushButton::clicked, this, [=]() {
            // qDebug() << "select: " << i;
            if (pScene_ == nullptr) {
                backSound->play();
                this->hide();
                pScene_ = new PlayScene(i+1); //将选择的关卡号 传入给PlayerScene
                pScene_->show();
            }

            // 监听翻金币界面返回按钮的信号槽
            connect(pScene_, &PlayScene::chooseSceneBack, this, [=]() {
                this->show();
                delete pScene_;
                pScene_ = nullptr;
            });
        });
    }
}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixBackground, pixTitle;

    // 设置背景图片和标题图片
    pixBackground.load(":/res/OtherSceneBg.png");
    pixTitle.load(":/res/Title.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pixBackground);
    painter.drawPixmap((this->width() - pixTitle.width())*0.5, 30,
                       pixTitle.width(), pixTitle.height(), pixTitle);
}


