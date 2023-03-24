#ifndef GRAPHERGUI_H
#define GRAPHERGUI_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QRegExp>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QShortcut>
#include <QGroupBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QMenu>
#include <QToolButton>
#include <QDomDocument>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>


#include <qwt_plot.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

#include <painter.h>
#include <mymagnifier.h>
#include <myqframe.h>


#define WINDOW_ID "return_Window_id"


namespace Ui {
class GrapherGui;
}

class GrapherGui : public QMainWindow
{
    Q_OBJECT

public:
//    GrapherGui();
    explicit GrapherGui(QApplication &app, Painter::ParamPainter settings, QWidget *parent = 0);
    ~GrapherGui();



    struct graph_meta_data
    {
        bool isDefault;
        int index_vector;
    };

    struct linesItemData
    {
        int index;
        QString key;
    };

    QApplication *m_app = nullptr;

    void setBackGround(int index, bool isDefault);
    void setCurve(QwtPlotCurve *curve);
    void moveAndDrag();
    void setStatusBar();

    void setSetingsFile(bool isVisible);
    void setViewMode(bool isVisible);
    void setScaleVisible(bool isVisible);
    void readSettingsJsonfile(QString pathSettings = "./settings.json");
    void readAssociateFile();
    void filesApply();
    void setPaint(int index, bool isDefault);
    void setPlot();
    void saveGraph(int index);
    void setLineEditData();
    void setVisibleSecondY(bool isVisible);
    void setScaleView();
    void setItemListGraph();
    void setZoomer();
    void setKeyVisible(bool isVisible);
    void setDeleteMode(bool isVisible);
    void getEnabledKeyAdd();
    void setJsonAssociateJson();
    void setJsonSettings();
    void setAttributeGraph();
    void setLegendIcon(QIcon *icon_legend, QString position);
    void createSettingsAttr(int index, bool isAtribute);
    void setNewPositionWidget();
    void setMenuToAddButton();
    QVBoxLayout* createLayoutsAttr(int index);
    QString get_next_color_from_iter();
    QString get_prev_color_from_iter();
    graph_meta_data getMetaDataVector();
    void setPriorityLine(QVector<linesItemData> new_priority);
    void setAxisData(QwtInterval x, QwtInterval y, QwtInterval y2);
    bool getValueIsMarkersInAllGraph();
    void setAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);
    void changeSVGColor(QIcon *icon, QString path_to_icon, QString color, QString border_color, int size_border);
    void styleDialog(QWidget *parent, QMessageBox *msgBox);
    void setSpacerHiden(bool isVisible);
    void setColorIconsToolBar(QString color, QString border_color, int size_border);
    void setColorIconsContexMenu(QString color, QString border_color, int size_border);
    void setColorIconsButton(QString color, QString border_color, int size_border);


