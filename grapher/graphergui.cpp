#include "graphergui.h"
#include "ui_graphergui.h"


GrapherGui::GrapherGui(QApplication &app, Painter::ParamPainter set, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GrapherGui),
    m_painter(app, set)
{
    // Настройки графической части
    ui->setupUi(this);

    if(!set.settingsFile.isEmpty())
    {
        readSettingsJsonfile(set.settingsFile);
    }
    else
    {
        readSettingsJsonfile();
    }

    m_app = &app;

    m_painter.setFlagWaterMark(m_settings.compareRmNumber || m_settings.compareStartDate);

    m_polyline_color.color = m_settings.colorIconsPolyline.color;
    m_polyline_color.border_color = m_settings.colorIconsPolyline.border_color;
    m_polyline_color.size_border = m_settings.colorIconsPolyline.size_border;

    if(m_settings.styleSheet)
    {
        QFile styleSheet(":/style/styles/nk_kittp_ti_style.qss");
        if (!styleSheet.open(QIODevice::ReadOnly))
        {
            qFatal("Не удалось открыть файл");
        }
        all_style = styleSheet.readAll();
        m_app->setStyleSheet(all_style);

        QFile styleSheet_qfiledialog(":/style/styles/style_for_qFile_dialog.qss");
        if (!styleSheet_qfiledialog.open(QIODevice::ReadOnly))
        {
            qFatal("Не удалось открыть файл");
        }

        qfileDialog_style = styleSheet_qfiledialog.readAll();

//        setColorIcons("#CD4B00");
        setColorIconsToolBar(m_settings.colorIconsToolBar.color, m_settings.colorIconsToolBar.border_color, m_settings.colorIconsToolBar.size_border);
        setColorIconsContexMenu(m_settings.colorIconsContextMenu.color, m_settings.colorIconsContextMenu.border_color, m_settings.colorIconsContextMenu.size_border);
        setColorIconsButton(m_settings.colorIconsButton.color, m_settings.colorIconsButton.border_color, m_settings.colorIconsButton.size_border);

        m_attribute_color.color = m_settings.colorIconsAttributeEdit.color;
        m_attribute_color.border_color = m_settings.colorIconsAttributeEdit.border_color;
        m_attribute_color.size_border = m_settings.colorIconsAttributeEdit.size_border;

        m_polyline_color.color = m_settings.colorIconsPolylineWithStyleSheets.color;
        m_polyline_color.border_color = m_settings.colorIconsPolylineWithStyleSheets.border_color;
        m_polyline_color.size_border = m_settings.colorIconsPolylineWithStyleSheets.size_border;
    }

//    ui->action_addKey->setVisible(false);

    if(m_settings.embeded)
    {
        setWindowFlags(Qt::BypassWindowManagerHint|Qt::X11BypassWindowManagerHint|Qt::FramelessWindowHint|Qt::MSWindowsFixedSizeDialogHint);
        setFixedSize(1, 1);
        ui->action_addKey->setVisible(false);
    }
    qDebug() << WINDOW_ID << GrapherGui::winId() << WINDOW_ID;



    m_key_escape = new QShortcut(this);
    m_key_escape->setKey(Qt::Key_Escape);

    m_key_enter = new QShortcut(this);
    m_key_enter->setKey(Qt::Key_Enter);

    m_key_ctrl_up = new QShortcut(this);
    m_key_ctrl_up->setKey(Qt::Key_Control | Qt::Key_Up);
    m_key_ctrl_down = new QShortcut(this);
    m_key_ctrl_up->setKey(Qt::Key_Control | Qt::Key_Down);

//    ui->action_addKey->setVisible(false);

    m_interactiveFontSize = true;
//    QValidator *validator = new QDoubleValidator(this);
    QValidator *validator = new QRegExpValidator(QRegExp("^([-]?[0-9][0-9]*|0)(\\.|,)[0-9]{9}"), this);

    readAssociateFile();
    setStatusBar();
    setPlot();
    setScaleVisible(false);
    setKeyVisible(false);
    ui->pushButton_createGraph->setEnabled(false);
//    ui->pushButton_deleteMode->setEnabled(false);
    ui->pushButton_key_add->setEnabled(false);
    ui->pushButton_key_delete->setEnabled(false);
    ui->line_scale_2->setVisible(false);
    setDeleteMode(false);

    ui->action_graph->setChecked(true);

    ui->action_save->setEnabled(false);
    ui->action_newGraph->setEnabled(false);


    ui->radioButton_users->setEnabled(m_settings.userIsEnable);
//    ui->pushButton_deleteMode->setVisible(false);
//    ui->pushButton_deleteMode->setEnabled(false);
    ui->pushButton_delete_selected_graph->setEnabled(false);
    ui->pushButton_editGraph->setEnabled(false);
    ui->action_editing->setEnabled(false);
    ui->action_view->setChecked(true);

    // Парсим json описывающие графики
    m_painter.parceJson();
    m_painter.readJsonGraphToVectorStruct();


    // Основыне настройки класса
    setSetingsFile(false);
    setViewMode(true);


    ui->lineEdit_Xmax->setValidator(validator);
    ui->lineEdit_Xmin->setValidator(validator);

    ui->lineEdit_Ymax->setValidator(validator);
    ui->lineEdit_Ymin->setValidator(validator);

    ui->lineEdit_SecondYmax->setValidator(validator);
    ui->lineEdit_SecondYmin->setValidator(validator);

//    ui->label_Xmax->setText("<p style=\"text-align: left;\">S = &pi;R&sup2;</p>");

    ui->radioButton_default->setChecked(true);
//    ui->radioButton_graph_atTime->setChecked(true);
    setItemListGraph();



    QMenu *menuGraph = new QMenu;
    menuGraph->addAction(ui->action_newGraph_atTime);
    menuGraph->addAction(ui->action_newGraph_atParam);
//    menuGraph->setContextMenuPolicy(Qt::PreventContextMenu);

//    ui->action_newGraph->setMenu(menuGraph);
    ui->action_newGraph->setVisible(false);

    menuGraph->setTitle(ui->action_newGraph->text());

//    m_toolButton_new_graph->set
    m_toolButton_new_graph = new QToolButton(ui->toolBar);
    m_toolButton_new_graph->setText(ui->action_newGraph->text());
    m_toolButton_new_graph->setIcon(ui->action_newGraph->icon());
    m_toolButton_new_graph->setToolButtonStyle(ui->toolBar->toolButtonStyle());
    m_toolButton_new_graph->setPopupMode(QToolButton::InstantPopup);
    m_toolButton_new_graph->setMenu(menuGraph);


    qDebug() << "ui->action_newGraph->text()" << ui->action_newGraph->text();
//    m_toolButton_new_graph->

    ui->toolBar->insertWidget(ui->action_newGraph, m_toolButton_new_graph);
    m_toolButton_new_graph->setEnabled(false);



    QList<QString>::Iterator get_color = m_painter.m_colors.begin();
    QList<QString>::Iterator get_color_begin = m_painter.m_colors.begin();
    QList<QString>::Iterator get_color_end = m_painter.m_colors.end();

    m_iter_color = get_color;
    m_iter_color_begin = get_color_begin;
    m_iter_color_end = get_color_end;

    ui->listLines->installEventFilter(this);
    ui->listGraph->installEventFilter(this);

//    ui->action_new_vertical_line->installEventFilter(m_menu_param);
//    ui->action_new_param_line->installEventFilter(m_menu_param);

//    m_menu_param


    // line edit
    connect(ui->lineEdit_Xmax, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineXMax(const QString &)));
    connect(ui->lineEdit_Xmin, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineXMin(const QString &)));
    connect(ui->lineEdit_Ymax, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineYMax(const QString &)));
    connect(ui->lineEdit_Ymin, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineYMin(const QString &)));
    connect(ui->lineEdit_SecondYmax, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineSecondYMax(const QString &)));
    connect(ui->lineEdit_SecondYmin, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineSecondYMin(const QString &)));
    connect(ui->lineEdit_key, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineKey(const QString &)));
    connect(ui->lineEdit_keyName, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineKeyName(const QString &)));
    connect(ui->lineEdit_keyReg, SIGNAL(textEdited(const QString &)), this, SLOT(editingLineKeyReg(const QString &)));


    // Радио кнопки
    connect(ui->radioButton_default, SIGNAL(clicked()), this, SLOT(click_on_radioButton_default()));
    connect(ui->radioButton_users, SIGNAL(clicked()), this, SLOT(click_on_radioButton_users()));

    // Кнопки
    connect(ui->pushButton_delete, SIGNAL(clicked()), this, SLOT(click_on_button_delete()));
    connect(ui->pushButton_addAttr, SIGNAL(clicked()), this, SLOT(click_on_button_addAttr()));
    connect(ui->pushButton_addFile, SIGNAL(clicked()), this, SLOT(click_on_button_addFile()));
    connect(ui->pushButton_deleteFile, SIGNAL(clicked()), this, SLOT(click_on_button_deleteFile()));
    connect(ui->pushButton_createGraph, SIGNAL(clicked()), this, SLOT(click_on_button_createGraph()));
    connect(ui->pushButton_editGraph, SIGNAL(clicked()), this, SLOT(click_on_button_editGraph()));
    connect(ui->pushButton_defaultScale, SIGNAL(clicked()), this, SLOT(click_on_button_defaultScale()));
    connect(ui->pushButton_key_add, SIGNAL(clicked()), this, SLOT(click_on_button_key_add()));
    connect(ui->pushButton_key_delete, SIGNAL(clicked()), this, SLOT(click_on_button_key_delete()));
    connect(ui->pushButton_view, SIGNAL(clicked()), this, SLOT(click_on_button_view()));
    connect(ui->pushButton_edit_save, SIGNAL(clicked()), this, SLOT(click_on_button_save_pattern()));
    connect(ui->pushButton_deleteMode, SIGNAL(clicked()), this, SLOT(click_on_button_delete_mode()));
    connect(ui->pushButton_delete_selected_graph, SIGNAL(clicked()), this, SLOT(click_on_button_delteGraph()));
    connect(ui->pushButton_back_to_view, SIGNAL(clicked()), this, SLOT(click_on_button_view()));



    // Экшены
    connect(ui->action_files, SIGNAL(triggered()), this, SLOT(click_on_action_files()));
    connect(ui->action_graph, SIGNAL(triggered()), this, SLOT(click_on_action_graph()));
    connect(ui->action_newGraph_atTime, SIGNAL(triggered()), this, SLOT(click_on_action_newGraph_atTime()));
    connect(ui->action_newGraph_atParam, SIGNAL(triggered()), this, SLOT(click_on_action_newGraph_atParam()));
//    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(click_on_action_save()));
    connect(ui->action_scale, SIGNAL(toggled(bool)), this, SLOT(togled_checkBox_scale(bool)));
    connect(ui->action_addKey, SIGNAL(triggered()), this, SLOT(click_on_action_addKey()));
    connect(ui->action_view, SIGNAL(triggered()), this, SLOT(click_on_action_view()));
    connect(ui->action_editing, SIGNAL(triggered()), this, SLOT(click_on_action_editing()));
    connect(ui->action_new_vertical_line, SIGNAL(triggered()), this, SLOT(click_on_action_add_vertical_line()));
    connect(ui->action_is_markers, SIGNAL(toggled(bool)), this, SLOT(togled_checkBox_isMarkers_all_graph(bool)));
    connect(ui->action_save_this_graph, SIGNAL(triggered()), this, SLOT(click_on_action_save_this_graph()));
    connect(ui->action_delete_lines, SIGNAL(triggered()), this, SLOT(click_on_action_delete_this_attribute()));
    connect(ui->action_edit_this_graph, SIGNAL(triggered()), this, SLOT(click_on_action_editing()));

    connect(ui->action_resert_edited, SIGNAL(triggered()), this, SLOT(click_on_action_reset_edited()));
    connect(ui->action_delete_this_graph, SIGNAL(triggered()), this, SLOT(click_on_action_delete_this_graph()));



    // Лист виджет
    connect(ui->listGraph, SIGNAL(currentRowChanged(int)), this, SLOT(selectItemGraph(int)));
    connect(ui->listGraph, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectItemGraph(QListWidgetItem *)));
    connect(ui->listGraph, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(double_click_item_setCheckBox(QListWidgetItem*)));
    connect(ui->listGraph, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(togled_checkBox_item_list_attr(QListWidgetItem*)));
    connect(ui->listGraph, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(listGraphContextMenu(QPoint)));



    connect(ui->listAttribute, SIGNAL(currentRowChanged(int)), this, SLOT(selectItemAttribute(int)));
    connect(ui->listAttribute, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(clickItemAttribute(QListWidgetItem *)));
    connect(ui->listAttribute, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(listAttributeContextyMenu(QPoint)));
    connect(ui->listLines, SIGNAL(currentRowChanged(int)), this, SLOT(selectItemLines(int)));
    connect(ui->listLines, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectItemLines(QListWidgetItem *)));
    connect(ui->listLines, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(listLinesContextyMenu(QPoint)));

    connect(ui->listKey, SIGNAL(currentRowChanged(int)), this, SLOT(selectItemKey(int)));

    connect(m_key_escape, SIGNAL(activated()), this, SLOT(shortCutEscape()));
    connect(m_key_enter, SIGNAL(activated()), this, SLOT(shortCutEnter()));

    connect(m_key_ctrl_up, SIGNAL(activated()), this, SLOT(shortCutCtrlUp()));
    connect(m_key_ctrl_down, SIGNAL(activated()), this, SLOT(shortCutCtrlDown()));


}

void GrapherGui::setColorIconsButton(QString color, QString border_color, int size_border)
{


    m_attribute_button_color.color = color;
    m_attribute_button_color.border_color = border_color;
    m_attribute_button_color.size_border = size_border;

    QIcon *icon_pushButton_editGraph = new QIcon();
    changeSVGColor(icon_pushButton_editGraph, ":/svg/icons_svg/edit.svg", color, border_color, size_border);
    ui->pushButton_editGraph->setIcon(*icon_pushButton_editGraph);

    QIcon *icon_pushButton_createGraph = new QIcon();
    changeSVGColor(icon_pushButton_createGraph, ":/svg/icons_svg/png-file.svg", color, border_color, size_border);
    ui->pushButton_createGraph->setIcon(*icon_pushButton_createGraph);

    QIcon *icon_pushButton_deleteMode = new QIcon();
    changeSVGColor(icon_pushButton_deleteMode, ":/svg/icons_svg/trash.svg", color, border_color, size_border);
    ui->pushButton_deleteMode->setIcon(*icon_pushButton_deleteMode);

    QIcon *icon_pushButton_delete_selected_graph = new QIcon();
    changeSVGColor(icon_pushButton_delete_selected_graph, ":/svg/icons_svg/trash.svg", color, border_color, size_border);
    ui->pushButton_delete_selected_graph->setIcon(*icon_pushButton_delete_selected_graph);

    QIcon *icon_pushButton_back_to_view = new QIcon();
    changeSVGColor(icon_pushButton_back_to_view, ":/svg/icons_svg/arrow_back.svg", color, border_color, size_border);
    ui->pushButton_back_to_view->setIcon(*icon_pushButton_back_to_view);

    QIcon *icon_pushButton_view = new QIcon();
    changeSVGColor(icon_pushButton_view, ":/svg/icons_svg/visible.svg", color, border_color, size_border);
    ui->pushButton_view->setIcon(*icon_pushButton_view);

    QIcon *icon_pushButton_edit_save = new QIcon();
    changeSVGColor(icon_pushButton_edit_save, ":/svg/icons_svg/save.svg", color, border_color, size_border);
    ui->pushButton_edit_save->setIcon(*icon_pushButton_edit_save);

    QIcon *icon_pushButton_addAttr = new QIcon();
    changeSVGColor(icon_pushButton_addAttr, ":/svg/icons_svg/create.svg", color, border_color, size_border);
    ui->pushButton_addAttr->setIcon(*icon_pushButton_addAttr);

    QIcon *icon_pushButton_delete = new QIcon();
    changeSVGColor(icon_pushButton_delete, ":/svg/icons_svg/trash.svg", color, border_color, size_border);
    ui->pushButton_delete->setIcon(*icon_pushButton_delete);

    QIcon *icon_pushButton_key_add = new QIcon();
    changeSVGColor(icon_pushButton_key_add, ":/svg/icons_svg/key.svg", color, border_color, size_border);
    ui->pushButton_key_add->setIcon(*icon_pushButton_key_add);

    QIcon *icon_pushButton_key_delete = new QIcon();
    changeSVGColor(icon_pushButton_key_delete, ":/svg/icons_svg/trash.svg", color, border_color, size_border);
    ui->pushButton_key_delete->setIcon(*icon_pushButton_key_delete);

    QIcon *icon_pushButton_addFile = new QIcon();
    changeSVGColor(icon_pushButton_addFile, ":/svg/icons_svg/add-file.svg", color, border_color, size_border);
    ui->pushButton_addFile->setIcon(*icon_pushButton_addFile);

    QIcon *icon_pushButton_deleteFile = new QIcon();
    changeSVGColor(icon_pushButton_deleteFile, ":/svg/icons_svg/trash.svg", color, border_color, size_border);
    ui->pushButton_deleteFile->setIcon(*icon_pushButton_deleteFile);

    QIcon *icon_pushButton_defaultScale = new QIcon();
    changeSVGColor(icon_pushButton_defaultScale, ":/svg/icons_svg/restart.svg", color, border_color, size_border);
    ui->pushButton_defaultScale->setIcon(*icon_pushButton_defaultScale);
}


void GrapherGui::setColorIconsContexMenu(QString color, QString border_color, int size_border)
{
    QIcon *icon_action_edit_this_graph = new QIcon();
    changeSVGColor(icon_action_edit_this_graph, ":/svg/icons_svg/edit.svg", color, border_color, size_border);
    ui->action_edit_this_graph->setIcon(*icon_action_edit_this_graph);

    QIcon *icon_action_new_vertical_line = new QIcon();
    changeSVGColor(icon_action_new_vertical_line, ":/svg/icons_svg/vertical_line.svg", color, border_color, size_border);
    ui->action_new_vertical_line->setIcon(*icon_action_new_vertical_line);

    QIcon *icon_action_new_param_line = new QIcon();
    changeSVGColor(icon_action_new_param_line, ":/svg/icons_svg/polyline.svg", color, border_color, size_border);
    ui->action_new_param_line->setIcon(*icon_action_new_param_line);

    QIcon *icon_action_save_this_graph = new QIcon();
    changeSVGColor(icon_action_save_this_graph, ":/svg/icons_svg/png-file.svg", color, border_color, size_border);
    ui->action_save_this_graph->setIcon(*icon_action_save_this_graph);

    QIcon *icon_action_delete_lines = new QIcon();
    changeSVGColor(icon_action_delete_lines, ":/svg/icons_svg/trash.svg", color, border_color, size_border);
    ui->action_delete_lines->setIcon(*icon_action_delete_lines);

    QIcon *icon_action_add_line_graph_lines = new QIcon();
    changeSVGColor(icon_action_add_line_graph_lines, ":/svg/icons_svg/create.svg", color, border_color, size_border);
    ui->action_add_line_graph_lines->setIcon(*icon_action_add_line_graph_lines);

    QIcon *icon_action_delete_graph = new QIcon();
    changeSVGColor(icon_action_delete_graph, ":/svg/icons_svg/trash.svg", color, border_color, size_border);
    ui->action_delete_this_graph->setIcon(*icon_action_delete_graph);

    QIcon *icon_action_resert_edited = new QIcon();
    changeSVGColor(icon_action_resert_edited, ":/svg/icons_svg/restart.svg", color, border_color, size_border);
    ui->action_resert_edited->setIcon(*icon_action_resert_edited);
}


void GrapherGui::setColorIconsToolBar(QString color, QString border_color, int size_border)
{
    QIcon *icon_action_files = new QIcon();
    changeSVGColor(icon_action_files, ":/svg/icons_svg/file.svg", color, border_color, size_border);
    ui->action_files->setIcon(*icon_action_files);

    QIcon *icon_action_newGraph = new QIcon();
    changeSVGColor(icon_action_newGraph, ":/svg/icons_svg/new_file.svg", color, border_color, size_border);
    ui->action_newGraph->setIcon(*icon_action_newGraph);

    QIcon *icon_action_graph = new QIcon();
    changeSVGColor(icon_action_graph, ":/svg/icons_svg/graphs.svg", color, border_color, size_border);
    ui->action_graph->setIcon(*icon_action_graph);

    QIcon *icon_action_scale = new QIcon();
    changeSVGColor(icon_action_scale, ":/svg/icons_svg/ruler.svg", color, border_color, size_border);
    ui->action_scale->setIcon(*icon_action_scale);

    QIcon *icon_action_addKey = new QIcon();
    changeSVGColor(icon_action_addKey, ":/svg/icons_svg/key.svg", color, border_color, size_border);
    ui->action_addKey->setIcon(*icon_action_addKey);

    QIcon *icon_action_view = new QIcon();
    changeSVGColor(icon_action_view, ":/svg/icons_svg/visible.svg", color, border_color, size_border);
    ui->action_view->setIcon(*icon_action_view);

    QIcon *icon_action_editing = new QIcon();
    changeSVGColor(icon_action_editing, ":/svg/icons_svg/edit-image.svg", color, border_color, size_border);
    ui->action_editing->setIcon(*icon_action_editing);
}


void GrapherGui::shortCutCtrlUp()
{
    qDebug() << "UP";
}

void GrapherGui::shortCutCtrlDown()
{
    qDebug() << "Down";
}



void GrapherGui::shortCutEnter()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    qDebug() << "Enter";
    click_on_button_edit_close();
}

void GrapherGui::shortCutEscape()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    click_on_button_defaultScale();
    click_on_button_edit_close();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-= line edit =-=-=-=-=-=-=-=
void GrapherGui::editingLine_edit_line_name_legend(const QString &str)
{
    m_is_edited = true;

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
    QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;
    m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.name_line = str;

    ui->listLines->currentItem()->setText(str);

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, false);
    // установить фокус на edit_point_x
    m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.lineEditCurveTitle->setFocus();

}



void GrapherGui::editingLine_edit_vline_text_x(const QString &str)
{
    m_is_edited = true;
    int index = m_selected_graph;
    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    QString text = str;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_x = text.replace(",", ".").toDouble();
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_y = m_vector_info[ui->listAttribute->currentRow()].vLine.text_y->text().replace(",", ".").toDouble();

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на edit_point_x
    m_vector_info[ui->listAttribute->currentRow()].vLine.text_x->setFocus();
}


void GrapherGui::editingLine_edit_vline_text_y(const QString &str)
{
    m_is_edited = true;
    int index = m_selected_graph;
    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    QString text = str;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_y = text.replace(",", ".").toDouble();
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_x = m_vector_info[ui->listAttribute->currentRow()].vLine.text_x->text().replace(",", ".").toDouble();

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на edit_point_x
    m_vector_info[ui->listAttribute->currentRow()].vLine.text_y->setFocus();
}


void GrapherGui::editingLine_edit_vline_text(const QString &str)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text = str;

    ui->listAttribute->currentItem()->setText(str);

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на edit_point_x
    m_vector_info[ui->listAttribute->currentRow()].vLine.textEdit->setFocus();
}


void GrapherGui::editingLine_edit_vline_y_max(const QString &str)
{
    m_is_edited = true;
    QString value = str;
    value = value.replace(",", ".");
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_min = m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_min->text().replace(",", ".").toDouble();
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_max = value.toDouble();

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на edit_y_max
    m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_max->setFocus();
}


void GrapherGui::editingLine_edit_vline_y_min(const QString &str)
{
    m_is_edited = true;
    QString value = str;
    value = value.replace(",", ".");
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_min = value.toDouble();
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_max = m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_max->text().replace(",", ".").toDouble();

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на edit_y_min
    m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_min->setFocus();
}


void GrapherGui::editingLine_edit_vline_x_point(const QString &str)
{
    m_is_edited = true;
    QString value = str;
    value = value.replace(",", ".");
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].point_x = value.toDouble();

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на edit_point_x
    m_vector_info[ui->listAttribute->currentRow()].vLine.edit_point_x->setFocus();
}


void GrapherGui::editingLine_edit_second_y_title(const QString &str)
{
    m_is_edited = true;
//    m_editingSettGraph.y2_title = str;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    m_painter.m_vector_of_parameters[index_vector][index].second_y_title = str;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

        m_vector_info[ui->listAttribute->currentRow()].edit_y2->setFocus();

//        click_on_button_edit_close();
    }
}

void GrapherGui::editingLine_edit_y_title(const QString &str)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    m_painter.m_vector_of_parameters[index_vector][index].y_title = str;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

        m_vector_info[ui->listAttribute->currentRow()].edit_y->setFocus();

//        click_on_button_edit_close();
    }
}

void GrapherGui::editingLine_edit_x_title(const QString &str)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    m_painter.m_vector_of_parameters[index_vector][index].x_title = str;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

        m_vector_info[ui->listAttribute->currentRow()].edit_x->setFocus();

//        click_on_button_edit_close();
    }
}

void GrapherGui::editingLine_edit_main_title(const QString &str)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;


    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;
//    qDebug() << "MAIN TITLE index_vector" << index_vector << "index" << index << "isDefault" << isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].main_title = str;
    m_painter.m_vector_of_parameters[index_vector][index].namefile = str;
    m_painter.m_vector_of_parameters[index_vector][index].nameTargetFile = str + ".png";
//    qDebug() << "После присвоений";
//    QString nameTarget;
    ui->listGraph->setCurrentRow(index);
    ui->listGraph->currentItem()->setText(str);
//    qDebug() << "Задача текста";
//    m_painter.m_vector_of_parameters[0][i].namefile

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setScaleView();
        setPaint(index, isDefault);
//        qDebug() << "setPaint";
        setVisibleSecondY(m_painter.m_is_second_y);
//        qDebug() << "VisibleY";

        createSettingsAttr(m_last_attr_selected, true);
//        qDebug() << "Creatte attr";

        m_vector_info[ui->listAttribute->currentRow()].edit_title->setFocus();

//        click_on_button_edit_close();
    }
}

void GrapherGui::editingLineKey(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    getEnabledKeyAdd();
}


void GrapherGui::editingLineKeyName(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    getEnabledKeyAdd();
}


void GrapherGui::editingLineKeyReg(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    getEnabledKeyAdd();
}


void GrapherGui::editingLineXMax(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setScaleView();
}
void GrapherGui::editingLineXMin(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setScaleView();
}


void GrapherGui::editingLineYMax(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setScaleView();
}
void GrapherGui::editingLineYMin(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setScaleView();
}


