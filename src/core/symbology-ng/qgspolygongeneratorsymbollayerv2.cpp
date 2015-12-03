#include "qgspolygongeneratorsymbollayerv2.h"
#include "qgsgeometry.h"

QgsSymbolLayerV2* QgsPolygonGeneratorSymbolLayerV2::create( const QgsStringMap& properties )
{
  return new QgsPolygonGeneratorSymbolLayerV2( QgsFillSymbolV2::createSimple( properties ), properties );
}

QgsPolygonGeneratorSymbolLayerV2::QgsPolygonGeneratorSymbolLayerV2( QgsFillSymbolV2* symbol, const QgsStringMap& properties )
    : QgsFillSymbolLayerV2( QgsSymbolV2::Fill )
    , mExpression( new QgsExpression( properties.value( "geometryModifier" ) ) )
    , mSymbol( symbol )
{
}

QString QgsPolygonGeneratorSymbolLayerV2::layerType() const
{
  return "PolygonGenerator";
}

void QgsPolygonGeneratorSymbolLayerV2::startRender( QgsSymbolV2RenderContext& context )
{
  mExpression->prepare( &context.expressionContext() );

  subSymbol()->startRender( context.renderContext() );
}

void QgsPolygonGeneratorSymbolLayerV2::stopRender( QgsSymbolV2RenderContext& context )
{
  if ( mSymbol )
    mSymbol->stopRender( context.renderContext() );
}

QgsSymbolLayerV2* QgsPolygonGeneratorSymbolLayerV2::clone() const
{
  QgsPolygonGeneratorSymbolLayerV2* clone = new QgsPolygonGeneratorSymbolLayerV2( mSymbol->clone() );
  clone->mExpression.reset( new QgsExpression( mExpression->expression() ) );

  return clone;
}

QgsStringMap QgsPolygonGeneratorSymbolLayerV2::properties() const
{
  QgsStringMap props;
  props.insert( "geometryModifier" , mExpression->expression() );
  return props;
}

void QgsPolygonGeneratorSymbolLayerV2::drawPreviewIcon( QgsSymbolV2RenderContext& context, QSize size )
{
  if ( mSymbol )
    mSymbol->drawPreviewIcon( context.renderContext().painter(), size );
}

void QgsPolygonGeneratorSymbolLayerV2::setGeometryModifier( const QString& exp )
{
  mExpression.reset( new QgsExpression( exp ) );
}

bool QgsPolygonGeneratorSymbolLayerV2::setSubSymbol( QgsSymbolV2* symbol )
{
  mSymbol = dynamic_cast<QgsFillSymbolV2*>( symbol );
  return true;
}

QSet<QString> QgsPolygonGeneratorSymbolLayerV2::usedAttributes() const
{
  return mSymbol->usedAttributes() + mExpression->referencedColumns().toSet();
}

bool QgsPolygonGeneratorSymbolLayerV2::isCompatibleWithSymbol( QgsSymbolV2* symbol )
{
  Q_UNUSED( symbol )
  return true;
}

void QgsPolygonGeneratorSymbolLayerV2::renderPolygon( const QPolygonF& points, QList<QPolygonF>* rings, QgsSymbolV2RenderContext& context )
{
  Q_UNUSED( points )
  Q_UNUSED( rings )

  QgsGeometry geom = mExpression->evaluate( &context.renderContext().expressionContext() ).value<QgsGeometry>();
  if ( context.feature() )
  {
    QgsFeature f = *context.feature();
    f.setGeometry( geom );

    mSymbol->renderFeature( f, context.renderContext() );
  }
}


