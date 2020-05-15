/***************************************************************************
                         qgsalgorithmextractlabelpositions.h
                         ------------------------
    begin                : May 2020
    copyright            : (C) 2020 by Matthias Kuhn
    email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSEXTRACTLABELPOSITIONSALGORITHM_H
#define QGSEXTRACTLABELPOSITIONSALGORITHM_H

#include "qgsprocessingalgorithm.h"
#include "qgslabelsink.h"
#include "qgsrendercontext.h"

class QgsLabellerJob;

class QgsExtractLabelPositionsAlgorithm : public QgsProcessingAlgorithm, public QgsLabelSink
{
  public:
    QgsExtractLabelPositionsAlgorithm();
    QString name() const override;
    QString displayName() const override;
    QStringList tags() const override;
    QString group() const override;
    QString groupId() const override;
    QString shortHelpString() const override;
    void initAlgorithm( const QVariantMap & ) override;
    QgsExtractLabelPositionsAlgorithm *createInstance() const override SIP_FACTORY;

    void drawLabel( const QString &layerId, QgsRenderContext &context, pal::LabelPosition *label, const QgsPalLayerSettings &settings );

  protected:
    bool prepareAlgorithm( const QVariantMap &parameters, QgsProcessingContext &context, QgsProcessingFeedback *feedback ) override;
    QVariantMap processAlgorithm( const QVariantMap &parameters, QgsProcessingContext &context, QgsProcessingFeedback *feedback ) override;

  private:
    QString mXFieldName;
    QString mYFieldName;
    double mScale = std::numeric_limits<double>::signaling_NaN();
    std::unique_ptr<QgsLabelingEngine> mLabelingEngine;
    QgsMapSettings mMapSettings;
    QgsLabellerJob *mJob = nullptr;
    QList<QgsMapLayer *> mLayers;
    std::unique_ptr<QgsFeatureSink> mSink;
    QgsFields mFields;
};

#endif // QGSEXTRACTLABELPOSITIONSALGORITHM_H
