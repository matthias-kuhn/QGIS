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

#ifndef QGSFIELDCHOOSERWIDGET_H
#define QGSFIELDCHOOSERWIDGET_H

#include "qgslayerchooserwidget.h"
#include "qgsvectorlayer.h"

#include <QObject>

class GUI_EXPORT QgsFieldChooserWidget : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief The visibility of the field in the combo depending on the result of the filter
     */
    enum DisplayStatus
    {
      enabled,
      disabled,
      hidden
    };

    class QgsFieldChooserFilter
    {
      public:
        /**
         * @brief define if the field should be listed or not.
         * @param idx the field index
         * @return a DisplayStatus (enabled, disabled or hidden)
         */
        virtual DisplayStatus acceptField( int idx ) { Q_UNUSED( idx ); return enabled; }
    };

    QgsFieldChooserWidget( QgsLayerChooserWidget* layerChooser, QObject *parent = 0 );

    /**
     * @brief set the filter to be used to determine layers visibility
     * @param filter
     */
    void setFilter( QgsFieldChooserFilter* filter );

    virtual bool initWidget( QWidget* ) = 0;
    virtual void clearWidget() = 0;
    virtual void addField( QString fieldAlias, QString fieldName, DisplayStatus display ) = 0;
    virtual void unselect() = 0;
    virtual int getFieldIndex() = 0;
    virtual QString getFieldName() = 0;

 public slots:
    virtual void setField( QString fieldName )=0;

  signals:
    void fieldChanged( int );

  public slots:
    void layerChanged( QgsMapLayer*layer );
    void layerChanged();

  protected slots:
    void layerDeleted();

  protected:
    QgsLayerChooserWidget* mLayerChooser;
    QgsFieldChooserFilter* mFilter;
    QgsVectorLayer* mLayer;
};

#endif // QGSFIELDCHOOSERWIDGET_H
