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

#include "qgsmaptooltouch.h"
#include "qgsmapcanvas.h"
#include "qgsmapcanvasmap.h"
#include "qgscursors.h"
#include "qgsmaptopixel.h"
#include <QBitmap>
#include <QCursor>
#include <QMouseEvent>
#include <qgslogger.h>
#include <QApplication>


QgsMapToolTouch::QgsMapToolTouch( QgsMapCanvas* canvas )
    : QgsMapTool( canvas ), mDragging( false ), mPinching( false )
{
  // set cursor
//  QBitmap panBmp = QBitmap::fromData( QSize( 16, 16 ), pan_bits );
//  QBitmap panBmpMask = QBitmap::fromData( QSize( 16, 16 ), pan_mask_bits );
//  mCursor = QCursor( panBmp, panBmpMask, 5, 5 );

}

QgsMapToolTouch::~QgsMapToolTouch()
{
  mCanvas->ungrabGesture( Qt::PinchGesture );
}

void QgsMapToolTouch::activate()
{
  mCanvas->grabGesture( Qt::PinchGesture );
  QgsMapTool::activate();
}

void QgsMapToolTouch::deactivate()
{
  mCanvas->ungrabGesture( Qt::PinchGesture );
  QgsMapTool::deactivate();
}

void QgsMapToolTouch::canvasMoveEvent( QMouseEvent * e )
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

void QgsMapToolTouch::canvasReleaseEvent( QMouseEvent * e )
{
  if ( mDragging )
  {
    mCanvas->freeze( false );
    mCanvas->refresh();
    mDragging = false;
  }
  e->accept();
}

void QgsMapToolTouch::canvasDoubleClickEvent( QMouseEvent *e )
{
  if ( !mPinching )
  {
    mCanvas->zoomWithCenter( e->x(), e->y(), true );
  }
}

bool QgsMapToolTouch::gestureEvent( QGestureEvent *event )
{
  if ( QGesture *gesture = event->gesture( Qt::PinchGesture ) )
  {
    mPinching = true;

    event->accept();
    pinchTriggered( static_cast<QPinchGesture *>( gesture ) );
  }
  return true;
}


void QgsMapToolTouch::pinchTriggered( QPinchGesture *gesture )
{
  QPoint currentPos = mCanvas->mapFromGlobal( gesture->centerPoint().toPoint() );

  if ( gesture->state() == Qt::GestureStarted )
  {
    mCanvas->freeze( true );
    mDragging = false;

    mPinchCenterPoint = mCanvas->getCoordinateTransform()->toMapCoordinates( currentPos );
  }
  if ( gesture->state() == Qt::GestureUpdated )
  {
    if ( ! ( 0.98 < gesture->totalScaleFactor()  && gesture->totalScaleFactor() < 1.02 ) )
    {
      QgsPoint oldCenter = mCanvas->center();
      QgsPoint mapPos( mCanvas->getCoordinateTransform()->toMapCoordinates( currentPos ) );
      double dx = mPinchCenterPoint.x() - mapPos.x();
      double dy = mPinchCenterPoint.y() - mapPos.y();
      QgsPoint newCenter( mapPos.x() + (( oldCenter.x() - mapPos.x() ) / gesture->scaleFactor() ) + dx,
                          mapPos.y() + (( oldCenter.y() - mapPos.y() ) / gesture->scaleFactor() ) + dy );

      mCanvas->zoomByFactor( 1/gesture->scaleFactor(), &newCenter );
      mPinchCenterPoint = mCanvas->getCoordinateTransform()->toMapCoordinates( currentPos );
    }
  }
  if ( gesture->state() == Qt::GestureFinished )
  {
    mCanvas->freeze( false );
    //a very small totalScaleFactor indicates a two finger tap (pinch gesture without pinching)
    if ( 0.98 < gesture->totalScaleFactor()  && gesture->totalScaleFactor() < 1.02 )
    {
      QgsPoint currentPinchCenter = mCanvas->getCoordinateTransform()->toMapCoordinates( currentPos );
      mCanvas->zoomByFactor( 0.5, &currentPinchCenter );
    }
    else
    {
      mCanvas->refresh();
    }
    mPinching = false;
  }
}