void GrapherGui::editingLineSecondYMax(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setScaleView();
}
void GrapherGui::editingLineSecondYMin(const QString &str)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setScaleView();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-= Радио кнопки =-=-=-=-=-=-=-=
void GrapherGui::click_on_radioButton_users()
{
    //qDebug() << "=============\n" <<__FUNCTION__;

    ui->pushButton_deleteMode->setVisible(ui->radioButton_users->isChecked()); //


    ui->listGraph->clear();
    setItemListGraph();
    ui->listGraph->setCurrentRow(-1);

    ui->pushButton_editGraph->setEnabled(false);
    ui->action_editing->setEnabled(false);
    if(ui->listGraph->count() > 0)
    {
        ui->pushButton_deleteMode->setEnabled(true);
    }
    else
    {
        ui->pushButton_deleteMode->setEnabled(false);
    }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::click_on_radioButton_default()
{
    //qDebug() << "=============\n" <<__FUNCTION__;

    ui->pushButton_deleteMode->setVisible(ui->radioButton_users->isChecked()); //

    ui->listGraph->clear();
    setItemListGraph();
    QModelIndex index = ui->listGraph->model()->index(0,0);
    ui->listGraph->setCurrentIndex(index);
    ui->pushButton_editGraph->setEnabled(false);
    ui->action_editing->setEnabled(false);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-= Кнопки =-=-=-=-=-=-=-=

void GrapherGui::click_on_button_delete_mode()
{
    int len_attr = ui->listGraph->count();
    for(int i = 0; i < len_attr; i++)
    {
        if(ui->listGraph->item(i)->checkState())
        {
            ui->listGraph->item(i)->setCheckState(Qt::Unchecked);
        }
    }
    m_isDeleteMode = true;
    setDeleteMode(m_isDeleteMode);
    setItemListGraph();
}

void GrapherGui::click_on_button_save_pattern()
{

    qDebug() << "save_pattern";

    int index = m_selected_graph;


    if(index > -1 && index < ui->listGraph->count())
    {
        if(ui->radioButton_default->isChecked())
        {
            m_painter.structToJson(index, true, true, m_settings.pathToUserJsonFile);
            m_painter.m_vector_of_parameters[0][index].is_from_json = true;
            qDebug() << "!!!! is_a_not_save_edited СБРОС  vector 0 index" << index;
            m_painter.m_vector_of_parameters[0][index].is_a_not_save_edited = false;

        }
        else if(ui->radioButton_users->isChecked())
        {
            m_painter.structToJson(index, false, true, m_settings.pathToUserJsonFile);
            m_painter.m_vector_of_parameters[1][index].is_from_json = true;
            qDebug() << "!!!! is_a_not_save_edited СБРОС  vector 1 index" << index;
            m_painter.m_vector_of_parameters[1][index].is_a_not_save_edited = false;
        }
    }



    if(m_settings.styleSheet)
    {
        QMessageBox reply;
        styleDialog(this, &reply);
        reply.setIcon(QMessageBox::Information);
        reply.setInformativeText("Шаблон сохранен.");
        reply.setText("Редактирование");
        reply.exec();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "Редактирование", "Шаблон сохранен.", QMessageBox::Ok);
    }

    m_is_edited = false;

}


void GrapherGui::click_on_button_edit_line_color()
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    m_app->setStyleSheet(all_style + qfileDialog_style);
    QColor color = QColorDialog::getColor(QColor(255,100,200,255));
    m_app->setStyleSheet(all_style);
    if (!color.isValid()) {
    // Cancel
    }
    else
    {
        int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
        QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.color_line = color.name().toUpper();
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboColor->setItemText(0, color.name().toUpper());
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboColor->setCurrentIndex(0);

//        setAttributeGraph();


        QIcon *icon_polyline = new QIcon();
        if(m_polyline_color.color != "none")
        {
            changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", m_polyline_color.color, m_polyline_color.border_color, m_polyline_color.size_border);
        }
        else
        {
            changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", color.name().toUpper(), m_polyline_color.border_color, m_polyline_color.size_border);
        }

        ui->listLines->currentItem()->setIcon(*icon_polyline);

        qDebug() << "PARAM TEXT" <<  ui->listLines->currentItem()->text();
        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, false);
    }
}


void GrapherGui::click_on_button_edit_vline_color()
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    m_app->setStyleSheet(all_style + qfileDialog_style);
    QColor color = QColorDialog::getColor(QColor(255,100,200,255));
    m_app->setStyleSheet(all_style);
    if (!color.isValid()) {
    // Cancel
    }
    else
    {
        int i = m_vector_info[ui->listAttribute->currentRow()].index;
        m_painter.m_vector_of_parameters[index_vector][index].vlines[i].color = color.name().toUpper();

        m_vector_info[ui->listAttribute->currentRow()].vLine.comboColor->setItemText(0, color.name().toUpper());
        m_vector_info[ui->listAttribute->currentRow()].vLine.comboColor->setCurrentIndex(0);

//        qDebug() << color.name().toUpper();

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);
    }

}


void GrapherGui::click_on_button_edit_apply_second_y_title()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].second_y_title = m_editingSettGraph.y2_title;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

        click_on_button_edit_close();
    }
}

void GrapherGui::click_on_button_edit_apply_y_title()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].y_title = m_editingSettGraph.y_title;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

        click_on_button_edit_close();
    }
}

void GrapherGui::click_on_button_edit_apply_x_title()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].x_title = m_editingSettGraph.x_title;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

        click_on_button_edit_close();
    }
}



void GrapherGui::click_on_button_edit_apply_header()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].main_title = m_editingSettGraph.header;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

        click_on_button_edit_close();
    }
}

void GrapherGui::click_on_button_edit_second_y_title_reset()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].second_y_title = m_startSettGraph.y2_title;
    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {
        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);
        createSettingsAttr(m_last_attr_selected, true);
    }
}

void GrapherGui::click_on_button_edit_y_title_reset()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].y_title = m_startSettGraph.y_title;
    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {
        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);
        createSettingsAttr(m_last_attr_selected, true);
    }
}

void GrapherGui::click_on_button_edit_x_title_reset()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].x_title = m_startSettGraph.x_title;
    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {
        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);
        createSettingsAttr(m_last_attr_selected, true);
    }
}


void GrapherGui::click_on_button_edit_header_reset()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].main_title = m_startSettGraph.header;
    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {
        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);
        createSettingsAttr(m_last_attr_selected, true);
    }
}


void GrapherGui::click_on_button_edit_close()
{
    m_is_edited = true;

    if(isShowWindowSettingsGraph)
    {
        m_windowSettingsGraph->close();
        isShowWindowSettingsGraph = false;
    }
}


void GrapherGui::click_on_button_edit_legend_reset()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].pos_legend = m_startSettGraph.pos_legend;

//    QStringList pos = {"top_left", "top_right", "top_center", "bottom_left", "bottom_right", "bottom_center", "left_center", "right_center"};

//    qDebug() << "index" << index << pos[index];



    if(m_painter.m_vector_of_parameters[index_vector].size() > index)
    {

        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        QIcon *icon_legend = new QIcon();
        setLegendIcon(icon_legend, m_startSettGraph.pos_legend);
        ui->listAttribute->currentItem()->setIcon(*icon_legend);

        createSettingsAttr(m_last_attr_selected, true);
    }
}


void GrapherGui::click_on_button_view()
{
    //qDebug() << "=============\n" <<__FUNCTION__;

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    if(m_is_edited)
    {
        m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;
        if(m_settings.styleSheet)
        {

            QMessageBox reply;
            styleDialog(this, &reply);
            reply.setIcon(QMessageBox::Question);
            reply.setInformativeText("При выходе из приложения, изменения будут утеряны. \nСохранить изменения в шаблон \"" + ui->listGraph->currentItem()->text() + "\"?");
            reply.setText("Редактирование");
            qDebug() << "!!!!!!!!!!!!!! VIEW index_vector" << index_vector << "index" << index;
            if(!m_painter.m_vector_of_parameters[index_vector][index].is_from_json)
            {
                reply.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            }
            else
            {
                reply.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Reset);
            }

            int answ =  reply.exec();

            if(answ == QMessageBox::Yes)
            {
                click_on_button_save_pattern();
            }

            if(answ == QMessageBox::Reset)
            {
                qDebug() << "Сброс изменений" << ui->listGraph->currentItem()->text();
                m_painter.readCurrentGraphFromJson(index_vector, index);
                qDebug() << "!!!! is_a_not_save_edited СБРОС  vector" << index_vector << "index" << index;
                m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = false;

            }
        }
        else
        {
            QMessageBox::StandardButton reply;
            qDebug() << "!!!!!!!!!!!!!! VIEW index_vector" << index_vector << "index" << index;
            if(!m_painter.m_vector_of_parameters[index_vector][index].is_from_json)
            {
                reply = QMessageBox::question(this, "Редактирование", "При выходе из приложения, изменения будут утеряны. \nСохранить изменения в шаблон \"" + ui->listGraph->currentItem()->text() + "\"?", QMessageBox::Yes|QMessageBox::No);
            }
            else
            {
                reply = QMessageBox::question(this, "Редактирование", "При выходе из приложения, изменения будут утеряны. \nСохранить изменения в шаблон \"" + ui->listGraph->currentItem()->text() + "\"?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Reset);
            }


            if(reply == QMessageBox::Yes)
            {
                click_on_button_save_pattern();
            }

            if(reply == QMessageBox::Reset)
            {
                qDebug() << "Сброс изменений" << ui->listGraph->currentItem()->text();
                m_painter.readCurrentGraphFromJson(index_vector, index);
                qDebug() << "!!!! is_a_not_save_edited СБРОС  vector" << index_vector << "index" << index;
                m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = false;

            }
        }

        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);


    }
    setDeleteMode(false);

    ui->pushButton_editGraph->setEnabled(false);
    ui->action_editing->setEnabled(false);

    m_isDeleteMode = false;
    m_is_edited = false;

    ui->listGraph->clear();
    setItemListGraph();
//    ui->listGraph->setCurrentRow(-1);

    if(isShowWindowSettingsGraph)
    {
        m_windowSettingsGraph->close();
//        delete m_windowSettingsGraph;
        m_vector_info.clear();
        isShowWindowSettingsGraph = false;
        m_iter_color = m_iter_color_begin;
    }


    if(m_windowSettingsGraph != nullptr)
    {
        qDebug() << "Закрытие и удаление виджета";

//        delete m_windowSettingsGraph;
//        m_windowSettingsGraph = nullptr;
    }

//    int index = m_selected_graph;
    if(ui->listAttribute->count() > 4)
    {
        if(index > -1 && index < ui->listGraph->count())
        {
            if(ui->radioButton_default->isChecked())
            {
                m_painter.structToJson(index, true, false);
            }
            else if(ui->radioButton_users->isChecked())
            {
                m_painter.structToJson(index, false, false);
            }
        }
    }

    setScaleVisible(true);
    setViewMode(true);

    click_on_button_defaultScale();
}


void GrapherGui::click_on_button_key_delete()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
//    qDebug() << "Todo keyDelete";



    int answ;
    if(m_settings.styleSheet)
    {
        QMessageBox reply;
        styleDialog(this, &reply);
        reply.setIcon(QMessageBox::Question);
        reply.setInformativeText("Вы уверены?");
        reply.setText("Удаление ключа");
        reply.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        answ =  reply.exec();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление ключа", "Вы уверены?", QMessageBox::Yes|QMessageBox::No);
        answ = reply;
    }

    if(answ == QMessageBox::Yes)
    {
        int index = ui->listKey->currentRow();
        if(index > -1 && m_vector_key.size() > index)
        {
            m_vector_key.remove(index);

            ui->listKey->clear();
            m_max_size_key.key_size_max = 0;
            m_max_size_key.name_size_max = 0;
            m_max_size_key.reg_size_max = 0;

            QVectorIterator<keyRegName> key_filter_size(m_vector_key);

            while (key_filter_size.hasNext()) {
                keyRegName reg_struct = key_filter_size.next();

                if(m_max_size_key.key_size_max < reg_struct.key.size())
                {
                    m_max_size_key.key_size_max = reg_struct.key.size();
                }

                if(m_max_size_key.name_size_max < reg_struct.name.size())
                {
                    m_max_size_key.name_size_max = reg_struct.name.size();
                }

                if(m_max_size_key.reg_size_max < reg_struct.reg.size())
                {
                    m_max_size_key.reg_size_max = reg_struct.reg.size();
                }

            }

            QVectorIterator<keyRegName> key_filter(m_vector_key);

            while (key_filter.hasNext()) {
                keyRegName reg_struct = key_filter.next();

                ui->listKey->addItem(QString("%1").arg(reg_struct.key, m_max_size_key.key_size_max) + " | " +
                                     QString("%1").arg(reg_struct.reg, m_max_size_key.reg_size_max) + " | " +
                                     QString("%1").arg(reg_struct.name, m_max_size_key.name_size_max) + " | ");

            }

            setJsonAssociateJson();
            ui->pushButton_key_delete->setEnabled(false);
        }
        else
        {
            qDebug() << "несуществующий index" << index;
        }
    }
    else
    {

    }


}


void GrapherGui::click_on_button_key_add()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    keyRegName reg_struct;
    reg_struct.key = ui->lineEdit_key->text();
    reg_struct.name = ui->lineEdit_keyName->text();
    reg_struct.reg = ui->lineEdit_keyReg->text().toLower();


    bool isValid = true;
    for(int i = 0; i < m_vector_key.size(); i++)
    {
        if(m_vector_key[i].key == reg_struct.key)
        {
            isValid = false;
            break;
        }
    }
    if(isValid)
    {
        m_vector_key.append(reg_struct);

        ui->lineEdit_key->setText("");
        ui->lineEdit_keyName->setText("");
        ui->lineEdit_keyReg->setText("");

        ui->pushButton_key_add->setEnabled(false);

        ui->listKey->clear();
        m_max_size_key.key_size_max = 0;
        m_max_size_key.name_size_max = 0;
        m_max_size_key.reg_size_max = 0;
        QVectorIterator<keyRegName> key_filter_size(m_vector_key);
        while (key_filter_size.hasNext()) {
            keyRegName reg_struct = key_filter_size.next();

            if(m_max_size_key.key_size_max < reg_struct.key.size())
            {
                m_max_size_key.key_size_max = reg_struct.key.size();
            }

            if(m_max_size_key.name_size_max < reg_struct.name.size())
            {
                m_max_size_key.name_size_max = reg_struct.name.size();
            }

            if(m_max_size_key.reg_size_max < reg_struct.reg.size())
            {
                m_max_size_key.reg_size_max = reg_struct.reg.size();
            }

        }

        QVectorIterator<keyRegName> key_filter(m_vector_key);
        while (key_filter.hasNext()) {
            keyRegName reg_struct = key_filter.next();

            ui->listKey->addItem(QString("%1").arg(reg_struct.key, m_max_size_key.key_size_max) + " | " +
                                 QString("%1").arg(reg_struct.reg, m_max_size_key.reg_size_max) + " | " +
                                 QString("%1").arg(reg_struct.name, m_max_size_key.name_size_max) + " | ");
        }
        setJsonAssociateJson();
    }
    else
    {

        if(m_settings.styleSheet)
        {
            QMessageBox reply;
            styleDialog(this, &reply);
            reply.setIcon(QMessageBox::Warning);
            reply.setInformativeText("Такой ключ уже существует.");
            reply.setText("Внимание");
            reply.exec();
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, "Внимание", "Такой ключ уже существует.", QMessageBox::Ok);
        }
    }
}


void GrapherGui::click_on_button_defaultScale()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    ui->lineEdit_Xmax->setText("0");
    ui->lineEdit_Xmin->setText("0");

    ui->lineEdit_Ymax->setText("0");
    ui->lineEdit_Ymin->setText("0");

    ui->lineEdit_SecondYmax->setText("0");
    ui->lineEdit_SecondYmin->setText("0");

//    qDebug() << "m_selected_graph" << m_selected_graph;

    setScaleView();
//    selectItemGraph(m_selected_graph);
}


void GrapherGui::click_on_button_editGraph()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
//    setScaleVisible(false);
    m_is_edited = false;
    setAttributeGraph();
    setViewMode(false);
}


void GrapherGui::click_on_button_delteGraph()
{

    int len_attr = ui->listGraph->count();
    QString delete_names = "Будут удалены:";
    for(int i = 0; i < len_attr; i++)
    {
        if(ui->listGraph->item(i)->checkState())
        {
            qDebug() << "Check at" << i;
            delete_names += "\n" + ui->listGraph->item(i)->text();
        }
    }

    int answ;
    if(m_settings.styleSheet)
    {
        QMessageBox reply;
        styleDialog(this, &reply);
        reply.setIcon(QMessageBox::Question);
        reply.setInformativeText(delete_names + "\nВы уверены?");
        reply.setText("Удаление шаблонов");
        reply.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        answ =  reply.exec();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление шаблонов", delete_names + "\nВы уверены?", QMessageBox::Yes|QMessageBox::No);
        answ = reply;
    }

    if(answ == QMessageBox::Yes)
    {
//        int len_attr = ui->listGraph->count();

        m_interactiveFontSize = false;

        for(int i = len_attr-1; i >= 0; i--)
        {
            if(ui->listGraph->item(i)->checkState())
            {
                qDebug() << "Check at" << i;
                ui->listGraph->item(i)->setCheckState(Qt::Unchecked);
                graph_meta_data data = getMetaDataVector();
                int index_vector = data.index_vector;
                bool isDefault = data.isDefault;

                delete ui->listGraph->takeItem(i);
                m_painter.delete_json_file(i, index_vector, m_settings.pathToUserJsonFile);
            }
        }

        m_interactiveFontSize = true;
        if(m_painter.m_vector_of_parameters[1].size() > 0)
        {
            ui->listGraph->clear();
            setItemListGraph();
            ui->listGraph->setCurrentRow(-1);
        }
        else
        {
            setPlot();
            m_plot->replot();

        }
        QMessageBox msgBox;
        if(m_settings.styleSheet)
        {
            styleDialog(this, &msgBox);
        }

        msgBox.setText("Графики удалены.");
        msgBox.exec();
    }

    if(ui->listGraph->count() > 0)
    {
        ui->pushButton_deleteMode->setEnabled(true);
    }
    else
    {
        ui->pushButton_deleteMode->setEnabled(false);
//        m_painter.structToJson(0, false, false, "");
//        m_painter.assertPaint(0, false);
    }



}


void GrapherGui::click_on_button_createGraph()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
//    qDebug() << "TODO create graph";
//    qDebug() << "index" << ;
//    qDebug() << ui->listGraph->itemWidget(ui->listGraph->item(m_selected_graph));

    int len_attr = ui->listGraph->count();

    m_app->setStyleSheet(all_style + qfileDialog_style);

    QFileDialog *existDir = new QFileDialog;
    existDir->setWindowTitle("Выберите директорию");
    existDir->setAcceptMode(QFileDialog::AcceptSave);
    existDir->setLabelText(QFileDialog::LookIn, "Директория");
    existDir->setLabelText(QFileDialog::Accept, "Сохранить");
    existDir->setFileMode(QFileDialog::Directory);
    existDir->setViewMode(QFileDialog::Detail);
    existDir->setOption(QFileDialog::ShowDirsOnly, true);
    existDir->setOption(QFileDialog::DontUseNativeDialog, true);
    existDir->resize(700, 500);
    QString nameFile = "";
    if(existDir->exec() == QDialog::Accepted)
    {
        nameFile = existDir->selectedFiles()[0];
    }
    qDebug() << "nameFile" << nameFile;

//    QString nameFile = QFileDialog::getExistingDirectory(this, "Выберите директорию", m_settings.pathToTargetFile, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//    QFileDialog::setDirectory()

    m_app->setStyleSheet(all_style);
    m_app->setStyleSheet("QwtScaleWidget{color: black;}");

    if(!nameFile.isEmpty())
    {
        m_interactiveFontSize = false;

        for(int i = 0; i < len_attr; i++)
        {
            if(ui->listGraph->item(i)->checkState())
            {
                qDebug() << "Check at" << i;
                ui->listGraph->item(i)->setCheckState(Qt::Unchecked);
                saveGraph(i);
                m_painter.m_param_graph.nameTargetFile = nameFile +"/"+ m_painter.m_param_graph.nameTargetFile;
                m_painter.output(m_plot, m_painter.m_param_graph.nameTargetFile);
            }
        }
        m_interactiveFontSize = true;
        selectItemGraph(m_selected_graph);

//        qDebug() << "nameFile" << nameFile;
        m_app->setStyleSheet(all_style);
        QMessageBox msgBox;
        if(m_settings.styleSheet)
        {
            styleDialog(this, &msgBox);
        }
        msgBox.setText("Графики успешно созданы.");
        msgBox.exec();
    }

//    m_plot->replot();
}


