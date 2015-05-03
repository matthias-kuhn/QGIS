/***************************************************************************
    qgsmaptooltouch.cpp  -  map tool for zooming and panning using qgestures
    ----------------------
    begin                : February 2012
    copyright            : (C) 2012 by Marco Bernasocchi
    email                : marco at bernawebdesign.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsmaptoolpan.h"
#include "qgsmapcanvas.h"
#include "qgscursors.h"
#include "qgsmaptopixel.h"
#include <QBitmap>
#include <QCursor>
#include <QMouseEvent>
#include <qgslogger.h>
#include <QApplication>


QgsMapToolPan::QgsMapToolPan( QgsMapCanvas* canvas )
    : QgsMapTool( canvas ), mDragging( false ), mPinching( false )
{
  // set cursor
//  QBitmap panBmp = QBitmap::fromData( QSize( 16, 16 ), pan_bits );
//  QBitmap panBmpMask = QBitmap::fromData( QSize( 16, 16 ), pan_mask_bits );
//  mCursor = QCursor( panBmp, panBmpMask, 5, 5 );
}

QgsMapToolPan::~QgsMapToolPan()
{
  mCanvas->ungrabGesture( Qt::PinchGesture );
}

void QgsMapToolPan::activate()
{
  mCanvas->grabGesture( Qt::PinchGesture );
  QgsMapTool::activate();
}

void QgsMapToolPan::deactivate()
{
  mCanvas->ungrabGesture( Qt::PinchGesture );
  QgsMapTool::deactivate();
}

void QgsMapToolPan::canvasMoveEvent( QMouseEvent * e )
{
  if ( !mPinching )
  {
    if ( e->buttons() & Qt::LeftButton )
    {
      QgsPoint pos = mCanvas->getCoordinateTransform()->toMapCoordinates( e->pos() );

      if ( !mDragging )
      {
        mDragging = true;

        mMouseAnchorPoint = pos;
        mCanvas->freeze();
      }
      else
      {
        const QgsRectangle& oldExtent = mCanvas->extent();

        double dx = mMouseAnchorPoint.x() - pos.x();
        double dy = mMouseAnchorPoint.y() - pos.y();
        QgsRectangle extent = QgsRectangle( oldExtent.xMinimum() + dx,
                                            oldExtent.yMinimum() + dy,
                                            oldExtent.xMaximum() + dx,
                                            oldExtent.yMaximum() + dy );

        mCanvas->setExtent( extent );
        mCanvas->refresh();
      }
    }
  }
  e->accept();
}

void QgsMapToolPan::canvasReleaseEvent( QMouseEvent * e )
{
  if ( mDragging )
  {
    mCanvas->freeze( false );
    mCanvas->refresh();
    mDragging = false;
  }
  e->accept();
}

void QgsMapToolPan::canvasDoubleClickEvent( QMouseEvent *e )
{
  if ( !mPinching )
  {
    mCanvas->zoomWithCenter( e->x(), e->y(), true );
  }
}

bool QgsMapToolPan::gestureEvent( QGestureEvent *event )
{
  if ( QGesture *gesture = event->gesture( Qt::PinchGesture ) )
  {
    mPinching = true;

    event->accept();
    pinchTriggered( static_cast<QPinchGesture *>( gesture ) );
  }
  return true;
}


void QgsMapToolPan::pinchTriggered( QPinchGesture *gesture )
{
  if ( gesture->state() == Qt::GestureFinished )
  {
    //a very small totalScaleFactor indicates a two finger tap (pinch gesture without pinching)
    if ( 0.98 < gesture->totalScaleFactor()  && gesture->totalScaleFactor() < 1.02 )
    {
      mCanvas->zoomOut();
    }
    else
    {
      //Transfor global coordinates to widget coordinates
      QPoint pos = gesture->centerPoint().toPoint();
      pos = mCanvas->mapFromGlobal( pos );
      // transform the mouse pos to map coordinates
      QgsPoint center  = mCanvas->getCoordinateTransform()->toMapPoint( pos.x(), pos.y() );
      QgsRectangle r = mCanvas->extent();
      r.scale( 1 / gesture->totalScaleFactor(), &center );
      mCanvas->setExtent( r );
      mCanvas->refresh();
    }
    mPinching = false;
  }
}
