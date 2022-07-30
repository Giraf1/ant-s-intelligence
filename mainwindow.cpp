#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMimeData>
#include <QPoint>
#include <QPointF>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <functional>
#include <vector>
#include <math.h>
#include <QTimer>
#include "QtTest/QTest"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setSceneRect(this->frameGeometry());
    QPixmap img("C:/Users/Girafi/Documents/untitled/sticker.png");
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect());
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(6);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete ellipse;
    delete pixmap;
}


int firststart = 0;
QVector <QPoint> points;

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint remapped = ui->graphicsView->mapFromParent(event->pos());
    if (ui->graphicsView->rect().contains(remapped) && event->button() == Qt::LeftButton)
    {
         QPointF mousePoint = ui->graphicsView->mapToScene(remapped);
         points.push_back(mousePoint.toPoint());
         MainWindow::updating_scene(mousePoint.toPoint());
    }
    ui->label->setText("Кількість точек: " + QString::number(points.size()));
}

void MainWindow::justfortimer()
{
    qDebug() << "timer";
}

void MainWindow::drawline(int a, int b)
{
    line = scene->addLine(points[a].x(), points[a].y(), points[b].x(), points[b].y(), QPen(Qt::black, 2)); //і намалювання палки
    scene->update();
}






/*void MainWindow::paintEvent2(QPaintEvent *)
{
    if(firststart++) {
        qDebug() << "paintevent";
        QPainter p(this);
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(50);
        points.push_back(QWidget::mapFromGlobal(QCursor::pos()));
        for(int i = 0; i < points.size(); i++) p.drawEllipse(points[i], 3, 3);
    }
}*/

void MainWindow::on_pushButton_clicked()
{
    ui->progressBar->setValue(0);
    scene->clear();
    points.clear();
    ui->textEdit->clear();
    qDebug() << "___________________________________________";
}

void MainWindow::updating_scene(QVector<QPoint> points)
{
    scene->clear();
    for (int i = 0; i < points.size(); i++) {
        ellipse = scene->addEllipse(points[i].x() - 3, points[i].y() - 3, 6, 6, QPen(Qt::black), QBrush(Qt::red));
    }
}

