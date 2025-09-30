#include "drawingwidget.h"

#include <QPainterPath>
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <set>
#include <cmath>

DrawingWidget::DrawingWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
}

bool DrawingWidget::nearlyEqual(double a, double b)
{
    return std::fabs(a - b) <= 1e-9;
}

double DrawingWidget::orient(const QPointF &a, const QPointF &b, const QPointF &c, long long &counter)
{
    counter++;
    return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
}

std::vector<QPointF> DrawingWidget::bruteForceHull(const std::vector<QPointF> &pts, long long &counter)
{
    int n = (int)pts.size();
    if (n <= 1) return pts;
    std::set<std::pair<double,double>> hullPts;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            bool pos = false, neg = false;
            for (int k = 0; k < n; ++k) {
                if (k == i || k == j) continue;
                double val = (pts[j].x() - pts[i].x()) * (pts[k].y() - pts[i].y()) -
                             (pts[j].y() - pts[i].y()) * (pts[k].x() - pts[i].x());
                counter++;
                if (val > 0) pos = true;
                else if (val < 0) neg = true;
                if (pos && neg) break;
            }
            if (!(pos && neg)) {
                hullPts.insert({pts[i].x(), pts[i].y()});
                hullPts.insert({pts[j].x(), pts[j].y()});
            }
        }
    }

    std::vector<QPointF> hull;
    for (auto &p : hullPts) hull.emplace_back(p.first, p.second);
    if (hull.size() <= 2) return hull;

    double cx = 0, cy = 0;
    for (auto &p : hull) { cx += p.x(); cy += p.y(); }
    cx /= hull.size(); cy /= hull.size();

    std::sort(hull.begin(), hull.end(), [&](const QPointF &a, const QPointF &b){
        double angA = std::atan2(a.y() - cy, a.x() - cx);
        double angB = std::atan2(b.y() - cy, b.x() - cx);
        return angA < angB;
    });
    return hull;
}

std::vector<QPointF> DrawingWidget::monotoneChain(std::vector<QPointF> pts, long long &iter, long long &sort_comps)
{
    if (pts.size() <= 1) return pts;
    std::sort(pts.begin(), pts.end(), [&](const QPointF &p1, const QPointF &p2){
        sort_comps++;
        if (!nearlyEqual(p1.x(), p2.x())) return p1.x() < p2.x();
        return p1.y() < p2.y();
    });

    std::vector<QPointF> lower;
    for (const auto &p : pts) {
        while (lower.size() >= 2) {
            double cross = orient(lower[lower.size()-2], lower[lower.size()-1], p, iter);
            if (cross <= 0) lower.pop_back();
            else break;
        }
        lower.push_back(p);
    }

    std::vector<QPointF> upper;
    for (int i = (int)pts.size() - 1; i >= 0; --i) {
        const auto &p = pts[i];
        while (upper.size() >= 2) {
            double cross = orient(upper[upper.size()-2], upper[upper.size()-1], p, iter);
            if (cross <= 0) upper.pop_back();
            else break;
        }
        upper.push_back(p);
    }

    if (!lower.empty()) lower.pop_back();
    if (!upper.empty()) upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

void DrawingWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(rect(), Qt::white);

    // draw points
    painter.setPen(Qt::black);
    for (auto &pt : points) painter.drawEllipse(pt, 3, 3);

    // slow hull dashed red
    if (!hull_slow.empty()) {
        QPen penSlow(Qt::red);
        penSlow.setStyle(Qt::DashLine);
        penSlow.setWidth(2);
        painter.setPen(penSlow);
        QPainterPath path;
        path.moveTo(hull_slow[0]);
        for (size_t i = 1; i < hull_slow.size(); ++i) path.lineTo(hull_slow[i]);
        path.closeSubpath();
        painter.drawPath(path);
    }

    // fast hull solid blue
    if (!hull_fast.empty()) {
        QPen penFast(Qt::blue);
        penFast.setWidth(2);
        painter.setPen(penFast);
        QPainterPath path;
        path.moveTo(hull_fast[0]);
        for (size_t i = 1; i < hull_fast.size(); ++i) path.lineTo(hull_fast[i]);
        path.closeSubpath();
        painter.drawPath(path);
    }

    painter.setPen(Qt::black);
    painter.drawText(10, 20, QString("Slow iters: %1").arg(iter_slow));
    painter.drawText(10, 36, QString("Fast iters: %1 (sort comps: %2)")
                                 .arg(iter_fast).arg(sort_comparisons));
    painter.drawText(10, height()-20, "Red dashed = brute-force, Blue = monotone chain");
}

void DrawingWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        points.emplace_back(event->position());
#else
        points.emplace_back(event->pos());
#endif
        hull_slow.clear();
        hull_fast.clear();
        iter_slow = iter_fast = sort_comparisons = 0;
        update();
    }
}

void DrawingWidget::runConvexHull()
{
    iter_slow = iter_fast = sort_comparisons = 0;

    std::vector<QPointF> pts = points;
    std::sort(pts.begin(), pts.end(), [](const QPointF &a, const QPointF &b){
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    });
    pts.erase(std::unique(pts.begin(), pts.end(), [](const QPointF &a, const QPointF &b){
                  return nearlyEqual(a.x(), b.x()) && nearlyEqual(a.y(), b.y());
              }), pts.end());

    hull_slow = bruteForceHull(pts, iter_slow);
    hull_fast = monotoneChain(pts, iter_fast, sort_comparisons);

    update();
}

void DrawingWidget::clearAll()
{
    points.clear();
    hull_slow.clear();
    hull_fast.clear();
    iter_slow = iter_fast = sort_comparisons = 0;
    update();
}
