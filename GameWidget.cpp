#include <QDebug>
#include "GameWidget.h"
#include<QCoreApplication>
// ��ɫ���� �洢ÿ�����ֶ�Ӧ�ı���ɫ
QColor digitBkg[11] = {QColor::fromRgb(0xEE, 0xE5, 0xDB), QColor::fromRgb(0xEC, 0xE0, 0xC8),
                            QColor::fromRgb(0xF2, 0xAF, 0x78), QColor::fromRgb(0xEE, 0x8A, 0x54),
                            QColor::fromRgb(0xFE, 0x76, 0x5E), QColor::fromRgb(0xE7, 0x58, 0xC),
                            QColor::fromRgb(0xFF, 0x66, 0x66), QColor::fromRgb(0xF1, 0xCF, 0x48),
                            QColor::fromRgb(0xCC, 0x33, 0x33), QColor::fromRgb(0xE3, 0xB9, 0x11),
                            QColor::fromRgb(0xFF, 0x00, 0x00)};

// ÿ������λ�õ�����
QPointF dPos[5] = {QPointF(-10, 0), QPointF(10, 0), QPointF(0, -10), QPointF(0, 10), QPointF(-500, -500)};

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent)
{
    // ���������ƶ��źź���Ӧ�Ĳۺ���
    connect(this, SIGNAL(HandWayMove(HandWay)), SLOT(onHandway(HandWay)));
    // ��ʼ��board����
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            board[i][j]=0;
        }
    }
    // �����������2
    board[rand() % 4][rand() % 4] = 2;//��һ������ν�������
    //�ڶ����������ڵ�һ��������
    while(true){
        int x = rand() % 4;
        int y = rand() % 4;
        if (board[x][y] != 0) {
            continue;
        }
        else {
            board[x][y] = 2;
            break;
        }
    }

    // ������ʼ��Ϊ0
    score = 0;
    // ���������ʼ��Ϊ2
    digitNumber = 2;
    isPlayingAnimation = false;
    cacheImg = NULL;

    qDebug() << digitNumber;
}

//���̲���
void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (isPlayingAnimation)// ��ֹ�ظ���������
        return;
    switch (event->key()) {
        case Qt::Key_Left:
            emit HandWayMove(LEFT);
        break;
        case Qt::Key_Right:
            emit HandWayMove(RIGHT);
        break;
        case Qt::Key_Down:
            emit HandWayMove(DOWN);
        break;
        case Qt::Key_Up:
            emit HandWayMove(UP);
        break;
        default:
        break;
    }
    QWidget::keyPressEvent(event);
}
//��갴�����ƣ�����ʱ��ȡ�������
void GameWidget::mousePressEvent(QMouseEvent *e)
{
    // ��ȡ�������
    startPos = e->pos();

    //qDebug() <<startPos;
}

//����ͷ����ƣ��ж����ĸ������ƶ�
void GameWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // ����ڲ��Ŷ���Ч����ֱ���˳���ֹ�ظ����������¼�
    if (isPlayingAnimation)
        return;
    // �����յ����������������XY���������
    float dX = (float)(e->pos().x() - startPos.x());
    float dY = (float)(e->pos().y() - startPos.y());

    qDebug() <<QObject::tr("dx")<<dX;
    qDebug() <<QObject::tr("dY")<<dY;

    //����x��y��  ����x��y��   ����x��y��  ����x��y��
    if (dX<0&&dY>0)
            emit HandWayMove(LEFT);   
    else if(dX>0&&dY<0)
        emit HandWayMove(RIGHT);
    else if(dX<0&&dY<0)
            emit HandWayMove(UP);
    else
         emit HandWayMove(DOWN);
}

