#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QList>
#include <QSpinBox>
#include <QTimer>
#include <QPainterPath>
#include <QResizeEvent>
#include "Sprite.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupSpriteCountSpinBox();
    void setupTimer();
    void updateBoundary(const QSize& size);
    void updateBackground();
    void setSpriteTextureSize();
    void createSprites(int spriteCount);
    bool tryPlaceSprite(const QList<QPainterPath>& existingShapes);
    void clearSprites();
    void resizeSprites(double scaleFactor);
    double calculateScaleFactor(); // Новый метод для вычисления коэффициента масштабирования
    void updateSpriteCount(int count);
    void onTimeout();

    QPixmap background_;
    QPixmap spriteTexture_;
    QList<Sprite*> sprites_;
    QRect boundary_;
    QSpinBox* spriteCountSpinBox_;
    QTimer* timer_;
    double spriteScaleFactor_; // Хранит коэффициент масштабирования
};

#endif // MAINWINDOW_H
