#ifndef MYMAGNIFIER_H
#define MYMAGNIFIER_H

#include "qwt_plot_magnifier.h"
#include "qwt_interval.h"

class MyMagnifier : public QwtPlotMagnifier
{
    Q_OBJECT
public:
    MyMagnifier(QWidget *canvas);

    void setScaleRange(double min, double max);

    virtual void rescale(double factor) override;
    virtual void widgetMousePressEvent(QMouseEvent *event) override;
    double getScale();
    void resetScale();

private:
    QwtInterval m_scaleLimit;
    double m_scale = 1;
    QMap<int, bool> m_rescale;

};

#endif // MYMAGNIFIER_H