void GrapherGui::click_on_button_delete()
{


    bool is_at_time = m_painter.m_is_at_time;
    int index = m_selected_graph;
    int index_attr = ui->listAttribute->currentRow();

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    if(index_attr > -1)
    {

        if(index_attr > 4 && index_attr < ui->listAttribute->count())
        {
            if(!m_vector_info[index_attr].vLine.isEmpty)
            {

                int index_vline = m_vector_info[index_attr].vLine.index_vline;

                m_painter.delete_vertical_line(index_vector, index, index_vline);


                if(index > -1 && index < ui->listGraph->count())
                {
                    if(ui->radioButton_default->isChecked())
                    {
                        m_painter.structToJson(index, true, false);
                    }
                    else if(ui->radioButton_users->isChecked())
                    {
                        m_painter.structToJson(index, false, false);
                    }
                }

            }
        }
    }

    int index_row_list = ui->listLines->currentRow();
    int count_list_attr = ui->listAttribute->count();
    index_attr = index_row_list + count_list_attr;

    qDebug() << "index_attr" << index_attr << "4 + m_vector_info[index_attr].size_vlines" << 4 + m_vector_info[index_attr].size_vlines;

//        if(index_attr >= 4 + m_vector_info[index_attr].size_vlines && index_attr < ui->listAttribute->count())
    if(m_is_selected_lines_item && index_row_list > -1)
    {
        if(!m_vector_info[index_attr].line.isEmpty)
        {
            get_prev_color_from_iter();

            QString group = m_vector_info[index_attr].line.key;
            QString name_line = m_vector_info[index_attr].line.name;
            QString name_x = m_vector_info[index_attr].line.comboParam_x->currentText();
            QString name_y = m_vector_info[index_attr].line.comboParam_y->currentText();

            if(m_painter.m_is_at_time)
            {
                m_painter.delete_line_param(group, name_line, index_vector, index);

                if(index > -1 && index < ui->listGraph->count())
                {
                    if(ui->radioButton_default->isChecked())
                    {
                        m_painter.structToJson(index, true, false);
                    }
                    else if(ui->radioButton_users->isChecked())
                    {
                        m_painter.structToJson(index, false, false);
                    }
                }
            }
            else
            {
                m_painter.delete_line_param_x_y(group, name_x, name_y, index_vector, index);

                if(index > -1 && index < ui->listGraph->count())
                {
                    if(ui->radioButton_default->isChecked())
                    {
                        m_painter.structToJson(index, true, false);
                    }
                    else if(ui->radioButton_users->isChecked())
                    {
                        m_painter.structToJson(index, false, false);
                    }
                }
            }
        }
    }



    setPaint(index, isDefault);
    m_painter.m_is_at_time = is_at_time;
    setVisibleSecondY(m_painter.m_is_second_y);

    click_on_button_edit_close();


    setAttributeGraph();
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::click_on_button_addAttr()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    qDebug() << "TODO addAttr";
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::click_on_button_addFile()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QVectorIterator<keyRegName> key_filter(m_vector_key);
    QString filter;

    filter += m_settings.fileFilter;
    while (key_filter.hasNext()) {
        keyRegName reg_struct = key_filter.next();
        filter += ";;" + reg_struct.name + " (" + reg_struct.reg + ")";
    }

    qDebug() << filter;
    m_app->setStyleSheet(all_style + qfileDialog_style);
    QString nameFile = QFileDialog::getOpenFileName(this, "Выберите файл", m_settings.startPathFindFile, filter);
    m_app->setStyleSheet(all_style);

    bool isEvents = false;
    bool isNotTxt = false;

    if(!nameFile.isEmpty())
    {
        QDir dir(nameFile);
        QFileInfo info(nameFile);
        QString dirPath = info.absolutePath();

        QVectorIterator<keyRegName> key_filter_ass(m_vector_key);

        nameFile = dir.dirName();
        nameFile = nameFile.toLower();
        QString key;

        m_settings.pathToTargetFile = dirPath;
        m_settings.startPathFindFile = dirPath;
        setJsonSettings();
        while (key_filter_ass.hasNext()) {

            bool isFind = false;

//            map_ass.next();
            keyRegName reg_struct = key_filter_ass.next();
            QString find = reg_struct.reg;
            QRegExp re(find);

            qDebug() << reg_struct.key << ": " << reg_struct.name << reg_struct.reg;
            if(re.indexIn(nameFile) != -1)
            {
                isFind = true;

            }

            if(isFind)
            {
                QRegExp rexEve("events");
                if(rexEve.indexIn(nameFile) != -1)
                {
                    isFind = false;
                    isEvents = true;
                }

                QRegExp rexNotTxt("(.txt)|(.csv)");
                if(rexNotTxt.indexIn(nameFile) == -1)
                {
                    isFind = false;
                    isNotTxt = true;
                }
            }

            if(isFind)
            {
                key = reg_struct.key;
                break;
            }
        }


        qDebug() << "nameFile" << dir.dirName() << "key" << key;

        if(!nameFile.isEmpty() && nameFile != "." && !key.isEmpty())
        {
            bool isMatches = false;
            for(int i = 0; i < m_fileMetaDataVector.size(); i++)
            {
                if(m_fileMetaDataVector[i].key == key)
                {
                    isMatches = true;
                    break;
                }
            }
            if(!isMatches)
            {
                metaDataFile file;
                file.key = key;
                file.name = dir.dirName();
                file.path = dir.path();

                m_fileMetaDataVector.push_back(file);



                ui->listFile->clear();

                m_max_size_file.key_size_max = 0;
                m_max_size_file.name_size_max = 0;
                m_max_size_file.path_size_max = 0;

                for(int i = 0; i < m_fileMetaDataVector.size(); i++)
                {
                    if(m_max_size_file.key_size_max < m_fileMetaDataVector[i].key.size())
                    {
                        m_max_size_file.key_size_max = m_fileMetaDataVector[i].key.size();
                    }

                    if(m_max_size_file.name_size_max < m_fileMetaDataVector[i].name.size())
                    {
                        m_max_size_file.name_size_max = m_fileMetaDataVector[i].name.size();
                    }

                    if(m_max_size_file.path_size_max < m_fileMetaDataVector[i].path.size())
                    {
                        m_max_size_file.path_size_max = m_fileMetaDataVector[i].path.size();
                    }

                }
                for(int i = 0; i < m_fileMetaDataVector.size(); i++)
                {
                    for(int indKey = 0; indKey < m_vector_key.size(); indKey++)
                    {
                        if(m_fileMetaDataVector[i].key == m_vector_key[indKey].key)
                        {
                            ui->listFile->addItem(QString("%1").arg(m_vector_key[indKey].name, m_max_size_key.name_size_max) + " | " +
                                                  QString("%1").arg(m_fileMetaDataVector[i].name, m_max_size_file.name_size_max) + " | ");

                        }
                    }
                }

                filesApply();
                setMenuToAddButton();

                if(ui->listFile->count() > 0)
                {
//                    ui->action_newGraph->setEnabled(true);
                    m_toolButton_new_graph->setEnabled(true);
                }
                else
                {
//                    ui->action_newGraph->setEnabled(false);
                    m_toolButton_new_graph->setEnabled(false);
                    ui->pushButton_editGraph->setEnabled(false);
                    ui->action_editing->setEnabled(false);
                    ui->listGraph->setCurrentRow(-1);
                }

            }
            else
            {
                qDebug() << "Такой файл уже есть";

                if(m_settings.styleSheet)
                {
                    QMessageBox reply;
                    styleDialog(this, &reply);
                    reply.setIcon(QMessageBox::Warning);
                    reply.setInformativeText("Такой файл уже есть.");
                    reply.setText("Внимание");
                    reply.exec();
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "Внимание", "Такой файл уже есть.", QMessageBox::Ok);
                }
            }

        }
        else
        {
            if(isNotTxt)
            {
                qDebug() << "Неверный формат файла.";
                if(m_settings.styleSheet)
                {
                    QMessageBox reply;
                    styleDialog(this, &reply);
                    reply.setIcon(QMessageBox::Warning);
                    reply.setInformativeText("Невозможное расширение файла.");
                    reply.setText("Внимание");
                    reply.exec();
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "Внимание", "Невозможное расширение файла.", QMessageBox::Ok);
                }

            }
            else if(isEvents)
            {
                qDebug() << "Неверный формат файла.";

                if(m_settings.styleSheet)
                {
                    QMessageBox reply;
                    styleDialog(this, &reply);
                    reply.setIcon(QMessageBox::Warning);
                    reply.setInformativeText("Неверный формат файла.");
                    reply.setText("Внимание");
                    reply.exec();
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "Внимание", "Неверный формат файла.", QMessageBox::Ok);
                }
            }
            else
            {
                qDebug() << "Не удалось определить принадлежность файла.";

                if(m_settings.styleSheet)
                {
                    QMessageBox reply;
                    styleDialog(this, &reply);
                    reply.setIcon(QMessageBox::Warning);
                    reply.setInformativeText("Не удалось определить принадлежность файла.");
                    reply.setText("Внимание");
                    reply.exec();
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "Внимание", "Не удалось определить принадлежность файла.", QMessageBox::Ok);
                }
            }
        }

    }
    else
    {
        qDebug() << "Вы ничего не добавили";
//        QMessageBox msgBox;
//        msgBox.setText("Вы ничего не добавили.");
//        msgBox.exec();
    }

    ui->action_files->setChecked(true);
    ui->action_graph->setChecked(false);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::click_on_button_deleteFile()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    int selectedRow = ui->listFile->currentRow();

    if(selectedRow > -1)
    {
        QString deleted_key = m_fileMetaDataVector[selectedRow].key;
        m_fileMetaDataVector.removeAt(ui->listFile->currentRow());


        ui->listFile->clear();
        m_max_size_file.key_size_max = 0;
        m_max_size_file.name_size_max = 0;
        m_max_size_file.path_size_max = 0;

        for(int i = 0; i < m_fileMetaDataVector.size(); i++)
        {
            if(m_max_size_file.key_size_max < m_fileMetaDataVector[i].key.size())
            {
                m_max_size_file.key_size_max = m_fileMetaDataVector[i].key.size();
            }

            if(m_max_size_file.name_size_max < m_fileMetaDataVector[i].name.size())
            {
                m_max_size_file.name_size_max = m_fileMetaDataVector[i].name.size();
            }

            if(m_max_size_file.path_size_max < m_fileMetaDataVector[i].path.size())
            {
                m_max_size_file.path_size_max = m_fileMetaDataVector[i].path.size();
            }

        }

        for(int i = 0; i < m_fileMetaDataVector.size(); i++)
        {
            for(int indKey = 0; indKey < m_vector_key.size(); indKey++)
            {
                if(m_fileMetaDataVector[i].key == m_vector_key[indKey].key)
                {
                    ui->listFile->addItem(QString("%1").arg(m_vector_key[indKey].name, m_max_size_key.name_size_max) + " | " +
                                          QString("%1").arg(m_fileMetaDataVector[i].name, m_max_size_file.name_size_max) + " | ");

                }
            }
        }
        m_painter.m_allGroupFromTables.clear();


        filesApply();

        if(!m_isViewMode)
        {
            int index = m_selected_graph;

            graph_meta_data data = getMetaDataVector();
            int index_vector = data.index_vector;
            bool isDefault = data.isDefault;

            for(int i = 0; i < m_vector_info.size(); i++)
            {
                if(!m_vector_info[i].line.isEmpty)
                {
                    qDebug() << "m_vector_info[i].line.key" << m_vector_info[i].line.key << "deleted_key" << deleted_key;
                    if(m_vector_info[i].line.key == deleted_key)
                    {
                        m_painter.delete_all_param_at_group(deleted_key, index_vector, index);

                        if(index > -1 && index < ui->listGraph->count())
                        {
                            m_painter.structToJson(index, isDefault, false);
                        }

                        m_vector_info.removeAt(i);
                        i--;

                        if(isDefault)
                        {
                            click_on_radioButton_default();
                        }
                        else
                        {
                            click_on_radioButton_users();
                        }

                    }
                }
            }

            setPaint(index, isDefault);
            setVisibleSecondY(m_painter.m_is_second_y);

            click_on_button_edit_close();

            setAttributeGraph();
            click_on_action_files();
    //        selectItemGraph(0);

        }
        else
        {
            setMenuToAddButton();
        }
    //    setAttributeGraph();


        if(ui->listFile->count() > 0)
        {
    //        ui->action_newGraph->setEnabled(true);
            m_toolButton_new_graph->setEnabled(true);
        }
        else
        {
    //        ui->action_newGraph->setEnabled(false);
            m_toolButton_new_graph->setEnabled(false);
            ui->pushButton_editGraph->setEnabled(false);
            ui->action_editing->setEnabled(false);
            ui->listGraph->setCurrentRow(-1);
            m_selected_graph = -1;
            setPlot();
            m_plot->replot();
    //        qDebug() << "setCurrentRow -1";
        }

        ui->grapherWidget->setVisible(false);
        ui->action_scale->setEnabled(false);

    //    ui->action_files->setChecked(
        ui->action_addKey->setChecked(false);
        ui->action_graph->setChecked(false);
        ui->action_files->setChecked(true);

    }


}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-= Экшены =-=-=-=-=-=-=-=
void GrapherGui::click_on_action_delete_this_attribute()
{
    click_on_button_delete();
}


void GrapherGui::click_on_action_save_this_graph()
{
    int index_selected_graph = ui->listGraph->currentRow();
    m_app->setStyleSheet(all_style + qfileDialog_style);
    QString nameFile = QFileDialog::getExistingDirectory(this, "Выберите директорию", m_settings.pathToTargetFile);
    m_app->setStyleSheet(all_style);
    m_app->setStyleSheet("QwtScaleWidget{color: black;}");
    if(!nameFile.isEmpty())
    {
        m_interactiveFontSize = false;

        saveGraph(index_selected_graph);
        m_painter.m_param_graph.nameTargetFile = nameFile +"/"+ m_painter.m_param_graph.nameTargetFile;
        m_painter.output(m_plot, m_painter.m_param_graph.nameTargetFile);

        m_interactiveFontSize = true;
        selectItemGraph(m_selected_graph);

//        qDebug() << "nameFile" << nameFile;
        QMessageBox msgBox;
        if(m_settings.styleSheet)
        {
            styleDialog(this, &msgBox);
        }
        m_app->setStyleSheet(all_style);
        msgBox.setText("График успешно создан.");
        msgBox.exec();
    }

}


void GrapherGui::click_on_action_add_vertical_line()
{

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;


    m_painter.add_vertical_line(index_vector, index);

    if(index > -1 && index < ui->listGraph->count())
    {
        if(ui->radioButton_default->isChecked())
        {
            m_painter.structToJson(index, true, false);
        }
        else if(ui->radioButton_users->isChecked())
        {
            m_painter.structToJson(index, false, false);
        }
    }


    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    click_on_button_edit_close();

    setAttributeGraph();
}


void GrapherGui::click_on_action_x_y_line()
{


    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    QAction *action = qobject_cast<QAction*>(sender());
    QString key = action->data().toString();
//    QString param = action->text();

    qDebug() << "group" << key << "index_vector" << index_vector;


    m_painter.add_line_param_x_y(key, "", get_next_color_from_iter(), index_vector, index);

    if(index > -1 && index < ui->listGraph->count())
    {
        if(ui->radioButton_default->isChecked())
        {
            m_painter.structToJson(index, true, false);
        }
        else if(ui->radioButton_users->isChecked())
        {
            m_painter.structToJson(index, false, false);
        }
    }

    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    click_on_button_edit_close();

    setAttributeGraph();
}


void GrapherGui::click_on_action_param()
{
    m_is_edited = true;

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    QAction *action = qobject_cast<QAction*>(sender());
    if(action)
    {

            QString token = action->data().toString();
            QStringList tokenList = token.split("|");
            QString key = tokenList[0];
            QString indexTable = tokenList[1];
            QString eng_param_name = tokenList[2];
            QString param = action->text();

            qDebug() << "click param" << param << eng_param_name << "group" << key << "index_vector" << index_vector << "index_graph" << index;


            if(index > -1 && index < ui->listGraph->count())
            {
//                qDebug() << m_painter.m_tables[i]

                m_painter.add_line_param(key, eng_param_name, param, get_next_color_from_iter(), index_vector, index);

                if(ui->radioButton_default->isChecked())
                {
                    m_painter.structToJson(index, true, false);
                    m_painter.m_vector_of_parameters[0][index].is_a_not_save_edited = true;
                }
                else if(ui->radioButton_users->isChecked())
                {
                    m_painter.structToJson(index, false, false);
                    m_painter.m_vector_of_parameters[1][index].is_a_not_save_edited = true;
                }
            }
    }

    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    click_on_button_edit_close();
    click_on_button_defaultScale();

    setAttributeGraph();
}



void GrapherGui::click_on_action_view()
{
    //qDebug() << "=============\n" <<__FUNCTION__;

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;
    if(m_is_edited)
    {

        int answ;
        if(m_settings.styleSheet)
        {
            QMessageBox reply;
            styleDialog(this, &reply);
            reply.setIcon(QMessageBox::Question);
            reply.setInformativeText("При выходе из приложения, изменения будут утеряны. \nСохранить изменения в шаблон \"" + ui->listGraph->currentItem()->text() + "\"?");
            reply.setText("Редактирование");
            qDebug() << "!!!!!!!!!!!!!! VIEW index_vector" << index_vector << "index" << index;
            if(!m_painter.m_vector_of_parameters[index_vector][index].is_from_json)
            {
                reply.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            }
            else
            {
                reply.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Reset);
            }

            answ = reply.exec();
        }
        else
        {

            QMessageBox::StandardButton reply;
            qDebug() << "!!!!!!!!!!!!!! VIEW index_vector" << index_vector << "index" << index;

            if(!m_painter.m_vector_of_parameters[index_vector][index].is_from_json)
            {
                reply = QMessageBox::question(this, "Редактирование", "При выходе из приложения, изменения будут утеряны. \nСохранить изменения в шаблон \"" + ui->listGraph->currentItem()->text() + "\"?", QMessageBox::Yes|QMessageBox::No);

            }
            else
            {
                reply = QMessageBox::question(this, "Редактирование", "При выходе из приложения, изменения будут утеряны. \nСохранить изменения в шаблон \"" + ui->listGraph->currentItem()->text() + "\"?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Reset);

            }
            answ = reply;
        }

        if(answ == QMessageBox::Yes)
        {
            click_on_button_save_pattern();
        }

        if(answ == QMessageBox::Reset)
        {
            qDebug() << "Сброс изменений" << ui->listGraph->currentItem()->text();
            m_painter.readCurrentGraphFromJson(index_vector, index);
            qDebug() << "!!!! is_a_not_save_edited СБРОС  vector" << index_vector << "index" << index;
            m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = false;
        }


        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);
    }

    ui->pushButton_editGraph->setEnabled(false);
    ui->action_editing->setEnabled(false);

    m_is_edited = false;

    if(isShowWindowSettingsGraph)
    {
        m_vector_info.clear();

        m_windowSettingsGraph->close();
//        delete m_windowSettingsGraph;
        isShowWindowSettingsGraph = false;

        m_iter_color = m_iter_color_begin;
    }

    if(m_windowSettingsGraph != nullptr)
    {
        qDebug() << "Закрытие и удаление виджета";
//        delete m_windowSettingsGraph;
//        m_windowSettingsGraph = nullptr;
    }

    ui->pushButton_view->setFocus();
    m_isViewMode = true;
//    int index = m_selected_graph;

    if(ui->listAttribute->count() > 4)
    {
        if(index > -1 && index < ui->listGraph->count())
        {
            if(index > -1 && index < ui->listGraph->count())
            {
                if(ui->radioButton_default->isChecked())
                {
                    m_painter.structToJson(index, true, false);
                }
                else if(ui->radioButton_users->isChecked())
                {
                    m_painter.structToJson(index, false, false);
                }
            }
        }
    }

//    ui->listGraph->setCurrentRow(index);

    setScaleVisible(true);
    setViewMode(true);

    click_on_button_defaultScale();
    selectItemGraph(index);
}


void GrapherGui::click_on_action_editing()
{
    //qDebug() << "=============\n" <<__FUNCTION__;

    ui->pushButton_editGraph->setFocus();
    m_is_edited = false;
    m_isViewMode = false;
    setScaleVisible(false);
    setAttributeGraph();
    setViewMode(false);
//    qDebug() << "index past past" << m_selected_graph;
}


void GrapherGui::click_on_action_addKey()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    ui->grapherWidget->setVisible(false);
    ui->action_scale->setEnabled(false);

    m_is_files_action_open = false;
    m_is_key_action_open = true;
//    ui->checkBox_scale->setVisible(false);

//    ui->action_addKey->setChecked(false);
    ui->action_graph->setChecked(false);
    ui->action_files->setChecked(false);
    ui->action_addKey->setChecked(true);
    setScaleVisible(false);

    // кнопка файлы
    setSetingsFile(false);

    setKeyVisible(true);
//    qDebug() << "TODO Key";
}


void GrapherGui::click_on_action_save()
{
    //qDebug() << "=============\n" <<__FUNCTION__;

    qDebug() << "TODO save";

//    qDebug() << "d_plot->axisScaleDiv(QwtPlot::xBottom)" << m_plot->axisScaleDiv(QwtPlot::xBottom).interval().minValue() << m_plot->axisScaleDiv(QwtPlot::xBottom).interval().maxValue();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::click_on_action_graph()
{
    //qDebug() << "=============\n" <<__FUNCTION__;

    // кнопка файлы
    setSetingsFile(false);
    setKeyVisible(false);
    m_is_files_action_open = false;
    m_is_key_action_open = false;

    ui->grapherWidget->setVisible(true);
    ui->action_scale->setEnabled(true);
//    ui->action_scale->setEnabled(m_isViewMode);

//    ui->action_graph->setChecked(true);
    ui->action_files->setChecked(false);
    ui->action_addKey->setChecked(false);
    ui->action_graph->setChecked(true);
//    ui->checkBox_scale->setVisible(true);
    setScaleVisible(true);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void GrapherGui::click_on_action_newGraph_atParam()
{
//    m_is_a_new_graph = true;
    //qDebug() << "=============\n" <<__FUNCTION__;
    setDeleteMode(false);
    setPlot();
    m_plot->replot();

    int count_graph = m_painter.m_vector_of_parameters[1].size();
    int index_last_graph = count_graph -1;
    if(index_last_graph != -1)
    {
        if(m_painter.m_vector_of_parameters[1][index_last_graph].line_params_curve.size() == 0 && m_painter.m_vector_of_parameters[1][index_last_graph].vlines.size() == 0)
        {
            qDebug() << "index_last_graph" << index_last_graph << m_painter.m_vector_of_parameters[1][index_last_graph].main_title;
            delete ui->listGraph->takeItem(index_last_graph);
            m_painter.delete_json_file(index_last_graph, 1, m_settings.pathToUserJsonFile);
            count_graph--;
        }
    }


    qDebug() << "count_graph" << count_graph;
    m_painter.setParamGraph(false);
//    count_graph = m_painter.m_vector_of_parameters[1].size();
    m_painter.m_is_at_time = false;
    ui->radioButton_users->setChecked(true);

    ui->listGraph->clear();
    setItemListGraph();

    if(count_graph > -1)
    {
        ui->listGraph->setCurrentRow(count_graph);
    }
    else
    {
        ui->listGraph->setCurrentRow(-1);
    }

    m_selected_graph = count_graph;

    setScaleVisible(false);
    ui->listAttribute->clear();
    m_vector_info.clear();
    setAttributeGraph();
    setViewMode(false);

    if(ui->listGraph->count() > 0)
    {
        ui->pushButton_deleteMode->setEnabled(true);
    }
    else
    {
        ui->pushButton_deleteMode->setEnabled(false);
    }


    click_on_action_graph();
    m_painter.m_vector_of_parameters[1][index_last_graph].is_a_new_graph = true;
    setPaint(count_graph, false);
    setVisibleSecondY(m_painter.m_is_second_y);
}


void GrapherGui::click_on_action_newGraph_atTime()
{
//    m_is_a_new_graph = true;
    //qDebug() << "=============\n" <<__FUNCTION__;

//    for(int i = 0; i < m_painter.m_vector_of_parameters[1].size(); i++)
//    {
//        m_painter.structToJson(i, false, false, "");
//    }
    setDeleteMode(false);
    setPlot();
    m_plot->replot();

    int count_graph = m_painter.m_vector_of_parameters[1].size();
    int index_last_graph = count_graph -1;
    if(index_last_graph != -1)
    {
        if(m_painter.m_vector_of_parameters[1][index_last_graph].line_params_curve.size() == 0 && m_painter.m_vector_of_parameters[1][index_last_graph].vlines.size() == 0)
        {
            qDebug() << "index_last_graph" << index_last_graph << m_painter.m_vector_of_parameters[1][index_last_graph].main_title;
            delete ui->listGraph->takeItem(index_last_graph);
            m_painter.delete_json_file(index_last_graph, 1, m_settings.pathToUserJsonFile);
            count_graph--;
        }
    }

    m_painter.setParamGraph(true);
//    count_graph = m_painter.m_vector_of_parameters[1].size();
    m_painter.m_is_at_time = true;
    ui->radioButton_users->setChecked(true);

    ui->listGraph->clear();
    setItemListGraph();

    if(count_graph > -1)
    {
        ui->listGraph->setCurrentRow(count_graph);
//        selectItemGraph(m_selected_graph);
    }
    else
    {
        ui->listGraph->setCurrentRow(-1);
    }

    m_selected_graph = count_graph;
//    ui->listGraph->setCurrentRow(m_selected_graph);

    setScaleVisible(false);
    ui->listAttribute->clear();
    m_vector_info.clear();
    setAttributeGraph();
    setViewMode(false);

    if(ui->listGraph->count() > 0)
    {
        ui->pushButton_deleteMode->setEnabled(true);
    }
    else
    {
        ui->pushButton_deleteMode->setEnabled(false);
    }

    click_on_action_graph();
    m_painter.m_vector_of_parameters[1][index_last_graph].is_a_new_graph = true;
    setPaint(count_graph, false);
    setVisibleSecondY(m_painter.m_is_second_y);
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::click_on_action_reset_edited()
{
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    qDebug() << "Сброс изменений" << ui->listGraph->currentItem()->text();
    m_painter.readCurrentGraphFromJson(index_vector, index);
    qDebug() << "!!!! is_a_not_save_edited СБРОС  vector" << index_vector << "index" << index;
    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = false;

    setItemListGraph();

    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);
}


void GrapherGui::click_on_action_delete_this_graph()
{

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    int answ;
    if(m_settings.styleSheet)
    {
        QMessageBox reply;
        styleDialog(this, &reply);
        reply.setIcon(QMessageBox::Question);
        reply.setInformativeText("Удалить " + ui->listGraph->currentItem()->text() + "\nВы уверены?");
        reply.setText("Удаление шаблонов");
        reply.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        answ =  reply.exec();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление шаблонов", "Удалить " +  ui->listGraph->currentItem()->text() + "\nВы уверены?", QMessageBox::Yes|QMessageBox::No);
        answ = reply;
    }

    if(answ == QMessageBox::Yes)
    {


        delete ui->listGraph->takeItem(index);
        m_painter.delete_json_file(index, index_vector, m_settings.pathToUserJsonFile);


        if(m_painter.m_vector_of_parameters[index_vector].size() > 0)
        {
            ui->listGraph->clear();
            setItemListGraph();
            ui->listGraph->setCurrentRow(-1);
        }
        else
        {
            setPlot();
            m_plot->replot();

        }
        QMessageBox msgBox;
        if(m_settings.styleSheet)
        {
            styleDialog(this, &msgBox);
        }

        msgBox.setText("График удален");
        msgBox.exec();
    }




}


void GrapherGui::click_on_action_files()
{

    m_is_files_action_open = true;
    m_is_key_action_open = false;
    //qDebug() << "=============\n" <<__FUNCTION__;
    ui->grapherWidget->setVisible(false);
    ui->action_scale->setEnabled(false);

//    ui->action_files->setChecked(

    ui->action_files->setChecked(true);
    ui->action_addKey->setChecked(false);
    ui->action_graph->setChecked(false);
//    ui->checkBox_scale->setVisible(false);
    setScaleVisible(false);
    setKeyVisible(false);

    // кнопка файлы
    setSetingsFile(true);

    if(ui->listFile->count() == 0)
    {
        click_on_button_addFile();
    }
}


void GrapherGui::togled_checkBox_isMarkers_all_graph(bool isCheked)
{

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index && index > -1)
    {
        int size_lines = m_painter.m_vector_of_parameters[index_vector][index].line_params_curve.size();

        for(int i = 0; i < size_lines; i++)
        {
            m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_dots = isCheked;
        }
    }

    if(index > -1 && index < ui->listGraph->count())
    {
        if(ui->radioButton_default->isChecked())
        {
            m_painter.structToJson(index, true, false);
        }
        else if(ui->radioButton_users->isChecked())
        {
            m_painter.structToJson(index, false, false);
        }
    }

    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);


}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::togled_checkBox_scale(bool isCheked)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setScaleVisible(isCheked);

    setNewPositionWidget();
//    setVisibleSecondY(false);
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-= Лист виджет =-=-=-=-=-=-=-=
void GrapherGui::selectItemKey(int index)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    if(m_vector_key.size() > index && index > -1)
    {
        qDebug() << index << m_vector_key[index].key << m_vector_key[index].name << m_vector_key[index].reg;

//        ui->lineEdit_key->setText(m_vector_key[index].key);
//        ui->lineEdit_keyName->setText(m_vector_key[index].name);
//        ui->lineEdit_keyReg->setText(m_vector_key[index].reg);

        ui->lineEdit_key->setText("");
        ui->lineEdit_keyName->setText("");
        ui->lineEdit_keyReg->setText("");

        ui->pushButton_key_delete->setEnabled(true);
        ui->pushButton_key_add->setEnabled(false);
    }
}


void GrapherGui::setLegendIcon(QIcon *icon_legend, QString position)
{
//    QStringList pos = {"top_left", "top_right", "top_center", "bottom_left", "bottom_right", "bottom_center", "left_center", "right_center"};

    if(m_painter.m_map_position.contains(position))
    {

        if(m_settings.styleSheet)
        {
            changeSVGColor(icon_legend, m_painter.m_map_position[position].path_toIconSVG, m_attribute_color.color, m_attribute_color.border_color, m_attribute_color.size_border);
//            qDebug() << "m_painter.m_map_position[position].path_toIconSVG" << m_painter.m_map_position[position].path_toIconSVG;
        }
        else
        {
            icon_legend->addFile(m_painter.m_map_position[position].path_toIcon);
        }
    }
    else
    {

        if(m_settings.styleSheet)
        {
            changeSVGColor(icon_legend, ":/image/icons/legend_pos_left_top.svg", m_attribute_color.color, m_attribute_color.border_color, m_attribute_color.size_border);
        }
        else
        {
            icon_legend->addFile(":/image/icons/legend_pos_left_top.png");
        }
    }
}