void MainWindow::updating_scene(QPoint point)
{
    ellipse = scene->addEllipse(point.x() - 3, point.y() - 3, 6, 6, QPen(Qt::black), QBrush(Qt::red));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum((points.size()*2)*points.size());
    ui->textEdit->clear(); //очистка поля виводу
    QVector <QVector<double>> chance   (points.size(),QVector<double>(points.size()));// Створення основних носіїв данних
    QVector <QVector<double>> pheromone(points.size(),QVector<double>(points.size()));
    QVector <QVector<double>> closeness(points.size(),QVector<double>(points.size()));
    QVector <QVector<double>> lenofways(points.size(),QVector<double>(points.size()));
    QVector <QVector<int>> way(points.size(),QVector<int>(points.size() + 1));
    int pow_for_len = 4, pow_for_pheromone = 1;
    QVector <int> minimal_way (points.size() + 1);
    int minimal_len_of_way;
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) { //запуск вложеного циклу що заповнить вектор "феромонів" на початкове значення, у моєму випадку 0.2, та порахує відстань та близкість між точками
            pheromone[i][j] = 0.1;
            double len = std::hypot(points[j].x() - points[i].x(), points[j].y() - points[i].y()); // формула знаходження відстані між точками і та j
            closeness[i][j] = 200/len; // а тут вираховується близькість, значення, обернене до відсатні. Константа 200, а не 1, щоб були більш адекватні значення
            lenofways[i][j] = len;
            if (i == j) continue; //тут йде обрив ітерації щоб не виводити зайву інформацію, типу "відстань між точками 0 і 0"
            //ui->textEdit->append("Растояние между точками " + QString::number(i) + " и " + QString::number(j) + ": " + QString::number(len));
            //ui->textEdit->append("Близость: " + QString::number(closeness[i][j]));
        }
    }

    int count_of_iterations = 0;

    for (int count = 0; count < points.size()*2; count++) {
        for (int index = 0; index < points.size(); index++) {
            count_of_iterations++;
            QVector <int> local_minimum_way;
            int local_len_of_way;
            way[index][0] = index;
            way[index][points.size()] = index;
            int i = index, max = 0;
            //qDebug() << "index:" << index;
            for (QVector <int> usual_variebles; usual_variebles.size() != points.size() - 1; max++) {
                double sum(0);
                int temp_chance[100] {};
                //qDebug() << "iteration" << max;
                //if (max > 100) exit(142);
                for (int j = 0; j < points.size(); j++) {
                    if (i == j || usual_variebles.count(j) != 0) continue;
                    sum = sum + pow(pheromone[i][j], pow_for_pheromone) * pow(closeness[i][j], pow_for_len);
                }
                for (int j = 0, last_ruletka = 0; j < points.size(); j++) {
                    if (i == j || usual_variebles.count(j) != 0) continue;
                    chance[i][j] = (pow(pheromone[i][j], pow_for_pheromone) * pow(closeness[i][j], pow_for_len)) / sum;
                    //qDebug() << "Шанс перехода между" << i << "и" << j << (pheromone[i][j] * closeness[i][j])/sum;
                    //ui->textEdit->append("Шанс перехода между " + QString::number(i) + " и " + QString::number(j) + " = " + QString::number((pheromone[i][j] * closeness[i][j])/sum));
                    int t = ((chance[i][j]*100 + 0.5)/100)*100;
                    //qDebug() << "t" << t;
                    for (int z = last_ruletka; z < t + last_ruletka; z++) {
                        //qDebug() << "j" << j << "z" << z;
                        temp_chance[z] = j;
                    }
                    last_ruletka = t + last_ruletka;
                    /*for (int z = 0, temp = 0; z < 100; z++) {
                    if (z >= int((chance[i][temp]*10)/10*100) && temp != points.size() - 1) temp++;
                    temp_chance[z] = temp;
                }*/
                }
                for (int z = 0; z < 100; z++) {
                    //qDebug() << z << ':' << temp_chance[z];
                }
                int temp = QRandomGenerator::global()->bounded(0,100); //ось самий вибір напряму
                //qDebug() << "temp_chance[temp]" << temp_chance[temp];
                //ui->textEdit->append("temp_chance[temp] " + QString::number(temp_chance[temp]));
                if (std::find(usual_variebles.begin(), usual_variebles.end(), temp_chance[temp]) != usual_variebles.end() || i == temp_chance[temp] || index == temp_chance[temp]) continue;
                usual_variebles.append(i);
                MainWindow::drawline(i, temp_chance[temp]);
                //qDebug() << "Переход между" << i << "и" << temp_chance[temp];
                //ui->textEdit->append("Переход между " + QString::number(i) + " и " + QString::number(temp_chance[temp]));
                i = temp_chance[temp];
                way[index][usual_variebles.size()] = i;
                //QTest::qWait(1);

            }
            //if (max == 0) local_len_of_way = ;
            ui->progressBar->setValue(ui->progressBar->value() + 1);
            MainWindow::drawline(i, index);
            QTest::qWait(1);
            MainWindow::updating_scene(points);
        }


        for (int i = 0; i < points.size(); i++) {
            QString str_way {"Шлях #" + QString::number(i) + " був таким: "};
            for (int j = 0; j < points.size() + 1; j++) {
                str_way.append(QString::number(way[i][j]) + ", ");
            }
            qDebug() << str_way;
        }

        for (int i = 0; i < points.size(); i++) {
            int len_of_way = 0;
            for (int j = 0; j < points.size() - 1; j++) {
                len_of_way = len_of_way + lenofways[way[i][j]][way[i][j+1]];
            }
            qDebug() << "way[i].size() - 1" << way[i].size() - 1 << "lenofways[way[i][way[i].size() - 1]][0]" << lenofways[way[i][way[i].size() - 1]][0];
            len_of_way = len_of_way + lenofways[way[i][way[i].size() - 1]][way[i][0]];
            ui->textEdit->append("len of way # " + QString::number(i) + " = " + QString::number(len_of_way));
            for (int j = 0; j < points.size() - 1; j++) {
                pheromone[way[i][j]][way[i][j + 1]] = pheromone[way[i][j]][way[i][j + 1]] + (double) 200/len_of_way;

            }
            //qDebug() << "len_of_way #" << i << len_of_way;
            if (count == 0 && i == 0) minimal_len_of_way = len_of_way;
            if (len_of_way < minimal_len_of_way) {
                minimal_len_of_way = len_of_way;
                minimal_way = way[i];
            }
        }
        //qDebug() << "count:" << count;
        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < pheromone.size(); j++) {
                pheromone[i][j] = pheromone[i][j] * 0.66;
                //ui->textEdit->append("pheromone on way from " + QString::number(i) + " to " + QString::number(j) + " = " + QString::number(pheromone[i][j]));
                //qDebug() << "pheromone on way from" << i << "to" << j << '=' << pheromone[i][j];
            }
        }

    }

    ui->textEdit->append("Найкоротший шлях між точками = " + QString::number(minimal_len_of_way));
    qDebug() << "Найкоротший шлях між точками =" << minimal_len_of_way;
    QString str_way {"Цей шлях був такий: "};
    for (int i = 0; i < minimal_way.size(); i++) {
        str_way.append(QString::number(minimal_way[i]) + ", ");
    }
    ui->textEdit->append(str_way);
    MainWindow::draw_way(minimal_way);
    qDebug() << "count iter" << count_of_iterations;
}



//scene->addPixmap(img);
//img = new QPixmap("C:/Users/Girafi/Documents/untitled/sticker2.png");
//QPixmap pixmap("C:/Users/Girafi/Documents/untitled/sticker2.png");
//pixmap = new QPixmap("image.png");
//pixmap = new QPixmap("C:/Users/Girafi/Documents/untitled/sticker2.png");

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << scene->height() << ui->graphicsView->pos();
    scene->clear();
    points.clear();
    for (int i = 0; i < ui->spinBox->value(); i++) {
        points.append(QPoint(QRandomGenerator::global()->bounded(ui->graphicsView->pos().x(), ui->graphicsView->width()), QRandomGenerator::global()->bounded(ui->graphicsView->pos().y(), ui->graphicsView->size().height())));
        ellipse = scene->addEllipse(points[i].x(), points[i].y(), 6, 6, QPen(Qt::black), QBrush(Qt::red));

    }
    line = scene->addLine(ui->graphicsView->pos().x(), ui->graphicsView->pos().y(), ui->graphicsView->pos().x() + ui->graphicsView->width(), ui->graphicsView->pos().y() + ui->graphicsView->height());

}

void MainWindow::draw_way(QVector <int> way)
{
    for (int i = 0; i < way.size() - 1; i++) {
        MainWindow::drawline(way[i], way[i+1]);
    }
}

