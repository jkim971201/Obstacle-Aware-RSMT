#include "Painter.h"

Painter::Painter(QApplication* app, int maxX, int maxY)
  : app_                 (      app),
    offset_              (       20),
    baseColor_           (Qt::gray ),
    rectFillColor_       (Qt::white),
    rectLineColor_       (Qt::black),
    gridX_               (     maxX),
    gridY_               (     maxY),
    gridWidth_           (      0.0),
    gridHeight_          (      0.0),
    gCellWidth_          (      0.0),
    gCellHeight_         (      0.0)
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
    for(int j = gridY_ - 1; j >= 0; j--)
    {
      gCells_[i][j] = GCell(curX, curY, gCellWidth_, gCellHeight_);
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
Painter::drawRect(QPainter* painter, const QRectF& rect, QColor rectColor, QColor lineColor, int thickness)
{
  painter->setPen( QPen(lineColor , thickness) );
  painter->setBrush(rectColor);

  painter->drawRect( rect );
  painter->fillRect( rect , painter->brush() );
}

void
Painter::drawRect(QPainter* painter, int lx, int ly, int w, int h, QColor rectColor, QColor lineColor)
{
  painter->setPen(QPen(lineColor , 1));
  painter->setBrush(rectColor);

  QRect test(lx, ly, w, h);

  painter->drawRect(test);
  painter->fillRect(test, painter->brush() );
}

void
Painter::drawGCell(QPainter* painter, const GCell& gCell)
{
	if(gCell.isTerm())
    drawRect(painter, gCell.lx() - 1, gCell.ly() - 1, gCell.w() + 1, gCell.h() + 1, Qt::red, Qt::red);
	else if(gCell.isObs())
    drawRect(painter, gCell.rect(), Qt::darkGray, Qt::darkGray, 1);
	else
    drawRect(painter, gCell.rect(), Qt::white, Qt::white, 1);
}

void
Painter::setPath(const std::vector<int>& pathX,
                 const std::vector<int>& pathY)
{
  for(int i = 0; i < pathX.size(); i++)
    gCells_[pathX[i]][pathY[i]].setTerm();
}

void
Painter::setObstacles(const std::vector<int>& obsX1,
                      const std::vector<int>& obsY1,
                      const std::vector<int>& obsX2,
                      const std::vector<int>& obsY2)
{
  for(int i = 0; i < obsX1.size(); i++)
  {
    const int x1 = obsX1[i];
    const int y1 = obsY1[i];
    const int x2 = obsX2[i];
    const int y2 = obsY2[i];

    assert(x1 <= x2);
    assert(y1 <= y2);
  
    for(int x = x1; x < x2 + 1; x++)
    {
      for(int y = y1; y < y2 + 1; y++)
        gCells_[x][y].setObs();
    }
  }
}

void
Painter::setTerms(const std::vector<int>& termX,
                  const std::vector<int>& termY)
{
  for(int i = 0; i < termX.size(); i++)
    gCells_[termX[i]][termY[i]].setTerm();
}

void
Painter::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);

  painter.setPen( QPen(Qt::black, 6) );

  QRect canvas(offset_, offset_, windowWidth_ - 2 * offset_, windowHeight_ - 2 * offset_);
  painter.drawRect( canvas );

  for(int i = 0; i < gridX_; i++)
  {
    for(int j = 0; j < gridY_; j++)
			drawGCell(&painter, gCells_[i][j]);
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
