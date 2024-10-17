#ifndef SPRITE_H
#define SPRITE_H

#include <QPixmap>
#include <QPainterPath>
#include <QPoint>
#include <QList>

class Sprite {
public:
    Sprite(const QPixmap& texture, const QPainterPath& shape, const QPoint& velocity);

    void draw(QPainter& painter) const;
    void move(const QRect& boundary, const QList<Sprite*>& sprites);
    void resize(double scaleFactor);

    QPainterPath getOutline() const;
    QPainterPath getShape() const;

private:
    void handleBoundaryCollision(QPainterPath& newShape, const QRect& boundary);
    void handleSpriteCollision(QPainterPath& newShape, const QList<Sprite*>& sprites);

    QPixmap texture_;       // Текстура спрайта
    QPainterPath shape_;    // Форма спрайта
    QPoint velocity_;       // Скорость спрайта
};

#endif // SPRITE_H
