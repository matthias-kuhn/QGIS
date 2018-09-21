/***************************************************************************
    qgsgeometrycheckcontext.h
    ---------------------
    begin                : September 2018
    copyright            : (C) 2018 Matthias Kuhn
    email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsgeometrycheckcontext.h"
#include "qgsreadwritelocker.h"
#include "qgsthreadingutils.h"
#include "qgsvectorlayer.h"

QgsGeometryCheckContext::QgsGeometryCheckContext( int _precision, const QgsCoordinateReferenceSystem &_mapCrs, const QMap<QString, QgsFeaturePool *> &_featurePools, const QgsCoordinateTransformContext &transformContext )
  : tolerance( std::pow( 10, -_precision ) )
  , reducedTolerance( std::pow( 10, -_precision / 2 ) )
  , mapCrs( _mapCrs )
  , featurePools( _featurePools )
  , transformContext( transformContext )
{
}

double QgsGeometryCheckContext::layerScaleFactor( const QPointer<QgsVectorLayer> &layer )
{
  QgsReadWriteLocker locker( mCacheLock, QgsReadWriteLocker::Read );
  if ( !mScaleFactorCache.contains( layer ) )
  {
    double scaleFactor = 1.0;
    QgsThreadingUtils::runOnMainThread( [this, layer, &scaleFactor]()
    {
      QgsVectorLayer *lyr = layer.data();
      if ( lyr )
      {
        QgsCoordinateTransform ct( lyr->crs(), mapCrs, transformContext );
        scaleFactor = ct.scaleFactor( lyr->extent() );
      }
    } );

    locker.changeMode( QgsReadWriteLocker::Write );
    mScaleFactorCache[layer] = scaleFactor;
    locker.changeMode( QgsReadWriteLocker::Read );
  }

  return mScaleFactorCache.value( layer );
}
