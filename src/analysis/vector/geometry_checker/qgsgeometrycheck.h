/***************************************************************************
    qgsgeometrycheck.h
    ---------------------
    begin                : September 2014
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

#ifndef QGS_GEOMETRY_CHECK_H
#define QGS_GEOMETRY_CHECK_H

#include <QApplication>
#include <limits>
#include <QStringList>
#include <QPointer>

#include "qgis_analysis.h"
#include "qgsfeature.h"
#include "qgsvectorlayer.h"
#include "geometry/qgsgeometry.h"
#include "qgsgeometrycheckerutils.h"

class QgsGeometryCheckError;
class QgsFeaturePool;

#define FEATUREID_NULL std::numeric_limits<QgsFeatureId>::min()

class ANALYSIS_EXPORT QgsGeometryCheck
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS( QgsGeometryCheck )

  public:

    /**
     * A list of layers and feature ids for each of these layers.
     * In C++, the member `ids` can be accessed directly.
     * In Python some accessor methods will need to be written.
     *
     * \since QGIS 3.4
     */
    struct LayerFeatureIds
    {
      LayerFeatureIds() = default;
      LayerFeatureIds( const QMap<QString, QgsFeatureIds> &ids ) SIP_SKIP;

      QMap<QString, QgsFeatureIds> ids SIP_SKIP;

#ifndef SIP_RUN
      QMap<QString, QgsFeatureIds> toMap() const
      {
        return ids;
      }

      bool isEmpty() const
      {
        return ids.isEmpty();
      }
#endif
    };

    enum ChangeWhat
    {
      ChangeFeature,
      ChangePart,
      ChangeRing,
      ChangeNode
    };

    enum ChangeType
    {
      ChangeAdded,
      ChangeRemoved,
      ChangeChanged
    };

    enum CheckType
    {
      FeatureNodeCheck,
      FeatureCheck,
      LayerCheck
    };

    enum Flag
    {
      SingleGeometryCheck = 1 << 1,
      SingleLayerTopologyCheck = 1 << 2,
      AvailableInValidation = 1 << 3
    };
    Q_DECLARE_FLAGS( Flags, Flag )
    Q_FLAG( Flags )

    struct Change
    {
      Change() = default;
      Change( ChangeWhat _what, ChangeType _type, QgsVertexId _vidx = QgsVertexId() )
        : what( _what )
        , type( _type )
        , vidx( _vidx )
      {}
      ChangeWhat what;
      ChangeType type;
      QgsVertexId vidx;
      bool operator==( const Change &other )
      {
        return what == other.what && type == other.type && vidx == other.vidx;
      }
    };

    typedef QMap<QString, QMap<QgsFeatureId, QList<Change> > > Changes;

    QgsGeometryCheck( CheckType checkType, const QList<QgsWkbTypes::GeometryType> &compatibleGeometryTypes, QgsGeometryCheckContext *context )
      : mCheckType( checkType )
      , mCompatibleGeometryTypes( compatibleGeometryTypes )
      , mContext( context )
    {}
    virtual ~QgsGeometryCheck() = default;

    virtual bool isCompatible( QgsVectorLayer *layer ) const;
    virtual QgsGeometryCheck::Flags flags() const {return nullptr;}

    virtual void collectErrors( const QMap<QString, QgsFeaturePool *> &featurePools, QList<QgsGeometryCheckError *> &errors, QStringList &messages, QgsFeedback *feedback = nullptr, const LayerFeatureIds &ids = QgsGeometryCheck::LayerFeatureIds() ) const = 0;
    //! Fix the error \a error with the specified \a method.
    virtual void fixError( const QMap<QString, QgsFeaturePool *> &featurePools, QgsGeometryCheckError *error, int method, const QMap<QString, int> &mergeAttributeIndices, Changes &changes SIP_INOUT ) const SIP_SKIP;
    virtual QStringList resolutionMethods() const = 0;
    virtual QString errorDescription() const = 0;
    virtual QString errorName() const = 0;
    CheckType checkType() const { return mCheckType; }
    QgsGeometryCheckContext *context() const { return mContext; }

  protected:
    QMap<QString, QgsFeatureIds> allLayerFeatureIds( const QMap<QString, QgsFeaturePool *> &featurePools ) const SIP_SKIP;
    void replaceFeatureGeometryPart( const QMap<QString, QgsFeaturePool *> &featurePools, const QString &layerId, QgsFeature &feature, int partIdx, QgsAbstractGeometry *newPartGeom, Changes &changes ) const SIP_SKIP;
    void deleteFeatureGeometryPart( const QMap<QString, QgsFeaturePool *> &featurePools, const QString &layerId, QgsFeature &feature, int partIdx, Changes &changes ) const SIP_SKIP;
    void deleteFeatureGeometryRing( const QMap<QString, QgsFeaturePool *> &featurePools, const QString &layerId, QgsFeature &feature, int partIdx, int ringIdx, Changes &changes ) const SIP_SKIP;

    const CheckType mCheckType;
    QList<QgsWkbTypes::GeometryType> mCompatibleGeometryTypes;
    QgsGeometryCheckContext *mContext;

    double scaleFactor( QPointer<QgsVectorLayer> layer ) const SIP_SKIP;
};

#endif // QGS_GEOMETRY_CHECK_H
