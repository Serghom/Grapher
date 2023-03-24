#ifndef PAINTER_H
#define PAINTER_H

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QtSvg/QSvgGenerator>
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QGraphicsSvgItem>
#include <QTextCodec>

#include <qwt_series_data.h>
#include <qwt_text.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_legenditem.h>
#include <qwt_curve_fitter.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_round_scale_draw.h>
#include <qwt_scale_engine.h>

#include <memory>

#include <qscaledraw.h>
#include <parcer.h>

#define Q_OS_WIN32
class Painter
{
    public:

        struct ParamPainter {
            QString source;
            bool isSource;

            QString jsonFile;
            bool isJsonFile;

            QString targetPath;
            bool isTargetPath;

            bool is_jFol;
            bool is_interactive;

            QString m_defaultJsonFolder;
            QString m_userJsonFolder;

            QString settingsFile;
        };

        struct paramToTable
        {
            QString group;
            QVector<QString> params;
        };

        struct paramCurve
        {

            QString color_line;
            QString dash_line;
            QString name_line;
            QString param_x;
            QString param_y;
            QString param_line;

            bool is_dots;
            bool is_second_y;
            bool is_Border;
            int dots_size;
            int width_line;
            QString dots_style;
        };

        struct paramVline
        {
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
            double point_x;
            double y_min;
            double y_max;

        };

        struct paramGraph
        {
            QString main_title;
            QString x_title;
            QString y_title;
            QString second_y_title;

            QString pos_legend;

            double x_min_scale;
            double x_max_scale;
            double y_min_scale;
            double y_max_scale;
            double second_y_min_scale;
            double second_y_max_scale;

            double move_x;
            double move_y;
            double move_second_y;

            QString nameTargetFile;
            QString namefile;

            QString namefile_readed_from_folder;

            bool is_a_new_graph = false;
            bool is_from_json = false;
            bool is_a_not_save_edited = false;

//            QMap<QString, QVector<paramCurve>> map_params_curve;
            QVector<QPair<QString, paramCurve>> line_params_curve;
            QVector<paramVline> vlines;
        };


        QJsonArray m_jsonArr;
        QJsonArray m_defaultJsonArr;
        QJsonArray m_userJsonArr;

        QVector<QJsonArray> m_all_jsons;


        struct painter_color
        {
            QString color;
            QString color_rus;
            Qt::GlobalColor qt_color;
        };

        struct painter_orientation
        {
            QString orientation;
            QString orientation_rus;
            Qt::Orientation qt_orientation;
        };

        struct painter_penStyle
        {
            QString penStyle;
            QString penStyle_rus;
            Qt::PenStyle qt_penStyle;
        };

        struct painter_style_symbols
        {
            QString style_symbols;
            QString style_symbols_rus;
            QwtSymbol::Style qwt_style_symbols;
        };

        struct painter_position
        {
            QString position;
            QString position_rus;
            Qt::AlignmentFlag qt_position_first;
            Qt::AlignmentFlag qt_position_second;
            QString path_toIcon;
            QString path_toIconSVG;
        };

        struct sizeVlines
        {
            double y_max;
            double y_min;
        };

        QMap<QString, painter_color> m_map_colors;
        QMap<QString, painter_orientation> m_map_orientation;
        QMap<QString, painter_penStyle> m_map_penStyle;
        QMap<QString, painter_style_symbols> m_map_style_symbols;
        QMap<QString, painter_position> m_map_position;

        QStringList m_colors = {"blue", "red", "yellow", "green", "darkCyan", "black",  "darkGray", "gray", "lightGray",
                             "cyan", "magenta", "darkYellow", "darkRed", "darkGreen", "darkBlue",  "darkMagenta"};

//        QStringList m_text_orientation = {"Horizontal", "Vertical"};

//        QStringList m_penStyle = {"SolidLine", "DashLine", "DotLine", "DashDotLine", "DashDotDotLine"};

//        QStringList m_style_symbols = {"Ellipse", "Rect", "Diamond", "Triangle", "DTriangle", "UTriangle",
//                                     "LTriangle", "RTriangle", "Cross", "XCross", "HLine", "VLine", "Star1", "Star2", "Hexagon"};

//        QStringList m_pos = {"top_left", "top_right", "top_center", "bottom_left", "bottom_right", "bottom_center", "left_center", "right_center"};