void GrapherGui::setMenuToAddButton()
{
//    ui->pushButton_addAttr->

//    QMenu *menu = new QMenu;
//    if(m_menu_param != nullptr)
//    {
//        delete m_menu_param;
//    }

    m_menu_param = new QMenu;
    QMenu *menu_line = new QMenu;
    QList<QAction*> list_action_group;

    QMap<QString, QStringList> name_of_graph;
//    QStringList name_of_graph;

    for(int i = 0; i < m_vector_info.size(); i++)
    {
        if(!m_vector_info[i].line.name.isEmpty())
        {
            name_of_graph[m_vector_info[i].key].append(m_vector_info[i].line.name);
//            qDebug() << "m_vector_info[i].line.name" << m_vector_info[i].line.name << m_vector_info[i].key;
        }

    }

//    qDebug() << name_of_graph;

    for(int i = 0; i < m_painter.m_tables.size(); i++)
    {
        QMenu *menu_param = new QMenu;
        QString name_key;
        for(int ind_key = 0; ind_key < m_vector_key.size(); ind_key++)
        {
            if(m_vector_key[ind_key].key == m_painter.m_tables[i]->m_group)
            {
                name_key = m_vector_key[ind_key].name;
            }
        }
        QAction *action_group = new QAction(name_key, this);
        QList<QAction*> list_action_param;

        QString lang = "eng";

        if(m_painter.m_tables[i]->m_title.contains(m_language_param) && m_settings.useRusTitle)
        {
            lang = m_language_param;
        }
        else
        {
            lang = "eng";
        }

        for(int param = 0; param < m_painter.m_tables[i]->m_title[lang].size(); param++)
        {
            if(!name_of_graph[m_painter.m_tables[i]->m_group].contains(m_painter.m_tables[i]->m_title["eng"][param].name))
            {
                QAction *action_param = new QAction(m_painter.m_tables[i]->m_title[lang][param].name, this);
                QVariant group;
                group = m_painter.m_tables[i]->m_group + "|" + i + "|" + m_painter.m_tables[i]->m_title["eng"][param].name;
                action_param->setData(group);
                connect(action_param, SIGNAL(triggered()), this, SLOT(click_on_action_param()));

                list_action_param.append(action_param);
            }
        }
        menu_param->addActions(list_action_param);
        action_group->setMenu(menu_param);
        list_action_group.append(action_group);
    }


    if(m_painter.m_is_at_time)
    {
        menu_line->addActions(list_action_group);
        ui->action_new_param_line->setMenu(menu_line);
        m_menu_param->addAction(ui->action_new_vertical_line);
        m_menu_param->addAction(ui->action_new_param_line);
        ui->pushButton_addAttr->setMenu(m_menu_param);
//        ui->action_add_line_graph->setMenu(m_menu_param);
        ui->action_add_line_graph_lines->setMenu(m_menu_param);
    }
    else
    {
        QMenu *menu_param = new QMenu;

        for(int i = 0; i < m_painter.m_tables.size(); i++)
        {
            QString name_key;
            for(int ind_key = 0; ind_key < m_vector_key.size(); ind_key++)
            {
                if(m_vector_key[ind_key].key == m_painter.m_tables[i]->m_group)
                {
                    name_key = m_vector_key[ind_key].name;
                }

            }
            QAction *action_group = new QAction(name_key, this);

            QVariant group;
            group = m_painter.m_tables[i]->m_group;
            action_group->setData(group);

            connect(action_group, SIGNAL(triggered()), this, SLOT(click_on_action_x_y_line()));
            menu_param->addAction(action_group);
//            action_group->setMenu(menu_param);
        }


        ui->action_new_param_line->setMenu(menu_param);
        m_menu_param->addAction(ui->action_new_param_line);
        ui->pushButton_addAttr->setMenu(m_menu_param);
//        ui->action_add_line_graph->setMenu(m_menu_param);
        ui->action_add_line_graph_lines->setMenu(m_menu_param);
    }

    m_menu_param->installEventFilter(this);

//    connect(m_menu_param, SIGNAL(hovered(QAction *)), this, SLOT(selectEditMenuItemHovered(QAction *)));

//    ui->action_new_vertical_line->installEventFilter(this);
//    ui->action_new_param_line->installEventFilter(this);

}

void GrapherGui::selectEditMenuItemHovered(QAction *action)
{
//    qDebug() << "=============\n" <<__FUNCTION__;
//    if(m_settings.styleSheet)
//    {
//        // ===========================================
//        if(action == ui->action_new_vertical_line && m_contex_menu_color.action_new_vertical_line == "black")
//        {
////            m_test_counter_2++;
////            qDebug() << "hovered counter" << m_test_counter_2;
////            qDebug() << ui->action_new_vertical_line->data().toString() << ui->action_new_vertical_line->text();
////            qDebug() << action->text();
//            QIcon *icon_action_new_vertical_line = new QIcon();
//            changeSVGColor(icon_action_new_vertical_line, ":/svg/icons_svg/vertical_line.svg", "#CD4B00");
//            ui->action_new_vertical_line->setIcon(*icon_action_new_vertical_line);
//            QString color = "#CD4B00";
//            m_contex_menu_color.action_new_vertical_line=color;

//            QIcon *icon_action_new_param_line = new QIcon();
//            changeSVGColor(icon_action_new_param_line, ":/svg/icons_svg/polyline.svg", "black");
//            QString color_2 = "black";
//            ui->action_new_param_line->setIcon(*icon_action_new_param_line);
//            m_contex_menu_color.action_new_param_line=color_2;
//        }
//        else if(action == ui->action_new_param_line && m_contex_menu_color.action_new_param_line == "black")
//        {
////            m_test_counter_2++;
////            qDebug() << "hovered counter" << m_test_counter_2;
////            qDebug() << ui->action_new_param_line->data().toString() << ui->action_new_param_line->text();
////            qDebug() << action->text();
//            QIcon *icon_action_new_param_line = new QIcon();
//            changeSVGColor(icon_action_new_param_line, ":/svg/icons_svg/polyline.svg", "#CD4B00");
//            QString color = "#CD4B00";
//            ui->action_new_param_line->setIcon(*icon_action_new_param_line);
//            m_contex_menu_color.action_new_param_line=color;

//            QIcon *icon_action_new_vertical_line = new QIcon();
//            changeSVGColor(icon_action_new_vertical_line, ":/svg/icons_svg/vertical_line.svg", "black");
//            QString color_2 = "black";
//            ui->action_new_vertical_line->setIcon(*icon_action_new_vertical_line);
//            m_contex_menu_color.action_new_vertical_line=color_2;
//        }
//        // ================================================
//        if(action == ui->action_add_line_graph_lines && m_contex_menu_color.action_add_line_graph_lines == "black")
//        {
////            m_test_counter_2++;
////            qDebug() << "hovered counter" << m_test_counter_2;
////            qDebug() << ui->action_add_line_graph_lines->data().toString() << ui->action_add_line_graph_lines->text();
//            QIcon *icon_action_add_line_graph_lines = new QIcon();
//            changeSVGColor(icon_action_add_line_graph_lines, ":/svg/icons_svg/create.svg", "#CD4B00");
//            QString color = "#CD4B00";
//            ui->action_add_line_graph_lines->setIcon(*icon_action_add_line_graph_lines);
//            m_contex_menu_color.action_add_line_graph_lines=color;

//            QIcon *icon_action_delete_lines = new QIcon();
//            changeSVGColor(icon_action_delete_lines, ":/svg/icons_svg/trash.svg", "black");
//            QString color_2 = "black";
//            ui->action_delete_lines->setIcon(*icon_action_delete_lines);
//            m_contex_menu_color.action_delete_lines=color_2;
//        }
//        else if(action == ui->action_delete_lines && m_contex_menu_color.action_delete_lines == "black")
//        {
////            m_test_counter_2++;
////            qDebug() << "hovered counter" << m_test_counter_2;
////            qDebug() << ui->action_delete_lines->data().toString() << ui->action_delete_lines->text();
//            QIcon *icon_action_delete_lines = new QIcon();
//            changeSVGColor(icon_action_delete_lines, ":/svg/icons_svg/trash.svg", "#CD4B00");
//            QString color = "#CD4B00";
//            ui->action_delete_lines->setIcon(*icon_action_delete_lines);
//            m_contex_menu_color.action_delete_lines=color;

//            QIcon *icon_action_add_line_graph_lines = new QIcon();
//            changeSVGColor(icon_action_add_line_graph_lines, ":/svg/icons_svg/create.svg", "black");
//            QString color_2 = "black";
//            ui->action_add_line_graph_lines->setIcon(*icon_action_add_line_graph_lines);
//            m_contex_menu_color.action_add_line_graph_lines=color_2;
//        }
//        // ===========================================

//        if(action == ui->action_editing && m_contex_menu_color.action_editing == "black")
//        {
////            m_test_counter_2++;
////            qDebug() << "hovered counter" << m_test_counter_2;
////            qDebug() << ui->action_editing->data().toString() << ui->action_editing->text();
//            QIcon *icon_action_editing = new QIcon();
//            changeSVGColor(icon_action_editing, ":/svg/icons_svg/edit-image.svg", "#CD4B00");
//            QString color = "#CD4B00";
//            ui->action_editing->setIcon(*icon_action_editing);
//            m_contex_menu_color.action_editing=color;

//            QIcon *icon_action_save_this_graph = new QIcon();
//            changeSVGColor(icon_action_save_this_graph, ":/svg/icons_svg/png-file.svg", "black");
//            QString color_2 = "black";
//            ui->action_save_this_graph->setIcon(*icon_action_save_this_graph);
//            m_contex_menu_color.action_save_this_graph=color_2;
//        }
//        else if(action == ui->action_save_this_graph && m_contex_menu_color.action_save_this_graph == "black")
//        {
////            m_test_counter_2++;
////            qDebug() << "hovered counter" << m_test_counter_2;
////            qDebug() << ui->action_save_this_graph->data().toString() << ui->action_save_this_graph->text();
//            QIcon *icon_action_save_this_graph = new QIcon();
//            changeSVGColor(icon_action_save_this_graph, ":/svg/icons_svg/png-file.svg", "#CD4B00");
//            QString color = "#CD4B00";
//            ui->action_save_this_graph->setIcon(*icon_action_save_this_graph);
//            m_contex_menu_color.action_save_this_graph=color;

//            QIcon *icon_action_editing = new QIcon();
//            changeSVGColor(icon_action_editing, ":/svg/icons_svg/edit-image.svg", "black");
//            QString color_2 = "black";
//            ui->action_editing->setIcon(*icon_action_editing);
//            m_contex_menu_color.action_editing=color_2;
//        }

//    }
}