private Q_SLOTS:
    void click_on_canvas(const QPoint &pos);

    void click_on_action_graph();
    void click_on_action_newGraph_atTime();
    void click_on_action_newGraph_atParam();
    void click_on_action_files();
    void click_on_action_save();
    void click_on_action_addKey();
    void click_on_action_view();
    void click_on_action_editing();
    void click_on_action_param();
    void click_on_action_add_vertical_line();
    void click_on_action_x_y_line();
    void click_on_action_save_this_graph();
    void click_on_action_delete_this_attribute();
    void click_on_action_reset_edited();
    void click_on_action_delete_this_graph();

    void click_on_button_editGraph();
    void click_on_button_createGraph();
    void click_on_button_delteGraph();
    void click_on_button_delete();
    void click_on_button_addAttr();
    void click_on_button_addFile();
    void click_on_button_deleteFile();
    void click_on_button_defaultScale();
    void click_on_button_key_add();
    void click_on_button_key_delete();
    void click_on_button_view();
    void click_on_button_edit_legend_reset();
    void click_on_button_edit_header_reset();
    void click_on_button_edit_x_title_reset();
    void click_on_button_edit_y_title_reset();
    void click_on_button_edit_second_y_title_reset();
    void click_on_button_edit_apply_header();
    void click_on_button_edit_apply_x_title();
    void click_on_button_edit_apply_y_title();
    void click_on_button_edit_apply_second_y_title();
    void click_on_button_edit_close();
    void click_on_button_edit_vline_color();
    void click_on_button_edit_line_color();
    void click_on_button_save_pattern();
    void click_on_button_delete_mode();

    void selectItemGraph(int index);
    void selectItemGraph(QListWidgetItem *item);
    void selectItemAttribute(int index);
    void selectItemLines(int index);
    void selectItemLines(QListWidgetItem *item);
    void clickItemAttribute(QListWidgetItem *item);
    void selectItemKey(int index);

    void click_on_radioButton_default();
    void click_on_radioButton_users();

    void togled_checkBox_scale(bool isCheked);
    void togled_checkBox_isMarkers_all_graph(bool isCheked);
    void togled_checkBox_item_list_attr(QListWidgetItem *item);
    void listGraphContextMenu(QPoint pos);
    void listLinesContextyMenu(QPoint pos);
    void listAttributeContextyMenu(QPoint pos);
    void double_click_item_setCheckBox(QListWidgetItem *item);
    void checked_edit_vline_auto_y(int state);
    void checked_edit_line_isVisible_line(int state);
    void checked_edit_line_isVisible_markers(int state);
    void checked_edit_line_isBorder(int state);
    void checked_edit_line_isSecond_y(int state);

    void editingLineXMax(const QString &str);
    void editingLineXMin(const QString &str);

    void editingLineYMax(const QString &str);
    void editingLineYMin(const QString &str);

    void editingLineSecondYMax(const QString &str);
    void editingLineSecondYMin(const QString &str);

    void editingLineKey(const QString &str);
    void editingLineKeyName(const QString &str);
    void editingLineKeyReg(const QString &str);

    void editingLine_edit_main_title(const QString &str);
    void editingLine_edit_x_title(const QString &str);
    void editingLine_edit_y_title(const QString &str);
    void editingLine_edit_second_y_title(const QString &str);

    void editingLine_edit_vline_x_point(const QString &str);
    void editingLine_edit_vline_y_max(const QString &str);
    void editingLine_edit_vline_y_min(const QString &str);
    void editingLine_edit_vline_text(const QString &str);
    void editingLine_edit_vline_text_x(const QString &str);
    void editingLine_edit_vline_text_y(const QString &str);

    void editingLine_edit_line_name_legend(const QString &str);

    void zoomerPlot(const QRectF &rect);

    void shortCutEscape();
    void shortCutEnter();
    void shortCutCtrlUp();
    void shortCutCtrlDown();

    void selectPosLegend(int index);
    void selectVlineColor(int select);
    void selectVlineDash(int select);
    void select_text_orientation(int index);
    void selectLineXparam(int select);
    void selectLineYparam(int select);

    void selectLineColor(int select);
    void selectLineDash(int select);
    void selectLineStyleDot(int select);

    void spinBox_changed_vline_sizeFont(int size);
    void spinBox_changed_vline_width(int width);

    void spinBox_changed_line_width(int width);
    void spinBox_changed_line_width_dot(int width);

    void selectEditMenuItemHovered(QAction *action);




