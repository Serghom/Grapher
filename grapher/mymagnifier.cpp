#include "mymagnifier.h"
#include "QDebug"
#include <QMouseEvent>
#include <qwt_plot.h>
#include <qwt_scale_widget.h>

MyMagnifier::MyMagnifier( QWidget *canvas ):
    QwtPlotMagnifier(canvas)
{
    for( int axisId = 0; axisId < QwtPlot::axisCnt; ++axisId)
    {
        m_rescale[axisId] = false;
        QwtScaleWidget *scaleWidget = plot()->axisWidget(axisId);
        if(scaleWidget)
        {
            connect(scaleWidget, &QwtScaleWidget::scaleDivChanged, [this, axisId]() {
                if(m_rescale[axisId])
                {
                    m_rescale[axisId] = false;
                }
                else
                {
//                    m_scale = 1;
                }
            });
        }
    }
}


void MyMagnifier::resetScale()
{
    m_scale = 1;
}


double MyMagnifier::getScale()
{
    return m_scale;
}

//void MyMagnifier::rescale(double factor)
//{
//        QwtPlotMagnifier::rescale(1/factor);
//}


void MyMagnifier::setScaleRange(double min, double max)
{
    m_scaleLimit.setInterval(min, max);
}


void MyMagnifier::widgetMousePressEvent(QMouseEvent *event)
{
    Qt::MouseButton button;
    Qt::KeyboardModifiers modifiers;
    getMouseButton(button, modifiers);
    if(event->button() == button && event->modifiers() == modifiers)
    {
        m_scale = 1;
    }
}

void MyMagnifier::rescale(double factor)
{
    double scale = m_scale / factor;
//    qDebug() << "scale" << scale << "factor" << factor << "m_scale" << m_scale;
    if(m_scaleLimit.isValid())
    {
        if(m_scaleLimit.contains(scale))
        {
            for( int axisId = 0; axisId < QwtPlot::axisCnt; ++axisId)
            {
                m_rescale[axisId] = true;
            }
            m_scale = scale;
            QwtPlotMagnifier::rescale(1/factor);
        }
    }
    else
    {
        QwtPlotMagnifier::rescale(1/factor);
    }
}
