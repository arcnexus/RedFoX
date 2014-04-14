#ifndef TIMEDMESSAGEBOX_H
#define TIMEDMESSAGEBOX_H

#include <QDialog>
#include <QMessageBox>
#include <QTimer>
#include <QHBoxLayout>
#include <QLabel>
#include <QMutex>

class TimedMessageBox : public QDialog
{
    Q_OBJECT
public:
    enum Icons{
        Infomation , Warning , Critical
    };

    explicit TimedMessageBox(QWidget *parent = 0, QString message = "" , Icons icon = Infomation);
    ~TimedMessageBox();
    inline static int getCount(){return count;}
    inline static void setCount(int value){count = value;}

    static void Box(QWidget *parent = 0, QString message = "" , Icons icon = Infomation);
signals:
    
public slots:
private slots:
    void startAnimation();
    void doHide();
private:
    static int count;
    static QMutex mutex;
    QHBoxLayout _layout;
    QLabel _icon;
    QLabel _message;
    QPixmap getIcon(Icons icon);
};

#endif // TIMEDMESSAGEBOX_H
