#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QtGui>
#include "GameWidget.h"

// �����ڲ�����
class Widget : public QWidget
{
    Q_OBJECT

public:
    // ����&��������
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    // ��restart����ť
    QPushButton *restartButton;
    QPushButton *closeButton;
    // ��score����ǩ
    QLabel *titleLabel;
    QLabel *tipsLabel;
    QLabel *scoreLbl;
    // ��HightScore����ǩ
    QLabel *highScoreLbl;
    // ��Ϸ����
    GameWidget *gameWidget;
    // ��Ⱥ͸߶ȵ����ű��� ����ʹ���ڲ����������ڵĳߴ�ı���ı�λ����ߴ�
    qreal ratioW, ratioH;
    // ��¼��߷�
    int highScore;
    QPoint dragPosition;
    bool bPressFlag;

protected:
    // ���ڳߴ�ı䴥�����¼�
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    // ������������źŵĲۺ���
    void onScoreInc(int);
    // ������Ϸ�����źŵĲۺ���
    void onGameOver();
    // ������Ϸ��ʤ�źŵĲۺ���
    void onWin();
};

#endif // WIDGET_H

