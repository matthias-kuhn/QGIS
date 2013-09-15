/***************************************************************************
                             qgsfieldchooserwidget.cpp
                             -------------------------
    begin                : September 2013
    copyright            : (C) 2013 Denis Rouzaud
    email                : denis.rouzaud@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qgsfieldchooserwidget.h"
#include "qgslayerchooserwidget.h"
#include "qgsvectorlayer.h"

QgsFieldChooserWidget::QgsFieldChooserWidget(QgsLayerChooserWidget* layerChooser, QObject *parent)
  : QObject(parent)
  , mLayerChooser( layerChooser )
  , mFilter( new QgsFieldChooserFilter() )
  , mLayer( 0 )
{
  connect(layerChooser, SIGNAL(layerChanged(QgsMapLayer*)), this, SLOT(layerChanged(QgsMapLayer*)));
}

void QgsFieldChooserWidget::setFilter(QgsFieldChooserFilter* filter)
{
  mFilter = filter;
}

void QgsFieldChooserWidget::layerChanged(QgsMapLayer* layer)
{
  if (mLayer)
  {
    disconnect(mLayer, SIGNAL(attributeAdded()), this, SLOT(layerChanged()));
    disconnect(mLayer, SIGNAL(attributeDeleted()), this, SLOT(layerChanged()));
    disconnect(mLayer, SIGNAL(layerDeleted()), this, SLOT(layerDeleted()));
  }
  mLayer = 0;
  clearWidget();

  if (!layer)
    return;

  QgsVectorLayer* vl = dynamic_cast<QgsVectorLayer*>(layer);
  if (!vl)
    return;

  mLayer = vl;

  connect(mLayer, SIGNAL(attributeAdded()), this, SLOT(layerChanged()));
  connect(mLayer, SIGNAL(attributeDeleted()), this, SLOT(layerChanged()));
  connect(mLayer, SIGNAL(layerDeleted()), this, SLOT(layerDeleted()));


  const QgsFields &fields = vl->pendingFields();
  for ( int idx = 0; idx < fields.count(); ++idx )
  {
    QString fieldName = fields[idx].name();
    QString fieldAlias = vl->attributeAlias(idx);
    DisplayStatus display = mFilter->acceptField(idx);
    if (display != hidden)
      addField(fieldAlias, fieldName, display);
  }

}

void QgsFieldChooserWidget::layerChanged()
{
  QgsMapLayer* layer = mLayerChooser->getLayer();
  layerChanged(layer);
}

void QgsFieldChooserWidget::layerDeleted()
{
  clearWidget();
  mLayer = 0;
}






