#ifndef QSCALEDRAW_H
#define QSCALEDRAW_H

#include <qwt_scale_draw.h>

class QScaleDraw : public QwtScaleDraw
{
public:

    explicit QScaleDraw(bool enableScientificNotation = false)
        : m_scientificNotationEnabled(enableScientificNotation)
    {

    }

    virtual QwtText label(double value) const override
    {
        if(m_scientificNotationEnabled)
        {
            return QwtScaleDraw::label(value);
        }
        else
        {
            QString text_value = QString::number(value, 'f', 9);
            QString pastDot = text_value.split(".")[1];
            int last_index = 0;
            for(int i = 8; i >= 0; i--)
            {
                QString ch = pastDot.at(i);
                if(ch == "0")
                {

                }
                else
                {
                    last_index = i+1;
                    break;
                }
            }
//            qDebug() << "text_value" << text_value << "pastDot" << pastDot << last_index;
            QString return_text = QString::number(value, 'f', last_index);
            return QwtText(return_text);
        }
    }

private:
    bool m_scientificNotationEnabled;
};

#endif // QSCALEDRAW_H
