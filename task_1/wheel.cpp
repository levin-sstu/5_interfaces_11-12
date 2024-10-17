#include "wheel.h"
#include <QPainter>
#include <QTransform>
#include <QtMath>

Wheel::Wheel(QWidget *parent)
    : texture(":/img/wheel.png"), currentAngle(0), xPos(0), yPos(300), radius(100) {}

void Wheel::draw(QPainter *painter) {
    QTransform transform;
    transform.translate(xPos + radius, yPos + radius);  // Перемещаем начало координат к центру колеса
    transform.rotate(currentAngle);  // Поворачиваем на текущий угол
    painter->setTransform(transform);

    // Рисуем колесо
    painter->drawPixmap(-radius, -radius, 2 * radius, 2 * radius, texture);
}

void Wheel::move(int deltaX) {
    // Перемещаем колесо по X
    xPos += deltaX;

    // Обновляем угол поворота: Δугол = ΔX / радиус * (180 / π)
    float deltaAngle = (deltaX / static_cast<float>(radius)) * 180.0 / M_PI;
    currentAngle += deltaAngle;
    if (currentAngle >= 360) currentAngle -= 360;
}
