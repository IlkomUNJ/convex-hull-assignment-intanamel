#include "mainwindow.h"
#include "drawingwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    auto *vbox = new QVBoxLayout(central);

    drawing = new DrawingWidget(this);
    drawing->setMinimumSize(640, 480);
    vbox->addWidget(drawing, 1);

    auto *hbox = new QHBoxLayout();
    QPushButton *btnRun = new QPushButton(tr("Run Convex Hull"), this);
    QPushButton *btnClear = new QPushButton(tr("Clear"), this);

    hbox->addWidget(btnRun);
    hbox->addWidget(btnClear);
    hbox->addStretch();

    vbox->addLayout(hbox);

    connect(btnRun, &QPushButton::clicked, drawing, &DrawingWidget::runConvexHull);
    connect(btnClear, &QPushButton::clicked, drawing, &DrawingWidget::clearCanvas);

    setWindowTitle(tr("Convex Hull Visualizer"));
}
