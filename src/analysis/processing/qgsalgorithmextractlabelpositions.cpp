/***************************************************************************
                         qgsalgorithmextractlabelpositions.cpp
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

#include "qgsalgorithmextractlabelpositions.h"
#include "qgsexpressioncontextutils.h"
#include "qgslabelingengine.h"
#include "qgsmapthemecollection.h"
#include "qgsmaprendererparalleljob.h"
#include "labelposition.h"

#include "qgsalgorithmextractlabelpositions_p.h"

#include "qgsmaplayerrenderer.h"
#include <QtConcurrent>

QgsExtractLabelPositionsAlgorithm::QgsExtractLabelPositionsAlgorithm()
  : QgsProcessingAlgorithm()
{

}

QString QgsExtractLabelPositionsAlgorithm::name() const
{
  return QStringLiteral( "extractlabelpositions" );
}

QString QgsExtractLabelPositionsAlgorithm::displayName() const
{
  return QObject::tr( "Extract label positions" );
}

QStringList QgsExtractLabelPositionsAlgorithm::tags() const
{
  return QObject::tr( "extract,create,add,label,positions" ).split( ',' );
}

QString QgsExtractLabelPositionsAlgorithm::group() const
{
  return QObject::tr( "Vector general" );
}

QString QgsExtractLabelPositionsAlgorithm::groupId() const
{
  return QStringLiteral( "vectorgeneral" );
}

QString QgsExtractLabelPositionsAlgorithm::shortHelpString() const
{
  return QObject::tr( "" ); // TODO
}

void QgsExtractLabelPositionsAlgorithm::initAlgorithm( const QVariantMap & )
{
  addParameter( new QgsProcessingParameterFeatureSink( QStringLiteral( "LABELS" ), QObject::tr( "Labels" ), QgsProcessing::TypeVectorPoint, QVariant(), true, true ) );
  addParameter( new QgsProcessingParameterMapTheme( QStringLiteral( "MAP_THEME" ), QObject::tr( "Map theme to use" ), QVariant(), true ) );
}

QgsExtractLabelPositionsAlgorithm *QgsExtractLabelPositionsAlgorithm::createInstance() const
{
  return new QgsExtractLabelPositionsAlgorithm();
}

void QgsExtractLabelPositionsAlgorithm::drawLabel( const QString &layerId, QgsRenderContext &context, pal::LabelPosition *label, const QgsPalLayerSettings &settings )
{
  Q_ASSERT( mSink );
  QgsFeature feature( mFields );
  feature.setAttribute( "X", label->getX() );
  feature.setAttribute( "Y", label->getY() );
  mSink->addFeature( feature );
}

bool QgsExtractLabelPositionsAlgorithm::prepareAlgorithm( const QVariantMap &parameters, QgsProcessingContext &context, QgsProcessingFeedback *feedback )
{
  QList<QgsMapLayer *> layers;
  const QString mapTheme { parameterAsString( parameters, QStringLiteral( "MAP_THEME" ), context ) };
  if ( ! mapTheme.isEmpty() && context.project()->mapThemeCollection()->hasMapTheme( mapTheme ) )
  {
    layers = context.project()->mapThemeCollection()->mapThemeVisibleLayers( mapTheme );
    QMap<QString, QString>themeOverrides = context.project()->mapThemeCollection( )->mapThemeStyleOverrides( mapTheme );
  }
#if 0
  // Still no layers? Get them all from the project
  if ( layers.size() == 0 )
  {
    const auto constLayers { context.project()->mapLayers().values() };
    for ( const QgsMapLayer *ml : constLayers )
    {
      mMapLayers.push_back( std::unique_ptr<QgsMapLayer>( ml->clone( ) ) );
    }
  }
#endif

  mMapSettings.setLayers( layers );

#if 0
  mRenderContext = QgsRenderContext();
  mRenderContext.setRendererScale( 1000 ); // TODO parameter
  mRenderContext.setExtent( layers.at( 0 )->extent() ); // TODO parameter

  mRenderContext.setScaleFactor( 96.0 / 25.4 ); // Does this matter?

  // TODO: probably get that from the processing context
  mRenderContext.expressionContext().appendScope( QgsExpressionContextUtils::projectScope( context.project() ) );
  mRenderContext.expressionContext().appendScope( QgsExpressionContextUtils::globalScope() );
#endif

  if ( !layers.empty() )
  {
    mMapSettings.setExtent( layers.at( 0 )->extent() );
    feedback->pushInfo( QStringLiteral( "Set extent to %1" ).arg( mMapSettings.extent().toString() ) );
  }

  mLabelingEngine = qgis::make_unique<QgsDefaultLabelingEngine>();
  mLabelingEngine->setMapSettings( mMapSettings );

  mJob = new QgsLabellerJob( mMapSettings );
  mJob->setLabelingEngine( mLabelingEngine.get() );
  mJob->prepare();

  return true;
}

QVariantMap QgsExtractLabelPositionsAlgorithm::processAlgorithm( const QVariantMap &parameters, QgsProcessingContext &context, QgsProcessingFeedback *feedback )
{
  mFields.append( QgsField( "X" ) );
  mFields.append( QgsField( "Y" ) );
  mFields.append( QgsField( "HAlign" ) );
  mFields.append( QgsField( "VAlign" ) );

  QString dest;
  mSink.reset( parameterAsSink( parameters, QStringLiteral( "LABELS" ), context, dest, mFields,
                                QgsWkbTypes::Point,  context.project()->crs(), QgsFeatureSink::RegeneratePrimaryKey ) );
  if ( !mSink )
    throw QgsProcessingException( invalidSinkError( parameters, QStringLiteral( "OUTPUT" ) ) );

  mJob->start();
  mJob->waitForFinished();
  feedback->pushInfo( "Finished" );

  QVariantMap outputs;
  outputs.insert( QStringLiteral( "OUTPUT" ), dest );
  return outputs;
}


QgsLabellerJob::QgsLabellerJob( const QgsMapSettings &settings )
  : QgsMapRendererJob( settings )
  , mStatus( Idle )
{
}

QgsLabellerJob::~QgsLabellerJob()
{
  if ( isActive() )
  {
    cancel();
  }
}

void QgsLabellerJob::prepare()
{
  mLayerJobs = prepareJobs( nullptr, mLabelingEngine );
  mLabelJob = prepareLabelingJob( nullptr, mLabelingEngine );
}

void QgsLabellerJob::start()
{
  if ( isActive() )
    return;

  mRenderingStart.start();

  mStatus = RenderingLayers;

  Q_ASSERT( mLabelingEngine );

  connect( &mFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderLayersFinished );

  mFuture = QtConcurrent::map( mLayerJobs, renderLayerStatic );
  mFutureWatcher.setFuture( mFuture );
}

void QgsLabellerJob::cancel()
{
  if ( !isActive() )
    return;

  mLabelJob.context.setRenderingStopped( true );
  for ( LayerRenderJobs::iterator it = mLayerJobs.begin(); it != mLayerJobs.end(); ++it )
  {
    it->context.setRenderingStopped( true );
    if ( it->renderer && it->renderer->feedback() )
      it->renderer->feedback()->cancel();
  }

  if ( mStatus == RenderingLayers )
  {
    disconnect( &mFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderLayersFinished );

    mFutureWatcher.waitForFinished();

    renderLayersFinished();
  }

  if ( mStatus == RenderingLabels )
  {
    disconnect( &mLabelingFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderingFinished );

    mLabelingFutureWatcher.waitForFinished();

    renderingFinished();
  }

  if ( mStatus == RenderingSecondPass )
  {
    Q_ASSERT( false );
#if 0
    disconnect( &mSecondPassFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderLayersSecondPassFinished );

    mSecondPassFutureWatcher.waitForFinished();

    renderLayersSecondPassFinished();
#endif
  }

  Q_ASSERT( mStatus == Idle );
}

void QgsLabellerJob::cancelWithoutBlocking()
{
  if ( !isActive() )
    return;

  QgsDebugMsgLevel( QStringLiteral( "PARALLEL cancel at status %1" ).arg( mStatus ), 2 );

  mLabelJob.context.setRenderingStopped( true );
  for ( LayerRenderJobs::iterator it = mLayerJobs.begin(); it != mLayerJobs.end(); ++it )
  {
    it->context.setRenderingStopped( true );
    if ( it->renderer && it->renderer->feedback() )
      it->renderer->feedback()->cancel();
  }

  if ( mStatus == RenderingLayers )
  {
    disconnect( &mFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderLayersFinished );
    connect( &mFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderingFinished );
  }
}

void QgsLabellerJob::waitForFinished()
{
  if ( !isActive() )
    return;

  if ( mStatus == RenderingLayers )
  {
    disconnect( &mFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderLayersFinished );

    QElapsedTimer t;
    t.start();

    mFutureWatcher.waitForFinished();

    renderLayersFinished();
  }

  if ( mStatus == RenderingSecondPass )
  {
    disconnect( &mSecondPassFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderLayersSecondPassFinished );

    QElapsedTimer t;
    t.start();

    mSecondPassFutureWatcher.waitForFinished();
    renderLayersSecondPassFinished();
  }

  if ( mStatus == RenderingLabels )
  {
    disconnect( &mLabelingFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderingFinished );

    QElapsedTimer t;
    t.start();

    mLabelingFutureWatcher.waitForFinished();

    renderingFinished();
  }

  Q_ASSERT( mStatus == Idle );
}

bool QgsLabellerJob::isActive() const
{
  return mStatus != Idle;
}

bool QgsLabellerJob::usedCachedLabels() const
{
  return mLabelJob.cached;
}

QgsLabelingResults *QgsLabellerJob::takeLabelingResults()
{
  if ( mLabelingEngine )
    return mLabelingEngine->takeResults();
  else
    return nullptr;
}

void QgsLabellerJob::renderLayersFinished()
{
  Q_ASSERT( mStatus == RenderingLayers );

  for ( const LayerRenderJob &job : qgis::as_const( mLayerJobs ) )
  {
    if ( !job.errors.isEmpty() )
    {
      mErrors.append( Error( job.layer->id(), job.errors.join( ',' ) ) );
    }
  }

  if ( mSettings.testFlag( QgsMapSettings::DrawLabeling ) && !mLabelJob.context.renderingStopped() )
  {
    mStatus = RenderingLabels;

    connect( &mLabelingFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderingFinished );

    // now start rendering of labeling!
    mLabelingFuture = QtConcurrent::run( renderLabelsStatic, this );
    mLabelingFutureWatcher.setFuture( mLabelingFuture );
    emit renderingLayersFinished();
  }
  else
  {
    renderingFinished();
  }
}

void QgsLabellerJob::renderingFinished()
{
  if ( ! mSecondPassLayerJobs.isEmpty() )
  {
    mStatus = RenderingSecondPass;
    // We have a second pass to do.
    mSecondPassFuture = QtConcurrent::map( mSecondPassLayerJobs, renderLayerStatic );
    mSecondPassFutureWatcher.setFuture( mSecondPassFuture );
    connect( &mSecondPassFutureWatcher, &QFutureWatcher<void>::finished, this, &QgsLabellerJob::renderLayersSecondPassFinished );
  }
  else
  {
    cleanupJobs( mLayerJobs );

    cleanupLabelJob( mLabelJob );

    mStatus = Idle;

    mRenderingTime = mRenderingStart.elapsed();

    emit finished();
  }
}

void QgsLabellerJob::renderLayersSecondPassFinished()
{
  // compose second pass images into first pass images
  composeSecondPass( mSecondPassLayerJobs, mLabelJob );

  cleanupJobs( mLayerJobs );

  cleanupSecondPassJobs( mSecondPassLayerJobs );

  cleanupLabelJob( mLabelJob );

  mStatus = Idle;

  mRenderingTime = mRenderingStart.elapsed();

  emit finished();
}

void QgsLabellerJob::renderLayerStatic( LayerRenderJob &job )
{
  if ( job.context.renderingStopped() )
    return;

  if ( job.cached )
    return;

  if ( job.img )
  {
    job.img->fill( 0 );
    job.imageInitialized = true;
  }

  QElapsedTimer t;
  t.start();
  try
  {
    job.renderer->render();
  }
  catch ( QgsException &e )
  {
    Q_UNUSED( e )
    QgsDebugMsg( "Caught unhandled QgsException: " + e.what() );
  }
  catch ( std::exception &e )
  {
    Q_UNUSED( e )
    QgsDebugMsg( "Caught unhandled std::exception: " + QString::fromLatin1( e.what() ) );
  }
  catch ( ... )
  {
    QgsDebugMsg( QStringLiteral( "Caught unhandled unknown exception" ) );
  }

  job.errors = job.renderer->errors();
  job.renderingTime += t.elapsed();
  QgsDebugMsgLevel( QStringLiteral( "job %1 end [%2 ms] (layer %3)" ).arg( reinterpret_cast< quint64 >( &job ), 0, 16 ).arg( job.renderingTime ).arg( job.layerId ), 2 );
}


void QgsLabellerJob::renderLabelsStatic( QgsLabellerJob *self )
{
  LabelRenderJob &job = self->mLabelJob;

  if ( !job.cached )
  {
    QElapsedTimer labelTime;
    labelTime.start();

    QPainter painter;

    // draw the labels!
    try
    {
      drawLabeling( job.context, self->mLabelingEngine, &painter );
    }
    catch ( QgsException &e )
    {
      Q_UNUSED( e )
      QgsDebugMsg( "Caught unhandled QgsException: " + e.what() );
    }
    catch ( std::exception &e )
    {
      Q_UNUSED( e )
      QgsDebugMsg( "Caught unhandled std::exception: " + QString::fromLatin1( e.what() ) );
    }
    catch ( ... )
    {
      QgsDebugMsg( QStringLiteral( "Caught unhandled unknown exception" ) );
    }

    job.renderingTime = labelTime.elapsed();
    job.complete = true;
    // TODO: was this needed?
    // job.participatingLayers = _qgis_listRawToQPointer( self->mLabelingEngine->participatingLayers() );
  }
}

QgsLabelingEngine *QgsLabellerJob::labelingEngine() const
{
  return mLabelingEngine;
}

void QgsLabellerJob::setLabelingEngine( QgsLabelingEngine *labelingEngine )
{
  mLabelingEngine = labelingEngine;
}
