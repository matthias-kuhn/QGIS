/***************************************************************************
    qgsgeometrylinelayerintersectioncheck.h
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

#ifndef QGSGEOMETRYLINELAYERINTERSECTIONCHECK_H
#define QGSGEOMETRYLINELAYERINTERSECTIONCHECK_H

#include "qgsgeometrycheck.h"

class ANALYSIS_EXPORT QgsGeometryLineLayerIntersectionCheck : public QgsGeometryCheck
{
  public:
    QgsGeometryLineLayerIntersectionCheck( QgsGeometryCheckContext *context, const QVariantMap &configuration )
      : QgsGeometryCheck( FeatureNodeCheck, {QgsWkbTypes::LineGeometry}, context, configuration )
    , mCheckLayer( configurationValue<QString>( "checkLayer" ) )
    {}
    void collectErrors( const QMap<QString, QgsFeaturePool *> &featurePools, QList<QgsGeometryCheckError *> &errors, QStringList &messages, QgsFeedback *feedback = nullptr, const LayerFeatureIds &ids = LayerFeatureIds() ) const override;
    void fixError( const QMap<QString, QgsFeaturePool *> &featurePools, QgsGeometryCheckError *error, int method, const QMap<QString, int> &mergeAttributeIndices, Changes &changes ) const override;
    QStringList resolutionMethods() const override;
    QString description() const override { return tr( "Intersection" ); }
    QString id() const override { return QStringLiteral( "QgsGeometryLineLayerIntersectionCheck" ); }

    enum ResolutionMethod { NoChange };

  private:
    QString mCheckLayer;
};

#endif // QGSGEOMETRYLINELAYERINTERSECTIONCHECK_H
