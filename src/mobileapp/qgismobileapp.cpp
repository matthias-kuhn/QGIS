/***************************************************************************
                            qgismobileapp.cpp  -  description
                              -------------------
              begin                : Wed Apr 04 10:48:28 CET 2012
              copyright            : (C) 2012 by Marco Bernasocchi
              email                : marco@bernawebdesign.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QtDeclarative>

#include "qgismobileapp.h"
#include "qgsmapcanvasproxy.h"

QgisMobileapp::QgisMobileapp( QgsApplication *app, QWidget *parent, Qt::WFlags flags )
    : QMainWindow( parent, flags )
{
  mView = new QDeclarativeView( this );

  initDeclarative();

  mView->setSource( QUrl( "qrc:/qml/qgismobileapp.qml" ) );
  mView->setResizeMode(QDeclarativeView::SizeRootObjectToView);

  connect(mView->engine(), SIGNAL(quit()), app, SLOT(quit()));

  mView->setGeometry( 0 ,0, 800, 480 );

  QgsMapCanvasProxy* mapCanvasProxy = mView->rootObject()->findChild<QgsMapCanvasProxy *>("theMapCanvas");
  if (mapCanvasProxy == 0)
  {
    qDebug() << "Map canvas don't returned from QML";
    abort();
  }
  // Setup map canvas
  mMapCanvas = mapCanvasProxy->mapCanvas();
  mMapCanvas->freeze(false);
  mMapCanvas->setVisible(true);

  show();
}

void QgisMobileapp::initDeclarative()
{
  // Register QML custom types
  // TODO create a QDeclarativeExtensionPlugin and move this to it.
  qmlRegisterType<QgsMapCanvasProxy>("org.qgis", 1, 0, "MapCanvas");
  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int dpi = dpiX < dpiY ? dpiX : dpiY; // In case of asymetrical DPI. Improbable
  float dp = dpi * 0.00768443;
  mView->rootContext()->setContextProperty( "dp", dp );
}

QgisMobileapp::~QgisMobileapp()
{

}
