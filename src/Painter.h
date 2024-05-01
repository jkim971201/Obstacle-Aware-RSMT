#pragma once

#include <filesystem>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QScreen>
#include <unordered_map>
#include <vector>
#include <cassert>

class GCell
{
  public:
 
    GCell() {}

    GCell(int lx, int ly, int w, int h)
      : lx_      (lx),
        ly_      (ly),
        w_       ( w),
        h_       ( h),
        isPath_  (false),
        isTerm_  (false),
        isObs_   (false)
    {
      rect_ = QRectF(lx, ly, w, h); 
    }

    void setPath()   { isPath_  = true; }
    void setObs()    { isObs_   = true; }
    void setTerm()   { isTerm_  = true; }

    // Getters
    const QRectF& rect() const { return rect_; }

    int w()  const { return w_;  }
    int h()  const { return h_;  }
    int lx() const { return lx_; }
    int ly() const { return ly_; }

    bool isPath() const { return isPath_; }
    bool isObs () const { return isObs_;  }
    bool isTerm() const { return isTerm_; }

  private:

    int w_;
    int h_;

    int lx_;
    int ly_;

    bool isPath_;
    bool isTerm_;
    bool isObs_;

    QRectF rect_;
};

class Painter : public QWidget
{
  public:

    // w : window width
    // h : window height
    // color : background color
    Painter(QApplication* app, int maxX, int maxY);

    // APIs
    void openWindow();

    void setPath(const std::vector<int>& pathX, 
                 const std::vector<int>& pathY);

    void setTerms(const std::vector<int>& termX, 
                  const std::vector<int>& termY);

    void setObstacles(const std::vector<int>& obsX1,
                      const std::vector<int>& obsY1,
                      const std::vector<int>& obsX2,
                      const std::vector<int>& obsY2);

  protected:

    void paintEvent(QPaintEvent* event);
    // This function is not for users

  private:

    QApplication* app_;
    QColor baseColor_;
    QColor rectFillColor_;
    QColor rectLineColor_;

    void drawGCell(QPainter* painter, const GCell& gCell);
    void drawLine (QPainter* painter, QPointF p1, QPointF p2, QColor lineColor = Qt::black, int thickness = 1);
    void drawRect (QPainter* painter, const QRectF& rect, QColor rectColor = Qt::white, QColor rectLineColor = Qt::black, int thickness = 1);
    void drawRect (QPainter* painter, int lx, int ly, int w, int h, QColor rectColor = Qt::white, QColor rectLineColor = Qt::white);

    int windowWidth_;
    int windowHeight_;
    int offset_;

    int   gridX_;
    int   gridY_;
    float gridWidth_;
    float gridHeight_;
    float gCellWidth_;
    float gCellHeight_;

    std::vector<std::vector<GCell>> gCells_;
};
