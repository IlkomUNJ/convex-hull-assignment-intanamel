#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DrawingWidget; // forward declare

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runConvexHull();
    void clearCanvas();

private:
    Ui::MainWindow *ui;
    DrawingWidget *drawing;
};

#endif // MAINWINDOW_H
