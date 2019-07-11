#ifndef HEATMAPPLOT_H
#define HEATMAPPLOT_H


#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>

class HeatMapPlot: public QwtPlot
{
    Q_OBJECT

public:
    enum ColorMap
    {
        RGBMap,
        IndexMap,
        HueMap,
        AlphaMap
    };

    HeatMapPlot( QWidget * = NULL );

public Q_SLOTS:
    void showContour( bool on );
    void showSpectrogram( bool on );
    void setColorMap( int );
    void setAlpha( int );

#ifndef QT_NO_PRINTER
    void printPlot();
#endif

private:
    QwtPlotSpectrogram *d_spectrogram;

    int d_mapType;
    int d_alpha;
};

#endif // HEATMAPPLOT_H