void GrapherGui::setAttributeGraph()
{
    ui->listAttribute->clear();
    ui->listLines->clear();
    m_vector_info.clear();
    m_iter_color = m_iter_color_begin;
//    m_is_edited = false;

//    setMenuToAddButton();
    qDebug() << "At time" << m_painter.m_is_at_time;

    int index = m_selected_graph;
    if(index > -1 && index < ui->listGraph->count())
    {
        if(index > -1 && index < ui->listGraph->count())
        {
            graph_meta_data data = getMetaDataVector();
            int index_vector = data.index_vector;
            bool isDefault = data.isDefault;

            QIcon *icon_reset = new QIcon(":/image/icons/restart.png");
            QIcon *icon_x = new QIcon(":/image/icons/x-mark.png");
//            QIcon *icon_check = new QIcon(":/image/icons/check.png");
            QIcon *icon_color = new QIcon(":/image/icons/color.png");

            if(m_settings.styleSheet)
            {
                changeSVGColor(icon_reset, ":/svg/icons_svg/restart.svg", m_attribute_button_color.color, m_attribute_button_color.border_color, m_attribute_button_color.size_border);
                changeSVGColor(icon_x, ":/svg/icons_svg/x-mark.svg", m_attribute_button_color.color, m_attribute_button_color.border_color, m_attribute_button_color.size_border);
//                changeSVGColor(icon_check, ":/svg/icons_svg/check.svg", m_attribute_button_color.color, m_attribute_button_color.border_color, m_attribute_button_color.size_border);
            }

            QValidator *validator = new QRegExpValidator(QRegExp("^([-]?[0-9][0-9]*|0)(\\.|,)[0-9]{9}"), this);

            // -=-=-=-=-=-=-=-=-= Шапка =-=-=-=-=-=-=-=-=-
            {
//                QPushButton *buttonApply = new QPushButton(" Применить");
                QPushButton *buttonReset = new QPushButton(" Сбросить");

//                buttonApply->setIcon(*icon_check);
                buttonReset->setIcon(*icon_reset);

                QHBoxLayout *hlayout = new QHBoxLayout;
//                hlayout->addWidget(buttonApply);
                hlayout->addWidget(buttonReset);
//                hlayout->addWidget(buttonCancel);

                QString name = "Заголовок графика";

                QLabel *label = new QLabel(name);
                QFont font;
                font.setPointSize(10);
                font.setBold(true);
                label->setFont(font);
//                label.setText(name);
                QLineEdit *edit = new QLineEdit;
                QVBoxLayout *vlayout = new QVBoxLayout;

                QPushButton *closeButton = new QPushButton();
                closeButton->setIcon(*icon_x);

                QHBoxLayout *exitLay = new QHBoxLayout;
//                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
                exitLay->addSpacerItem(r);
                exitLay->addWidget(closeButton);

                vlayout->addLayout(exitLay);
                vlayout->addWidget(label);
                vlayout->addWidget(edit);
                vlayout->addLayout(hlayout);

                QSize size;
                if(m_settings.styleSheet)
                {
                    size.setHeight(120);
                    size.setWidth(300);
                }
                else
                {
                    size.setHeight(120); //130
                    size.setWidth(300);
                }


                layoutItem infoLay;
                infoLay.lay = new QVBoxLayout;
                infoLay.lay->addLayout(vlayout);
                infoLay.edit_title = edit;
                infoLay.attr_size = size;

                m_vector_info.append(infoLay);

                m_startSettGraph.header = m_painter.m_vector_of_parameters[index_vector][index].main_title;
                edit->setText(m_startSettGraph.header);
                m_editingSettGraph.header = m_startSettGraph.header;

                QString name_head = name;
                QIcon *icon_head = new QIcon(":/image/icons/text.png");
                if(m_settings.styleSheet)
                {
                    changeSVGColor(icon_head, ":/svg/icons_svg/text.svg", m_attribute_color.color, m_attribute_color.border_color, m_attribute_color.size_border);
                }

                QListWidgetItem *item_head = new QListWidgetItem(*icon_head, name_head,  ui->listAttribute);


                connect(edit, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_main_title(const QString &)));
//                connect(buttonApply, SIGNAL(clicked()), this, SLOT(click_on_button_edit_apply_header()));
                connect(buttonReset, SIGNAL(clicked()), this, SLOT(click_on_button_edit_header_reset()));
                connect(closeButton, SIGNAL(clicked()), this, SLOT(click_on_button_edit_close()));
            }

            // -=-=-=-=-=-=-=-=-= Подпись ось X =-=-=-=-=-=-=-=-=-
            {
//                QPushButton *buttonApply = new QPushButton(" Применить");
                QPushButton *buttonReset = new QPushButton(" Сбросить");

//                buttonApply->setIcon(*icon_check);
                buttonReset->setIcon(*icon_reset);

                QHBoxLayout *hlayout = new QHBoxLayout;
//                hlayout->addWidget(buttonApply);
                hlayout->addWidget(buttonReset);
//                hlayout->addWidget(buttonCancel);

                QString name = "Подпись оси X";

                QLabel *label = new QLabel(name);
                QFont font;
                font.setPointSize(10);
                font.setBold(true);
                label->setFont(font);
                QLineEdit *edit = new QLineEdit;
                QVBoxLayout *vlayout = new QVBoxLayout;

                QPushButton *closeButton = new QPushButton();
                closeButton->setIcon(*icon_x);

                QHBoxLayout *exitLay = new QHBoxLayout;
//                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
                exitLay->addSpacerItem(r);
                exitLay->addWidget(closeButton);

                vlayout->addLayout(exitLay);
                vlayout->addWidget(label);
                vlayout->addWidget(edit);
                vlayout->addLayout(hlayout);

                QSize size;

                if(m_settings.styleSheet)
                {
                    size.setHeight(110);
                    size.setWidth(300);
                }
                else
                {
                    size.setHeight(110); //130
                    size.setWidth(300);
                }


                layoutItem infoLay;
                infoLay.lay = new QVBoxLayout;
                infoLay.lay->addLayout(vlayout);
                infoLay.edit_x = edit;
                infoLay.attr_size = size;

                m_vector_info.append(infoLay);

                m_startSettGraph.x_title = m_painter.m_vector_of_parameters[index_vector][index].x_title;

                m_startSettGraph.x_title = m_painter.m_vector_of_parameters[index_vector][index].x_title;
                edit->setText(m_startSettGraph.x_title);
                m_editingSettGraph.x_title = m_startSettGraph.x_title;

                QString name_xtitle = name;
                QIcon *icon_title = new QIcon(":/image/icons/text_horizontal.png");
                if(m_settings.styleSheet)
                {
                    changeSVGColor(icon_title, ":/svg/icons_svg/text_horizontal.svg", m_attribute_color.color, m_attribute_color.border_color, m_attribute_color.size_border);
                }
                QListWidgetItem *item_xtitle = new QListWidgetItem(*icon_title, name_xtitle,  ui->listAttribute);

                connect(edit, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_x_title(const QString &)));
//                connect(buttonApply, SIGNAL(clicked()), this, SLOT(click_on_button_edit_apply_x_title()));
                connect(buttonReset, SIGNAL(clicked()), this, SLOT(click_on_button_edit_x_title_reset()));
                connect(closeButton, SIGNAL(clicked()), this, SLOT(click_on_button_edit_close()));
            }

            // -=-=-=-=-=-=-=-=-= Подпись ось Y =-=-=-=-=-=-=-=-=-
            {
//                QPushButton *buttonApply = new QPushButton(" Применить");
                QPushButton *buttonReset = new QPushButton(" Сбросить");

//                buttonApply->setIcon(*icon_check);
                buttonReset->setIcon(*icon_reset);

                QHBoxLayout *hlayout = new QHBoxLayout;
//                hlayout->addWidget(buttonApply);
                hlayout->addWidget(buttonReset);
//                hlayout->addWidget(buttonCancel);

                QString name = "Подпись оси Y";

                QLabel *label = new QLabel(name);
                QFont font;
                font.setPointSize(10);
                font.setBold(true);
                label->setFont(font);
                QLineEdit *edit = new QLineEdit;
                QVBoxLayout *vlayout = new QVBoxLayout;

                QPushButton *closeButton = new QPushButton();
                closeButton->setIcon(*icon_x);

                QHBoxLayout *exitLay = new QHBoxLayout;
//                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
                exitLay->addSpacerItem(r);
                exitLay->addWidget(closeButton);

                vlayout->addLayout(exitLay);
                vlayout->addWidget(label);
                vlayout->addWidget(edit);
                vlayout->addLayout(hlayout);

                QSize size;
                if(m_settings.styleSheet)
                {
                    size.setHeight(110);
                    size.setWidth(300);
                }
                else
                {
                    size.setHeight(110); //130
                    size.setWidth(300);
                }

                layoutItem infoLay;
                infoLay.lay = new QVBoxLayout;
                infoLay.lay->addLayout(vlayout);
                infoLay.edit_y = edit;
                infoLay.attr_size = size;

                m_vector_info.append(infoLay);

//                m_startSettGraph.y_title = m_painter.m_vector_of_parameters[index_vector][index].y_title;

                m_startSettGraph.y_title = m_painter.m_vector_of_parameters[index_vector][index].y_title;
                edit->setText(m_startSettGraph.y_title);
                m_editingSettGraph.y_title = m_startSettGraph.y_title;

                QIcon *icon_title_y = new QIcon(":/image/icons/text_vertical_180.png");
                if(m_settings.styleSheet)
                {
                    changeSVGColor(icon_title_y, ":/svg/icons_svg/text_vertical_180.svg", m_attribute_color.color, m_attribute_color.border_color, m_attribute_color.size_border);
                }
                QString name_ytitle = name;
                QListWidgetItem *item_ytitle = new QListWidgetItem(*icon_title_y, name_ytitle,  ui->listAttribute);

                connect(edit, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_y_title(const QString &)));
//                connect(buttonApply, SIGNAL(clicked()), this, SLOT(click_on_button_edit_apply_y_title()));
                connect(buttonReset, SIGNAL(clicked()), this, SLOT(click_on_button_edit_y_title_reset()));
                connect(closeButton, SIGNAL(clicked()), this, SLOT(click_on_button_edit_close()));
            }
            if(m_painter.m_is_at_time)
            {
                // -=-=-=-=-=-=-=-=-= Подпись ось Y2 =-=-=-=-=-=-=-=-=-
                {
    //                QPushButton *buttonApply = new QPushButton(" Применить");
                    QPushButton *buttonReset = new QPushButton(" Сбросить");

    //                buttonApply->setIcon(*icon_check);
                    buttonReset->setIcon(*icon_reset);

                    QHBoxLayout *hlayout = new QHBoxLayout;
    //                hlayout->addWidget(buttonApply);
                    hlayout->addWidget(buttonReset);
    //                hlayout->addWidget(buttonCancel);

                    QString name = "Подпись оси Y2";

                    QLabel *label = new QLabel(name);
                    QFont font;
                    font.setPointSize(10);
                    font.setBold(true);
                    label->setFont(font);
                    QLineEdit *edit = new QLineEdit;
                    QVBoxLayout *vlayout = new QVBoxLayout;

                    QPushButton *closeButton = new QPushButton();
                    closeButton->setIcon(*icon_x);

                    QHBoxLayout *exitLay = new QHBoxLayout;
//                    QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
                    QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
                    exitLay->addSpacerItem(r);
                    exitLay->addWidget(closeButton);

                    vlayout->addLayout(exitLay);
                    vlayout->addWidget(label);
                    vlayout->addWidget(edit);
                    vlayout->addLayout(hlayout);

                    QSize size;
                    if(m_settings.styleSheet)
                    {
                        size.setHeight(110);
                        size.setWidth(300);
                    }
                    else
                    {
                        size.setHeight(110); //130
                        size.setWidth(300);
                    }

                    layoutItem infoLay;
                    infoLay.lay = new QVBoxLayout;
                    infoLay.lay->addLayout(vlayout);
                    infoLay.edit_y2 = edit;
                    infoLay.attr_size = size;

                    m_vector_info.append(infoLay);

                    m_startSettGraph.y2_title = m_painter.m_vector_of_parameters[index_vector][index].second_y_title;

                    m_startSettGraph.y2_title = m_painter.m_vector_of_parameters[index_vector][index].second_y_title;
                    edit->setText(m_startSettGraph.y2_title);
                    m_editingSettGraph.y2_title = m_startSettGraph.y2_title;

                    QIcon *icon_title_y2 = new QIcon(":/image/icons/text_vertical.png");
                    if(m_settings.styleSheet)
                    {
                        changeSVGColor(icon_title_y2, ":/svg/icons_svg/text_vertical.svg", m_attribute_color.color, m_attribute_color.border_color, m_attribute_color.size_border);
                    }
                    QString name_y2title = name;
                    QListWidgetItem *item_y2title = new QListWidgetItem(*icon_title_y2, name_y2title,  ui->listAttribute);


                    connect(edit, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_second_y_title(const QString &)));
    //                connect(buttonApply, SIGNAL(clicked()), this, SLOT(click_on_button_edit_apply_second_y_title()));
                    connect(buttonReset, SIGNAL(clicked()), this, SLOT(click_on_button_edit_second_y_title_reset()));
                    connect(closeButton, SIGNAL(clicked()), this, SLOT(click_on_button_edit_close()));
                }
            }

            // -=-=-=-=-=-=-=-=-= Позиция легенды =-=-=-=-=-=-=-=-=-
            {
                QPushButton *buttonReset = new QPushButton(" Сбросить");
                buttonReset->setIcon(*icon_reset);
//                QPushButton *buttonCancel = new QPushButton("Отмена");
                QHBoxLayout *hlayout = new QHBoxLayout;
                hlayout->addWidget(buttonReset);

//                hlayout->addWidget(buttonCancel);

                QString name = "Позиция легенды";

                QLabel *label = new QLabel(name);
                QFont font;
                font.setPointSize(10);
                font.setBold(true);
                label->setFont(font);
//                QLineEdit *edit = new QLineEdit;
                QComboBox *combo = new QComboBox;

                QMapIterator<QString, Painter::painter_position> position(m_painter.m_map_position);
                while (position.hasNext()) {
                    position.next();

                    combo->addItem(position.value().position_rus);
                    if(position.value().position == m_painter.m_vector_of_parameters[index_vector][index].pos_legend)
                    {
                        combo->setCurrentIndex(combo->count() -1);
                    }
                }


                QVBoxLayout *vlayout = new QVBoxLayout;

                QPushButton *closeButton = new QPushButton();
                closeButton->setIcon(*icon_x);

                QHBoxLayout *exitLay = new QHBoxLayout;
//                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
                QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
                exitLay->addSpacerItem(r);
                exitLay->addWidget(closeButton);

                vlayout->addLayout(exitLay);
                vlayout->addWidget(label);
                vlayout->addWidget(combo);
                vlayout->addLayout(hlayout);

                QSize size;
                if(m_settings.styleSheet)
                {
                    size.setHeight(110);
                    size.setWidth(300);
                }
                else
                {
                    size.setHeight(110); //130
                    size.setWidth(300);
                }

                layoutItem infoLay;
                infoLay.lay = new QVBoxLayout;
                infoLay.lay->addLayout(vlayout);
                infoLay.attr_size = size;

                infoLay.comboPosLegend = combo;

                m_vector_info.append(infoLay);

                m_startSettGraph.pos_legend = m_painter.m_vector_of_parameters[index_vector][index].pos_legend;

                QString name_legend = name;
                QIcon *icon_legend = new QIcon();
                setLegendIcon(icon_legend, m_painter.m_vector_of_parameters[index_vector][index].pos_legend);
                QListWidgetItem *item_legend = new QListWidgetItem(*icon_legend, name_legend,  ui->listAttribute);
//                item_legend->setFlags(item_legend->flags() | Qt::ItemIsDragEnabled);

                connect(combo, SIGNAL(activated(int)), this, SLOT(selectPosLegend(int)));
                connect(buttonReset, SIGNAL(clicked()), this, SLOT(click_on_button_edit_legend_reset()));
                connect(closeButton, SIGNAL(clicked()), this, SLOT(click_on_button_edit_close()));
            }

            if(m_painter.m_is_at_time)
            {
                // -=-=-=-=-=-=-=-=-= Вертикальные линии =-=-=-=-=-=-=-=-=-
                {

                    int size_vlines = m_painter.m_vector_of_parameters[index_vector][index].vlines.size();
        //                QString name_vertical_line = "[name_vertical_line]";
                    QIcon *icon_vertical = new QIcon(":/image/icons/vertical_line.png");
                    if(m_settings.styleSheet)
                    {
                        changeSVGColor(icon_vertical, ":/svg/icons_svg/vertical_line.svg", m_attribute_color.color, m_attribute_color.border_color, m_attribute_color.size_border);
                    }
                    for(int i = 0; i < size_vlines;  i++)
                    {
                        QPushButton *buttonReset = new QPushButton(" Сброс");
                        buttonReset->setIcon(*icon_reset);

                        QHBoxLayout *hlayout = new QHBoxLayout;
    //                    hlayout->addWidget(buttonReset);
    //                    hlayout->addWidget(buttonCancel);

                        QString name = "Вертикальная линия:\n" + m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text;

                        QLabel *label = new QLabel(name);
                        QFont font;
                        font.setPointSize(10);
                        font.setBold(true);
                        label->setFont(font);
        //                QLineEdit *edit = new QLineEdit;
    //                    QComboBox *combo = new QComboBox;
                        QVBoxLayout *vlayout = new QVBoxLayout;

                        QPushButton *closeButton = new QPushButton();
                        closeButton->setIcon(*icon_x);

                        QHBoxLayout *exitLay = new QHBoxLayout;
                        QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);

                        QFont font_text;
                        font.setPointSize(10);

                        // =-=-=-=-=-=-=-=-= Линия =-=-=-=-=-=-=-=-=
                        QGroupBox *lineBox = new QGroupBox("Линия");

                        QComboBox *comboColor = new QComboBox;
                        QComboBox *comboDash = new QComboBox;
                        QPushButton *buttonColor = new QPushButton("");
                        QSpinBox *spinBoxWidth = new QSpinBox;
                        QLineEdit *edit_point_x = new QLineEdit;
                        QLineEdit *edit_y_min = new QLineEdit;
                        QLineEdit *edit_y_max = new QLineEdit;
                        QCheckBox *check_y = new QCheckBox;


                        QRegularExpression hexMatcher("^#(?:[0-9a-fA-F]{3}){1,2}$", QRegularExpression::CaseInsensitiveOption);
                        QRegularExpressionMatch match = hexMatcher.match(m_painter.m_vector_of_parameters[index_vector][index].vlines[i].color);

                        comboColor->addItem("Не выбран");
                        for(int ind_color = 0; ind_color < m_painter.m_colors.size(); ind_color++)
                        {
                            comboColor->addItem(m_painter.m_map_colors[m_painter.m_colors[ind_color]].color_rus);

                            if(match.hasMatch())
                            {
                                comboColor->setItemText(0, m_painter.m_vector_of_parameters[index_vector][index].vlines[i].color);
                            }
                            else
                            {
                                if(m_painter.m_map_colors[m_painter.m_colors[ind_color]].color == m_painter.m_vector_of_parameters[index_vector][index].vlines[i].color)
                                {
                                    comboColor->setItemText(0, m_painter.m_map_colors[m_painter.m_colors[ind_color]].color_rus);
                                }
                            }
                        }

                        comboColor->setCurrentIndex(0);

                        QMapIterator<QString, Painter::painter_penStyle> penStyle(m_painter.m_map_penStyle);
                        while (penStyle.hasNext()) {
                            penStyle.next();
                            comboDash->addItem(penStyle.value().penStyle_rus);

                            if(penStyle.value().penStyle == m_painter.m_vector_of_parameters[index_vector][index].vlines[i].dash)
                            {
                                comboDash->setCurrentIndex(comboDash->count()-1);
                            }
                        }



                        spinBoxWidth->setMinimum(1);
                        spinBoxWidth->setMaximum(100);
                        spinBoxWidth->setValue(m_painter.m_vector_of_parameters[index_vector][index].vlines[i].width);
                        edit_point_x->setText(QString::number(m_painter.m_vector_of_parameters[index_vector][index].vlines[i].point_x));

                        double y_min = m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_min;
                        double y_max = m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_max;

                        edit_y_min->setText(QString::number(y_min));
                        edit_y_max->setText(QString::number(y_max));

    //                    qDebug() << "y_min" << y_min << "y_max" << y_max;

                        if(y_min == 0 && y_max == 0)
                        {
                            check_y->setChecked(true);
                            edit_y_min->setEnabled(false);
                            edit_y_max->setEnabled(false);
                        }
                        else
                        {
                            check_y->setChecked(false);
                            edit_y_min->setEnabled(true);
                            edit_y_max->setEnabled(true);
                        }



                        edit_point_x->setValidator(validator);
                        edit_y_min->setValidator(validator);
                        edit_y_max->setValidator(validator);

                        buttonColor->setIcon(*icon_color);
                        buttonColor->setMaximumSize(30, 30);

                        QLabel *labelColor = new QLabel("Цвет");
                        QLabel *labelDash = new QLabel("Тип");
                        QLabel *labelWidth = new QLabel("Толщина");
                        QLabel *labelPoint_x = new QLabel("Точка Х");
                        QLabel *labelY_min = new QLabel("Y min");
                        QLabel *labelY_max = new QLabel("Y max");
                        QLabel *labelCheck = new QLabel("Автоматически задавать Y");


                        labelColor->setFont(font_text);
                        labelDash->setFont(font_text);
                        labelWidth->setFont(font_text);
                        labelPoint_x->setFont(font_text);
                        labelY_min->setFont(font_text);
                        labelY_max->setFont(font_text);
                        labelCheck->setFont(font_text);

                        QVBoxLayout *vLayLine = new QVBoxLayout;
                        QHBoxLayout *hLayColor = new QHBoxLayout;
                        QHBoxLayout *hLayDash = new QHBoxLayout;
                        QHBoxLayout *hLayWidth = new QHBoxLayout;
                        QHBoxLayout *hLayPoint_x = new QHBoxLayout;
                        QHBoxLayout *hLaySize_y = new QHBoxLayout;
                        QHBoxLayout *hLayCheck = new QHBoxLayout;

                        hLayColor->addWidget(labelColor);
                        hLayColor->addWidget(comboColor);
                        hLayColor->addWidget(buttonColor);

                        hLayDash->addWidget(labelDash);
                        hLayDash->addWidget(comboDash);

                        hLayWidth->addWidget(labelWidth);
                        hLayWidth->addWidget(spinBoxWidth);

                        hLayPoint_x->addWidget(labelPoint_x);
                        hLayPoint_x->addWidget(edit_point_x);

                        hLayCheck->addWidget(labelCheck);
                        hLayCheck->addWidget(check_y);

                        hLaySize_y->addWidget(labelY_min);
                        hLaySize_y->addWidget(edit_y_min);
                        hLaySize_y->addWidget(labelY_max);
                        hLaySize_y->addWidget(edit_y_max);

                        vLayLine->addLayout(hLayColor);
                        vLayLine->addLayout(hLayDash);
                        vLayLine->addLayout(hLayWidth);
                        vLayLine->addLayout(hLayPoint_x);
                        vLayLine->addLayout(hLayCheck);
                        vLayLine->addLayout(hLaySize_y);


                        lineBox->setLayout(vLayLine);


                        // =-=-=-=-=-=-=-=-= Текст =-=-=-=-=-=-=-=-=
                        QGroupBox *textBox = new QGroupBox("Текст линии");

                        QLineEdit *textEdit = new QLineEdit;
                        QComboBox *comboOri = new QComboBox;
                        QSpinBox *spinBoxFontSize = new QSpinBox;
                        QLineEdit *text_x = new QLineEdit;
                        QLineEdit *text_y = new QLineEdit;

                        textEdit->setText(m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text);

                        QMapIterator<QString, Painter::painter_orientation> orientation(m_painter.m_map_orientation);
                        while (orientation.hasNext()) {
                            orientation.next();
                            comboOri->addItem(orientation.value().orientation_rus);

                            if(orientation.value().orientation == m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_orientation)
                            {
                                comboOri->setCurrentIndex(comboOri->count()-1);
                            }
                        }

//                        comboOri->addItems(m_painter.m_text_orientation_rus);
//                        QString orient = m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_orientation;
    //                    qDebug() << orient << m_painter.m_text_orientation.indexOf(orient);
//                        comboOri->setCurrentIndex(m_painter.m_text_orientation.indexOf(orient));

                        spinBoxFontSize->setValue(m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_font);
                        text_x->setText(QString::number(m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_x));
                        text_y->setText(QString::number(m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_y));

                        spinBoxFontSize->setMaximum(100);

                        text_x->setValidator(validator);
                        text_y->setValidator(validator);

                        QLabel *labelTextEdit = new QLabel("Текст");
                        QLabel *labelTextOrient = new QLabel("Ориентация");
                        QLabel *labelTextFont = new QLabel("Шрифт");
                        QLabel *labelTextCoord = new QLabel("Координаты x:");
                        QLabel *labelTexty = new QLabel("y:");

                        labelTextEdit->setFont(font_text);
                        labelTextOrient->setFont(font_text);
                        labelTextFont->setFont(font_text);
                        labelTextCoord->setFont(font_text);
                        labelTexty->setFont(font_text);

                        QVBoxLayout *vLayText = new QVBoxLayout;
                        QHBoxLayout *hLayTextEdit = new QHBoxLayout;
                        QHBoxLayout *hLayOrientation = new QHBoxLayout;
                        QHBoxLayout *hLayFontSize = new QHBoxLayout;
                        QHBoxLayout *hLayText_x_y = new QHBoxLayout;

                        hLayTextEdit->addWidget(labelTextEdit);
                        hLayTextEdit->addWidget(textEdit);

                        hLayOrientation->addWidget(labelTextOrient);
                        hLayOrientation->addWidget(comboOri);

                        hLayFontSize->addWidget(labelTextFont);
                        hLayFontSize->addWidget(spinBoxFontSize);

                        hLayText_x_y->addWidget(labelTextCoord);
                        hLayText_x_y->addWidget(text_x);
                        hLayText_x_y->addWidget(labelTexty);
                        hLayText_x_y->addWidget(text_y);

                        vLayText->addLayout(hLayTextEdit);
                        vLayText->addLayout(hLayOrientation);
                        vLayText->addLayout(hLayFontSize);
                        vLayText->addLayout(hLayText_x_y);
                        textBox->setLayout(vLayText);

                        exitLay->addSpacerItem(r);
                        exitLay->addWidget(closeButton);

                        vlayout->addLayout(exitLay);
    //                    vlayout->addWidget(label);
                        vlayout->addWidget(lineBox);
                        vlayout->addWidget(textBox);
                        vlayout->addLayout(hlayout);


                        QSize size;
                        if(m_settings.styleSheet)
                        {
                            size.setHeight(390);
                            size.setWidth(300);
                        }
                        else
                        {
                            size.setHeight(390); //430
                            size.setWidth(300);
                        }

                        layoutItem infoLay;
                        infoLay.lay = new QVBoxLayout;
                        infoLay.index = i;
                        infoLay.lay->addLayout(vlayout);
                        infoLay.attr_size = size;
                        infoLay.size_vlines = size_vlines;

                        infoLay.vLine.index_vline = i;

                        infoLay.vLine.comboColor = comboColor;
                        infoLay.vLine.comboDash = comboDash;
                        infoLay.vLine.spinBoxWidth = spinBoxWidth;
                        infoLay.vLine.edit_point_x = edit_point_x;
                        infoLay.vLine.edit_y_min = edit_y_min;
                        infoLay.vLine.edit_y_max = edit_y_max;
                        infoLay.vLine.check_y = check_y;

                        infoLay.vLine.textEdit = textEdit;
                        infoLay.vLine.comboOri = comboOri;
                        infoLay.vLine.spinBoxFontSize = spinBoxFontSize;
                        infoLay.vLine.text_x = text_x;
                        infoLay.vLine.text_y = text_y;

                        infoLay.vLine.isEmpty = false;

                        m_vector_info.append(infoLay);


    //                    m_attr_size.append(size);

                        QListWidgetItem *item_vertical = new QListWidgetItem(*icon_vertical, m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text,  ui->listAttribute);
//                        item_vertical->setFlags(item_vertical->flags() | Qt::ItemIsDragEnabled);

                        connect(buttonColor, SIGNAL(clicked()), this, SLOT(click_on_button_edit_vline_color()));
                        connect(comboColor, SIGNAL(activated(int)), this, SLOT(selectVlineColor(int)));
                        connect(comboDash, SIGNAL(activated(int)), this, SLOT(selectVlineDash(int)));
                        connect(spinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(spinBox_changed_vline_width(int)));
                        connect(edit_point_x, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_vline_x_point(const QString &)));
                        connect(edit_y_min, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_vline_y_min(const QString &)));
                        connect(edit_y_max, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_vline_y_max(const QString &)));
                        connect(check_y, SIGNAL(stateChanged(int)), this, SLOT(checked_edit_vline_auto_y(int)));

                        connect(textEdit, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_vline_text(const QString &)));
                        connect(comboOri, SIGNAL(activated(int)), this, SLOT(select_text_orientation(int)));
                        connect(spinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(spinBox_changed_vline_sizeFont(int)));
                        connect(text_x, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_vline_text_x(const QString &)));
                        connect(text_y, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_vline_text_y(const QString &)));


                        connect(closeButton, SIGNAL(clicked()), this, SLOT(click_on_button_edit_close()));
                    }
                }
            }


            // -=-=-=-=-=-=-=-=-= Линии параметров =-=-=-=-=-=-=-=-=-
            {

                QIcon *icon_curve = new QIcon();

//                QListWidgetItem *item_legend = new QListWidgetItem(*icon_legend, name_legend,  ui->listAttribute);


//                QIcon *icon_curve = new QIcon(":/image/icons/polyline.png");
//                QIcon *icon_curve = new QIcon(":/image/icons/polyline.svg");

//                QMapIterator<QString, QVector<Painter::paramCurve>> set_curve(m_painter.m_vector_of_parameters[index_vector][index].map_params_curve);
                QVectorIterator<QPair<QString, Painter::paramCurve>> set_curve(m_painter.m_vector_of_parameters[index_vector][index].line_params_curve);
                int index_curve = -1;
                while (set_curve.hasNext()) {
                    int height_widget = 0;
                    QPair<QString, Painter::paramCurve> curve = set_curve.next();
                    index_curve++;
//                    set_curve.i;
                    get_next_color_from_iter();
                    QPushButton *buttonReset = new QPushButton(" Сброс");
                    buttonReset->setIcon(*icon_reset);

                    QHBoxLayout *hlayout = new QHBoxLayout;

                    QFont font_text;
                    font_text.setPointSize(10);

                    QString paramName;
                    QString listLinesAttrName;

                    if(curve.second.param_line.isEmpty())
                    {
                        paramName = "x: " + curve.second.param_x + " y: " + curve.second.param_y;
                        listLinesAttrName = "Новый график";

                    }
                    else
                    {
                        if(m_settings.useRusTitle)
                        {
                            for(int index_table = 0; index_table < m_painter.m_tables.size(); index_table++)
                            {
                                if(m_painter.m_tables[index_table]->m_group == curve.first)
                                {

                                  paramName = m_painter.m_tables[index_table]->engTitleToRus(curve.second.param_line);
                                  listLinesAttrName = paramName;
                                }
                            }
                        }
                        else
                        {
                            paramName = curve.second.param_line;
                            listLinesAttrName = paramName;
                        }

                    }


                    QLabel *labelName = new QLabel("Параметр: " + paramName);

                    QString name_key;
                    for(int ind_key = 0; ind_key < m_vector_key.size(); ind_key++)
                    {
                        if(m_vector_key[ind_key].key == curve.first)
                        {
                            name_key = m_vector_key[ind_key].name;
                        }

                    }

                    QLabel *labelKey = new QLabel("Принадлежит: " + name_key);

                    height_widget += labelName->sizeHint().height();
                    height_widget += labelKey->sizeHint().height();

                    labelName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelKey->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);


                    labelName->setFont(font_text);
                    labelKey->setFont(font_text);

                    QLineEdit *lineEditCurveTitle = new QLineEdit;
                    QComboBox *comboColor = new QComboBox;
                    QPushButton *buttonColor = new QPushButton("");
                    QCheckBox *checkBoxisSecondY = new QCheckBox;

                    lineEditCurveTitle->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    comboColor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    buttonColor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    checkBoxisSecondY->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

//                    height_widget += lineEditCurveTitle->sizeHint().height();
//                    height_widget += comboColor->sizeHint().height();
//                    height_widget += buttonColor->sizeHint().height();
//                    height_widget += checkBoxisSecondY->sizeHint().height();

                    checkBoxisSecondY->setChecked(curve.second.is_second_y);


                    lineEditCurveTitle->setText(listLinesAttrName);
//                    lineEditCurveTitle->setText(curve.second.name_line);



                    QRegularExpression hexMatcher("^#(?:[0-9a-fA-F]{3}){1,2}$", QRegularExpression::CaseInsensitiveOption);
                    QRegularExpressionMatch match = hexMatcher.match(curve.second.color_line);

                    comboColor->addItem("Не выбран");

                    for(int ind_color = 0; ind_color < m_painter.m_colors.size(); ind_color++)
                    {
                        comboColor->addItem(m_painter.m_map_colors[m_painter.m_colors[ind_color]].color_rus);

                        if(match.hasMatch())
                        {
                            comboColor->setItemText(0, curve.second.color_line);
                        }
                        else
                        {
                            if(m_painter.m_map_colors[m_painter.m_colors[ind_color]].color == curve.second.color_line)
                            {
                                comboColor->setItemText(0, m_painter.m_map_colors[m_painter.m_colors[ind_color]].color_rus);
                            }
                        }
                    }

                    comboColor->setCurrentIndex(0);


                    QLabel *labelCurvetitle = new QLabel("Название в легенде");
                    QLabel *labelColor = new QLabel("Цвет");
                    QLabel *labelisSecondY = new QLabel("Вторая ось Y");

                    labelCurvetitle->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelColor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelisSecondY->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);



                    buttonColor->setIcon(*icon_color);
                    buttonColor->setMaximumSize(30, 30);

                    QHBoxLayout *hLayCurveTitle = new QHBoxLayout;
                    QHBoxLayout *hLayColor = new QHBoxLayout;
                    QHBoxLayout *hLayisSecondY = new QHBoxLayout;


                    hLayCurveTitle->addWidget(labelCurvetitle);
                    hLayCurveTitle->addWidget(lineEditCurveTitle);

                    hLayColor->addWidget(labelColor);
                    hLayColor->addWidget(comboColor);
                    hLayColor->addWidget(buttonColor);

                    hLayisSecondY->addWidget(labelisSecondY);
                    hLayisSecondY->addWidget(checkBoxisSecondY);

                    // =-=-=-=-=-=-=-=-= Линия =-=-=-=-=-=-=-=-=
                    QGroupBox *lineBox = new QGroupBox("Линия");
                    QCheckBox *checkBoxisLine = new QCheckBox;
                    QComboBox *comboDash = new QComboBox;
                    QComboBox *comboParam_x = new QComboBox;
                    QComboBox *comboParam_y = new QComboBox;

                    lineBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    checkBoxisLine->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    comboDash->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    comboParam_x->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    comboParam_y->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

//                    height_widget += lineBox->sizeHint().height();
//                    height_widget += checkBoxisLine->sizeHint().height();
//                    height_widget += comboDash->sizeHint().height();
//                    height_widget += comboParam_x->sizeHint().height();
//                    height_widget += comboParam_y->sizeHint().height();

                    QSpinBox *spinBoxWidth = new QSpinBox;

                    if(curve.second.width_line < 0)
                    {
                        checkBoxisLine->setChecked(false);
                        comboDash->setEnabled(false);
                        spinBoxWidth->setEnabled(false);
                    }
                    else
                    {
                        checkBoxisLine->setChecked(true);
                        comboDash->setEnabled(true);
                        spinBoxWidth->setEnabled(true);
                    }

                    QStringList all_params;




                    for(int group = 0; group < m_painter.m_tables.size(); group++)
                    {
                        if(m_painter.m_tables[group]->m_group == curve.first)
                        {
                            QString lang = "eng";
                            if(m_painter.m_tables[group]->m_title.contains(m_language_param) && m_settings.useRusTitle)
                            {
                                lang = m_language_param;
                            }
                            else
                            {
                                lang = "eng";
                            }
                            for(int param = 0; param < m_painter.m_tables[group]->m_title[lang].size(); param++)
                            {
//                                if(!name_of_graph.contains(m_painter.m_tables[i]->m_title[lang][param].name))
                                {
                                    all_params.append(m_painter.m_tables[group]->m_title[lang][param].name);
                                }
                            }
                        }
                    }

                    if(!m_painter.m_is_at_time)
                    {



                        comboParam_x->addItems(all_params);
                        comboParam_y->addItems(all_params);

                        comboParam_x->setCurrentIndex(all_params.indexOf(curve.second.param_x));
                        comboParam_y->setCurrentIndex(all_params.indexOf(curve.second.param_y));
                        qDebug() << "=-=-=-=-=-=-=-=-=-=-=-=-=-=\nkey" << curve.first;
                        qDebug() << "curve.second.param_x" << curve.second.param_x << "curve.second.param_y" << curve.second.param_y;
                        qDebug() << "ind .param_x" << all_params.indexOf(curve.second.param_x) << "ind .param_y" << all_params.indexOf(curve.second.param_y);
                        qDebug() << "\n" << all_params;

                    }


                    QMapIterator<QString, Painter::painter_penStyle> penStyle(m_painter.m_map_penStyle);
                    while (penStyle.hasNext()) {
                        penStyle.next();
                        comboDash->addItem(penStyle.value().penStyle_rus);

                        if(penStyle.value().penStyle == curve.second.dash_line)
                        {
                            comboDash->setCurrentIndex(comboDash->count()-1);
                        }
                    }


                    spinBoxWidth->setMinimum(1);
                    spinBoxWidth->setMaximum(100);
                    spinBoxWidth->setValue(curve.second.width_line);

                    QLabel *labelOnlyMarker = new QLabel("Линия");
                    QLabel *labelDash = new QLabel("Тип");
                    QLabel *labelWidth = new QLabel("Толщина");
                    QLabel *labelParam_x = new QLabel("X:");
                    QLabel *labelParam_y = new QLabel("Y:");

                    labelOnlyMarker->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelDash->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelWidth->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelParam_x->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelParam_y->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

                    labelColor->setFont(font_text);
                    labelDash->setFont(font_text);
                    labelWidth->setFont(font_text);


                    QVBoxLayout *vLayLine = new QVBoxLayout;
                    QHBoxLayout *hLayParams = new QHBoxLayout;
                    QHBoxLayout *hLayCheckBoxisLine = new QHBoxLayout;
                    QHBoxLayout *hLayDash = new QHBoxLayout;
                    QHBoxLayout *hLayWidth = new QHBoxLayout;

                    height_widget += hLayParams->sizeHint().height();
                    height_widget += hLayCheckBoxisLine->sizeHint().height();
                    height_widget += hLayDash->sizeHint().height();
                    height_widget += hLayWidth->sizeHint().height();

                    hLayCheckBoxisLine->addWidget(labelOnlyMarker);
                    hLayCheckBoxisLine->addWidget(checkBoxisLine);

                    hLayParams->addWidget(labelParam_x);
                    hLayParams->addWidget(comboParam_x);
                    hLayParams->addWidget(labelParam_y);
                    hLayParams->addWidget(comboParam_y);

                    hLayDash->addWidget(labelDash);
                    hLayDash->addWidget(comboDash);

                    hLayWidth->addWidget(labelWidth);
                    hLayWidth->addWidget(spinBoxWidth);

//                        vLayLine->addLayout();

                    vLayLine->addLayout(hLayCheckBoxisLine);
                    vLayLine->addLayout(hLayDash);
                    vLayLine->addLayout(hLayWidth);

                    lineBox->setLayout(vLayLine);


                    // =-=-=-=-=-=-=-=-= Маркер =-=-=-=-=-=-=-=-=
                    QGroupBox *lineDotBox = new QGroupBox("Маркер");
                    QCheckBox *checkBoxisMarkers = new QCheckBox;
                    QCheckBox *checkBoxisBorder = new QCheckBox;
                    QComboBox *comboStyleDot = new QComboBox;
                    QSpinBox *spinBoxWidthDot = new QSpinBox;

                    lineDotBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    checkBoxisMarkers->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    checkBoxisBorder->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    comboStyleDot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    spinBoxWidthDot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);



                    checkBoxisMarkers->setChecked(curve.second.is_dots);
                    checkBoxisBorder->setChecked(curve.second.is_Border);
                    comboStyleDot->setEnabled(curve.second.is_dots);
                    spinBoxWidthDot->setEnabled(curve.second.is_dots);


                    QMapIterator<QString, Painter::painter_style_symbols> style_symbols(m_painter.m_map_style_symbols);
                    while (style_symbols.hasNext()) {
                        style_symbols.next();
                        comboStyleDot->addItem(style_symbols.value().style_symbols_rus);

                        if(style_symbols.value().style_symbols == curve.second.dots_style)
                        {
                            comboStyleDot->setCurrentIndex(comboStyleDot->count()-1);
                        }
                    }


//                        comboStyleDot->addItems(m_painter.m_style_symbols_rus);
//                        comboStyleDot->setCurrentIndex(m_painter.m_style_symbols.indexOf(set_curve.value()[i].dots_style));
                    spinBoxWidthDot->setMaximum(100);
                    spinBoxWidthDot->setValue(curve.second.dots_size);


                    QLabel *labelCheckBoxisMarkers = new QLabel("Маркеры");
                    QLabel *labelCheckBoxisBorder = new QLabel("Контур");
                    QLabel *labelStyleDot = new QLabel("Стиль");
                    QLabel *labelWidthDot = new QLabel("Толщина");

                    labelCheckBoxisMarkers->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelCheckBoxisBorder->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelStyleDot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                    labelWidthDot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

                    QVBoxLayout *vLayLineDot = new QVBoxLayout;
                    QHBoxLayout *hLayCheckBoxMarker = new QHBoxLayout;
                    QHBoxLayout *hLayCheckBoxBorder = new QHBoxLayout;
                    QHBoxLayout *hLayStyleDot = new QHBoxLayout;
                    QHBoxLayout *hLayWidthDot = new QHBoxLayout;

                    height_widget += hLayCheckBoxMarker->sizeHint().height();
                    height_widget += hLayCheckBoxBorder->sizeHint().height();
                    height_widget += hLayStyleDot->sizeHint().height();
                    height_widget += hLayWidthDot->sizeHint().height();

                    hLayCheckBoxMarker->addWidget(labelCheckBoxisMarkers);
                    hLayCheckBoxMarker->addWidget(checkBoxisMarkers);

                    hLayCheckBoxBorder->addWidget(labelCheckBoxisBorder);
                    hLayCheckBoxBorder->addWidget(checkBoxisBorder);

                    hLayStyleDot->addWidget(labelStyleDot);
                    hLayStyleDot->addWidget(comboStyleDot);

                    hLayWidthDot->addWidget(labelWidthDot);
                    hLayWidthDot->addWidget(spinBoxWidthDot);

                    vLayLineDot->addLayout(hLayCheckBoxMarker);
                    vLayLineDot->addLayout(hLayCheckBoxBorder);
                    vLayLineDot->addLayout(hLayStyleDot);
                    vLayLineDot->addLayout(hLayWidthDot);

                    lineDotBox->setLayout(vLayLineDot);

                    QVBoxLayout *vlayout = new QVBoxLayout;

                    QPushButton *closeButton = new QPushButton();
                    closeButton->setIcon(*icon_x);