void GameWidget::onHandway(HandWay witchWay)
{
    int i, j, k;
    Animation a;
    // �Ƿ�ϲ�������
    bool combine = false;
    // ����ͬ����
    switch (witchWay)
    {
    // ����  ���ϵ���,��������
    case LEFT:
        // ѭ��ÿһ��
        for (i = 0; i < 4; i++)
        {
            /* ����j��ʾҪ�����������к�
             * k��ʾ��������λ�õ��к�
             */
            j = 0, k = 0, combine = false;
            while (true)
            {
                // ѭ���ҵ���һ������0�����ֶ�Ӧ���к�
                while (j < 4 && board[i][j] == 0)
                    j++;
                // ���������3��˵��������� �Ƴ�ѭ��
                if (j > 3)
                    break;
                // ������������
                qSwap(board[i][k], board[i][j]);
              //  qDebug() <<i<<k<<board[i][k];
                // ��¼������Ϣ
                a.type = MOVE;
                a.startPos = QPointF(w * j,h * i);
                qDebug() <<QObject::tr("�������")<<a.startPos;
                a.endPos = QPointF(w * k, h * i);
                qDebug() <<QObject::tr("�����յ�")<<a.endPos;
                a.digit = a.digit2 = board[i][k];
                a.way = LEFT;
                //������������������ǰһ�е�������ͬ
                if (!combine && k > 0 && board[i][k] == board[i][k - 1])
                {
                    // ǰһ�е�����*2
                    board[i][k - 1] =board[i][k - 1]*2;
                    // ��һ�е�������Ϊ0
                    board[i][k] = 0;
                    // ��¼������Ϣ
                    a.digit2 = board[i][k - 1];
                    a.endPos = QPointF(w * (k - 1),h * i);
                    // ���ӷ���
                    score += board[i][k - 1];
                    // �������ӷ������ź�
                    emit ScoreInc(score);
                    // �������-1
                    digitNumber--;
                    combine = true;
                }
                else
                    k++;
                j++;
                // ��ӵ���������
                animationList.append(a);
            }
        }
        break;
        // ����������������������
        //���ϵ���,��������
    case RIGHT:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3, combine = false;
            while (true)
            {
                while (j > -1 && board[i][j] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[i][k], board[i][j]);
                a.type = MOVE;
                a.startPos = QPointF(w * j, h * i);
                a.endPos = QPointF(w * k, h * i);
                a.digit = a.digit2 = board[i][k];
                a.way = RIGHT;
                if (!combine && k < 3 && board[i][k] == board[i][k + 1])
                {
                    board[i][k + 1]=board[i][k + 1]*2;
                    board[i][k] = 0;
                    a.digit2 = board[i][k + 1];
                    a.endPos = QPointF(w * (k + 1),h * i);
                    score += board[i][k + 1];
                    emit ScoreInc(score);
                    digitNumber--;
                    combine = true;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
        //�������ң����ϵ���
    case UP:
        for (i = 0; i < 4; i++)
        {
            j = 0, k = 0, combine = false;
            while (true)
            {
                while (j < 4 && board[j][i] == 0)
                    j++;
                if (j > 3)
                    break;
                qSwap(board[k][i], board[j][i]);
                a.type = MOVE;
                a.startPos = QPointF(w * i, h * j);
                a.endPos = QPointF(w * i,h* k);
                a.digit = a.digit2 = board[k][i];
                a.way = UP;
                if (!combine && k > 0 && board[k][i] == board[k - 1][i])
                {
                    board[k - 1][i]=board[k - 1][i]*2;
                    board[k][i] = 0;
                    a.digit2 = board[k - 1][i];
                    a.endPos = QPointF(w * i, h * (k - 1));
                    score += board[k - 1][i];
                    emit ScoreInc(score);
                    digitNumber--;
                    combine = true;
                }
                else
                    k++;
                j++;
                animationList.append(a);
            }
        }
        break;
    case DOWN:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3, combine = false;
            while (true)
            {
                while (j > -1 && board[j][i] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[k][i], board[j][i]);
                a.type = MOVE;
                a.startPos = QPointF(w * i,h * j);
                a.endPos = QPointF(w * i,h * k);
                a.digit = a.digit2 = board[k][i];
                a.way = DOWN;
                if (!combine && k < 3 && board[k][i] == board[k + 1][i])
                {
                    board[k + 1][i]=board[k + 1][i]*2;
                    board[k][i] = 0;
                    a.digit2 = board[k + 1][i];
                    a.endPos = QPointF(w * i, h* (k + 1));
                    score += board[k + 1][i];
                    emit ScoreInc(score);
                    digitNumber--;
                    combine = true;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    }
    //��������б�û���ƶ����Ͳ������µ�����
    bool isMove = false;
    for(int index = 0; index < animationList.size(); index++) {
        if (animationList.at(index).startPos != animationList.at(index).endPos) {
            isMove = true;
            break;
        }
    }
    // �������ľ���������ҳ��ֹ��ƶ�
    if (digitNumber != 16 && isMove)
    {
        // ��������кź��к�
        i = rand() % 4, j = rand() % 4;
        // ѭ��ֱ���к��ж�Ӧ��Ԫ��Ϊ0
        while (board[i][j] != 0)
            i = rand() % 4, j = rand() % 4;
        // ����2
        board[i][j] = 2;
        // ��¼������Ϣ
        a.type = START;
        a.startPos = a.endPos = QPointF(w * j,h * i);
        a.digit = board[i][j];
        // ���������һ
        digitNumber++;
    }
    else
    {
        // ������������� �����Ϸ�Ƿ�over
        if (checkGameOver())
            emit GameOver();// �����Ϸover������GameOver�ź�
    }

    // ��ʼ���ƶ���Ч��
    isPlayingAnimation = true;
    // �����б�ĵ�����
    QList<Animation>::iterator it;
    // �¼�ѭ�� ������ʱ
    QEventLoop eventLoop;
    // ɾ��֮ǰ�Ļ���ͼ��
    if (cacheImg)
        delete cacheImg;
    // ��������ͼ��
    cacheImg = new QImage(width(), height(), QImage::Format_ARGB32);
    // ���ͼ��
    cacheImg->fill(0);
    // ����һ��QPainter����
    QPainter painter(cacheImg);
    // ����
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40);
    painter.setFont(font);

    // ��ʶ���з��񶯻��Ƿ񶼲������
    bool ok = false;
    while (true)
    {
        // ����һ����ˢ ��ɫΪR G B�����ֱ�Ϊ141 121 81����ɫ
        QBrush brush(QColor::fromRgb(141, 121, 81));
        // ʹpainterӦ�������ˢ
        painter.setBrush(brush);

        // ����һ������
        painter.drawRect(2, 2, width() - 4, height() - 4);

        // ���û�ˢ��ɫΪ RGB����Ϊ171 165 141����ɫ
        brush.setColor(QColor::fromRgb(171, 165, 141));
        // Ӧ�������ˢ
        painter.setBrush(brush);

        // ѭ��������Ϸ���
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                // ����С����
                painter.drawRect(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h));

        // ���趼�������
        ok = true;

        // ѭ������ÿ�����񶯻�
        for (it = animationList.begin(); it != animationList.end(); it++)
            if (!playAnimation(*it, painter))
                ok = false;

        // ˢ�²���
        update();

        // ȫ�����������˳�
        if (ok)
            break;

        // ��ʱ5ms
        QTimer::singleShot(5, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
    // ���ŷ�����ֵĶ���
    while (!playAnimation(a, painter))
    {
        update();
        QTimer::singleShot(5, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
    //������ж���
    animationList.clear();
    //ˢ�µ�ǰ����
    isPlayingAnimation = false;

    // �����Ϸ�Ƿ��ʤ
    if (checkWin())
        emit win();// �����ʤ����win�ź�

    update();
}

bool GameWidget::playAnimation(Animation& a, QPainter& painter)
{
    bool rtn = false;
    QBrush brush(Qt::SolidPattern);

    // �ƶ�����λ��
    if (a.type == MOVE)
    {
        switch (a.way)
        {
        case LEFT:
            if (a.startPos.x() > a.endPos.x())
                a.startPos += dPos[LEFT];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case RIGHT:
            if (a.startPos.x() < a.endPos.x())
                a.startPos += dPos[RIGHT];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case UP:
            if (a.startPos.y() > a.endPos.y())
                a.startPos += dPos[UP];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case DOWN:
            if (a.startPos.y() < a.endPos.y())
                a.startPos += dPos[DOWN];
            else
                a.startPos = a.endPos, rtn = true;
        }
        // ��������ƶ����յ�
        if (!rtn)
        {
            //���ƺϲ������
            brush.setColor(digitBkg[getBitCount(a.digit)]);
            painter.setBrush(brush);
            painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(), w, h));
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter,
                             QString::number(a.digit));
        }
        else
        {
            //����δ�ϲ�ǰ����
            brush.setColor(digitBkg[getBitCount(a.digit2)]);
            painter.setBrush(brush);
            painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(), w, h));
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter,
                             QString::number(a.digit2));
        }
        //�û��ƹ����г��������ֱ߿�
        painter.setPen(Qt::NoPen);
    }
    else
    {
        // ������ֵĶ���Ч��
        if (a.startPos.x() > a.endPos.x())
            a.startPos += dPos[4];
        else
            a.startPos = a.endPos, rtn = true;
        brush.setColor(digitBkg[getBitCount(a.digit)]);
        painter.setBrush(brush);
        painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(),
                         w - 2 * (a.startPos.x() - a.endPos.x()),
                         h - 2 * (a.startPos.y() - a.endPos.y())));
        painter.setPen(QColor::fromRgb(0, 0, 0));
        painter.drawText(QRectF(a.endPos.x(), a.endPos.y(), w, h),
                         Qt::AlignCenter, QString::number(a.digit));
        painter.setPen(Qt::NoPen);
    }
    return rtn;
}

