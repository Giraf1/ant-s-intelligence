#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void mousePressEvent(QMouseEvent *event);

    void justfortimer();

    void drawline(int a, int b);

    void updating_scene(QVector <QPoint> points);

    void updating_scene(QPoint point);

    void on_pushButton_3_clicked();

    void draw_way(QVector <int> way);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    QGraphicsPixmapItem *pixmap;
    QGraphicsLineItem *line;
    QFont *font;
protected:

};
#endif // MAINWINDOW_H
