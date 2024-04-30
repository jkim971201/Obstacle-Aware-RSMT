#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <time.h>

#include "Painter.h"

namespace gui
{

Painter::Painter(QApplication* app, int maxX, int maxY)
  : app_                 (       app),
    offset_              (        20),
    baseColor_           ( Qt::gray ),
    rectFillColor_       ( Qt::white),
    rectLineColor_       ( Qt::black),
    gridX_               (      maxX),
    gridY_               (      maxY),
    gridWidth_           (       0.0),
    gridHeight_          (       0.0),
    gCellWidth_          (       0.0),
    gCellHeight_         (       0.0)
{
  QSize size = app->screens()[0]->size();

  int l = std::min(size.width(), size.height());
  windowWidth_  = l / 2;
  windowHeight_ = l / 2;

  this->resize(windowWidth_, windowHeight_);

  QPalette palette( Painter::palette() );
  palette.setColor( backgroundRole(), baseColor_ );

  this->setPalette(palette);
  this->setWindowTitle( "Shortest Path Visualization" );

  // Uniform GCell size
  // (we know that GCells don't have same sizes)
  gridWidth_   = static_cast<float>( windowWidth_  - 2 * offset_ );
  gridHeight_  = static_cast<float>( windowHeight_ - 2 * offset_ );
  gCellWidth_  = gridWidth_  / static_cast<float>(gridX_);
  gCellHeight_ = gridHeight_ / static_cast<float>(gridY_);

  printf("gCellWidth  (Visual) : %f\n", gCellWidth_);
  printf("gCellHeight (Visual) : %f\n", gCellHeight_);

  // Make GCells for visualization
  gCells_.resize(gridX_);
  for(auto& vec : gCells_)
    vec.resize(gridY_);

  float curX = static_cast<float>(offset_);
  float curY = static_cast<float>(offset_) + gridHeight_ - gCellHeight_;

  for(int i = 0; i < gridX_; i++)
  {
    for(int j = 0; j < gridY_; j++)
    {
      gCells_[i][j] = QRectF(curX, curY, gCellWidth_, gCellHeight_);
      curY -= gCellHeight_;
    }
    curY = static_cast<float>(offset_) + gridHeight_ - gCellHeight_;
    curX += gCellWidth_;
  }
}

void 
Painter::drawLine(QPainter* painter, QPointF p1, QPointF p2, QColor lineColor, int thickness)
{
  painter->setPen( QPen(lineColor, thickness) );
  painter->drawLine(p1, p2);
}

void
Painter::drawRect(QPainter* painter, const QRectF* rect, QColor rectColor, QColor lineColor, int thickness)
{
  painter->setPen( QPen(lineColor , thickness) );
  painter->setBrush(rectColor);

  drawRect(painter, *rect, rectColor, lineColor);
}

void
Painter::drawRect(QPainter* painter, const QRectF& rect, QColor rectColor, QColor lineColor, int thickness)
{
  painter->setPen( QPen(lineColor , thickness) );
  painter->setBrush(rectColor);

  painter->drawRect( rect );
  painter->fillRect( rect , painter->brush() );
}

void
Painter::drawRect(QPainter* painter, int lx, int ly, int w, int h)
{
  QRect test(lx, ly, w, h);

  painter->drawRect( test );
  painter->fillRect( test, painter->brush() );
}

void
Painter::drawGCell(QPainter* painter, const GCell& gCell, QColor rectColor, QColor rectLineColor, int thickness)
{
	drawRect(painter, gCell.rect(), rectColor, rectLineColor, thickness);
}

void
Painter::setPath(const std::vector<Point>& path)
{
	for(auto& n : path)
	{
		// printf("Set Path : (%d, %d)\n", n.x, n.y);
		gCellInsts_[n.x][n.y].setPath();
	}
}

void
Painter::setBlock(const std::vector<Point>& block)
{
	for(auto& n : block)
	{
		// printf("Set Block : (%d, %d)\n", n.x, n.y);
		gCellInsts_[n.x][n.y].setBlock();
	}
}

void
Painter::setSink(const std::vector<Point>& sink)
{
	for(auto& n : sink)
	{
		// printf("Set Sink : (%d, %d)\n", n.x, n.y);
		gCellInsts_[n.x][n.y].setSink();
	}
}

void
Painter::setSource(const Point& src)
{
	gCellInsts_[src.x][src.y].setSource();
}

void
Painter::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);

  painter.setPen( QPen(Qt::black, 6) );

  QRect canvas(offset_, offset_, windowWidth_ - 2 * offset_, windowHeight_ - 2 * offset_);
  painter.drawRect( canvas );

  for(int i = 0; i < numGCellX_; i++)
  {
    for(int j = 0; j < numGCellY_; j++)
    {
      if( gCellInsts_[i][j].isSource() )
        drawGCell( &painter, gCellInsts_[i][j], Qt::blue, Qt::black, 4);
			else if( gCellInsts_[i][j].isSink() )
        drawGCell( &painter, gCellInsts_[i][j], Qt::red, Qt::black, 4);
      else if( gCellInsts_[i][j].isPath() )
        drawGCell( &painter, gCellInsts_[i][j], Qt::darkRed, Qt::black, 4);
      else if( gCellInsts_[i][j].isBlock() )
        drawGCell( &painter, gCellInsts_[i][j], Qt::darkGray, Qt::black, 4);
      else 
				drawGCell( &painter, gCellInsts_[i][j], Qt::white, Qt::black, 4);
    }
  }
}

void
Painter::openWindow()
{
  printf("Width  of the window is : %d\n", windowWidth_);
  printf("Height of the window is : %d\n", windowHeight_);
  this->show();
  int app_exec = app_->exec();
}

}
