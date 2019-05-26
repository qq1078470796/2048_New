#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QList>

// ���Ƶķ���
enum HandWay
{
    LEFT = 0,   // ����
    RIGHT = 1,  // ����
    UP = 2,     // ����
    DOWN = 3    // ����
};

// ���嶯��������
enum AnimationType
{
    MOVE = 0,       // �����ƶ�����
    START = 1  // ������ֶ���
};

// �����ṹ��
struct Animation
{
    AnimationType type;     // ��������
    HandWay way;   // ����
    QPointF startPos;       // ��ʼ������ ���ֶ�������ʹ���������
    QPointF endPos;         // ��ֹ������ �ƶ��������յ�����
    int digit;              // ����
    int digit2;             // �ڶ����� ������ܱ��ϲ�
};

// ��Ϸ������ �̳���QWidget
class GameWidget : public QWidget
{
    Q_OBJECT
public:
    // ���캯��
    explicit GameWidget(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent * event);

private:
    // ��Ϸ��� �洢ÿ�����ӵ���ֵ
    int board[4][4];
    // ����ĸ��� �洢��ǰ����ϵ����ֵĸ���
    int digitNumber;
    // ���� �洢��ǰ�÷�
    int score;
    // ��ʼ������ �洢�����������
    QPoint startPos;
    // �洢������Ҫչ�ֵĶ���
    QList<Animation> animationList;
    // С���ӵĿ�Ⱥ͸߶�
    qreal w, h;
    // ����ͼ��
    QImage *cacheImg;
    // �Ƿ��ڲ��Ŷ���Ч��
    bool isPlayingAnimation;
    // �����Ϸ�Ƿ����
    bool checkGameOver();
    // �����Ϸ�Ƿ��ʤ
    bool checkWin();
    /* ��ȡһ�����ֵĶ�����λ�� ��Ȼ�����ȡ�Ĳ���ȫ�Ƕ�����λ�� ���Ƕ�Ӧ��ɫ������±�
    ���� 2 ��Ӧ 0  4 ��Ӧ1  8 ��Ӧ 2*/
    int getBitCount(int);
    // ���ƶ���Ч��
    bool playAnimation(Animation&, QPainter&);
    // ��갴�´������¼�
    void mousePressEvent(QMouseEvent *);
    // ����ͷŴ�����ʱ��
    void mouseReleaseEvent(QMouseEvent *);
    // �����¼�
    void paintEvent(QPaintEvent *);

    // ����ΪһЩ�ź�
signals:
    // �����ƶ��ź�
    void HandWayMove(HandWay);
    // ���������ź�
    void ScoreInc(int);
    // ��Ϸ�����ź�
    void GameOver();
    // ��Ϸ��ʤ�ź�
    void win();

    // ����ΪһЩ�ۺ���
public slots:
    // ���������ƶ��źŵĲۺ���
    void onHandway(HandWay);
    // ���¿�ʼ�Ĳۺ���
    void restart();

};

#endif // GAMEWIDGET_H

