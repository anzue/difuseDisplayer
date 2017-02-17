#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class Display;
}

class Display : public QMainWindow
{
    Q_OBJECT
    QTimer *timer;
public:
    explicit Display(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    ~Display();

private slots:
    void on_restart_clicked();

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