        bool m_is_second_y = false;
        bool m_is_at_time = true;

        bool m_is_write_water_mark = false;


         Painter(QApplication &app, ParamPainter settings);
        ~Painter();


        bool readJsonFile();
        bool readInteractiveJson();
        bool assertPaint(int graph);
        bool assertPaint(int graph, bool isDefault);
        void readJsonGraphToVectorStruct();
        void readCurrentGraphFromJson(int index_vector, int index_json);

        void output(QwtPlot *plot, QString targetPath);
        void setCurvePen(QwtPlotCurve *curve, QString color_line, QString style, int width);
        void setPenColor(QPen *pen, QString color_line);
        void setLegendPos(QwtPlotLegendItem *legendItem, QString pos_legend);
        void parceJson();
        void parceSource();
        void processPaint();
        void finalPaint();
        void setParamGraph(bool is_at_time);
        void setParamGraph(int graph);
        void setParamGraph(int graph, bool isDefault);
        void structToJson(int graph, bool isDefault, bool toSave, QString path_to_save = "");
        void delete_json_file(int graph, int index_vector, QString path_to_file);
        void setSettingsBackground(QwtPlot &plot, QwtPlotGrid &grid, bool isInteractive);
        void setFontSize(bool isInteractive);
        void setPointSymbol(QwtSymbol *symb, QString color_line, int dots_size, QString dots_style, bool isBorder);
        void drawLinesInGraph(QwtPlot &plot, QVector<Parcer::vlineSet> &vlines, QVector<QwtPlotCurve*> &lines);
        void paint_graph(QwtPlot &plot, int indTab, int indCurve, QVector<Parcer::vlineSet> &vlines, QVector<QwtPlotCurve*> &lines);
        void drawVerticalLines(QwtPlot &plot, QVector<Parcer::vlineSet> &vlines, QVector<QwtPlotCurve*> &lines, QVector<QwtPlotMarker*> &markers);
        void add_line_param(QString table_group, QString param_name, QString name_of_legend, QString color, int index_vector, int graph);
        void add_line_param_x_y(QString table_group, QString param_name, QString color, int index_vector, int graph);
        void delete_line_param(QString table_group, QString param_name, int index_vector, int graph);
        void delete_line_param_x_y(QString table_group, QString x, QString y, int index_vector, int graph);
        void delete_all_param_at_group(QString table_group, int index_vector, int graph);
        void add_vertical_line(int index_vector, int graph);
        void delete_vertical_line(int index_vector, int graph, int index_vline);

        void setFlagWaterMark(bool wm);



        QVector<Parcer::vlineSet> setVLinesGraph(int graph);
        QVector<Parcer::vlineSet> setVLinesGraph(int graph, bool isDefault);

        ParamPainter m_settings;
        QVector<Parcer*> m_tables;
        QStringList m_allGroupFromTables;
        paramGraph m_param_graph;
        QVector<paramGraph> m_default_vector_param_graph;
        QVector<paramGraph> m_user_vector_param_graph;

        QVector<QVector<paramGraph>> m_vector_of_parameters;


//        #ifdef Q_OS_WIN32
//            QString utf8tolocal(const QString &text);
//            QString localtoutf8(const QString &text);
//        #endif



private:

    sizeVlines m_sizeVlines;

    QJsonArray m_lineArr;
    QMap<QString, QStringList> m_params;


    QwtScaleDraw *m_secondYRota = nullptr;
    QApplication *m_app = nullptr;

    QFont m_fontTitle;
    QFont m_fontLegend;
    QFont m_fontAxis;



};

#endif // PAINTER_H

