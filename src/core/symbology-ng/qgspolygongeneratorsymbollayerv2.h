/***************************************************************************
 qgspolygongeneratorsymbollayerv2.h
 ---------------------
 begin                : November 2015
 copyright            : (C) 2015 by Matthias Kuhn
 email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSPOLYGONGENERATORSYMBOLLAYERV2_H
#define QGSPOLYGONGENERATORSYMBOLLAYERV2_H

#include "qgssymbollayerv2.h"

class CORE_EXPORT QgsPolygonGeneratorSymbolLayerV2 : public QgsFillSymbolLayerV2
{
  public:
    static QgsSymbolLayerV2* create( const QgsStringMap& properties = QgsStringMap() );

    QgsPolygonGeneratorSymbolLayerV2( QgsFillSymbolV2* symbol, const QgsStringMap& properties = QgsStringMap() );

    QString layerType() const override;

    void startRender( QgsSymbolV2RenderContext& context ) override;

    void stopRender( QgsSymbolV2RenderContext& context ) override;

    QgsSymbolLayerV2* clone() const override;

    QgsStringMap properties() const override;

    void drawPreviewIcon( QgsSymbolV2RenderContext& context, QSize size ) override;

    void setGeometryModifier( const QString& exp );

    QString geometryModifier() const { return mExpression->expression(); }

    virtual QgsSymbolV2* subSymbol() override { return mSymbol; }

    virtual bool setSubSymbol( QgsSymbolV2* symbol ) override;

    virtual QSet<QString> usedAttributes() const override;

    //! Will always return true.
    //! This is a hybrid layer, it constructs its own geometry so it does not
    //! care about the geometry of its parents.
    bool isCompatibleWithSymbol( QgsSymbolV2* symbol ) override;

    void renderPolygon( const QPolygonF& points, QList<QPolygonF>* rings, QgsSymbolV2RenderContext& context ) override;

  private:
    QScopedPointer<QgsExpression> mExpression;
    QgsFillSymbolV2* mSymbol;
};

#endif // QGSPOLYGONGENERATORSYMBOLLAYERV2_H
