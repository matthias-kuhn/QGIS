/***************************************************************************
    qgsgeometryselfintersectioncheck.h
    ---------------------
    begin                : September 2015
    copyright            : (C) 2014 by Sandro Mani / Sourcepole AG
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

#ifndef QGS_GEOMETRY_SELFINTERSECTION_CHECK_H
#define QGS_GEOMETRY_SELFINTERSECTION_CHECK_H

#include "qgsgeometryutils.h"
#include "qgssinglegeometrycheck.h"

class ANALYSIS_EXPORT QgsGeometrySelfIntersectionCheckError : public QgsSingleGeometryCheckError
{
  public:
    QgsGeometrySelfIntersectionCheckError( const QgsSingleGeometryCheck *check,
                                           const QgsGeometry &geometry,
                                           const QgsGeometry &errorLocation,
                                           QgsVertexId vertexId,
                                           const QgsGeometryUtils::SelfIntersection &intersection )
      : QgsSingleGeometryCheckError( check, geometry, errorLocation, vertexId )
      , mIntersection( intersection )
    {}

    const QgsGeometryUtils::SelfIntersection &intersection() const { return mIntersection; }
    bool isEqual( const QgsSingleGeometryCheckError *other ) const override;
    bool handleChanges( const QList<QgsGeometryCheck::Change> &changes ) override;
    void update( const QgsSingleGeometryCheckError *other ) override;

  private:
    QgsGeometryUtils::SelfIntersection mIntersection;
};

class ANALYSIS_EXPORT QgsGeometrySelfIntersectionCheck : public QgsSingleGeometryCheck
{
  public:
    explicit QgsGeometrySelfIntersectionCheck( const QgsGeometryCheckContext *context, const QVariantMap &configuration = QVariantMap() )
      : QgsSingleGeometryCheck( FeatureNodeCheck,
                                context,
                                configuration ) {}
    static QList<QgsWkbTypes::GeometryType> factoryCompatibleGeometryTypes() {return {QgsWkbTypes::LineGeometry, QgsWkbTypes::PolygonGeometry};}
    static bool factoryIsCompatible( QgsVectorLayer *layer ) SIP_SKIP { return factoryCompatibleGeometryTypes().contains( layer->geometryType() ); }
    QList<QgsWkbTypes::GeometryType> compatibleGeometryTypes() const override { return factoryCompatibleGeometryTypes(); }
    void fixError( const QMap<QString, QgsFeaturePool *> &featurePools, QgsGeometryCheckError *error, int method, const QMap<QString, int> &mergeAttributeIndices, Changes &changes ) const override;
    QStringList resolutionMethods() const override;
    QString description() const override { return factoryDescription(); }
    QString id() const override { return factoryId(); }
    QgsGeometryCheck::Flags flags() const override {return factoryFlags(); }
    QList<QgsSingleGeometryCheckError *> processGeometry( const QgsGeometry &geometry ) const override;

    static QString factoryDescription() { return tr( "Self intersection" ); }
    static QgsGeometryCheck::Flags factoryFlags() {return QgsGeometryCheck::SingleGeometryCheck; }
    static QString factoryId() { return QStringLiteral( "QgsGeometrySelfIntersectionCheck" ); }

    enum ResolutionMethod { ToMultiObject, ToSingleObjects, NoChange };
};

#endif // QGS_GEOMETRY_SELFINTERSECTION_CHECK_H
