#include "mainwindow.h"
#include <QPainter>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), wheel(new Wheel(this)) {
    setFixedSize(800, 600);  // Задаем размер окна
}

MainWindow::~MainWindow() {
    delete wheel;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap background(":/img/background.jpg");  // Загружаем фоновое изображение
    painter.drawPixmap(0, 0, width(), height(), background);  // Отрисовываем фон

    wheel->draw(&painter);  // Отрисовываем колесо
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Right) {
        wheel->move(10);  // Двигаем колесо вправо
    } else if (event->key() == Qt::Key_Left) {
        wheel->move(-10);  // Двигаем колесо влево
    }
    update();  // Перерисовываем окно для обновления положения колеса
}
