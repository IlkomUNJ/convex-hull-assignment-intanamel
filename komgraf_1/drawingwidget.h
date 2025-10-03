#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPoint>

class DrawingWidget : public QWidget
{
    Q_OBJECT

public:
    DrawingWidget(QWidget *parent = nullptr);

    void runConvexHull();
    void clearCanvas();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPoint> points;
    QVector<QPoint> hull;
    int slowIterations;
    int fastIterations;

    QVector<QPoint> slowHull(const QVector<QPoint> &pts, int &iterations);
    QVector<QPoint> fastHull(const QVector<QPoint> &pts, int &iterations);
};

#endif // DRAWINGWIDGET_H