//                    height_widget += vLayLineDot->sizeHint().height();
//                    height_widget += vlayout->sizeHint().height();

                    QHBoxLayout *exitLay = new QHBoxLayout;

                    height_widget += exitLay->sizeHint().height();

                    QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
                    exitLay->addSpacerItem(r);
                    exitLay->addWidget(closeButton);

                    vlayout->addLayout(exitLay);
                    if(m_painter.m_is_at_time)
                    {
                        vlayout->addWidget(labelName);
                    }
                    vlayout->addWidget(labelKey);
//                    vlayout->addWidget(vLine);
                    vlayout->addLayout(hLayCurveTitle);
                    vlayout->addLayout(hLayColor);
                    if(!m_painter.m_is_at_time)
                    {
                        vlayout->addLayout(hLayParams);
                    }
                    if(m_painter.m_is_at_time)
                    {
                        vlayout->addLayout(hLayisSecondY);
                    }

                    vlayout->addWidget(lineBox);
                    vlayout->addWidget(lineDotBox);
//                    vlayout->addWidget(edit);
                    vlayout->addLayout(hlayout);
                    QSize size;
//                    size.setHeight(460);
//                    size.setWidth(300);


                    size.setHeight(360);
                    size.setWidth(300);

                    layoutItem infoLay;
                    infoLay.lay = new QVBoxLayout;
                    infoLay.index = index_curve;
                    infoLay.key = curve.first;
                    infoLay.lay->addLayout(vlayout);
                    infoLay.attr_size = size;

                    infoLay.line.all_parameters = all_params;

                    infoLay.line.name = curve.second.param_line;
                    infoLay.line.key = curve.first;
                    infoLay.line.lineEditCurveTitle = lineEditCurveTitle;
                    infoLay.line.comboColor = comboColor;
                    infoLay.line.buttonColor = buttonColor;
                    infoLay.line.comboParam_x = comboParam_x;
                    infoLay.line.comboParam_y = comboParam_y;
                    infoLay.line.checkBoxisSecondY = checkBoxisSecondY;

                    infoLay.line.checkBoxisLine = checkBoxisLine;
                    infoLay.line.comboDash = comboDash;
                    infoLay.line.spinBoxWidth = spinBoxWidth;

                    infoLay.line.checkBoxisMarkers = checkBoxisMarkers;
                    infoLay.line.checkBoxisBorder = checkBoxisBorder;
                    infoLay.line.comboStyleDot = comboStyleDot;
                    infoLay.line.spinBoxWidthDot = spinBoxWidthDot;

                    infoLay.line.icon_curve = icon_curve;

                    infoLay.line.isEmpty = false;

                    m_vector_info.append(infoLay);
                    if(m_polyline_color.color != "none")
                    {
                        changeSVGColor(icon_curve, ":/svg/icons_svg/polyline.svg", m_polyline_color.color, m_polyline_color.border_color, m_polyline_color.size_border);
                    }
                    else
                    {
                        changeSVGColor(icon_curve, ":/svg/icons_svg/polyline.svg", curve.second.color_line, m_polyline_color.border_color, m_polyline_color.size_border);
                    }



                    QListWidgetItem *item_curve = new QListWidgetItem(*icon_curve, listLinesAttrName,  ui->listLines);
//                    linesItemData metaData;
//                    metaData.index = index_curve;
//                    metaData.key = curve.first;

                    QVariant index_this_curve = index_curve;
                    item_curve->setData(5, index_this_curve);
                    item_curve->setData(4, curve.first);

                    connect(comboParam_x, SIGNAL(activated(int)), this, SLOT(selectLineXparam(int)));
                    connect(comboParam_y, SIGNAL(activated(int)), this, SLOT(selectLineYparam(int)));

                    connect(checkBoxisSecondY, SIGNAL(stateChanged(int)), this, SLOT(checked_edit_line_isSecond_y(int)));
                    connect(checkBoxisMarkers, SIGNAL(stateChanged(int)), this, SLOT(checked_edit_line_isVisible_markers(int)));
                    connect(checkBoxisBorder, SIGNAL(stateChanged(int)), this, SLOT(checked_edit_line_isBorder(int)));
                    connect(comboStyleDot, SIGNAL(activated(int)), this, SLOT(selectLineStyleDot(int)));
                    connect(spinBoxWidthDot, SIGNAL(valueChanged(int)), this, SLOT(spinBox_changed_line_width_dot(int)));

                    connect(comboDash, SIGNAL(activated(int)), this, SLOT(selectLineDash(int)));
                    connect(spinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(spinBox_changed_line_width(int)));
                    connect(checkBoxisLine, SIGNAL(stateChanged(int)), this, SLOT(checked_edit_line_isVisible_line(int)));
                    connect(lineEditCurveTitle, SIGNAL(textEdited(const QString &)), this, SLOT(editingLine_edit_line_name_legend(const QString &)));
                    connect(comboColor, SIGNAL(activated(int)), this, SLOT(selectLineColor(int)));
                    connect(buttonColor, SIGNAL(clicked()), this, SLOT(click_on_button_edit_line_color()));
                    connect(closeButton, SIGNAL(clicked()), this, SLOT(click_on_button_edit_close()));

                }
            }


            QSize size;
            size.setHeight(m_sizeIconAttr);
            size.setWidth(m_sizeIconAttr);
            ui->listAttribute->setIconSize(size);

            m_count_attr_in_listattribute = ui->listAttribute->count();
//            ui->listAttribute->si
        }
    }

    setMenuToAddButton();
}

void GrapherGui::clickItemAttribute(QListWidgetItem *item)
{
    int index = ui->listAttribute->currentRow();
//    qDebug() << "Attrubute" << index; // << ui->listAttribute->currentItem()->text();

    if(index > -1 && index < ui->listAttribute->count())
    {
        createSettingsAttr(index, true);
    }
}


void GrapherGui::selectItemLines(QListWidgetItem *item)
{
    int index = ui->listLines->currentRow();
    qDebug() << "selectItemLines index lines" << index;
    ui->action_graph->setChecked(true);
    ui->action_files->setChecked(false);
    ui->action_addKey->setChecked(false);

    if(index > -1 && index < ui->listLines->count())
    {
        m_is_selected_attribute_item = false;
        m_is_selected_lines_item = true;
        click_on_action_graph();
        createSettingsAttr(index, false);
    }
}


void GrapherGui::selectItemLines(int index)
{
//    qDebug() << "index lines" << index;
    ui->action_graph->setChecked(true);
    ui->action_files->setChecked(false);
    ui->action_addKey->setChecked(false);



    if(index > -1 && index < ui->listLines->count())
    {
        m_is_selected_attribute_item = false;
        m_is_selected_lines_item = true;
        click_on_action_graph();
        createSettingsAttr(index, false);
    }
}

void GrapherGui::selectItemAttribute(int index)
{
//    qDebug() << "Attrubute" << index; // << ui->listAttribute->currentItem()->text();

    ui->action_graph->setChecked(true);
    ui->action_files->setChecked(false);
    ui->action_addKey->setChecked(false);



    if(index > -1 && index < ui->listAttribute->count())
    {
        m_is_selected_attribute_item = true;
        m_is_selected_lines_item = false;
        click_on_action_graph();
        createSettingsAttr(index, true);
    }
}

void GrapherGui::selectItemGraph(QListWidgetItem *item)
{
    if(item->isSelected() && !m_isDeleteMode)
    {
//        qDebug() << "xer";
        ui->pushButton_editGraph->setEnabled(true);
        setLineEditData();
    }
}


void GrapherGui::selectItemGraph(int index)
{
    if(!m_isDeleteMode)
    {
        if(index > -1 && ui->listGraph->count() > index)
        {
            m_selected_graph = index;
        }
        else
        {
            index = -1;
        }

    //    qDebug() << "m_selected_graph" << m_selected_graph;
    //    qDebug() << "=============\n" <<__FUNCTION__;

        ui->action_graph->setChecked(true);
        ui->action_files->setChecked(false);
        ui->action_addKey->setChecked(false);

        statusBar()->showMessage("");
        m_painter.m_is_second_y = false;
        qDebug() << "selectItemGraph index" << index;
        if(index > -1)
        {
            ui->pushButton_editGraph->setEnabled(true); //true
            ui->action_editing->setEnabled(true); //true

            click_on_action_graph();
            if(ui->radioButton_default->isChecked())
            {
                if(m_painter.m_vector_of_parameters[0].size() > index)
                {
                    bool isDefault = true;

                    setPaint(index, isDefault);
                    setVisibleSecondY(m_painter.m_is_second_y);
                    qDebug() << "==============";
                    qDebug() << "At time" << m_painter.m_is_at_time;
                }
            }
            else if(ui->radioButton_users->isChecked())
            {
                if(m_painter.m_vector_of_parameters[1].size() > index)
                {
                    bool isDefault = false;

                    setPaint(index, isDefault);
                    setVisibleSecondY(m_painter.m_is_second_y);
    //                m_painter.m_is_second_y = false;
                    qDebug() << "==============";
                    qDebug() << "At time" << m_painter.m_is_at_time;
                }
            }
        }
        setLineEditData();
    }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GrapherGui::double_click_item_setCheckBox(QListWidgetItem *item)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    if(item->checkState())
    {
        item->setCheckState(Qt::Unchecked);
    }
    else
    {
        item->setCheckState(Qt::Checked);
    }

}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-= Остальное =-=-=-=-=-=-=-=

QString GrapherGui::get_next_color_from_iter()
{
    QString color;
    color = *m_iter_color;
    m_iter_color++;

    if (m_iter_color == m_iter_color_end)
    {
        m_iter_color = m_iter_color_begin;
        color = *m_iter_color;
    }
    return color;
}


QString GrapherGui::get_prev_color_from_iter()
{

    if (m_iter_color == m_iter_color_begin)
    {
        m_iter_color = m_iter_color_end-1;

        m_iter_color--;
    }
    else
    {
         m_iter_color--;
        if (m_iter_color == m_iter_color_begin)
        {
            m_iter_color = m_iter_color_end-1;

        }
    }

    return *m_iter_color;
}


void GrapherGui::spinBox_changed_vline_sizeFont(int size)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;
    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_font = size;


    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    m_vector_info[ui->listAttribute->currentRow()].vLine.spinBoxFontSize->setFocus();
}

void GrapherGui::select_text_orientation(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;

    QMapIterator<QString, Painter::painter_orientation> orientation(m_painter.m_map_orientation);
    while (orientation.hasNext()) {
        orientation.next();

        if(orientation.value().orientation_rus == m_vector_info[ui->listAttribute->currentRow()].vLine.comboOri->currentText())
        {
            m_painter.m_vector_of_parameters[index_vector][index].vlines[i].text_orientation = orientation.value().orientation;
        }
    }


//    qDebug() << "width" << width;

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на spinBoxWidth
//    m_vector_info[ui->listAttribute->currentRow()].spinBoxWidth->setFocus();

}

void GrapherGui::checked_edit_line_isSecond_y(int state)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
    QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

    if(state == 0)
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_second_y = false;

    }
    else
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_second_y = true;
    }

    ui->lineEdit_SecondYmax->setText("0");
    ui->lineEdit_SecondYmin->setText("0");
    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, false);
}


void GrapherGui::checked_edit_line_isBorder(int state)
{

    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
    QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

    if(state == 0)
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_Border = false;
    }
    else
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_Border = true;
    }

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, false);

}

void GrapherGui::checked_edit_line_isVisible_markers(int state)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
    QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

    if(state == 0)
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_dots = false;
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.dots_size = -1;
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidthDot->setEnabled(false);
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboStyleDot->setEnabled(false);
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.checkBoxisBorder->setEnabled(false);

    }
    else
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_dots = true;
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.dots_size = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidthDot->value();
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidthDot->setEnabled(true);
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboStyleDot->setEnabled(true);
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.checkBoxisBorder->setEnabled(true);
    }

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, false);
    // установить фокус на edit_point_x
//    m_vector_info[ui->listAttribute->currentRow()].line.lineEditCurveTitle->setFocus();

}


void GrapherGui::checked_edit_line_isVisible_line(int state)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
    QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

    if(state == 0)
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.width_line = -1;
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboDash->setEnabled(false);
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidth->setEnabled(false);

    }
    else
    {
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.width_line = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidth->value();
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboDash->setEnabled(true);
        m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidth->setEnabled(true);
    }

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, false);
    // установить фокус на edit_point_x
//    m_vector_info[ui->listAttribute->currentRow()].line.lineEditCurveTitle->setFocus();

}


void GrapherGui::checked_edit_vline_auto_y(int state)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;

//    qDebug() << state;
    if(state == 0)
    {
        m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_max->setEnabled(true);
        m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_min->setEnabled(true);
        m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_max = m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_max->text().toDouble();
        m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_min = m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_min->text().toDouble();
    }
    else
    {

        m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_max->setEnabled(false);
        m_vector_info[ui->listAttribute->currentRow()].vLine.edit_y_min->setEnabled(false);
        m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_max = 0;
        m_painter.m_vector_of_parameters[index_vector][index].vlines[i].y_min = 0;
    }

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на spinBoxWidth

}

void GrapherGui::spinBox_changed_line_width_dot(int width)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
    QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;
    m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.dots_size = width;

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, false);
    m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidthDot->setFocus();


}


void GrapherGui::spinBox_changed_line_width(int width)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
    QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;
    m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.width_line = width;

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, false);
    m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.spinBoxWidth->setFocus();


}

void GrapherGui::spinBox_changed_vline_width(int width)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    int i = m_vector_info[ui->listAttribute->currentRow()].index;
    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].width = width;
//    qDebug() << "width" << width;

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);

    createSettingsAttr(m_last_attr_selected, true);
    // установить фокус на spinBoxWidth
    m_vector_info[ui->listAttribute->currentRow()].vLine.spinBoxWidth->setFocus();
}

void GrapherGui::selectLineXparam(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;
    if(select > -1)
    {

        QString name_param;

        int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
        QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

        for(int index_table = 0; index_table < m_painter.m_tables.size(); index_table++)
        {
            if(m_painter.m_tables[index_table]->m_group == key)
            {
               name_param = m_painter.m_tables[index_table]->rusTitleToEng(m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.all_parameters[select]);

            }
        }

        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.param_x = name_param;

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        bool param_x_is_empty = m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.param_x.isEmpty();
        bool param_y_is_empty = m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.param_y.isEmpty();

        if(!param_x_is_empty && !param_y_is_empty)
        {
//            qDebug() << m_painter.m_vector_of_parameters[index_vector][index].map_params_curve.size();
            if(m_painter.m_vector_of_parameters[index_vector][index].line_params_curve.size() < 2)
            {
                click_on_button_defaultScale();
            }
        }
        createSettingsAttr(m_last_attr_selected, false);
    }

}

void GrapherGui::selectLineYparam(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    if(select > -1)
    {

        QString name_param;

        int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
        QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

        for(int index_table = 0; index_table < m_painter.m_tables.size(); index_table++)
        {
            if(m_painter.m_tables[index_table]->m_group == key)
            {

               name_param = m_painter.m_tables[index_table]->rusTitleToEng(m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.all_parameters[select]);

            }
        }

        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.param_y = name_param;
//        qDebug() << select <<  m_painter.m_penStyle[select];

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        bool param_x_is_empty = m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.param_x.isEmpty();
        bool param_y_is_empty = m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.param_y.isEmpty();

        if(!param_x_is_empty && !param_y_is_empty)
        {
//            qDebug() << m_painter.m_vector_of_parameters[index_vector][index].map_params_curve.size();
            if(m_painter.m_vector_of_parameters[index_vector][index].line_params_curve.size() < 2)
            {
                click_on_button_defaultScale();
            }
        }


        createSettingsAttr(m_last_attr_selected, false);
    }

}


void GrapherGui::selectLineStyleDot(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    if(select > -1)
    {
        int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
        QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

        QMapIterator<QString, Painter::painter_style_symbols> style_symbols(m_painter.m_map_style_symbols);
        while (style_symbols.hasNext()) {
            style_symbols.next();

            if(style_symbols.value().style_symbols_rus == m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboStyleDot->currentText())
            {
                m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.dots_style = style_symbols.value().style_symbols;
            }
        }

//        qDebug() << select <<  m_painter.m_penStyle[select];

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, false);
    }
}




void GrapherGui::selectLineDash(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    if(select > -1)
    {
        int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
        QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

        QMapIterator<QString, Painter::painter_penStyle> penStyle(m_painter.m_map_penStyle);
        while (penStyle.hasNext()) {
            penStyle.next();

            if(penStyle.value().penStyle_rus == m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboDash->currentText())
            {
                m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.dash_line = penStyle.value().penStyle;
            }
        }

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, false);
    }
}


void GrapherGui::selectVlineDash(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    if(select > -1)
    {
        int i = m_vector_info[ui->listAttribute->currentRow()].index;

        QMapIterator<QString, Painter::painter_penStyle> penStyle(m_painter.m_map_penStyle);
        while (penStyle.hasNext()) {
            penStyle.next();

            if(penStyle.value().penStyle_rus == m_vector_info[ui->listAttribute->currentRow()].vLine.comboDash->currentText())
            {
                m_painter.m_vector_of_parameters[index_vector][index].vlines[i].dash = penStyle.value().penStyle;
            }
        }

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);
    }
}


void GrapherGui::selectLineColor(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    if(select -1 > -1)
    {
        int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
        QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;
        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.color_line = m_painter.m_colors[select-1];

//        changeSVGColor(m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.icon_curve, );

        QIcon *icon_polyline = new QIcon();
        if(m_polyline_color.color != "none")
        {
            changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", m_polyline_color.color, m_polyline_color.border_color, m_polyline_color.size_border);
        }
        else
        {
            changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", m_painter.m_colors[select-1], m_polyline_color.border_color, m_polyline_color.size_border);
        }

        ui->listLines->currentItem()->setIcon(*icon_polyline);
        qDebug() << "PARAM TEXT 1" <<  ui->listLines->currentItem()->text();

//        setAttributeGraph();

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, false);
    }
    else
    {

        QRegularExpression hexMatcher("^#(?:[0-9a-fA-F]{3}){1,2}$", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = hexMatcher.match(m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboColor->itemText(0));

        if(match.hasMatch())
        {
            int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
            QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;
            m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.color_line = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboColor->itemText(0);

//            setAttributeGraph();
//            changeSVGColor(m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.icon_curve, );

            QIcon *icon_polyline = new QIcon();
            if(m_polyline_color.color != "none")
            {
                changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", m_polyline_color.color, m_polyline_color.border_color, m_polyline_color.size_border);
            }
            else
            {
                changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboColor->itemText(0), m_polyline_color.border_color, m_polyline_color.size_border);
            }

            ui->listLines->currentItem()->setIcon(*icon_polyline);
//            qDebug() << "PARAM TEXT 2" <<  ui->listLines->currentItem()->text();

            setScaleView();
            setPaint(index, isDefault);
            setVisibleSecondY(m_painter.m_is_second_y);

            createSettingsAttr(m_last_attr_selected, false);
        }
        else
        {
            int i = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].index;
            QString key = m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].key;

            QMapIterator<QString, Painter::painter_color> colors(m_painter.m_map_colors);
            while (colors.hasNext()) {
                colors.next();

                if(colors.value().color_rus == m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.comboColor->itemText(0))
                {
                    m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.color_line = colors.value().color;

                    QIcon *icon_polyline = new QIcon();
                    if(m_polyline_color.color != "none")
                    {
                        changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", m_polyline_color.color, m_polyline_color.border_color, m_polyline_color.size_border);
                    }
                    else
                    {
                        changeSVGColor(icon_polyline, ":/svg/icons_svg/polyline.svg", colors.value().color, m_polyline_color.border_color, m_polyline_color.size_border);
                    }

                    ui->listLines->currentItem()->setIcon(*icon_polyline);
                    qDebug() << "PARAM TEXT 3" <<  ui->listLines->currentItem()->text();
                }
            }

//            setAttributeGraph();
//            changeSVGColor(m_vector_info[ui->listLines->currentRow() + ui->listAttribute->count()].line.icon_curve, );




            setScaleView();
            setPaint(index, isDefault);
            setVisibleSecondY(m_painter.m_is_second_y);

            createSettingsAttr(m_last_attr_selected, false);
        }


    }



//    m_vector_info[ui->listAttribute->currentRow()].line.lineEditCurveTitle->setFocus();
}


void GrapherGui::selectVlineColor(int select)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    if(select -1 > -1)
    {
        int i = m_vector_info[ui->listAttribute->currentRow()].index;
        m_painter.m_vector_of_parameters[index_vector][index].vlines[i].color = m_painter.m_colors[select-1];

        setScaleView();
        setPaint(index, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);
    }
    else
    {
        QRegularExpression hexMatcher("^#(?:[0-9a-fA-F]{3}){1,2}$", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = hexMatcher.match(m_vector_info[ui->listAttribute->currentRow()].vLine.comboColor->itemText(0));

        if(match.hasMatch())
        {
            int i = m_vector_info[ui->listAttribute->currentRow()].index;
            m_painter.m_vector_of_parameters[index_vector][index].vlines[i].color = m_vector_info[ui->listAttribute->currentRow()].vLine.comboColor->itemText(0);

            setScaleView();
            setPaint(index, isDefault);
            setVisibleSecondY(m_painter.m_is_second_y);

            createSettingsAttr(m_last_attr_selected, true);
        }
        else
        {
            int i = m_vector_info[ui->listAttribute->currentRow()].index;

            QMapIterator<QString, Painter::painter_color> colors(m_painter.m_map_colors);
            while (colors.hasNext()) {
                colors.next();

                if(colors.value().color_rus == m_vector_info[ui->listAttribute->currentRow()].vLine.comboColor->itemText(0))
                {
                    m_painter.m_vector_of_parameters[index_vector][index].vlines[i].color = colors.value().color;
                }
            }
            setScaleView();
            setPaint(index, isDefault);
            setVisibleSecondY(m_painter.m_is_second_y);

            createSettingsAttr(m_last_attr_selected, true);
        }

    }
}


void GrapherGui::selectPosLegend(int index)
{
//    QStringList pos = {"top_left", "top_right", "top_center", "bottom_left", "bottom_right", "bottom_center", "left_center", "right_center"};

    m_is_edited = true;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    int index_selected_graph = m_selected_graph;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index_selected_graph)
    {

        QMapIterator<QString, Painter::painter_position> position(m_painter.m_map_position);
        while (position.hasNext()) {
            position.next();

            if(position.value().position_rus == m_vector_info[ui->listAttribute->currentRow()].comboPosLegend->currentText())
            {
                m_painter.m_vector_of_parameters[index_vector][index_selected_graph].pos_legend = position.value().position;

                QIcon *icon_legend = new QIcon();
                setLegendIcon(icon_legend, position.value().position);
                ui->listAttribute->currentItem()->setIcon(*icon_legend);
            }
        }


        setPaint(index_selected_graph, isDefault);
        setVisibleSecondY(m_painter.m_is_second_y);

        createSettingsAttr(m_last_attr_selected, true);

    }

}


void GrapherGui::changeSVGColor(QIcon *icon, QString path_to_icon, QString color, QString border_color, int size_border)
{
//    qDebug() << "icon color" << color;

    QFile file(path_to_icon);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();

    QDomDocument doc;
    doc.setContent(baData);

    QDomElement elem = doc.documentElement();
    setAttrRecur(elem, "path", "fill", color);
    setAttrRecur(elem, "path", "stroke", border_color);
    setAttrRecur(elem, "path", "stroke-width", QString::number(size_border));
//    stroke="red" stroke-width="2"

    QSvgRenderer svgRenderer(doc.toByteArray());
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);
    QPainter pixPainter(&pix);
    svgRenderer.render(&pixPainter);
//    QIcon myicon(pix);
    icon->addPixmap(pix);

}

