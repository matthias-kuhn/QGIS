/***************************************************************************
                             QgsLayerChooserwidget.cpp
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

#ifndef QGSLAYERCHOOSERWIDGET_H
#define QGSLAYERCHOOSERWIDGET_H

#include "qgsmaplayer.h"
#include "qgis.h"

#include <QObject>
#include <QFlags>
#include <QComboBox>


/**
  * Handles widget related to selecting a layer from the list of
  * layers available in the project.
  *
  * Subclasses specific to the type of Widget.
  * @see QgsLayerChooserCombo
  */
class GUI_EXPORT QgsLayerChooserWidget : public QObject
{
    Q_OBJECT

  public:
    enum SortMode
    {
      sortByName,
      sortByGroup
    };

    /**
     * @brief The visibility of the layer in the combo depending on the result of the filter
     */
    enum DisplayStatus
    {
      enabled,
      disabled,
      hidden
    };

    /**
     * Filter to know how to display the layer in the widget
     */
    class QgsLayerChooserFilter
    {
      public:
        /**
         * @brief define if the layer should be listed or not.
         * @param layer the layer
         * @return a DisplayStatus (enabled, disabled or hidden)
         */
        virtual DisplayStatus acceptLayer( QgsMapLayer* layer ) { Q_UNUSED( layer ); return enabled; }
    };

    class QgsVectorLayerChooserFilter : public QgsLayerChooserFilter
    {
        virtual DisplayStatus acceptLayer( QgsMapLayer* layer );
    };

    /** constructor */
    QgsLayerChooserWidget( QObject *parent = 0 );

    virtual ~QgsLayerChooserWidget()
    {}

    /**
     * @brief set the filter to be used to determine layers visibility
     * @param filter
     */
    void setFilter( QgsLayerChooserFilter* filter );

    /**
     * @brief get currently selected layer in the widget
     * @return
     */
    virtual QgsMapLayer* getLayer() const = 0;

    /**
     * @brief initialize the widget to show the layers. Must be redefined in subclasses.
     * @param widget
     * @return true if initialization is successful
     */
    virtual bool initWidget( QWidget* widget ) = 0;

    virtual void clearWidget() = 0;
    virtual void addLayer( QgsMapLayer* layer, DisplayStatus display ) = 0;

  signals:
    /**
     * @brief layerChanged is emitted whenever the selected layer in the widget has changed
     */
    void layerChanged( QgsMapLayer* );

  public slots:
    /**
     * @brief set the current layer in the widget
     * @param layerid the layer id
     * @note must be define using keyword "using" in subclass to avoid shadowing
     */
    virtual void setLayer( QString layerid ) = 0;

    /**
     * @brief set the current layer in the widget
     * @param vl the vector layer
     */
    virtual void setLayer( QgsMapLayer* layer ) = 0;

  protected:
    /**
     * @brief populates the widget with layers names
     */
    void populateLayers();

  protected slots:
    void populateLayers( QList<QgsMapLayer*> layerList ) {Q_UNUSED( layerList ); return populateLayers();}
    void populateLayers( QStringList layerList ) {Q_UNUSED( layerList ); return populateLayers();}

  private:
    SortMode mSortMode;
    QgsLayerChooserFilter* mFilter;
};

#endif // QGSLAYERCHOOSERWIDGET_H
