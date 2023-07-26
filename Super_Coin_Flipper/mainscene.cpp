#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include "chooselevelscene.h"
#include "QTimer"
#include <QMessageBox>
#include <QSound>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 主场景窗口基本设置
    this->setFixedSize(390, 570);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("Super Coin Flipper");

    // 菜单栏设置
    // 点击【Game】-【Quit】退出游戏
    connect(ui->actionQuit, &QAction::triggered, this, [=]() {
        this->close();
    });

    // 点击【Help】-【Instructions】显示游戏说明
    connect(ui->instructionsAction, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, "Game Instructions", QString("There are 20 levels in this game, "
                                                        "and each level has a 4*4 grid with gold and silver coins distributed inside. "
                                                        "Click on the coin to turn the surrounding coins with it. "
                                                        "When only gold coins are left in the grid, "
                                                        "the level will be successfully broken."));
    });

    // 点击【Help】-【About】显示关于信息
    connect(ui->authorAction, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, "About", QString("<font size=\"5\" face=\"arial\" color=\"black\">Super Coin Flipper<br><br></font>"
                                                  "<font size=\"4\" face=\"arial\" color=\"black\">Based on Qt 5.12.12 (MinGW 7.3.0 64-bit)<br><br>"
                                                  "Built on Jul 26<br><br>"
                                                  "Copyright 2023 <font color=\"blue\">Kaixiang Wu.</font> All rights reserved.<br><br></font>"
                                                  "<font size=\"3\" face=\"arial\" color=\"black\">As a Qt beginner, I referred to many senior blogs and tutorials in the process of completing this project, "
                                                  "thank you all of you here! "
                                                  "Welcome to communicate with me or provide some opinions: kxwu2002@gmail.com</font>"));
    });

    // 创建开始按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5, this->height()*0.7);
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this); // 开始按钮音效

    // 开始按钮动作
    connect(startBtn, &MyPushButton::clicked, this, [=]() {
        backSound->play();
        // 开始按钮动画
        startBtn->ZoomDown(); //向下跳跃
        startBtn->ZoomUp();   //向上跳跃

        // 进入选择关卡场景
        QTimer::singleShot(500, this, [=](){
            this->hide();
            chooseScene->show();
        });
    });

    // 监听选择关卡场景的返回按钮
    connect(chooseScene, &ChooseLevelScene::chooseSceneBack, this, [=]() {
        chooseScene->hide();
        this->show();
    });
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixBackground, pixTitle;

    // 设置背景图片和标题图片
    pixBackground.load(":/res/PlayLevelSceneBg.png");
    pixTitle.load(":/res/Title.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pixBackground);
    painter.drawPixmap(70, 100, pixTitle.width(), pixTitle.height(), pixTitle);
}

