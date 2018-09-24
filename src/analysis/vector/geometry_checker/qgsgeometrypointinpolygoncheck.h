/***************************************************************************
    qgsgeometrypointinpolygoncheck.h
    ---------------------
    begin                : June 2017
    copyright            : (C) 2017 by Sandro Mani / Sourcepole AG
    email                : smani at sourcepole dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define SIP_NO_FILE

#ifndef QGSGEOMETRYPOINTINPOLYGONCHECK_H
#define QGSGEOMETRYPOINTINPOLYGONCHECK_H

#include "qgsgeometrycheck.h"

class ANALYSIS_EXPORT QgsGeometryPointInPolygonCheck : public QgsGeometryCheck
{
  public:
    QgsGeometryPointInPolygonCheck( QgsGeometryCheckContext *context, const QVariantMap &configuration )
      : QgsGeometryCheck( FeatureNodeCheck, {QgsWkbTypes::PointGeometry}, context, configuration )
    {}
    void collectErrors( const QMap<QString, QgsFeaturePool *> &featurePools, QList<QgsGeometryCheckError *> &errors, QStringList &messages, QgsFeedback *feedback = nullptr, const LayerFeatureIds &ids = LayerFeatureIds() ) const override;
    void fixError( const QMap<QString, QgsFeaturePool *> &featurePools, QgsGeometryCheckError *error, int method, const QMap<QString, int> &mergeAttributeIndices, Changes &changes ) const override;
    QStringList resolutionMethods() const override;
    QString description() const override { return tr( "Point not in polygon" ); }
    QString id() const override { return QStringLiteral( "QgsGeometryPointInPolygonCheck" ); }

    enum ResolutionMethod { NoChange };
};

#endif // QGSGEOMETRYPOINTINPOLYGONCHECK_H
