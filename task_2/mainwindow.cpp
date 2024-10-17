#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QDebug>
#include <QScopedPointer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    background_(":/resources/img/background_3.jpg"),
    spriteTexture_(":/resources/img/mario.png"),
    spriteScaleFactor_(1.0) {

    // Устанавливаем минимальный размер окна
    setMinimumSize(800, 600);

    // Настройка SpinBox для выбора количества спрайтов
    setupSpriteCountSpinBox();

    // Настраиваем таймер для анимации
    setupTimer();

    // Создаем вертикальный layout
    auto* container = new QWidget(this);
    auto* layout = new QVBoxLayout(container);
    layout->addWidget(spriteCountSpinBox_);

    // Добавляем центральный виджет для рисования
    auto* drawingArea = new QWidget(this);
    layout->addWidget(drawingArea);

    // Настройка центрального виджета
    setCentralWidget(container);

    // Устанавливаем размеры текстуры спрайта
    setSpriteTextureSize();

    // Устанавливаем границы окна для спрайтов
    boundary_ = QRect(0, 0, width(), height());

    // Создаем начальные спрайты с заданным количеством
    createSprites(1);

    // Вызываем обновление окна для отображения изменений
    update();
}


MainWindow::~MainWindow() {
    clearSprites();
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, background_);

    // Рисуем все спрайты
    for (const Sprite* sprite : sprites_) {
        sprite->draw(painter);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event); // Вызываем стандартное поведение
    updateBoundary(event->size());  // Обновляем границы окна
    updateBackground();  // Масштабируем фон
    resizeSprites(spriteScaleFactor_);  // Масштабируем спрайты
    update();  // Вызываем перерисовку
}

void MainWindow::setupSpriteCountSpinBox() {
    spriteCountSpinBox_ = new QSpinBox(this);
    spriteCountSpinBox_->setRange(1, 100);  // Диапазон: от 1 до 100
    spriteCountSpinBox_->setValue(1);  // Стартовое значение - 1 спрайт
    connect(spriteCountSpinBox_, &QSpinBox::valueChanged, this, &MainWindow::updateSpriteCount);
}

void MainWindow::setupTimer() {
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::onTimeout);
    timer_->start(16);  // Около 60 кадров в секунду
}

void MainWindow::updateBoundary(const QSize& size) {
    boundary_ = QRect(0, 0, size.width(), size.height());  // Обновляем границы окна
    setSpriteTextureSize();  // Устанавливаем размеры текстуры спрайта
}

void MainWindow::updateBackground() {
    background_ = background_.scaled(boundary_.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
}

void MainWindow::setSpriteTextureSize() {
    // Устанавливаем максимальный размер спрайта в 10% от ширины и высоты окна
    int maxSpriteWidth = width() * 0.1; // 10% от ширины окна
    int maxSpriteHeight = height() * 0.1; // 10% от высоты окна

    // Масштабируем текстуру, чтобы она вписывалась в максимальные размеры
    spriteTexture_ = spriteTexture_.scaled(maxSpriteWidth, maxSpriteHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Проверка на размер текстуры спрайта
    if (spriteTexture_.isNull()) {
        qDebug() << "Sprite texture is null!";
        return; // Прерываем выполнение, если текстура не загружена
    }

    qDebug() << "Scaled sprite size:" << spriteTexture_.size(); // Отладочная информация
}

void MainWindow::createSprites(int spriteCount) {
    qDebug() << "Creating" << spriteCount << "sprites"; // Отладочное сообщение

    if (boundary_.width() <= 0 || boundary_.height() <= 0) {
        qDebug() << "Invalid boundary:" << boundary_;
        return; // Прерываем выполнение, если границы неверные
    }

    QList<QPainterPath> existingShapes;
    for (Sprite* sprite : sprites_) {
        existingShapes.append(sprite->getShape());
    }

    const int maxAttempts = 100; // Максимальное количество попыток создания спрайта

    for (int i = 0; i < spriteCount; ++i) {
        bool placed = tryPlaceSprite(existingShapes);
        if (!placed) {
            qDebug() << "Unable to place sprite after" << maxAttempts << "attempts";
        }
    }
}

bool MainWindow::tryPlaceSprite(const QList<QPainterPath>& existingShapes) {
    int attempts = 0;
    const int maxAttempts = 100;

    while (attempts < maxAttempts) {
        // Генерация случайной скорости
        int vx = QRandomGenerator::global()->bounded(-5, 6);
        int vy = QRandomGenerator::global()->bounded(-5, 6);

        // Генерация случайного положения спрайта
        int x = QRandomGenerator::global()->bounded(boundary_.width() - spriteTexture_.width());
        int y = QRandomGenerator::global()->bounded(boundary_.height() - spriteTexture_.height());

        // Создаем новый QPainterPath для формы спрайта
        QPainterPath newShape;
        newShape.addRect(x, y, spriteTexture_.width(), spriteTexture_.height()); // Создаем форму

        // Проверка на пересечение с существующими спрайтами
        if (std::none_of(existingShapes.begin(), existingShapes.end(), [&](const QPainterPath& existingShape) {
                return newShape.intersects(existingShape);
            })) {
            // Если не обнаружено пересечений, размещаем спрайт
            sprites_.append(new Sprite(spriteTexture_, newShape, QPoint(vx, vy)));
            return true; // Успешно размещён
        } else {
            qDebug() << "Collision detected, trying again";
        }
        attempts++;
    }
    return false; // Не удалось разместить спрайт
}

void MainWindow::clearSprites() {
    qDeleteAll(sprites_);
    sprites_.clear();
}

void MainWindow::resizeSprites(double scaleFactor) {
    for (Sprite* sprite : sprites_) {
        sprite->resize(scaleFactor);  // Изменяем размер каждого спрайта
    }
}

void MainWindow::updateSpriteCount(int count) {
    clearSprites();
    createSprites(count);
    update(); // Обновляем окно после изменения количества спрайтов
}

void MainWindow::onTimeout() {
    for (Sprite* sprite : sprites_) {
        sprite->move(boundary_, sprites_);
    }
    update();  // Обновляем окно для перерисовки
}
