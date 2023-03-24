#ifndef PARCER_H
#define PARCER_H
#include <QtCore>



class Parcer
{
public:
    Parcer(QString filename, QString splitter, QString group, QMap<QString, QStringList> params, int time_ind);

    struct metaDataTitle
    {
        QString rm_number;
        QString start_date;
        QString start_time;
//        QString
    };

    struct ParamVec {
        QVector<double> time;
        QVector<double> data;
        int size;
        double y_min;
        double y_max;
    };

    struct ParamVecStr {
        QVector<double> time;
        QVector<QString> data;
        int size;
    };



    struct ParamXYVec {
        QVector<double> data_x;
        QVector<double> data_y;
        int size;
    };

    struct TitleInd
    {
        int ind;
        QString name;
    };


    QMap<QString, QVector<TitleInd>> m_title = {};

    struct vlineSet{
        QString text;
        QString text_orientation;
        int text_font;
        double text_x;
        double text_y;
        int xAxis;
        int yAxis;
        QString color;
        QString dash;
        int width;
        QVector<double> time;
        QVector<double> data;
    };

    metaDataTitle m_meta;

    QMap<QString, QVector<QPair<double, QVariant>>> m_data;
//    QMap<QString, QVector<ParamMX>> m_data;

    QFile m_file;
    QString m_splitter;
    QString m_group;
    QString m_last_data_string = "";

    bool is_success = true;

    bool is_title = false;
    bool is_title_writen = false;

    bool is_all_title_writen = false;
    bool is_title_found = false;

    QVector<QStringList> m_all_titles;
    QVector<QStringList> m_all_rus_title;

    bool is_last_void = false;

    ParamVec getVectorD(QString key, double move_x, double move_y);
    ParamVecStr getVectorStr(QString key, double move_x);
    ParamXYVec getXYVectorD(QString key_x, QString key_y, double move_x, double move_y);
    QString engTitleToRus(QString eng_name);
    QString rusTitleToEng(QString rus_name);
};

#endif // PARCER_H