void GameWidget::paintEvent(QPaintEvent *)
{
    // ����һ��QPainter���� ʹ���������л�ͼ
    QPainter painter(this);

    // ������ڲ��Ŷ���Ч������ƻ���λͼ
    if (isPlayingAnimation)
    {
        painter.drawImage(0, 0, *cacheImg);
        return;
    }

    // ����һ����ˢ ��ɫΪR G B�����ֱ�Ϊ141 121 81����ɫ
    QBrush brush(QColor::fromRgb(141, 121, 81));
    // ʹpainterӦ�������ˢ
    painter.setBrush(brush);

    // ���û���Ϊ�ձ� Ŀ����ʹ���Ƶ�ͼ��û�����
    painter.setPen(Qt::NoPen);

    // ����һ������
    painter.drawRect(2, 2, width() - 4, height() - 4);

    // ����ÿ��С���ӵĿ�Ⱥ͸߶�
    w = (width() - 24) / 4, h = (height() - 24) / 4;

    /* ����һ������
     * ��������ΪConsolas
     * ��������Ϊ����
     * �����СΪ40����
     * */
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40);
    // ʹpainterӦ���������
    painter.setFont(font);

    // ѭ��������Ϸ���
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            // ���������������
            if (board[i][j])
            {
                // ���û�ˢ��ɫΪ�����Ӧ����ɫ
                brush.setColor(digitBkg[getBitCount(board[i][j])]);
                // Ӧ�������ˢ
                painter.setBrush(brush);
                // ����һ��С����
                painter.drawRect(QRectF(6+(w + 4) * j, 6 + (h + 4) * i, w, h));
                // ���û���Ϊ��ɫ����
                painter.setPen(QColor::fromRgb(0, 0, 0));
                // ��������
                painter.drawText(QRectF(6+(w + 4) * j, 6 + (h + 4) * i, w, h), Qt::AlignCenter,
                                 QString::number(board[i][j]));
                // ���û���Ϊ�ձ�
                painter.setPen(Qt::NoPen);
            }
            // ���������û������
            else
            {
                // ���û�ˢ��ɫΪ RGB����Ϊ171 165 141����ɫ
                brush.setColor(QColor::fromRgb(171, 165, 141));
                // Ӧ�������ˢ
                painter.setBrush(brush);
                // ����С����
                painter.drawRect(QRectF(6+(w + 4) * j, 6 + (h + 4) * i, w, h));
            }
        }
}

void GameWidget::restart()
{
    // ��ʼ����ر���
    score = 0;
    digitNumber = 2;
    memset(board, 0, sizeof(board));
    board[rand() % 4][rand() % 4] = 2;
    while(true) {
        int x = rand() % 4;
        int y = rand() % 4;
        if (board[x][y] != 0) {
            continue;
        }
        else {
            board[x][y] = 2;
            break;
        }
    }
    emit ScoreInc(score);
    update();
}

bool GameWidget::checkGameOver()
{
    // ѭ������Ƿ������ڵ���ͬ����
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (j != 3 && board[i][j] == board[i][j + 1])
                return false;
            if (i != 3 && board[i][j] == board[i + 1][j])
                return false;
        }
    return true;
}

bool GameWidget::checkWin()
{
    // ѭ������Ƿ�ĳ�����������Ϊ2048
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (board[i][j] == 2048)
                return true;
    return false;
}

int GameWidget::getBitCount(int n)
{
    // ѭ����ȡ���ֶ�����λ��
    int c = 0;
    while (n = n>>1)
        c++;
    // ����λ��-1
    return c - 1;
}

