#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , bPressFlag(false)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#B22222"));
    setPalette(pal);

    titleLabel = new QLabel(tr("2048"), this);
    titleLabel->setStyleSheet("color:#473C8B;");
    titleLabel->setFont(QFont("arial", 30, QFont::Black));

    tipsLabel = new QLabel(tr("欢迎来到2048"), this);
    tipsLabel->setStyleSheet("color:#473C8B;");
    tipsLabel->setFont(QFont("arial", 10, QFont::Normal));
    // 初始化最高分为0
    highScore = 0;
    // 读取存储最高分的文件
    QFile file("high.txt");
    if (file.open(QIODevice::ReadOnly))
    {
        file.read((char *)&highScore, sizeof(highScore));
        file.close();
    }

    // 构造一个游戏部件对象
    gameWidget = new GameWidget(this);
    gameWidget->setFocus();
    connect(gameWidget, SIGNAL(ScoreInc(int)), this, SLOT(onScoreInc(int)));
    connect(gameWidget, SIGNAL(GameOver()), this, SLOT(onGameOver()));
    connect(gameWidget, SIGNAL(win()), this, SLOT(onWin()));

    // 构造一个字体对象
    QFont font;
    font.setFamily("Arial");
    font.setBold(true);
    font.setPixelSize(15);

    // 构造一个按钮对象
    restartButton = new QPushButton(tr("新的游戏"), this);
    restartButton->setFont(font);
    restartButton->setStyleSheet("QPushButton{color: white;background:#907963;border-style:flat;}");
    restartButton->setFocusPolicy(Qt::NoFocus);
    connect(restartButton, SIGNAL(clicked()), gameWidget, SLOT(restart()));

    // 构造一个标签对象
    highScoreLbl = new QLabel(QString(tr("最高分\n%1")).arg(highScore),this);
    highScoreLbl->setFont(font);
    highScoreLbl->setAlignment(Qt::AlignCenter);
    highScoreLbl->setStyleSheet("QLabel{color:#FFFFEE;background:#BEAD9D}");

    // 构造一个标签对象
    scoreLbl = new QLabel(tr("当前分数\n0"), this);
    scoreLbl->setFont(font);
    scoreLbl->setAlignment(Qt::AlignCenter);
    scoreLbl->setStyleSheet("QLabel{color:#FFFFEE;background:#BEAD9D}");

    closeButton = new QPushButton("x", this);
    closeButton->setFocusPolicy(Qt::NoFocus);
    closeButton->setFont(QFont("Arial", 11, QFont::Normal));

    closeButton->setStyleSheet("QPushButton{border-style:flat;color:#00FA9A}"
                        "QPushButton:hover{border-style:flat;color:#FF0000}");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setGeometry(400 - 18, 3, 15, 15);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    // 重置窗口大小
    resize(400, 510);
    move((QApplication::desktop()->width() - width())/2,  (QApplication::desktop()->height() - height())/2);
}

Widget::~Widget()
{
    // 释放相关对象
    delete restartButton;
    delete scoreLbl;
    delete highScoreLbl;
    delete gameWidget;
}

void Widget::onScoreInc(int score)
{
    // 更新分数显示
    scoreLbl->setText(QString(tr("最高分:\n %1")).arg(score));
    // 如果当前分数高于最高分
    if (score > highScore)
    {
        // 更新最高分
        highScore = score;
        highScoreLbl->setText(QString(tr("最高分:\n %1")).arg(highScore));

        // 将新的最高分存入文件
        QFile file("high.txt");
        file.open(QIODevice::WriteOnly);
        file.write((char *)&highScore, sizeof(highScore));
        file.close();
    }
}

void Widget::onGameOver()
{
    QMessageBox::information(this, tr("游戏结束"), "You lost !");
}

void Widget::onWin()
{
    QMessageBox::information(this, tr("胜利！"), "You win !");
}

void Widget::resizeEvent(QResizeEvent *)
{
    // 计算宽度和高度的缩放比例
    ratioW = width() / 400.0f;
    ratioH = height() / 510.0f;
    // 重置子部件大小和位置
    titleLabel->setGeometry(20 * ratioW, 40 * ratioH , 130 * ratioW, 50 * ratioH);
    tipsLabel->setGeometry(20 * ratioW, 100 * ratioH , 300 * ratioW, 20 * ratioH);
    gameWidget->setGeometry(18 * ratioW, 140 * ratioH, 365 * ratioW, 365 * ratioH);
    restartButton->setGeometry(210 * ratioW, 90 * ratioH, 170 * ratioW, 30 * ratioH);
    highScoreLbl->setGeometry(300 * ratioW, 40 * ratioH, 80 * ratioW, 40 * ratioH);
    scoreLbl->setGeometry(210 * ratioW, 40 * ratioH, 80 * ratioW, 40 * ratioH);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    gameWidget->setFocus();
    QWidget::keyPressEvent(event);
}
void Widget::mousePressEvent ( QMouseEvent * event)
{
    bPressFlag = true;
    dragPosition = event->pos();
    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (bPressFlag) {
        QPoint relaPos(QCursor::pos() - dragPosition);
        move(relaPos);
    }
    QWidget::mouseMoveEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    bPressFlag = false;
    QWidget::mouseReleaseEvent(event);
}

