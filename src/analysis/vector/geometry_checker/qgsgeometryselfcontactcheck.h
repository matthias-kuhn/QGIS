/***************************************************************************
    qgsgeometryselfcontactcheck.h
    ---------------------
    begin                : September 2017
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

#ifndef QGS_GEOMETRY_SELFCONTACT_CHECK_H
#define QGS_GEOMETRY_SELFCONTACT_CHECK_H

#include "qgssinglegeometrycheck.h"

class ANALYSIS_EXPORT QgsGeometrySelfContactCheck : public QgsSingleGeometryCheck
{
  public:
    QgsGeometrySelfContactCheck( QgsGeometryCheckContext *context, const QVariantMap &configuration )
      : QgsSingleGeometryCheck( FeatureNodeCheck, context, configuration ) {}
    QList<QgsWkbTypes::GeometryType> factoryCompatibleGeometryTypes() const {return {QgsWkbTypes::LineGeometry, QgsWkbTypes::PolygonGeometry};}
    QList<QgsWkbTypes::GeometryType> compatibleGeometryTypes() const override { return factoryCompatibleGeometryTypes(); }
    QList<QgsSingleGeometryCheckError *> processGeometry( const QgsGeometry &geometry ) const override;
    void fixError( const QMap<QString, QgsFeaturePool *> &featurePools, QgsGeometryCheckError *error, int method, const QMap<QString, int> &mergeAttributeIndices, Changes & ) const override;
    QStringList resolutionMethods() const override;
    QString description() const override { return tr( "Self contact" ); }
    QString id() const override { return QStringLiteral( "QgsGeometrySelfContactCheck" ); }

    enum ResolutionMethod { NoChange };
};

#endif // QGS_GEOMETRY_SELFCONTACT_CHECK_H
