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

    tipsLabel = new QLabel(tr("��ӭ����2048"), this);
    tipsLabel->setStyleSheet("color:#473C8B;");
    tipsLabel->setFont(QFont("arial", 10, QFont::Normal));
    // ��ʼ����߷�Ϊ0
    highScore = 0;
    // ��ȡ�洢��߷ֵ��ļ�
    QFile file("high.txt");
    if (file.open(QIODevice::ReadOnly))
    {
        file.read((char *)&highScore, sizeof(highScore));
        file.close();
    }

    // ����һ����Ϸ��������
    gameWidget = new GameWidget(this);
    gameWidget->setFocus();
    connect(gameWidget, SIGNAL(ScoreInc(int)), this, SLOT(onScoreInc(int)));
    connect(gameWidget, SIGNAL(GameOver()), this, SLOT(onGameOver()));
    connect(gameWidget, SIGNAL(win()), this, SLOT(onWin()));

    // ����һ���������
    QFont font;
    font.setFamily("Arial");
    font.setBold(true);
    font.setPixelSize(15);

    // ����һ����ť����
    restartButton = new QPushButton(tr("�µ���Ϸ"), this);
    restartButton->setFont(font);
    restartButton->setStyleSheet("QPushButton{color: white;background:#907963;border-style:flat;}");
    restartButton->setFocusPolicy(Qt::NoFocus);
    connect(restartButton, SIGNAL(clicked()), gameWidget, SLOT(restart()));

    // ����һ����ǩ����
    highScoreLbl = new QLabel(QString(tr("��߷�\n%1")).arg(highScore),this);
    highScoreLbl->setFont(font);
    highScoreLbl->setAlignment(Qt::AlignCenter);
    highScoreLbl->setStyleSheet("QLabel{color:#FFFFEE;background:#BEAD9D}");

    // ����һ����ǩ����
    scoreLbl = new QLabel(tr("��ǰ����\n0"), this);
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

    // ���ô��ڴ�С
    resize(400, 510);
    move((QApplication::desktop()->width() - width())/2,  (QApplication::desktop()->height() - height())/2);
}

Widget::~Widget()
{
    // �ͷ���ض���
    delete restartButton;
    delete scoreLbl;
    delete highScoreLbl;
    delete gameWidget;
}

void Widget::onScoreInc(int score)
{
    // ���·�����ʾ
    scoreLbl->setText(QString(tr("��߷�:\n %1")).arg(score));
    // �����ǰ����������߷�
    if (score > highScore)
    {
        // ������߷�
        highScore = score;
        highScoreLbl->setText(QString(tr("��߷�:\n %1")).arg(highScore));

        // ���µ���߷ִ����ļ�
        QFile file("high.txt");
        file.open(QIODevice::WriteOnly);
        file.write((char *)&highScore, sizeof(highScore));
        file.close();
    }
}

void Widget::onGameOver()
{
    QMessageBox::information(this, tr("��Ϸ����"), "You lost !");
}

void Widget::onWin()
{
    QMessageBox::information(this, tr("ʤ����"), "You win !");
}

void Widget::resizeEvent(QResizeEvent *)
{
    // �����Ⱥ͸߶ȵ����ű���
    ratioW = width() / 400.0f;
    ratioH = height() / 510.0f;
    // �����Ӳ�����С��λ��
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