void GrapherGui::setAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
{
    if(elem.tagName().compare(strtagname) == 0)
    {
        elem.setAttribute(strattr, strattrval);
//        elem.setAttribute("stroke", "red");
    }

    for(int i = 0; i < elem.childNodes().count(); i++)
    {
        if(!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        QDomElement elem_v = elem.childNodes().at(i).toElement();
        setAttrRecur(elem_v, strtagname, strattr, strattrval);
    }
}

void GrapherGui::createSettingsAttr(int index,  bool isAtribute)
{
//    qDebug() << "index create" << index << "m_count_attr_in_listattribute" << m_count_attr_in_listattribute << "m_last_attr_selected" << m_last_attr_selected;


    if(isAtribute)
    {
        ui->listLines->setCurrentRow(-1);
        index = ui->listAttribute->currentRow();

    }
    else
    {
        ui->listAttribute->setCurrentRow(-1);
        index = ui->listLines->currentRow() + m_count_attr_in_listattribute;
    }


    if(m_windowSettingsGraph != nullptr)
    {
//        qDebug() << "delete widget" << "m_last_attr_selected" << m_last_attr_selected << "index" << index;
        if(m_vector_info.size() > m_last_attr_selected)
        {
            m_windowSettingsGraph->layout()->removeItem(m_vector_info[m_last_attr_selected].lay);
        }


        m_windowSettingsGraph->close();

        m_windowSettingsGraph = nullptr;
    }


    m_windowSettingsGraph = new QFrame(ui->grapherWidget);
    QRect geometry = ui->grapherWidget->geometry();
    geometry.setWidth(m_vector_info[index].attr_size.width());
//    m_windowSettingsGraph->setStyleSheet("background-color: rgba(0,0,0,0)");

    m_windowSettingsGraph->setGeometry(geometry);



    MyQFrame *window = new MyQFrame(0, 0);

    window->setFrameShape(MyQFrame::Box);
    window->setLineWidth(1);
    window->setAutoFillBackground(true);
    window->setLayout(m_vector_info[index].lay);

    m_vector_info[index].attr_size.setHeight(window->height());

    QVBoxLayout *mainwindow = new QVBoxLayout();
    mainwindow->addWidget(window);



    QVBoxLayout *downVlayout = new QVBoxLayout();

    QSpacerItem *r = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    downVlayout->addSpacerItem(r);
//    downVlayout->


    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(3, 0, 4, 0);
//    vlayout->setS
    vlayout->addLayout(mainwindow);
    vlayout->addLayout(downVlayout);


    m_windowSettingsGraph->setLayout(vlayout);

    m_windowSettingsGraph->raise();


    isShowWindowSettingsGraph = true;

//    m_mousePos = ui->listAttribute->mapFromGlobal(QCursor::pos());

    int scale_size = 0;
    if(ui->action_scale->isChecked())
    {
        scale_size = m_scale_size;
//        qDebug() << "scale_size" << scale_size;
    }

    if(m_is_selected_attribute_item)
    {
        int index_row = ui->listAttribute->currentRow();
        int x = ( width() - m_windowSettingsGraph->width() - ui->listAttribute->width() - 3);
        int y;

        if(height() - m_size_status_bar - scale_size > (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index_row + m_vector_info[index_row].attr_size.height())
        {
            y = (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index_row - 5;
        }
        else
        {
            int h = height() - m_size_status_bar - scale_size;
            int h_wid = (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index_row + m_vector_info[index_row].attr_size.height();
            int raz = h_wid - h;

            y = (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index_row - raz -3;
        }

        m_windowSettingsGraph->move(x, y);
    }

    if(m_is_selected_lines_item)
    {
        int index_row = ui->listLines->currentRow();
        if(index_row > -1 && index_row < ui->listLines->count())
        {
            int x = (width() - m_windowSettingsGraph->width() - ui->listLines->width() - 3);
            int y;
            int h_win = (m_sizeIconAttr + ui->listLines->spacing() * 2) * index_row + m_vector_info[index_row + ui->listAttribute->count()].attr_size.height() + ui->listAttribute->height();
//            qDebug() << "height" << height() - m_size_status_bar << "h_w" << h_win;
            if(height() - m_size_status_bar - scale_size > h_win)
            {
                y = (m_sizeIconAttr + ui->listLines->spacing() * 2) * index_row - 5 + ui->listAttribute->height();
            }
            else
            {
                int h = height() -m_size_status_bar - scale_size;
                int h_wid = h_win;
                int raz = h_wid - h;
//                qDebug() << "raz" << raz << "h_wid" << h_wid;
//                qDebug() << "height()" << height();
                y = (m_sizeIconAttr + ui->listLines->spacing() * 2) * index_row - raz -3 + ui->listAttribute->height();
            }
//                qDebug() << "y" << y << "ui->listAttribute->height()" << ui->listAttribute->height();
            m_windowSettingsGraph->move(x, y);
        }
    }

    m_windowSettingsGraph->show();
    m_last_attr_selected = index;
}


void GrapherGui::resizeEvent( QResizeEvent* )
{
    setNewPositionWidget();
}


void GrapherGui::setNewPositionWidget()
{
    int scale_size = 0;
    if(ui->action_scale->isChecked())
    {
        scale_size = m_scale_size;
//        qDebug() << "resize scale_size" << scale_size;
    }
    if(isShowWindowSettingsGraph)
    {
        if(m_is_selected_attribute_item)
        {
            int index = ui->listAttribute->currentRow();
            if(index > -1 && index < ui->listAttribute->count())
            {
                int x = (width() - m_windowSettingsGraph->width() - ui->listAttribute->width() - 3);
                int y;
                if(height() -m_size_status_bar - scale_size > (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index + m_vector_info[index].attr_size.height())
                {
                    y = (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index - 5;
                }
                else
                {
                    int h = height() -m_size_status_bar - scale_size;
                    int h_wid = (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index + m_vector_info[index].attr_size.height();
                    int raz = h_wid - h;
    //                qDebug() << "raz" << raz << "h_wid" << h_wid;
    //                qDebug() << "height()" << height();
                    y = (m_sizeIconAttr + ui->listAttribute->spacing() * 2) * index - raz -3;
                }

                m_windowSettingsGraph->move(x, y);
            }
        }
        if(m_is_selected_lines_item)
        {
            int index = ui->listLines->currentRow();
            if(index > -1 && index < ui->listLines->count())
            {
                int x = (width() - m_windowSettingsGraph->width() - ui->listLines->width() - 3);
                int y;
                int h_win = (m_sizeIconAttr + ui->listLines->spacing() * 2) * index + m_vector_info[index + ui->listAttribute->count()].attr_size.height() + ui->listAttribute->height();
//                qDebug() << "height" << height() - m_size_status_bar << "h_w" << h_win;
                if(height() - m_size_status_bar - scale_size > h_win)
                {
                    y = (m_sizeIconAttr + ui->listLines->spacing() * 2) * index - 5 + ui->listAttribute->height();
                }
                else
                {
                    int h = height() -m_size_status_bar - scale_size;
                    int h_wid = h_win;
                    int raz = h_wid - h;
//                    qDebug() << "raz" << raz << "h_wid" << h_wid;
    //                qDebug() << "height()" << height();
                    y = (m_sizeIconAttr + ui->listLines->spacing() * 2) * index - raz -3 + ui->listAttribute->height();
                }
//                qDebug() << "y" << y << "ui->listAttribute->height()" << ui->listAttribute->height();
                m_windowSettingsGraph->move(x, y);
            }
        }
    }
}


void GrapherGui::setJsonSettings()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QJsonObject mainObj;
//    QJsonObject subPathObject;
    mainObj["pathToDefaultJsonFile"] = m_settings.pathToDefaultJsonFile;
    mainObj["pathToUserJsonFile"] = m_settings.pathToUserJsonFile;
    mainObj["startPathFindFile"] = m_settings.startPathFindFile;
    mainObj["fileFilter"] = m_settings.fileFilter;
    mainObj["pathToAssociateFile"] = m_settings.pathToAssociateFile;
    mainObj["pathToTargetFile"] = m_settings.pathToTargetFile;
    mainObj["userIsEnable"] = m_settings.userIsEnable;
    mainObj["embeded"] = m_settings.embeded;
    mainObj["styleSheet"] = m_settings.styleSheet;
    mainObj["compareRmNumber"] = m_settings.compareRmNumber;
    mainObj["compareStartDate"] = m_settings.compareStartDate;
    mainObj["useRusTitle"] = m_settings.useRusTitle;

    QJsonObject color;

    QJsonObject colorIconsAttributeEdit;
    colorIconsAttributeEdit["color"] = m_settings.colorIconsAttributeEdit.color;
    colorIconsAttributeEdit["border_color"] = m_settings.colorIconsAttributeEdit.border_color;
    colorIconsAttributeEdit["border_size"] = m_settings.colorIconsAttributeEdit.size_border;

    QJsonObject colorIconsButton;
    colorIconsButton["color"] = m_settings.colorIconsButton.color;
    colorIconsButton["border_color"] = m_settings.colorIconsButton.border_color;
    colorIconsButton["border_size"] = m_settings.colorIconsButton.size_border;

    QJsonObject colorIconsContextMenu;
    colorIconsContextMenu["color"] = m_settings.colorIconsContextMenu.color;
    colorIconsContextMenu["border_color"] = m_settings.colorIconsContextMenu.border_color;
    colorIconsContextMenu["border_size"] = m_settings.colorIconsContextMenu.size_border;

    QJsonObject colorIconsToolBar;
    colorIconsToolBar["color"] = m_settings.colorIconsToolBar.color;
    colorIconsToolBar["border_color"] = m_settings.colorIconsToolBar.border_color;
    colorIconsToolBar["border_size"] = m_settings.colorIconsToolBar.size_border;

    QJsonObject polyline;
    QJsonObject colorIconsPolyline;
    colorIconsPolyline["color"] = m_settings.colorIconsPolyline.color;
    colorIconsPolyline["border_color"] = m_settings.colorIconsPolyline.border_color;
    colorIconsPolyline["border_size"] = m_settings.colorIconsPolyline.size_border;

    QJsonObject colorIconsPolylineWithStyleSheets;
    colorIconsPolylineWithStyleSheets["color"] = m_settings.colorIconsPolylineWithStyleSheets.color;
    colorIconsPolylineWithStyleSheets["border_color"] = m_settings.colorIconsPolylineWithStyleSheets.border_color;
    colorIconsPolylineWithStyleSheets["border_size"] = m_settings.colorIconsPolylineWithStyleSheets.size_border;

    polyline["colorIconsPolyline"] = colorIconsPolyline;
    polyline["colorIconsPolylineWithStyleSheets"] = colorIconsPolylineWithStyleSheets;

    color["colorIconsAttributeEdit"] = colorIconsAttributeEdit;
    color["colorIconsButton"] = colorIconsButton;
    color["colorIconsContextMenu"] = colorIconsContextMenu;
    color["colorIconsToolBar"] = colorIconsToolBar;

    color["polyline"] = polyline;
    mainObj["color"] = color;


    qDebug() << mainObj;

    QFile file;
    file.setFileName(m_settings.pathSettings);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Неудалось открыть Json файл" << file.fileName();
        return;
    }
    file.write(QJsonDocument(mainObj).toJson(QJsonDocument::Indented));
    file.close();

}


void GrapherGui::setJsonAssociateJson()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QJsonObject mainObj;
    for(int i = 0; i < m_vector_key.size(); i++)
    {
        QJsonObject subPathObject;
        subPathObject["name"] = m_vector_key[i].name;
        subPathObject["reg"] = m_vector_key[i].reg;

        mainObj[m_vector_key[i].key] = subPathObject;
    }
    qDebug() << mainObj;

    QFile file;
    file.setFileName(m_settings.pathToAssociateFile);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Неудалось открыть Json файл" << file.fileName();
        return;
    }
    file.write(QJsonDocument(mainObj).toJson(QJsonDocument::Indented));
    file.close();

}


void GrapherGui::getEnabledKeyAdd()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QString key = ui->lineEdit_key->text();
    QString reg = ui->lineEdit_keyReg->text();
    QString name = ui->lineEdit_keyName->text();
    ui->pushButton_key_delete->setEnabled(false);

    if(!key.isEmpty() && !reg.isEmpty() && !name.isEmpty())
    {
        ui->pushButton_key_add->setEnabled(true);
//        ui->pushButton_key_delete->setEnabled(false);
    }
    else
    {
        ui->pushButton_key_add->setEnabled(false);
    }
}

void GrapherGui::setDeleteMode(bool isVisible)
{
    ui->pushButton_delete_selected_graph->setVisible(isVisible);
    ui->pushButton_back_to_view->setVisible(isVisible);

    ui->pushButton_editGraph->setVisible(!isVisible);
    ui->pushButton_createGraph->setVisible(!isVisible);
    ui->pushButton_deleteMode->setVisible(!isVisible);

    ui->radioButton_default->setVisible(!isVisible);
    ui->radioButton_users->setVisible(!isVisible);
}

void GrapherGui::setKeyVisible(bool isVisible)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    ui->listKey->setVisible(isVisible);

    ui->label_key->setVisible(isVisible);

    ui->lineEdit_key->setVisible(isVisible);
    ui->lineEdit_keyName->setVisible(isVisible);
    ui->lineEdit_keyReg->setVisible(isVisible);
    ui->lineEdit_title_params->setVisible(false);
    ui->line_key->setVisible(isVisible);

    ui->pushButton_key_add->setVisible(isVisible);
    ui->pushButton_key_delete->setVisible(isVisible);
}


void GrapherGui::zoomerPlot(const QRectF &rect)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    ui->lineEdit_Xmax->setText(QString::number(rect.right(), 'f', 9));
    ui->lineEdit_Xmin->setText(QString::number(rect.left(), 'f', 9));

    ui->lineEdit_Ymax->setText(QString::number(rect.bottom(), 'f', 9));
    ui->lineEdit_Ymin->setText(QString::number(rect.top(), 'f', 9));

//    qDebug() << rect.left() << rect.right() << rect.bottom() << rect.top();


}


bool GrapherGui::getValueIsMarkersInAllGraph()
{

    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index && index > -1)
    {
        int size_lines = m_painter.m_vector_of_parameters[index_vector][index].line_params_curve.size();

        for(int i = 0; i < size_lines; i++)
        {
            if(!m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_dots)
            {
                return false;
            }
        }
        return true;
    }
}


void GrapherGui::listAttributeContextyMenu(QPoint pos)
{
//    if(m_menu_listAttribute != nullptr)
//    {
//        delete m_menu_listAttribute;
//    }
    if(ui->listAttribute->selectedItems().size() != 0)
    {
        if(ui->listAttribute->currentRow() > 4)
        {
//            QMenu menu;
            m_menu_listAttribute = new QMenu;
            m_menu_listAttribute->addAction(ui->action_add_line_graph_lines);
            m_menu_listAttribute->addAction(ui->action_delete_lines);

            m_menu_listAttribute->exec(ui->listAttribute->mapToGlobal(pos));
        }
        else
        {
//            QMenu menu;
            m_menu_listAttribute = new QMenu;
            m_menu_listAttribute->addAction(ui->action_add_line_graph_lines);

            m_menu_listAttribute->exec(ui->listAttribute->mapToGlobal(pos));
        }
    }
    else
    {
//        QMenu menu;
        m_menu_listAttribute = new QMenu;
        m_menu_listAttribute->addAction(ui->action_add_line_graph_lines);

        m_menu_listAttribute->exec(ui->listAttribute->mapToGlobal(pos));
    }
    m_menu_listAttribute->installEventFilter(this);
//    connect(m_menu_listAttribute, SIGNAL(hovered(QAction *)), this, SLOT(selectEditMenuItemHovered(QAction *)));

}


void GrapherGui::listLinesContextyMenu(QPoint pos)
{

//    if(m_menu_listLines != nullptr)
//    {
//        delete m_menu_listLines;
//    }

    if(ui->listLines->selectedItems().size() != 0)
    {
        qDebug() << "выбран итем" << ui->listLines->currentRow() << pos;
//        QMenu menu;
        m_menu_listLines = new QMenu;
        m_menu_listLines->addAction(ui->action_add_line_graph_lines);
        m_menu_listLines->addAction(ui->action_delete_lines);

        m_menu_listLines->exec(ui->listLines->mapToGlobal(pos));
    }
    else
    {
//        QMenu menu;
        m_menu_listLines = new QMenu;
        m_menu_listLines->addAction(ui->action_add_line_graph_lines);

        m_menu_listLines->exec(ui->listLines->mapToGlobal(pos));
    }

    m_menu_listLines->installEventFilter(this);
//    connect(m_menu_listLines, SIGNAL(hovered(QAction *)), this, SLOT(selectEditMenuItemHovered(QAction *)));
}


void GrapherGui::listGraphContextMenu(QPoint pos)
{

    if(ui->listGraph->selectedItems().size() != 0 && !m_isDeleteMode)
    {

        int index = m_selected_graph;

        graph_meta_data data = getMetaDataVector();
        int index_vector = data.index_vector;
        bool isDefault = data.isDefault;

        qDebug() << "выбран итем" << ui->listGraph->currentRow() << pos;
//        QMenu menu;
        m_menu_listGraph = new QMenu;
        ui->action_is_markers->setChecked(getValueIsMarkersInAllGraph());
        m_menu_listGraph->addAction(ui->action_is_markers);
        m_menu_listGraph->addAction(ui->action_edit_this_graph);
        m_menu_listGraph->addAction(ui->action_save_this_graph);


         if(!m_painter.m_vector_of_parameters[index_vector][index].is_from_json)
         {
             m_menu_listGraph->addAction(ui->action_delete_this_graph);
         }

         if(m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited && m_painter.m_vector_of_parameters[index_vector][index].is_from_json)
         {
             m_menu_listGraph->addAction(ui->action_resert_edited);
         }


        m_menu_listGraph->exec(ui->listGraph->mapToGlobal(pos));

        m_menu_listGraph->installEventFilter(this);
//        connect(m_menu_listGraph, SIGNAL(hovered(QAction *)), this, SLOT(selectEditMenuItemHovered(QAction *)));
    }
}

void GrapherGui::togled_checkBox_item_list_attr(QListWidgetItem *item)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
//    qDebug() << "test";
    int len_attr = ui->listGraph->count();
    bool isVisibleSave = false;
    for(int i = 0; i < len_attr; i++)
    {
        if(ui->listGraph->item(i)->checkState())
        {
            isVisibleSave = true;
        }
    }

    if(m_isDeleteMode)
    {
        ui->pushButton_delete_selected_graph->setEnabled(isVisibleSave);
    }
    else
    {
        ui->pushButton_createGraph->setEnabled(isVisibleSave);
    }


//    ui->pushButton_deleteMode->setEnabled(isVisibleSave);
//    ui->pushButton_deleteMode->setEnabled(false);
}


void GrapherGui::setScaleView()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    if(m_painter.m_vector_of_parameters[index_vector].size() > index && index > -1)
    {
//                qDebug() << "index" << index << ui->lineEdit_Xmax->text().replace(",", ".").toDouble();
        m_painter.m_vector_of_parameters[index_vector][index].x_max_scale = ui->lineEdit_Xmax->text().replace(",", ".").toDouble();
        m_painter.m_vector_of_parameters[index_vector][index].x_min_scale = ui->lineEdit_Xmin->text().replace(",", ".").toDouble();

        m_painter.m_vector_of_parameters[index_vector][index].y_max_scale = ui->lineEdit_Ymax->text().replace(",", ".").toDouble();
        m_painter.m_vector_of_parameters[index_vector][index].y_min_scale = ui->lineEdit_Ymin->text().replace(",", ".").toDouble();

        m_painter.m_vector_of_parameters[index_vector][index].second_y_max_scale = ui->lineEdit_SecondYmax->text().replace(",", ".").toDouble();
        m_painter.m_vector_of_parameters[index_vector][index].second_y_min_scale = ui->lineEdit_SecondYmin->text().replace(",", ".").toDouble();

        setPaint(index, isDefault);
    }

}

void GrapherGui::setVisibleSecondY(bool isVisible)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    if(ui->action_scale->isChecked())
    {
        ui->lineEdit_SecondYmax->setVisible(isVisible);
        ui->lineEdit_SecondYmin->setVisible(isVisible);

        ui->label_SecondYmax->setVisible(isVisible);
        ui->label_SecondYmin->setVisible(isVisible);

        ui->line_scale_2->setVisible(isVisible);
    }

}


void GrapherGui::setLineEditData()
{
//    qDebug() << "=============\n" <<__FUNCTION__;
    double xmax = m_plot->axisScaleDiv(QwtPlot::xBottom).interval().maxValue();
    double xmin = m_plot->axisScaleDiv(QwtPlot::xBottom).interval().minValue();

    double ymax = m_plot->axisScaleDiv(QwtPlot::yLeft).interval().maxValue();
    double ymin = m_plot->axisScaleDiv(QwtPlot::yLeft).interval().minValue();

    double second_ymax = m_plot->axisScaleDiv(QwtPlot::yRight).interval().maxValue();
    double second_ymin = m_plot->axisScaleDiv(QwtPlot::yRight).interval().minValue();


//    qDebug() << "xmax" << xmax << "xmin" << xmin << "ymax" << ymax << "ymin" << ymin << "second_ymax" << second_ymax << "second_ymin" << second_ymin;

    ui->lineEdit_Xmax->setText(QString::number(xmax, 'f', 9));
    ui->lineEdit_Xmin->setText(QString::number(xmin, 'f', 9));

    ui->lineEdit_Ymax->setText(QString::number(ymax, 'f', 9));
    ui->lineEdit_Ymin->setText(QString::number(ymin, 'f', 9));

    ui->lineEdit_SecondYmax->setText(QString::number(second_ymax, 'f', 9));
    ui->lineEdit_SecondYmin->setText(QString::number(second_ymin, 'f', 9));

    m_data_axis.width = m_plot->axisScaleDiv(QwtPlot::xBottom).interval().width();
    m_data_axis.height = m_plot->axisScaleDiv(QwtPlot::yLeft).interval().width();

    m_data_axis.S_rect = m_data_axis.width * m_data_axis.height;
}


void GrapherGui::click_on_canvas(const QPoint &pos)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    // считываем значения координат клика
    double x = m_plot->invTransform(QwtPlot::xBottom, pos.x());
    double y = m_plot->invTransform(QwtPlot::yLeft, pos.y());

    double y2 = m_plot->invTransform(QwtPlot::yRight, pos.y());

    QString info;

    if(!m_painter.m_param_graph.x_title.isEmpty())
    {
        info += m_painter.m_param_graph.x_title + " = " + QString::number(x, 'f', 7);
    }
    else
    {
        info +=  "X = " + QString::number(x, 'f', 7);
    }

    if(!m_painter.m_param_graph.y_title.isEmpty())
    {
        info += "; " + m_painter.m_param_graph.y_title + " = " + QString::number(y, 'f', 7);
    }
    else
    {
        info += "; Y = " + QString::number(y, 'f', 7);
    }



    if(m_painter.m_is_second_y)
    {
        if(!m_painter.m_param_graph.second_y_title.isEmpty())
        {
            info += "; " + m_painter.m_param_graph.second_y_title + " = " + QString::number(y2, 'f', 7);
        }
        else
        {
            info += "; Y2 = " + QString::number(y2, 'f', 7);
        }

    }

    // отображаем информацию в строке состояния
    statusBar()->showMessage(info);

    click_on_button_edit_close();
}


void GrapherGui::setSpacerHiden(bool isVisible)
{
    if(isVisible)
    {
        ui->horizontalSpacer->changeSize(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    else
    {
        ui->horizontalSpacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
}


void GrapherGui::setScaleVisible(bool isVisible)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    if(ui->action_scale->isChecked() && !m_is_files_action_open && !m_is_key_action_open) //&& m_isViewMode
    {
        ui->line_scale->setVisible(isVisible);
//        ui->line_scale_2->setVisible(isVisible);
        ui->line_scale_3->setVisible(isVisible);

        ui->lineEdit_Xmax->setVisible(isVisible);
        ui->label_Xmax->setVisible(isVisible);

        ui->lineEdit_Xmin->setVisible(isVisible);
        ui->label_Xmin->setVisible(isVisible);

        ui->lineEdit_Ymax->setVisible(isVisible);
        ui->label_Ymax->setVisible(isVisible);

        ui->lineEdit_Ymin->setVisible(isVisible);
        ui->label_Ymin->setVisible(isVisible);

        setSpacerHiden(isVisible);

        if(m_painter.m_is_second_y)
        {
            setVisibleSecondY(isVisible);
        }

        ui->pushButton_defaultScale->setVisible(isVisible);

    }
    else
    {
        ui->line_scale->setVisible(false);
        ui->line_scale_2->setVisible(false);
        ui->line_scale_3->setVisible(false);

        ui->lineEdit_Xmax->setVisible(false);
        ui->label_Xmax->setVisible(false);

        ui->lineEdit_Xmin->setVisible(false);
        ui->label_Xmin->setVisible(false);

        ui->lineEdit_Ymax->setVisible(false);
        ui->label_Ymax->setVisible(false);

        ui->lineEdit_Ymin->setVisible(false);
        ui->label_Ymin->setVisible(false);

        ui->lineEdit_SecondYmax->setVisible(false);
        ui->label_SecondYmax->setVisible(false);

        ui->lineEdit_SecondYmin->setVisible(false);
        ui->label_SecondYmin->setVisible(false);

        ui->pushButton_defaultScale->setVisible(false);

        setSpacerHiden(false);

//        ui->horizontalLayout_11->setEnabled(false);
    }

}


void GrapherGui::saveGraph(int index)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    if(index > -1 && ui->listGraph->count() > index)
    {
        m_selected_graph = index;
    }
    else
    {
        index = -1;
    }

    if(index > -1)
    {
        click_on_action_graph();
        if(ui->radioButton_default->isChecked())
        {
            if(m_painter.m_vector_of_parameters[0].size() > index)
            {
                bool isDefault = true;
                setPaint(index, isDefault);
                setVisibleSecondY(m_painter.m_is_second_y);
                m_painter.m_is_second_y = false;

            }


        }
        else if(ui->radioButton_users->isChecked())
        {
            if(m_painter.m_vector_of_parameters[1].size() > index)
            {
                bool isDefault = false;
                setPaint(index, isDefault);
                setVisibleSecondY(m_painter.m_is_second_y);
                m_painter.m_is_second_y = false;
            }

        }
    }
}


void GrapherGui::setItemListGraph()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    ui->listGraph->clear();
    if(ui->radioButton_default->isChecked())
    {
        for(int i = 0; i < m_painter.m_vector_of_parameters[0].size(); i++)
        {
            QString name;
            name = m_painter.m_vector_of_parameters[0][i].namefile;
            qDebug() << "m_painter.m_vector_of_parameters[0][i].is_a_not_save_edited" << m_painter.m_vector_of_parameters[0][i].is_a_not_save_edited << i;
            if(!m_painter.m_vector_of_parameters[0][i].is_from_json)
            {
                name += "*";
            }

            if(m_painter.m_vector_of_parameters[0][i].is_a_not_save_edited && m_painter.m_vector_of_parameters[0][i].is_from_json)
            {
                name += "*";
            }

//            #ifdef Q_OS_WIN32
//                name = m_painter.utf8tolocal(name);
//            #endif

            QListWidgetItem* item = new QListWidgetItem(name,  ui->listGraph);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag

            item->setCheckState(Qt::Unchecked); // AND initialize check state
            if(m_painter.assertPaint(i, true))
            {
//                item->setFlags(item->flags() & Qt::ItemIsEnabled);
            }
            else
            {
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            }
        }
    }
    else if(ui->radioButton_users->isChecked())
    {
//        qDebug() << "Пользовательская";
        for(int i = 0; i < m_painter.m_vector_of_parameters[1].size(); i++)
        {
//            QString name = m_painter.m_vector_of_parameters[1][i].namefile;

            QString name;
            qDebug() << "m_painter.m_vector_of_parameters[1][i].is_a_not_save_edited" << m_painter.m_vector_of_parameters[1][i].is_a_not_save_edited << i;
            name = m_painter.m_vector_of_parameters[1][i].namefile;
            if(!m_painter.m_vector_of_parameters[1][i].is_from_json)
            {
                name += "*";
            }

            if(m_painter.m_vector_of_parameters[1][i].is_a_not_save_edited && m_painter.m_vector_of_parameters[1][i].is_from_json)
            {
                name += "*";
            }


            qDebug() << "name" << name;

            QListWidgetItem* item = new QListWidgetItem(name,  ui->listGraph);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Unchecked); // AND initialize check state
            if(!m_isDeleteMode)
            {
                if(m_painter.assertPaint(i, false))
                {
    //                item->setFlags(item->flags() & Qt::ItemIsEnabled);
                }
                else
                {
                    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
                }
            }
            else
            {

            }

        }
    }
}

