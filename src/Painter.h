#pragma once

#include <filesystem>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QScreen>
#include <unordered_map>
#include <vector>

#include "Grid.h"

namespace gui
{

class Rect
{
  public:
 
		Rect() {}

    Rect(int lx, int ly, int w, int h)
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

		// Getters
    const QRectF& rect() const { return rect_; }

		int x() const { return x_; }
		int y() const { return y_; }

		bool isPath() const { return isPath_; }
		bool isObs () const { return isObs_;  }
		bool isTerm() const { return isTerm_; }

	private:

    int x_;
		int y_;

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

    void drawLine(QPainter* painter, QPointF p1, QPointF p2, QColor lineColor = Qt::black, int thickness = 1);
    void drawRect(QPainter* painter, const QRectF& rect, QColor rectColor = Qt::white, QColor rectLineColor = Qt::black, int thickness = 1);
    void drawRect(QPainter* painter, int lx, int ly, int w, int h);

    int windowWidth_;
    int windowHeight_;
    int offset_;

    int   gridX_;
    int   gridY_;
    float gridWidth_;
    float gridHeight_;
    float gCellWidth_;
    float gCellHeight_;

		std::vector<std::vector<bool>>   
		std::vector<std::vector<QRectF>> gCells_;
    std::vector<QRectF> obstacles_;
};

}
