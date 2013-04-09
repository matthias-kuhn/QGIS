/***************************************************************************
    testqgsvectorlayercache.cpp
     --------------------------------------
    Date                 : 20.2.2013
    Copyright            : (C) 2013 Matthias Kuhn
    Email                : matthias dot kuhn at gmx dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QtTest>
#include <QObject>
#include <QTemporaryFile>

//qgis includes...
#include <qgsvectorlayer.h>
#include <qgsapplication.h>
#include <qgsmaplayerregistry.h>
#include <qgsmaprenderer.h>
#include <qgspallabeling.h>
#include <qgsdatasourceuri.h>

/** @ingroup UnitTests
 * This is a unit test for the vector layer cache
 *
 * @see QgsVectorLayerCache
 */
class TestPalLabelling: public QObject
{
    Q_OBJECT

  private slots:
    void initTestCase();      // will be called before the first testfunction is executed.
    void cleanupTestCase();   // will be called after the last testfunction was executed.
    void init() {};           // will be called before each testfunction is executed.
    void cleanup() {};        // will be called after every testfunction.

    void testLabelling();

  private:
    QgsVectorLayer* mPointsLayer;
    QgsMapRenderer* mMapRenderer;
    QgsPalLabeling* mPalLabeling;
};

// runs before all tests
void TestPalLabelling::initTestCase()
{
  QgsApplication::init();
  QgsApplication::initQgis();
  QgsApplication::showSettings();

  //
  // load a vector layer
  //
  QString myDataDir( TEST_DATA_DIR ); //defined in CmakeLists.txt
  QString myTestDataDir = myDataDir + QDir::separator();

  QString myPointsFileName = myTestDataDir + "points.shp";
  QFileInfo myPointFileInfo( myPointsFileName );

/*  mPointsLayer = new QgsVectorLayer( myPointFileInfo.filePath(),
                                     myPointFileInfo.completeBaseName(), "ogr" );*/

  QgsDataSourceURI uri;
  uri.setDatabase( "arbon" );
  uri.setConnection( "", "arbon", "kk", "C30987" );
  uri.setUseEstimatedMetadata( true );
  uri.setWkbType( QGis::WKBPolygon );
  uri.setDataSource( "qgep", "od_wastewater_structure", "detail_geometry_geometry" );

  mPointsLayer = new QgsVectorLayer( uri.uri(), uri.table(), "postgres" );

  mPointsLayer->enableLabels( true );

  // Register the layer with the registry
  QgsMapLayerRegistry::instance()->addMapLayers(
    QList<QgsMapLayer *>() << mPointsLayer );

  mPalLabeling = new QgsPalLabeling();

  mMapRenderer = new QgsMapRenderer();
  mMapRenderer->setLayerSet( QStringList() << mPointsLayer->id() );
  mMapRenderer->setLabelingEngine( mPalLabeling );

  QgsPalLayerSettings pls;
  pls.enabled = true;
  pls.fieldName = "Class";
  pls.fieldIndex = 0;
  pls.writeToLayer( mPointsLayer );
}

//runs after all tests
void TestPalLabelling::cleanupTestCase()
{
  delete mPointsLayer;
}

void TestPalLabelling::testLabelling()
{
  mMapRenderer->setExtent( QgsRectangle( 747293.69, 261700.41, 752563.12,266086.47 ) );
  mMapRenderer->setOutputSize( QSizeF( 10, 10 ), 10 );
  QPainter p( new QWidget() );
  mMapRenderer->render( &p );
}

QTEST_MAIN( TestPalLabelling )
#include "moc_testqgspallabelling.cxx"

