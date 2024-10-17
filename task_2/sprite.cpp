#include "Sprite.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>

Sprite::Sprite(const QPixmap& texture, const QPainterPath& shape, const QPoint& velocity)
    : texture_(texture), shape_(shape), velocity_(velocity) {}

void Sprite::draw(QPainter& painter) const {
    // Отрисовка текстуры в рамках формы спрайта
    painter.drawPixmap(shape_.boundingRect().toRect(), texture_);
}

void Sprite::move(const QRect& boundary, const QList<Sprite*>& sprites) {
    QPainterPath newShape = shape_;
    newShape.translate(velocity_);

    // Проверка на выход за границы окна
    handleBoundaryCollision(newShape, boundary);

    // Проверка на коллизии со спрайтами
    handleSpriteCollision(newShape, sprites);

    shape_ = newShape; // Обновляем текущую форму спрайта
}

void Sprite::handleBoundaryCollision(QPainterPath& newShape, const QRect& boundary) {
    QRectF boundingRect = newShape.boundingRect();

    if (boundingRect.left() < boundary.left()) {
        newShape.translate(boundary.left() - boundingRect.left(), 0);
        velocity_.setX(-velocity_.x());
    }
    if (boundingRect.right() > boundary.right()) {
        newShape.translate(boundary.right() - boundingRect.right(), 0);
        velocity_.setX(-velocity_.x());
    }
    if (boundingRect.top() < boundary.top()) {
        newShape.translate(0, boundary.top() - boundingRect.top());
        velocity_.setY(-velocity_.y());
    }
    if (boundingRect.bottom() > boundary.bottom()) {
        newShape.translate(0, boundary.bottom() - boundingRect.bottom());
        velocity_.setY(-velocity_.y());
    }
}

void Sprite::handleSpriteCollision(QPainterPath& newShape, const QList<Sprite*>& sprites) {
    for (Sprite* other : sprites) {
        if (other != this && newShape.intersects(other->getOutline())) {
            // Обнаружена коллизия
            QRectF intersection = newShape.boundingRect().intersected(other->getOutline().boundingRect());

            // Определяем направление отталкивания
            QPointF centerA = newShape.boundingRect().center();
            QPointF centerB = other->getOutline().boundingRect().center();
            QPointF direction = centerA - centerB;

            // Нормализация направления
            qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
            if (length > 0) {
                direction /= length; // Нормализуем направление
            }

            // Задаем максимальное расстояние, на которое спрайты могут "оттолкнуться"
            qreal pushDistance = intersection.width() / 2;

            // Перемещение спрайтов
            newShape.translate(direction * pushDistance);
            other->shape_.translate(-direction * pushDistance); // Перемещаем другой спрайт в обратном направлении

            // Меняем направление скорости
            velocity_.setX(-velocity_.x());
            velocity_.setY(-velocity_.y());
        }
    }
}

void Sprite::resize(double scaleFactor) {
    texture_ = texture_.scaled(texture_.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Изменяем размер формы, сохраняя пропорции
    QPolygonF originalPolygon = shape_.toFillPolygon();
    QPolygonF scaledPolygon;

    for (const QPointF& point : originalPolygon) {
        scaledPolygon.append(point * scaleFactor);
    }

    shape_ = QPainterPath();
    shape_.addPolygon(scaledPolygon); // Добавляем масштабированный полигон в новую форму
}

QPainterPath Sprite::getOutline() const {
    return shape_; // Возвращаем форму спрайта для проверки коллизий
}

QPainterPath Sprite::getShape() const {
    return shape_; // Возвращаем форму спрайта
}
