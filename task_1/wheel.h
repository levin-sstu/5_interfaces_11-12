#ifndef WHEEL_H
#define WHEEL_H

#include <QWidget>
#include <QPixmap>

class Wheel {
public:
    Wheel(QWidget *parent = nullptr);
    void draw(QPainter *painter);
    void move(int deltaX);  // Метод для перемещения колеса

private:
    QWidget* parent;
    QPixmap texture;
    float currentAngle;
    int xPos, yPos;
    int radius;
};

#endif // WHEEL_H