private:
    Ui::GrapherGui *ui;
    bool eventFilter(QObject *object, QEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent( QResizeEvent* );

    QwtPlot *m_plot = nullptr;
    QwtPlotGrid *m_grid = nullptr;
    QHBoxLayout *m_layoutPlot = nullptr;
    QwtPlotPanner *d_panner = nullptr;
    MyMagnifier *m_magnifier = nullptr;
    QShortcut *m_key_escape = nullptr;
    QShortcut *m_key_enter = nullptr;
    QShortcut *m_key_ctrl_up = nullptr;
    QShortcut *m_key_ctrl_down = nullptr;

//    QWidget *m_windowSettingsGraph = nullptr;
    QFrame *m_windowSettingsGraph = nullptr;

    QToolButton *m_toolButton_new_graph = nullptr;

//    QSpinBox *m_spinBoxWidth_vline;


    struct contexMenuColorElements
    {
        QString action_new_param_line;
        QString action_new_vertical_line;

        QString action_add_line_graph_lines;
        QString action_delete_lines;

//        QString action_add_line_graph_lines;
//        QString action_delete_lines;

        QString action_editing;
        QString action_save_this_graph;

    };

    struct axisData
    {
        double width;
        double height;
        double S_rect;
    };

    struct colorSvg
    {
        QString color;
        QString border_color;
        int size_border;
    };

    struct settingsGrapherGui
    {
        QString pathSettings;
        QString fileFilter;
        QString pathToAssociateFile;
        QString pathToDefaultJsonFile;
        QString pathToUserJsonFile;
        QString startPathFindFile;
        QString pathToTargetFile;
        bool embeded;
        bool compareRmNumber;
        bool compareStartDate;
        bool userIsEnable;
        bool styleSheet;
        bool useRusTitle;

        colorSvg colorIconsAttributeEdit;
        colorSvg colorIconsButton;
        colorSvg colorIconsContextMenu;
        colorSvg colorIconsToolBar;

        colorSvg colorIconsPolyline;
        colorSvg colorIconsPolylineWithStyleSheets;
    };

    struct metaDataFile
    {
        QString path;
        QString name;
        QString key;
    };

    struct metaDataFileMaxSize
    {
        int path_size_max = 0;
        int name_size_max = 0;
        int key_size_max = 0;
    };

    struct keyRegName
    {
        QString key;
        QString name;
        QString reg;
    };


    struct keyRegNameMaxSize
    {
        int key_size_max = 0;
        int name_size_max = 0;
        int reg_size_max = 0;
    };

    struct settingsGraph
    {
        QString pos_legend;
        QString x_title;
        QString y_title;
        QString y2_title;
        QString header;
    };

    struct metaDataVline
    {
        int index_vline;

        QComboBox *comboColor;
        QComboBox *comboDash;
        QSpinBox *spinBoxWidth;
        QLineEdit *edit_point_x;
        QLineEdit *edit_y_min;
        QLineEdit *edit_y_max;
        QCheckBox *check_y;

        QLineEdit *textEdit;
        QComboBox *comboOri;
        QSpinBox *spinBoxFontSize;
        QLineEdit *text_x;
        QLineEdit *text_y;

        bool isEmpty = true;
    };

    struct metaDataLine
    {
        QString name;
        QString key;

        QStringList all_parameters;

        QLineEdit *lineEditCurveTitle;
        QComboBox *comboColor;
        QPushButton *buttonColor;
        QComboBox *comboParam_x;
        QComboBox *comboParam_y;
        QCheckBox *checkBoxisSecondY;

        QCheckBox *checkBoxisLine;
        QComboBox *comboDash;
        QSpinBox *spinBoxWidth;

        QCheckBox *checkBoxisMarkers;
        QCheckBox *checkBoxisBorder;
        QComboBox *comboStyleDot;
        QSpinBox *spinBoxWidthDot;

        QIcon *icon_curve;

        bool isEmpty = true;
    };


    struct layoutItem
    {
        QVBoxLayout* lay;
        QSize attr_size;
        QString key;
        int index;
        int size_vlines = 0;

        QComboBox *comboPosLegend;

        QLineEdit *edit_title;
        QLineEdit *edit_x;
        QLineEdit *edit_y;
        QLineEdit *edit_y2;

        //vline
        metaDataVline vLine;
        metaDataLine line;

    };




//    QVector<QString> m_fileNameVector;
    QVector<metaDataFile> m_fileMetaDataVector;

    Painter m_painter;
    settingsGrapherGui m_settings;

    QMenu *m_menu_param = nullptr;
    QMenu *m_menu_listAttribute = nullptr;
    QMenu *m_menu_listLines = nullptr;
    QMenu *m_menu_listGraph = nullptr;

    settingsGraph m_startSettGraph;
    settingsGraph m_editingSettGraph;
    QVector<keyRegName> m_vector_key;
    keyRegNameMaxSize m_max_size_key;
    metaDataFileMaxSize m_max_size_file;
    QVector<layoutItem> m_vector_info;

    QByteArray qfileDialog_style;
    QByteArray all_style;

//    contexMenuColorElements m_contex_menu_color;

    colorSvg m_attribute_color;
    colorSvg m_attribute_button_color;

    colorSvg m_polyline_color;

    QList<QString>::Iterator m_iter_color;
    QList<QString>::Iterator m_iter_color_begin;
    QList<QString>::Iterator m_iter_color_end;

    bool m_is_edited = false;
    bool m_is_files_action_open = false;
    bool m_is_key_action_open = false;

    bool m_is_selected_lines_item = false;
    bool m_is_selected_attribute_item = false;

//    QStringList

    axisData m_data_axis;
    QPoint m_mousePos;
    bool isShowWindowSettingsGraph = false;

    bool m_interactiveFontSize = false;
    bool m_isViewMode = true;
    bool m_isDeleteMode = false;


    int m_last_attr_selected = -1;
    int m_selected_graph;
    int m_count_attr_in_listattribute = 0;

    double m_scale = 0;
    int m_sizeIconAttr = 20;
    int m_size_status_bar = 75;
    int m_scale_size = 50;
    double m_scale_max = 50;
    double m_scale_min = 0.001;
    bool m_is_at_Time;

    int m_test_counter = 0;
    int m_test_counter_2 = 0;

    QString m_language_param = "rus";



};

#endif // GRAPHERGUI_H
