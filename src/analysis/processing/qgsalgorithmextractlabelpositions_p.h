#ifndef QGSEXTRACTLABELRENDERER_P_H
#define QGSEXTRACTLABELRENDERER_P_H

#include "qgsmaprendererjob.h"

/**
 * \ingroup core
 * Job implementation that renders all layers in parallel.
 *
 * The resulting map image can be retrieved with renderedImage() function.
 * It is safe to call that function while rendering is active to see preview of the map.
 *
 * \since QGIS 2.4
 */

class CORE_EXPORT QgsLabellerJob : public QgsMapRendererJob
{
    Q_OBJECT
  public:
    QgsLabellerJob( const QgsMapSettings &settings );
    ~QgsLabellerJob() override;

    void prepare();
    void start() override;
    void cancel() override;
    void cancelWithoutBlocking() override;
    void waitForFinished() override;
    bool isActive() const override;

    bool usedCachedLabels() const override;
    QgsLabelingResults *takeLabelingResults() SIP_TRANSFER override;

    QgsLabelingEngine *labelingEngine() const;
    void setLabelingEngine( QgsLabelingEngine *labelingEngine );

  private slots:
    //! layers are rendered, labeling is still pending
    void renderLayersFinished();
    //! the second pass of layer rendering is finished
    void renderLayersSecondPassFinished();
    //! all rendering is finished, including labeling
    void renderingFinished();

  private:

    //! \note not available in Python bindings
    static void renderLayerStatic( LayerRenderJob &job ) SIP_SKIP;
    //! \note not available in Python bindings
    static void renderLabelsStatic( QgsLabellerJob *self ) SIP_SKIP;

    //! \note not available in Python bindings
    enum { Idle, RenderingLayers, RenderingSecondPass, RenderingLabels } mStatus SIP_SKIP;

    QFuture<void> mFuture;
    QFutureWatcher<void> mFutureWatcher;

    LayerRenderJobs mLayerJobs;
    LabelRenderJob mLabelJob;

    LayerRenderJobs mSecondPassLayerJobs;
    QFuture<void> mSecondPassFuture;
    QFutureWatcher<void> mSecondPassFutureWatcher;

    QgsLabelingEngine *mLabelingEngine = nullptr;
    QFuture<void> mLabelingFuture;
    QFutureWatcher<void> mLabelingFutureWatcher;
};


#endif // QGSEXTRACTLABELRENDERER_P_H