void GrapherGui::filesApply()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QString source;
    for(int i = 0; i < m_fileMetaDataVector.size(); i++)
    {
        source +=  m_fileMetaDataVector[i].key + "|" + m_fileMetaDataVector[i].path + ";";
    }
    m_painter.m_tables.clear();
    if(!source.isEmpty())
    {
        source = source.left(source.size() -1);
        m_painter.m_settings.source = source;
        m_painter.m_settings.isSource = true;

        m_painter.parceSource();
    }

    if(m_painter.m_tables.size() > 1 && (m_settings.compareRmNumber || m_settings.compareStartDate))
    {
        for(int i = 1; i < m_painter.m_tables.size(); i++)
        {
            bool isCompare = false;
            if(m_settings.compareRmNumber && m_painter.m_tables[0]->m_meta.rm_number != m_painter.m_tables[i]->m_meta.rm_number)
            {
                qDebug() << "не сходяться РМ";
                isCompare = true;

                if(m_settings.styleSheet)
                {
                    QMessageBox reply;
                    styleDialog(this, &reply);
                    reply.setIcon(QMessageBox::Warning);
                    reply.setInformativeText("Номера РМ различаются.");
                    reply.setText("Внимание");
                    reply.exec();
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "Внимание", "Номера РМ различаются.", QMessageBox::Ok);
                }
            }
            else if(m_settings.compareStartDate && m_painter.m_tables[0]->m_meta.start_date + m_painter.m_tables[0]->m_meta.start_time != m_painter.m_tables[i]->m_meta.start_date + m_painter.m_tables[i]->m_meta.start_time)
            {
                qDebug() << "не сходяться Время Начала";
                isCompare = true;
                if(m_settings.styleSheet)
                {
                    QMessageBox reply;
                    styleDialog(this, &reply);
                    reply.setIcon(QMessageBox::Warning);
                    reply.setInformativeText("Дата и время начала различаются.");
                    reply.setText("Внимание");
                    reply.exec();
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "Внимание", "Дата и время начала различаются.", QMessageBox::Ok);
                }
            }

            if(isCompare)
            {
                for(int index_data = 0; index_data < m_fileMetaDataVector.size(); index_data++)
                {
                    if(m_fileMetaDataVector[index_data].path == m_painter.m_tables[i]->m_file.fileName())
                    {
                        ui->listFile->setCurrentRow(index_data);
                        click_on_button_deleteFile();
                        break;
                    }
                }
            }
        }
    }




    setItemListGraph();
    if(m_fileMetaDataVector.size() > 0)
    {
//        ui->action_editing->setEnabled(true);
//        ui->pushButton_editGraph->setEnabled(true);
    }
    else
    {
        ui->pushButton_editGraph->setEnabled(false); //false
        ui->action_editing->setEnabled(false); //false
    }
}


void GrapherGui::setViewMode(bool isVisible)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    m_isViewMode = isVisible;

//    ui->radioButton_graph_atParam->setVisible(!isVisible);
//    ui->radioButton_graph_atTime->setVisible(!isVisible);

    ui->action_view->setChecked(isVisible);
    ui->action_editing->setChecked(!isVisible); // !isVisible

    ui->pushButton_addAttr->setVisible(!isVisible);
    ui->pushButton_delete->setVisible(!isVisible);
    ui->pushButton_edit_save->setVisible(!isVisible);
    ui->pushButton_view->setVisible(!isVisible);
    ui->listAttribute->setVisible(!isVisible);
    ui->listLines->setVisible(!isVisible);
    ui->label_attribute->setVisible(!isVisible);

    if(ui->listGraph->currentRow() > -1)
    {
//        qDebug() << "selectItemGraph index" << index;
         ui->action_editing->setChecked(!isVisible);
    }
    else
    {
         ui->action_editing->setChecked(false);
    }

//    ui->action_scale->setEnabled(isVisible);

    if(isVisible)
    {
        ui->pushButton_deleteMode->setVisible(ui->radioButton_users->isChecked()); //
    }
    else
    {
        ui->pushButton_deleteMode->setVisible(false);
    }


    ui->pushButton_createGraph->setVisible(isVisible);
    ui->pushButton_editGraph->setVisible(isVisible); //isVisible
    ui->label_graph->setVisible(isVisible);
    ui->listGraph->setVisible(isVisible);
    ui->radioButton_default->setVisible(isVisible);
    ui->radioButton_users->setVisible(isVisible);
}


//void GrapherGui::setEditMode(bool isVisible)
//{

//}


void GrapherGui::setSetingsFile(bool isVisible)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    ui->label_file->setVisible(isVisible);
    ui->listFile->setVisible(isVisible);
    ui->pushButton_addFile->setVisible(isVisible);
    ui->pushButton_deleteFile->setVisible(isVisible);
}


void GrapherGui::setPaint(int index, bool isDefault)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setPlot();
    setBackGround(index, isDefault);
    bool assert = m_painter.assertPaint(index, isDefault);
    qDebug() << "assert" << assert;

    if(assert)
    {
        QVector<Parcer::vlineSet> vlines = m_painter.setVLinesGraph(index, isDefault);
        QVector<QwtPlotCurve*> lines;
        m_painter.drawLinesInGraph(*m_plot, vlines, lines);

        // Отрисовка вертикальных линий
        QVector<QwtPlotMarker*> markers;
        m_painter.drawVerticalLines(*m_plot, vlines, lines, markers);
    }
}


void GrapherGui::setStatusBar()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    (void)statusBar();
}


void GrapherGui::setZoomer()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QwtPlotZoomer *zoomer;
    zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, m_plot->canvas(), true);

    zoomer->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::NoButton);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::NoButton);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect4, Qt::LeftButton);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect5, Qt::NoButton);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect6, Qt::NoButton);

    zoomer->setRubberBandPen(QPen(Qt::red, 2));
//    zoomer->installEventFilter(this);

    // зумер
    connect(zoomer, SIGNAL(zoomed(const QRectF &)), this, SLOT(zoomerPlot(const QRectF &)));

//    setScaleView();
}


void GrapherGui::setPriorityLine(QVector<linesItemData> new_priority)
{
    m_is_edited = true;
    int index = m_selected_graph;

    graph_meta_data data = getMetaDataVector();
    int index_vector = data.index_vector;
    bool isDefault = data.isDefault;

    m_painter.m_vector_of_parameters[index_vector][index].is_a_not_save_edited = true;

    QVector<QPair<QString, Painter::paramCurve>> new_line_params_curve;


//        m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[i].second.is_second_y = false;

    for(int i = 0; i < new_priority.size(); i++)
    {

        QPair<QString, Painter::paramCurve> param = m_painter.m_vector_of_parameters[index_vector][index].line_params_curve[new_priority[i].index];
        new_line_params_curve.push_back(param);
        qDebug() << param.second.param_x;
    }

    m_painter.m_vector_of_parameters[index_vector][index].line_params_curve = new_line_params_curve;

    click_on_button_edit_close();

    m_painter.structToJson(index, isDefault, false, "");

    setAttributeGraph();

    setScaleView();
    setPaint(index, isDefault);
    setVisibleSecondY(m_painter.m_is_second_y);
//    createSettingsAttr(m_last_attr_selected, false);
}


bool GrapherGui::eventFilter(QObject *object, QEvent *event)
{
    //qDebug() << "=============\n" <<__FUNCTION__;

//    if(m_settings.styleSheet)
//    {

//        qDebug() << event->type() << object;


//        if(object == m_menu_param && event->type() == QEvent::Leave)
//        {
////            m_test_counter++;
//            QIcon *icon_action_new_param_line = new QIcon();
//            changeSVGColor(icon_action_new_param_line, ":/svg/icons_svg/polyline.svg", "black");
//            QString color = "black";
//            ui->action_new_param_line->setIcon(*icon_action_new_param_line);
////            ui->action_new_param_line->setData(color);
//            m_contex_menu_color.action_new_param_line = color;

//            QIcon *icon_action_new_vertical_line = new QIcon();
//            changeSVGColor(icon_action_new_vertical_line, ":/svg/icons_svg/vertical_line.svg", "black");
//            ui->action_new_vertical_line->setIcon(*icon_action_new_vertical_line);
//            m_contex_menu_color.action_new_vertical_line = color;

//        }

//        if(object == m_menu_listAttribute && event->type() == QEvent::Leave)
//        {
////            m_test_counter++;

//            QIcon *icon_action_add_line_graph_lines = new QIcon();
//            changeSVGColor(icon_action_add_line_graph_lines, ":/svg/icons_svg/create.svg", "black");
//            QString color = "black";
//            ui->action_add_line_graph_lines->setIcon(*icon_action_add_line_graph_lines);
//            m_contex_menu_color.action_add_line_graph_lines = color;

//            QIcon *icon_action_delete_lines = new QIcon();
//            changeSVGColor(icon_action_delete_lines, ":/svg/icons_svg/trash.svg", "black");
//            ui->action_delete_lines->setIcon(*icon_action_delete_lines);
//            m_contex_menu_color.action_delete_lines = color;

//        }

//        if(object == m_menu_listLines && event->type() == QEvent::Leave)
//        {
////            m_test_counter++;
//            QIcon *icon_action_add_line_graph_lines = new QIcon();
//            changeSVGColor(icon_action_add_line_graph_lines, ":/svg/icons_svg/create.svg", "black");
//            QString color = "black";
//            ui->action_add_line_graph_lines->setIcon(*icon_action_add_line_graph_lines);
//            m_contex_menu_color.action_add_line_graph_lines = color;


//            QIcon *icon_action_delete_lines = new QIcon();
//            changeSVGColor(icon_action_delete_lines, ":/svg/icons_svg/trash.svg", "black");
//            ui->action_delete_lines->setIcon(*icon_action_delete_lines);
//            m_contex_menu_color.action_delete_lines = color;
//        }

//        if(object == m_menu_listGraph && event->type() == QEvent::Leave)
//        {
////            m_test_counter++;
//            QIcon *icon_action_editing = new QIcon();
//            ui->action_editing->icon();
//            changeSVGColor(icon_action_editing, ":/svg/icons_svg/edit-image.svg", "black");
//            QString color = "black";
//            ui->action_editing->setIcon(*icon_action_editing);
//            m_contex_menu_color.action_editing = color;

//            QIcon *icon_action_save_this_graph = new QIcon();
//            changeSVGColor(icon_action_save_this_graph, ":/svg/icons_svg/png-file.svg", "black");
//            ui->action_save_this_graph->setIcon(*icon_action_save_this_graph);
//            m_contex_menu_color.action_save_this_graph = color;
//        }
////        qDebug() << "event counter" << m_test_counter;
//    }

    if(object == ui->listLines && event->type() == QEvent::ChildRemoved)
    {
        QVector<linesItemData> new_priority;
//        qDebug() << event->type();
        for(int i = 0; i < ui->listLines->count(); i++)
        {
//            qDebug() << ui->listLines->item(i)->text() << ui->listLines->item(i)->data(3) << ui->listLines->item(i)->data(4);
            linesItemData data;
            data.index = ui->listLines->item(i)->data(5).toInt();
            data.key = ui->listLines->item(i)->data(4).toString();

            new_priority.push_back(data);
        }
        setPriorityLine(new_priority);
    }

    if(object == d_panner)
    {
//        qDebug() << event->type();
    }

    if(object == d_panner && event->type() == QEvent::Show)
    {
        click_on_button_edit_close();
    }

    if(object == d_panner && event->type() == QEvent::HideToParent)
    {
//        qDebug() << event->type();
//        setAxisData(m_plot->axisScaleDiv(QwtPlot::xBottom).interval(), m_plot->axisScaleDiv(QwtPlot::yLeft).interval(), m_plot->axisScaleDiv(QwtPlot::yRight).interval());

        setLineEditData();
    }

//    if(object == d_panner && event->type() == QEvent::Hide)
//    {

//    }

    return false;
}


void GrapherGui::setAxisData(QwtInterval x, QwtInterval y, QwtInterval y2)
{

//    int index = m_selected_graph;

//    graph_meta_data data = getMetaDataVector();
//    int index_vector = data.index_vector;
//    bool isDefault = data.isDefault;

    qDebug() << "x" << x.minValue() << x.maxValue() << "y" << y.minValue()  << y.maxValue() << "y2" << y2.minValue() << y2.maxValue();


    setLineEditData();

}


void GrapherGui::wheelEvent(QWheelEvent *event)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    double width = m_plot->axisScaleDiv(QwtPlot::xBottom).interval().width();
    double height = m_plot->axisScaleDiv(QwtPlot::yLeft).interval().width();

//    qDebug() << m_plot->axisScaleDiv(QwtPlot::xBottom).interval() << m_plot->axisScaleDiv(QwtPlot::yLeft).interval() << m_plot->axisScaleDiv(QwtPlot::yRight).interval();

//    setAxisData(m_plot->axisScaleDiv(QwtPlot::xBottom).interval(), m_plot->axisScaleDiv(QwtPlot::yLeft).interval(), m_plot->axisScaleDiv(QwtPlot::yRight).interval());
    setLineEditData();



//    qDebug() << width << height << m_data_axis.width << m_data_axis.height << m_magnifier->m_scale;
    if(m_magnifier->getScale() > m_scale_max -1)
    {
        double S_now = width * height;
        if(S_now < m_data_axis.S_rect)
        {
//            qDebug() << "S_now" << S_now << "m_data_axis" << m_data_axis.S_rect;
            m_magnifier->resetScale();
            setZoomer();
        }
    }
    m_plot->axisScaleDraw(QwtPlot::yLeft)->label(height);
}

void GrapherGui::moveAndDrag()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    setZoomer();

    m_magnifier = new MyMagnifier(m_plot->canvas());
    m_magnifier->setMouseButton(Qt::MidButton);
    m_magnifier->setScaleRange(m_scale_min, m_scale_max);
//    m_magnifier->installEventFilter(m_plot);


    d_panner = new QwtPlotPanner(m_plot->canvas());
    d_panner->setMouseButton(Qt::RightButton);
    d_panner->installEventFilter(this);

    // Включить отображение координат курсора и двух перпендикулярных
    // линий в месте его отображения
    // #include <qwt_plot_picker.h>

    // настройка функций
    QwtPlotPicker *d_picker = new QwtPlotPicker(
        QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
        QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
        QwtPicker::ActiveOnly, // включение/выключение
        m_plot->canvas()); // ассоциация с полем


    // Цвет перпендикулярных линий
    d_picker->setRubberBandPen(QColor(Qt::red));

    // цвет координат положения указателя
    d_picker->setTrackerPen(QColor(Qt::black));


    // непосредственное включение вышеописанных функций
    d_picker->setStateMachine(new QwtPickerDragPointMachine());


    // коннектить нужно именно к d_picker, но не к d_plot!
    connect(d_picker, SIGNAL(appended(const QPoint &)), SLOT(click_on_canvas(const QPoint &)));
}


void GrapherGui::setCurve(QwtPlotCurve *curve)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    // Кривая
    //#include <qwt_plot_curve.h>

    curve->setTitle("Demo Curve");
    curve->setPen( Qt::blue, 2 ); // цвет и толщина кривой
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true); // сглаживание

    // Добавить точки на ранее созданную кривую
    QPolygonF points;

    points << QPointF(1.0, 1.0) // координаты x, y
            << QPointF(1.5, 2.0) << QPointF( 3.0, 2.0 )
            << QPointF(3.5, 3.0) << QPointF( 5.0, 3.0 );

    curve->setSamples(points); // ассоциировать набор точек с кривой

    curve->attach(m_plot); // отобразить кривую на графике

    m_plot->setAxisScale(QwtPlot::xBottom, 0, 10);
}


void GrapherGui::setBackGround(int index, bool isDefault)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    m_painter.setParamGraph(index, isDefault);
    m_painter.setSettingsBackground(*m_plot, *m_grid, m_interactiveFontSize);
    m_plot->replot();
}

void GrapherGui::setPlot()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
//    delete ui->grapherWidget->layout();
//    ui->grapherWidget->
    if(m_layoutPlot != nullptr)
    {
        delete m_layoutPlot;
        m_layoutPlot = nullptr;
    }

    if(m_plot != nullptr)
    {
        delete m_plot;
        m_plot = nullptr;
    }

    m_plot = new QwtPlot();
    m_grid = new QwtPlotGrid();

//    m_plot->installEventFilter(this);

    m_layoutPlot = new QHBoxLayout(ui->grapherWidget);
    m_layoutPlot->addWidget(m_plot);
    m_layoutPlot->setMargin(0);
    ui->grapherWidget->setLayout(m_layoutPlot);

    moveAndDrag();
}

GrapherGui::graph_meta_data GrapherGui::getMetaDataVector()
{
    graph_meta_data data;
    if(ui->radioButton_default->isChecked())
    {
        data.index_vector = 0;
        data.isDefault = true;
    }
    else if(ui->radioButton_users->isChecked())
    {
        data.index_vector = 1;
        data.isDefault = false;
    }

    return data;
}


void GrapherGui::readSettingsJsonfile(QString pathSettings)
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QFile file;
    m_settings.pathSettings = pathSettings;
    file.setFileName(m_settings.pathSettings);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Неудалось открыть Json файл" << file.fileName();
        return;
    }

    QTextStream in(&file);
    QString str;

    while(!in.atEnd())
    {
        str += in.readLine();
    }

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray byteArray = codec->fromUnicode(str);
    QJsonParseError parceError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray, &parceError);
    if(parceError.error != QJsonParseError::NoError)
    {
        qDebug() << "Ошибка чтения Json:" << parceError.offset << ":" << parceError.errorString();
        return;
    }
//    qDebug() << jsonDoc.object()["fileFilter"].toString();
//    qDebug() << jsonDoc;


    m_settings.fileFilter = jsonDoc.object()["fileFilter"].toString();
//    #ifdef Q_OS_WIN32
//        m_settings.fileFilter = m_painter.utf8tolocal(m_settings.fileFilter);
//    #endif
    m_settings.pathToAssociateFile = jsonDoc.object()["pathToAssociateFile"].toString();
    m_settings.pathToDefaultJsonFile = jsonDoc.object()["pathToDefaultJsonFile"].toString();
    m_settings.pathToUserJsonFile = jsonDoc.object()["pathToUserJsonFile"].toString();
    m_settings.startPathFindFile = jsonDoc.object()["startPathFindFile"].toString();
    m_settings.embeded = jsonDoc.object()["embeded"].toBool();
    m_settings.pathToTargetFile = jsonDoc.object()["pathToTargetFile"].toString();
    m_settings.userIsEnable = jsonDoc.object()["userIsEnable"].toBool();
    m_settings.styleSheet = jsonDoc.object()["styleSheet"].toBool();
    m_settings.compareRmNumber = jsonDoc.object()["compareRmNumber"].toBool();
    m_settings.compareStartDate = jsonDoc.object()["compareStartDate"].toBool();
    m_settings.useRusTitle = jsonDoc.object()["useRusTitle"].toBool();

    m_settings.colorIconsAttributeEdit.color = jsonDoc.object()["color"].toObject()["colorIconsAttributeEdit"].toObject()["color"].toString();
    m_settings.colorIconsAttributeEdit.border_color = jsonDoc.object()["color"].toObject()["colorIconsAttributeEdit"].toObject()["border_color"].toString();
    m_settings.colorIconsAttributeEdit.size_border = jsonDoc.object()["color"].toObject()["colorIconsAttributeEdit"].toObject()["border_size"].toInt();

    m_settings.colorIconsButton.color = jsonDoc.object()["color"].toObject()["colorIconsButton"].toObject()["color"].toString();
    m_settings.colorIconsButton.border_color = jsonDoc.object()["color"].toObject()["colorIconsButton"].toObject()["border_color"].toString();
    m_settings.colorIconsButton.size_border = jsonDoc.object()["color"].toObject()["colorIconsButton"].toObject()["border_size"].toInt();

    m_settings.colorIconsContextMenu.color = jsonDoc.object()["color"].toObject()["colorIconsContextMenu"].toObject()["color"].toString();
    m_settings.colorIconsContextMenu.border_color = jsonDoc.object()["color"].toObject()["colorIconsContextMenu"].toObject()["border_color"].toString();
    m_settings.colorIconsContextMenu.size_border = jsonDoc.object()["color"].toObject()["colorIconsContextMenu"].toObject()["border_size"].toInt();

    m_settings.colorIconsToolBar.color = jsonDoc.object()["color"].toObject()["colorIconsToolBar"].toObject()["color"].toString();
    m_settings.colorIconsToolBar.border_color = jsonDoc.object()["color"].toObject()["colorIconsToolBar"].toObject()["border_color"].toString();
    m_settings.colorIconsToolBar.size_border = jsonDoc.object()["color"].toObject()["colorIconsToolBar"].toObject()["border_size"].toInt();

    m_settings.colorIconsPolyline.color = jsonDoc.object()["color"].toObject()["polyline"].toObject()["colorIconsPolyline"].toObject()["color"].toString();
    m_settings.colorIconsPolyline.border_color = jsonDoc.object()["color"].toObject()["polyline"].toObject()["colorIconsPolyline"].toObject()["border_color"].toString();
    m_settings.colorIconsPolyline.size_border = jsonDoc.object()["color"].toObject()["polyline"].toObject()["colorIconsPolyline"].toObject()["border_size"].toInt();

    m_settings.colorIconsPolylineWithStyleSheets.color = jsonDoc.object()["color"].toObject()["polyline"].toObject()["colorIconsPolylineWithStyleSheets"].toObject()["color"].toString();
    m_settings.colorIconsPolylineWithStyleSheets.border_color = jsonDoc.object()["color"].toObject()["polyline"].toObject()["colorIconsPolylineWithStyleSheets"].toObject()["border_color"].toString();
    m_settings.colorIconsPolylineWithStyleSheets.size_border = jsonDoc.object()["color"].toObject()["polyline"].toObject()["colorIconsPolylineWithStyleSheets"].toObject()["border_size"].toInt();

//    qDebug() << jsonDoc.object()["color"].toObject()["polyline"].toObject() << m_settings.colorIconsPolylineWithStyleSheets.border_color;

    if(m_settings.colorIconsAttributeEdit.color.isEmpty())
    {
        m_settings.colorIconsAttributeEdit.color = "none";
    }

    if(m_settings.colorIconsAttributeEdit.border_color.isEmpty())
    {
        m_settings.colorIconsAttributeEdit.border_color = "none";
    }

    if(m_settings.colorIconsButton.color.isEmpty())
    {
        m_settings.colorIconsButton.color = "none";
    }

    if(m_settings.colorIconsButton.border_color.isEmpty())
    {
        m_settings.colorIconsButton.border_color = "none";
    }

    if(m_settings.colorIconsContextMenu.color.isEmpty())
    {
        m_settings.colorIconsContextMenu.color = "none";
    }

    if(m_settings.colorIconsContextMenu.border_color.isEmpty())
    {
        m_settings.colorIconsContextMenu.border_color = "none";
    }

    if(m_settings.colorIconsToolBar.color.isEmpty())
    {
        m_settings.colorIconsToolBar.color = "none";
    }

    if(m_settings.colorIconsToolBar.border_color.isEmpty())
    {
        m_settings.colorIconsToolBar.border_color = "none";
    }

    if(m_settings.colorIconsPolyline.color.isEmpty())
    {
        m_settings.colorIconsPolyline.color = "none";
    }

    if(m_settings.colorIconsPolyline.border_color.isEmpty())
    {
        m_settings.colorIconsPolyline.border_color = "none";
    }

    if(m_settings.colorIconsPolylineWithStyleSheets.color.isEmpty())
    {
        m_settings.colorIconsPolylineWithStyleSheets.color = "none";
    }

    if(m_settings.colorIconsPolylineWithStyleSheets.border_color.isEmpty())
    {
        m_settings.colorIconsPolylineWithStyleSheets.border_color = "none";
    }

    m_painter.m_settings.m_defaultJsonFolder = m_settings.pathToDefaultJsonFile;
    m_painter.m_settings.m_userJsonFolder = m_settings.pathToUserJsonFile;
}


void GrapherGui::readAssociateFile()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    QFile file;
    file.setFileName(m_settings.pathToAssociateFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Неудалось открыть Json файл" << file.fileName();
        return;
    }

    QTextStream in(&file);
    QString str;

    while(!in.atEnd())
    {
        str += in.readLine();
    }

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray byteArray = codec->fromUnicode(str);
    QJsonParseError parceError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray, &parceError);
    if(parceError.error != QJsonParseError::NoError)
    {
        qDebug() << "Ошибка чтения Json:" << parceError.offset << ":" << parceError.errorString();
        return;
    }

    QStringList keys = jsonDoc.object().keys();

    for(int i = 0; i < keys.size(); i++)
    {
        keyRegName reg_struct;
        reg_struct.key = keys[i];
        reg_struct.name = jsonDoc.object()[keys[i]].toObject()["name"].toString();
        reg_struct.reg = jsonDoc.object()[keys[i]].toObject()["reg"].toString();

        m_vector_key.append(reg_struct);

        if(m_max_size_key.key_size_max < reg_struct.key.size())
        {
            m_max_size_key.key_size_max = reg_struct.key.size();
        }

        if(m_max_size_key.name_size_max < reg_struct.name.size())
        {
            m_max_size_key.name_size_max = reg_struct.name.size();
        }

        if(m_max_size_key.reg_size_max < reg_struct.reg.size())
        {
            m_max_size_key.reg_size_max = reg_struct.reg.size();
        }

    }

    QVectorIterator<keyRegName> key_filter(m_vector_key);

    qDebug() << "key" << m_max_size_key.key_size_max << "reg" << m_max_size_key.reg_size_max << "name" << m_max_size_key.name_size_max;

    while (key_filter.hasNext()) {
        keyRegName reg_struct = key_filter.next();

        ui->listKey->addItem(QString("%1").arg(reg_struct.key, m_max_size_key.key_size_max) + " | " +
                             QString("%1").arg(reg_struct.reg, m_max_size_key.reg_size_max) + " | " +
                             QString("%1").arg(reg_struct.name, m_max_size_key.name_size_max) + " | ");
    }

}

void GrapherGui::styleDialog(QWidget *parent, QMessageBox *msgBox)
{
    msgBox->setParent(parent);
    msgBox->setWindowOpacity(0.8);
    msgBox->setFocusPolicy(Qt::StrongFocus);
    msgBox->setModal(true);
    msgBox->setWindowModality(Qt::ApplicationModal);
    //setWindowState(windowState() | Qt::WindowFullScreen);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(parent);
    shadowEffect->setBlurRadius(15.0);
    shadowEffect->setColor(QColor(0,0,0,160));
    shadowEffect->setOffset(6.0);
    msgBox->setGraphicsEffect(shadowEffect);

    QFont font;
    font.setPointSize(16);
    msgBox->setFont(font);
    msgBox->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
}


GrapherGui::~GrapherGui()
{
    //qDebug() << "=============\n" <<__FUNCTION__;
    delete ui;
}

