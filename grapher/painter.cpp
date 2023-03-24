#include <painter.h>

Painter::Painter(QApplication &app, ParamPainter settings)
{
    m_settings = settings;
    m_app = &app;

    m_secondYRota = new QwtScaleDraw();
    m_secondYRota->setLabelRotation(180);

    //position
    painter_position position;

    position.position = "top_left";
    position.position_rus = "Сверху-слева";
    position.qt_position_first = Qt::AlignLeft;
    position.qt_position_second = Qt::AlignTop;
    position.path_toIcon = ":/image/icons/legend_pos_left_top.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_left_top.svg";
    m_map_position["top_left"] = position;

    position.position = "top_right";
    position.position_rus = "Сверху-справа";
    position.qt_position_first = Qt::AlignRight;
    position.qt_position_second = Qt::AlignTop;
    position.path_toIcon = ":/image/icons/legend_pos_right_top.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_right_top.svg";
    m_map_position["top_right"] = position;

    position.position = "top_center";
    position.position_rus = "Сверху";
    position.qt_position_first = Qt::AlignHCenter;
    position.qt_position_second = Qt::AlignTop;
    position.path_toIcon = ":/image/icons/legend_pos_top.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_top.svg";
    m_map_position["top_center"] = position;

    position.position = "bottom_left";
    position.position_rus = "Снизу-слева";
    position.qt_position_first = Qt::AlignLeft;
    position.qt_position_second = Qt::AlignBottom;
    position.path_toIcon = ":/image/icons/legend_pos_left_bottom.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_left_bottom.svg";
    m_map_position["bottom_left"] = position;

    position.position = "bottom_right";
    position.position_rus = "Снизу-справа";
    position.qt_position_first = Qt::AlignRight;
    position.qt_position_second = Qt::AlignBottom;
    position.path_toIcon = ":/image/icons/legend_pos_right_bottom.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_right_bottom.svg";
    m_map_position["bottom_right"] = position;

    position.position = "bottom_center";
    position.position_rus = "Снизу";
    position.qt_position_first = Qt::AlignHCenter;
    position.qt_position_second = Qt::AlignBottom;
    position.path_toIcon = ":/image/icons/legend_pos_bottom.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_bottom.svg";
    m_map_position["bottom_center"] = position;

    position.position = "left_center";
    position.position_rus = "Слева";
    position.qt_position_first = Qt::AlignLeft;
    position.qt_position_second = Qt::AlignVCenter;
    position.path_toIcon = ":/image/icons/legend_pos_left.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_left.svg";
    m_map_position["left_center"] = position;

    position.position = "right_center";
    position.position_rus = "Справа";
    position.qt_position_first = Qt::AlignRight;
    position.qt_position_second = Qt::AlignVCenter;
    position.path_toIcon = ":/image/icons/legend_pos_right.png";
    position.path_toIconSVG = ":/svg/icons_svg/legend_pos_right.svg";
    m_map_position["right_center"] = position;
    //=-=-=-=-=-=-=-=-=-=-=-=


    // style_symbols
    painter_style_symbols style_symbols;

    style_symbols.style_symbols = "Ellipse";
    style_symbols.style_symbols_rus = "Элипс";
    style_symbols.qwt_style_symbols = QwtSymbol::Ellipse;
    m_map_style_symbols["Ellipse"] = style_symbols;

    style_symbols.style_symbols = "Rect";
    style_symbols.style_symbols_rus = "Квадрат";
    style_symbols.qwt_style_symbols = QwtSymbol::Rect;
    m_map_style_symbols["Rect"] = style_symbols;

    style_symbols.style_symbols = "Diamond";
    style_symbols.style_symbols_rus = "Алмаз";
    style_symbols.qwt_style_symbols = QwtSymbol::Diamond;
    m_map_style_symbols["Diamond"] = style_symbols;

    style_symbols.style_symbols = "Triangle";
    style_symbols.style_symbols_rus = "Треугольник";
    style_symbols.qwt_style_symbols = QwtSymbol::Triangle;
    m_map_style_symbols["Triangle"] = style_symbols;

    style_symbols.style_symbols = "DTriangle";
    style_symbols.style_symbols_rus = "Треуголник вниз";
    style_symbols.qwt_style_symbols = QwtSymbol::DTriangle;
    m_map_style_symbols["DTriangle"] = style_symbols;

    style_symbols.style_symbols = "UTriangle";
    style_symbols.style_symbols_rus = "Треуголник вверх";
    style_symbols.qwt_style_symbols = QwtSymbol::UTriangle;
    m_map_style_symbols["UTriangle"] = style_symbols;

    style_symbols.style_symbols = "LTriangle";
    style_symbols.style_symbols_rus = "Треуголник влево";
    style_symbols.qwt_style_symbols = QwtSymbol::LTriangle;
    m_map_style_symbols["LTriangle"] = style_symbols;

    style_symbols.style_symbols = "RTriangle";
    style_symbols.style_symbols_rus = "Треуголник вправо";
    style_symbols.qwt_style_symbols = QwtSymbol::RTriangle;
    m_map_style_symbols["RTriangle"] = style_symbols;

    style_symbols.style_symbols = "Cross";
    style_symbols.style_symbols_rus = "Крест";
    style_symbols.qwt_style_symbols = QwtSymbol::Cross;
    m_map_style_symbols["Cross"] = style_symbols;

    style_symbols.style_symbols = "XCross";
    style_symbols.style_symbols_rus = "X-Крест";
    style_symbols.qwt_style_symbols = QwtSymbol::XCross;
    m_map_style_symbols["XCross"] = style_symbols;

    style_symbols.style_symbols = "HLine";
    style_symbols.style_symbols_rus = "Горизонтальная линия";
    style_symbols.qwt_style_symbols = QwtSymbol::HLine;
    m_map_style_symbols["HLine"] = style_symbols;

    style_symbols.style_symbols = "VLine";
    style_symbols.style_symbols_rus = "Вертикальная линия";
    style_symbols.qwt_style_symbols = QwtSymbol::VLine;
    m_map_style_symbols["VLine"] = style_symbols;

    style_symbols.style_symbols = "Star1";
    style_symbols.style_symbols_rus = "Звезда1";
    style_symbols.qwt_style_symbols = QwtSymbol::Star1;
    m_map_style_symbols["Star1"] = style_symbols;

    style_symbols.style_symbols = "Star2";
    style_symbols.style_symbols_rus = "Звезда2";
    style_symbols.qwt_style_symbols = QwtSymbol::Star2;
    m_map_style_symbols["Star2"] = style_symbols;

    style_symbols.style_symbols = "Hexagon";
    style_symbols.style_symbols_rus = "Шестиугольник";
    style_symbols.qwt_style_symbols = QwtSymbol::Hexagon;
    m_map_style_symbols["Hexagon"] = style_symbols;
    //=-=-=-=-=-=-=-=-=-=-=-=


    // penStyle
    painter_penStyle penStyle;

    penStyle.penStyle = "SolidLine";
    penStyle.penStyle_rus = "Целая";
    penStyle.qt_penStyle = Qt::SolidLine;
    m_map_penStyle["SolidLine"] = penStyle;

    penStyle.penStyle = "DashLine";
    penStyle.penStyle_rus = "Прирывистая";
    penStyle.qt_penStyle = Qt::DashLine;
    m_map_penStyle["DashLine"] = penStyle;

    penStyle.penStyle = "DotLine";
    penStyle.penStyle_rus = "Точечная";
    penStyle.qt_penStyle = Qt::DotLine;
    m_map_penStyle["DotLine"] = penStyle;

    penStyle.penStyle = "DashDotLine";
    penStyle.penStyle_rus = "Прирывисто-точечная";
    penStyle.qt_penStyle = Qt::DashDotLine;
    m_map_penStyle["DashDotLine"] = penStyle;

    penStyle.penStyle = "DashDotDotLine";
    penStyle.penStyle_rus = "Прирывисто-точечная-точечная";
    penStyle.qt_penStyle = Qt::DashDotDotLine;
    m_map_penStyle["DashDotDotLine"] = penStyle;
    //=-=-=-=-=-=-=-=-=-=-=-=


    // orientation
    painter_orientation orientation;

    orientation.orientation = "Horizontal";
    orientation.orientation_rus = "Горизонтальный";
    orientation.qt_orientation = Qt::Horizontal;
    m_map_orientation["Horizontal"] =  orientation;

    orientation.orientation = "Vertical";
    orientation.orientation_rus = "Вертикальный";
    orientation.qt_orientation = Qt::Vertical;
    m_map_orientation["Vertical"] =  orientation;
    //=-=-=-=-=-=-=-=-=-=-=-=


    // colors
    painter_color color;

    color.color = "blue";
    color.color_rus = "Синий";
    color.qt_color = Qt::blue;
    m_map_colors["blue"] = color;

    color.color = "red";
    color.color_rus = "Красный";
    color.qt_color = Qt::red;
    m_map_colors["red"] = color;

    color.color = "yellow";
    color.color_rus = "Желтый";
    color.qt_color = Qt::yellow;
    m_map_colors["yellow"] = color;

    color.color = "green";
    color.color_rus = "Зеленый";
    color.qt_color = Qt::green;
    m_map_colors["green"] = color;

    color.color = "darkCyan";
    color.color_rus = "Темно-бирюзовый";
    color.qt_color = Qt::darkCyan;
    m_map_colors["darkCyan"] = color;

    color.color = "black";
    color.color_rus = "Черный";
    color.qt_color = Qt::black;
    m_map_colors["black"] = color;

    color.color = "darkGray";
    color.color_rus = "Темно-серый";
    color.qt_color = Qt::darkGray;
    m_map_colors["darkGray"] = color;

    color.color = "gray";
    color.color_rus = "Серый";
    color.qt_color = Qt::gray;
    m_map_colors["gray"] = color;

    color.color = "lightGray";
    color.color_rus = "Светло-серый";
    color.qt_color = Qt::lightGray;
    m_map_colors["lightGray"] = color;

    color.color = "cyan";
    color.color_rus = "Бирюзовый";
    color.qt_color = Qt::cyan;
    m_map_colors["cyan"] = color;

    color.color = "magenta";
    color.color_rus = "Фиолетовый";
    color.qt_color = Qt::magenta;
    m_map_colors["magenta"] = color;

    color.color = "darkYellow";
    color.color_rus = "Темно-желтый";
    color.qt_color = Qt::darkYellow;
    m_map_colors["darkYellow"] = color;

    color.color = "darkRed";
    color.color_rus = "Темно-красный";
    color.qt_color = Qt::darkRed;
    m_map_colors["darkRed"] = color;

    color.color = "darkGreen";
    color.color_rus = "Темно-зеленый";
    color.qt_color = Qt::darkGreen;
    m_map_colors["darkGreen"] = color;

    color.color = "darkBlue";
    color.color_rus = "Тёмно-синий";
    color.qt_color = Qt::darkBlue;
    m_map_colors["darkBlue"] = color;

    color.color = "darkMagenta";
    color.color_rus = "Тёмно-фиолетовый";
    color.qt_color = Qt::darkMagenta;
    m_map_colors["darkMagenta"] = color;
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


}

Painter::~Painter()
{
//    delete m_app;
}

void Painter::setFlagWaterMark(bool wm)
{
    m_is_write_water_mark = wm;
}

void Painter::output(QwtPlot *plot, QString targetPath)
{
    const int width  = 1920;
    const int height = 1080;

//    const int width  = 3840;
//    const int height = 2160;

//    const int width  = 7680;
//    const int height = 4320;

    QwtPlotRenderer *plotRender;
    plotRender = new QwtPlotRenderer();


    QImage image( width,height, QImage::Format_RGB32 );
    image.fill(0xFFFFFF);

    QPainter painter(&image);




    plotRender->render(plot, &painter, QRectF(50, 20, width-100, height-50));
//    plotRender->render(plot, &painter, QRectF(50, 20, width, height-50));


    QString str_water_mark = "РМ " + m_tables[0]->m_meta.rm_number + " " +  m_tables[0]->m_meta.start_date + " " + m_tables[0]->m_meta.start_time;
    int point_size = 15;

    QPoint point;
//    qDebug() << "отступ water mark" << (str_water_mark.size() * (point_size -3));
    point.setX(width-(str_water_mark.size() * (point_size -3)));
    point.setY(height-35);

    QColor color(0, 0, 0);

    QBrush br;
    br.setColor(color);
    QFont font;
    font.setPointSize(point_size);

    painter.setBrush(br);
    painter.setFont(font);


    if(m_is_write_water_mark)
    {
        painter.drawText(point, str_water_mark);
    }


    image.save(targetPath, "PNG");
}


void Painter::setPenColor(QPen *pen, QString color_line)
{
    QRegularExpression hexMatcher("^#(?:[0-9a-fA-F]{3}){1,2}$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = hexMatcher.match(color_line);



    if(match.hasMatch())
    {
        pen->setColor(QColor(color_line));
    }
    else
    {
        if(m_map_colors.contains(color_line))
        {
            pen->setColor(m_map_colors[color_line].qt_color);
        }
        else
        {
            pen->setColor(Qt::blue);
        }
    }
}


void Painter::setCurvePen(QwtPlotCurve *curve, QString color_line, QString style, int width)
{
    QPen pen;
    if(width < 0)
    {
        curve->setPen(QPen(Qt::NoPen));
        return;
    }
    pen.setWidth(width);



    if(m_map_penStyle.contains(style))
    {
        pen.setStyle(m_map_penStyle[style].qt_penStyle);
    }
    else
    {
        pen.setStyle(Qt::SolidLine);
    }

    setPenColor(&pen, color_line);


    curve->setPen(pen);
}


void Painter::setPointSymbol(QwtSymbol *symb, QString color_line, int dots_size, QString dots_style, bool isBorder)
{

    qDebug() << "isBorder" << isBorder;

    symb->setSize(QSize(dots_size, dots_size));

    if(m_map_style_symbols.contains(dots_style))
    {
        symb->setStyle(m_map_style_symbols[dots_style].qwt_style_symbols);
    }
    else
    {
        symb->setStyle(QwtSymbol::Diamond);
    }


    QRegularExpression hexMatcher("^#(?:[0-9a-fA-F]{3}){1,2}$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = hexMatcher.match(color_line);

    if(match.hasMatch())
    {
        symb->setColor(QColor(color_line));
        if(isBorder)
        {
            symb->setPen(QPen(Qt::black, 1));
        }
        else
        {
            symb->setPen(QPen(QColor(color_line), 1));
        }
    }
    else
    {

        if(m_map_colors.contains(color_line))
        {
            symb->setColor(m_map_colors[color_line].qt_color);
            if(isBorder)
            {
                symb->setPen(QPen(Qt::black, 1));
            }
            else
            {
                symb->setPen(QPen(m_map_colors[color_line].qt_color, 1));
            }

        }
        else
        {
            symb->setColor(Qt::blue);
            if(isBorder)
            {
                symb->setPen(QPen(Qt::black, 1));
            }
            else
            {
                symb->setPen(QPen(Qt::blue, 1));
            }
        }
    }


//    point->setValue(QPointF(data.time[ind], data.data[ind]));
}


void Painter::setLegendPos(QwtPlotLegendItem *legendItem, QString pos_legend)
{

//    QStringList pos = {"top_left", "top_right", "top_center", "bottom_left", "bottom_right", "bottom_center", "left_center", "right_center"};

    if(m_map_position.contains(pos_legend))
    {
        legendItem->setAlignment(m_map_position[pos_legend].qt_position_first | m_map_position[pos_legend].qt_position_second);
    }
    else
    {
        legendItem->setAlignment(Qt::AlignRight | Qt::AlignTop);
    }
}


bool Painter::readInteractiveJson()
{

    {
        QDir dir(m_settings.m_defaultJsonFolder);
        QFileInfoList dirContent = dir.entryInfoList(QStringList() << "*.json", QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        int size_json_folder = dirContent.size();
        qDebug() << "Кол-во json файлов:" << size_json_folder;
        for(int i = 0; i < size_json_folder; i++)
        {
            qDebug() << "JsonDirContent" << QString("%1").arg(i, 3) << dirContent.at(i).absoluteFilePath();
            QFile file;
            file.setFileName(dirContent.at(i).absoluteFilePath());

            if(!file.open(QIODevice::ReadOnly))
            {
                qDebug() << "Неудалось открыть Json файл" << dirContent.at(i).absoluteFilePath();
                return false;
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
                return false;
            }
            for(int indJson = 0; indJson < jsonDoc.array().size(); indJson++)
            {
                m_defaultJsonArr += jsonDoc.array()[indJson];
            }

        }
    }
    {
        QDir dir(m_settings.m_userJsonFolder);
        QFileInfoList dirContent = dir.entryInfoList(QStringList() << "*.json", QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        int size_json_folder = dirContent.size();
        qDebug() << "Кол-во json файлов:" << size_json_folder;
        for(int i = 0; i < size_json_folder; i++)
        {
            qDebug() << "JsonDirContent" << QString("%1").arg(i, 3) << dirContent.at(i).absoluteFilePath();
            QFile file;
            file.setFileName(dirContent.at(i).absoluteFilePath());

            if(!file.open(QIODevice::ReadOnly))
            {
                qDebug() << "Неудалось открыть Json файл" << dirContent.at(i).absoluteFilePath();
                return false;
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
                return false;
            }
            for(int indJson = 0; indJson < jsonDoc.array().size(); indJson++)
            {
                m_userJsonArr += jsonDoc.array()[indJson];
            }

        }
    }
    return true;
}


bool Painter::readJsonFile()
{

    if(m_settings.is_jFol)
    {
        QDir dir(m_settings.jsonFile);
        QFileInfoList dirContent = dir.entryInfoList(QStringList() << "*.json", QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        int size_json_folder = dirContent.size();
        qDebug() << "Кол-во json файлов:" << size_json_folder;
        for(int i = 0; i < size_json_folder; i++)
        {
            qDebug() << "JsonDirContent" << QString("%1").arg(i, 3) << dirContent.at(i).absoluteFilePath();
            QFile file;
            file.setFileName(dirContent.at(i).absoluteFilePath());

            if(!file.open(QIODevice::ReadOnly))
            {
                qDebug() << "Неудалось открыть Json файл" << dirContent.at(i).absoluteFilePath();
                return false;
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
                return false;
            }
            for(int indJson = 0; indJson < jsonDoc.array().size(); indJson++)
            {
                m_jsonArr += jsonDoc.array()[indJson];
            }

        }
    }
    else
    {
        qDebug() << "Read Json File:";

        QFile file;
        file.setFileName(m_settings.jsonFile);
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Неудалось открыть Json файл" << m_settings.jsonFile;
            return false;
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
            return false;
        }

        qDebug() << "Json прочтен успешно";
//        qDebug() << "Json прочтен успешно";
       m_jsonArr = jsonDoc.array();
    }

    return true;
}

//#ifdef Q_OS_WIN32
//QString Painter::utf8tolocal(const QString &text)
//{
////    text.co

////    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
////    QTextCodec *cp1251 = QTextCodec::codecForLocale();

////    QByteArray byteArrayContent;
////    byteArrayContent.insert(0, text);

////    QString sometext;
////    sometext = utf8->toUnicode(byteArrayContent);
////    sometext = cp1251->fromUnicode(sometext);

////    return sometext;

//    const auto &bytes = text.toUtf8();
//    const char *sometext = bytes.constData();
//    return QString::fromUtf8(sometext);
//}
//#endif

//#ifdef Q_OS_WIN32
//QString Painter::localtoutf8(const QString &text)
//{
////    const auto &bytes = text.toUtf8();
////    const char *sometext = bytes.constData();
//    return text.toUtf8();
//}
//#endif


void Painter::parceJson()
{
    if(m_settings.is_interactive)
    {
        if(!readInteractiveJson())
        {
            return;
        }

        for(int graph = 0; graph < m_defaultJsonArr.size(); graph++)
        {
            QJsonArray graphArr = m_defaultJsonArr[graph].toObject()["graph"].toArray();

//            qDebug() << "========================";
            for(int i = 0; i < graphArr.size(); i++)
            {

                QString key = graphArr[i].toObject().keys()[0];
                QJsonObject paramObj = graphArr[i].toObject()[key].toObject();
//                qDebug() << key << paramObj;

                QString param_x = paramObj["param_x"].toString();
                QString param_y = paramObj["param_y"].toString();
                QString param_line = paramObj["param"].toString();

//                qDebug() << "param_line" << param_line;

                if(!param_x.isEmpty())
                {
                    m_params[key].append(param_x);
                }

                if(!param_y.isEmpty())
                {
                    m_params[key].append(param_y);
                }

                if(!param_line.isEmpty())
                {
                    m_params[key].append(param_line);
                }
            }
        }

        for(int graph = 0; graph < m_userJsonArr.size(); graph++)
        {
            QJsonArray graphArr = m_userJsonArr[graph].toObject()["graph"].toArray();
            for(int i = 0; i < graphArr.size(); i++)
            {
                QString key = graphArr[i].toObject().keys()[0];
                QJsonObject paramObj = graphArr[i].toObject()[key].toObject();

                QString param_x = paramObj["param_x"].toString();
                QString param_y = paramObj["param_y"].toString();
                QString param_line = paramObj["param"].toString();

                if(!param_x.isEmpty())
                {
                    m_params[key].append(param_x);
                }

                if(!param_y.isEmpty())
                {
                    m_params[key].append(param_y);
                }

                if(!param_line.isEmpty())
                {
                    m_params[key].append(param_line);
                }
            }
        }

    }
    else
    {
        if(!readJsonFile())
        {
            return;
        }

    //    QVector<paramToTable> listParams;
        for(int graph = 0; graph < m_jsonArr.size(); graph++)
        {
            QJsonArray graphArr = m_jsonArr[graph].toObject()["graph"].toArray();
            for(int i = 0; i < graphArr.size(); i++)
            {
                QString key = graphArr[i].toObject().keys()[0];
                QJsonObject paramObj = graphArr[i].toObject()[key].toObject();
//                qDebug() << key << paramObj;

                QString param_x = paramObj["param_x"].toString();
                QString param_y = paramObj["param_y"].toString();
                QString param_line = paramObj["param"].toString();

//                qDebug() << "param_line" << param_line;

                if(!param_x.isEmpty())
                {
                    m_params[key].append(param_x);
                }

                if(!param_y.isEmpty())
                {
                    m_params[key].append(param_y);
                }

                if(!param_line.isEmpty())
                {
                    m_params[key].append(param_line);
                }
            }
        }
    }
}


void Painter::parceSource()
{
    QStringList sources_list = m_settings.source.split(";");
    for(int i = 0; i < sources_list.size(); i++)
    {
        QStringList source = sources_list[i].split("|");

        if(source.size() == 3)
        {
            QString filename = source[2];
            QString group = source[0];
            QString splitter = source[1];
            int time_ind = 0;
            if(filename.split("{").size() == 2)
            {
                QString ind = filename.split("{")[1].split("}")[0];
                filename = filename.split("{")[0];
                time_ind = ind.toInt();
            }

            qDebug() << "filename" << filename << "group" << group << "splitter" << splitter << "time_ind" << time_ind;
            Parcer *table = new Parcer(filename, splitter, group, m_params, time_ind);
            if(!table->is_success)
            {
                return;
            }
            m_tables.push_back(table);
        }
        else
        {
            QString filename = source[1];
            QString group = source[0];
            int time_ind = 0;
            if (filename.split("{").size() == 2)
            {
                QString ind = filename.split("{")[1].split("}")[0];
                filename = filename.split("{")[0];
                time_ind = ind.toInt();
            }

            qDebug() << "\n=-=-=-=-=-=-=-=-=-=-=-=\nfilename" << filename << "group" << group << "time_ind" << time_ind;
            Parcer *table = new Parcer(filename, "|", group, m_params, time_ind);
            if(!table->is_success)
            {
                return;
            }
            m_tables.push_back(table);
        }
        // ;csv|/home/lebedenko/Desktop/test/souz.csv{1};sau|/home/lebedenko/Desktop/test/sau-bkkittp-flight-31002-21.09.2022_18.07.02.350.txt;gnss|/home/lebedenko/Desktop/test/gnss-32005-24.08.2022_12.10.48.597.txt

    }


    for(int i = 0; i < m_tables.size(); i++)
    {
        m_allGroupFromTables.push_back(m_tables[i]->m_group);
    }
}

void Painter::readCurrentGraphFromJson(int index_vector, int index_json)
{
    qDebug() << "index_vector" << index_vector << "index_json" << index_json << m_all_jsons[index_vector][index_json].toObject()["title_main"].toString();

    paramGraph set_graph;
    set_graph.main_title = m_all_jsons[index_vector][index_json].toObject()["title_main"].toString();
    set_graph.x_title = m_all_jsons[index_vector][index_json].toObject()["title_x"].toString();
    set_graph.y_title = m_all_jsons[index_vector][index_json].toObject()["title_y"].toString();
    set_graph.second_y_title = m_all_jsons[index_vector][index_json].toObject()["second_title_y"].toString();


    set_graph.pos_legend = m_all_jsons[index_vector][index_json].toObject()["legend_pos"].toString();


    set_graph.x_min_scale = m_all_jsons[index_vector][index_json].toObject()["x_min_scale"].toVariant().toDouble();
    set_graph.x_max_scale = m_all_jsons[index_vector][index_json].toObject()["x_max_scale"].toVariant().toDouble();

    set_graph.y_min_scale = m_all_jsons[index_vector][index_json].toObject()["y_min_scale"].toVariant().toDouble();
    set_graph.y_max_scale = m_all_jsons[index_vector][index_json].toObject()["y_max_scale"].toVariant().toDouble();

    set_graph.second_y_min_scale = m_all_jsons[index_vector][index_json].toObject()["second_y_min_scale"].toVariant().toDouble();
    set_graph.second_y_max_scale = m_all_jsons[index_vector][index_json].toObject()["second_y_max_scale"].toVariant().toDouble();

    set_graph.move_x = m_all_jsons[index_vector][index_json].toObject()["move_x"].toVariant().toDouble();
    set_graph.move_y = m_all_jsons[index_vector][index_json].toObject()["move_y"].toVariant().toDouble();
    set_graph.move_second_y = m_all_jsons[index_vector][index_json].toObject()["move_second_y"].toVariant().toDouble();

    set_graph.namefile = m_all_jsons[index_vector][index_json].toObject()["namefile"].toString();

    set_graph.namefile_readed_from_folder = set_graph.namefile;

    set_graph.is_from_json = true;


    QStringList list = m_settings.targetPath.split("/");

    if(list[list.size()-1] == "")
    {
        QDir dir(m_settings.targetPath);
        if(!dir.exists())
        {
            dir.mkpath(".");
        }
        QString namefile = m_all_jsons[index_vector][index_json].toObject()["namefile"].toString();

        set_graph.nameTargetFile = m_settings.targetPath + namefile  + ".png";
    }
    else
    {
        QDir dir(m_settings.targetPath);
        if(!dir.exists())
        {
            dir.mkpath(".");
        }
        QString namefile = m_all_jsons[index_vector][index_json].toObject()["namefile"].toString();
        set_graph.nameTargetFile = m_settings.targetPath + "/" + namefile + ".png";
    }

    int size_graph = m_all_jsons[index_vector][index_json].toObject()["graph"].toArray().size();
    for(int i = 0; i < size_graph; i++)
    {
        QString key_curve = m_all_jsons[index_vector][index_json].toObject()["graph"].toArray()[i].toObject().keys()[0];
        QJsonObject param_curve = m_all_jsons[index_vector][index_json].toObject()["graph"].toArray()[i].toObject()[key_curve].toObject();
//            qDebug() << "key_curve" << key_curve;

        QPair<QString, paramCurve> pair_key_param;

        paramCurve param;
        param.is_second_y = param_curve["second_y"].toBool();
        param.param_line = param_curve["param"].toString();
        param.param_x = param_curve["param_x"].toString();
        param.param_y = param_curve["param_y"].toString();
        param.name_line = param_curve["name"].toString();

        param.width_line = param_curve["width"].toInt();
        param.dash_line = param_curve["dash"].toString();
        param.color_line = param_curve["color"].toString();
        param.is_dots = param_curve["dots"].toBool();
        param.dots_size = param_curve["dots_size"].toInt();
        param.dots_style = param_curve["dots_style"].toString();
        param.is_Border = param_curve["is_Border"].toBool();

        pair_key_param = {key_curve, param};

        set_graph.line_params_curve.push_back(pair_key_param);
    }

    QJsonArray vlinesArray;
    vlinesArray = m_all_jsons[index_vector][index_json].toObject()["vlines"].toArray();
    for(int i = 0; i < vlinesArray.size(); i++)
    {
        paramVline data;
        data.text = vlinesArray[i].toObject()["text"].toString();
        data.text_orientation = vlinesArray[i].toObject()["text_orientation"].toString();
        data.text_font = vlinesArray[i].toObject()["text_font_size"].toVariant().toInt();
        data.text_x = vlinesArray[i].toObject()["text_coord"].toObject()["x"].toVariant().toDouble();
        data.text_y = vlinesArray[i].toObject()["text_coord"].toObject()["y"].toVariant().toDouble();

//            #ifdef Q_OS_WIN32
//                data.text = utf8tolocal(data.text);
//            #endif

        data.color = vlinesArray[i].toObject()["color"].toString();
        data.dash = vlinesArray[i].toObject()["dash"].toString();
        data.width = vlinesArray[i].toObject()["width"].toVariant().toInt();

        data.y_max = vlinesArray[i].toObject()["y_max"].toVariant().toDouble();
        data.y_min = vlinesArray[i].toObject()["y_min"].toVariant().toDouble();
        data.point_x = vlinesArray[i].toObject()["point_x"].toVariant().toDouble();

        set_graph.vlines.push_back(data);
    }

    if(index_vector == 0)
    {
        m_defaultJsonArr[index_json] = m_all_jsons[index_vector][index_json];
    }
    if(index_vector == 1)
    {
        m_userJsonArr[index_json] = m_all_jsons[index_vector][index_json];
    }
    m_vector_of_parameters[index_vector][index_json] = set_graph;



}

void Painter::readJsonGraphToVectorStruct()
{
    m_all_jsons.append(m_defaultJsonArr);
    m_all_jsons.append(m_userJsonArr);

    for(int graph = 0; graph < m_defaultJsonArr.size(); graph++)
    {
//        qDebug() << "========================";
        paramGraph set_graph;
        set_graph.main_title = m_defaultJsonArr[graph].toObject()["title_main"].toString();
        set_graph.x_title = m_defaultJsonArr[graph].toObject()["title_x"].toString();
        set_graph.y_title = m_defaultJsonArr[graph].toObject()["title_y"].toString();
        set_graph.second_y_title = m_defaultJsonArr[graph].toObject()["second_title_y"].toString();


        set_graph.pos_legend = m_defaultJsonArr[graph].toObject()["legend_pos"].toString();


        set_graph.x_min_scale = m_defaultJsonArr[graph].toObject()["x_min_scale"].toVariant().toDouble();
        set_graph.x_max_scale = m_defaultJsonArr[graph].toObject()["x_max_scale"].toVariant().toDouble();

        set_graph.y_min_scale = m_defaultJsonArr[graph].toObject()["y_min_scale"].toVariant().toDouble();
        set_graph.y_max_scale = m_defaultJsonArr[graph].toObject()["y_max_scale"].toVariant().toDouble();

        set_graph.second_y_min_scale = m_defaultJsonArr[graph].toObject()["second_y_min_scale"].toVariant().toDouble();
        set_graph.second_y_max_scale = m_defaultJsonArr[graph].toObject()["second_y_max_scale"].toVariant().toDouble();

        set_graph.move_x = m_defaultJsonArr[graph].toObject()["move_x"].toVariant().toDouble();
        set_graph.move_y = m_defaultJsonArr[graph].toObject()["move_y"].toVariant().toDouble();
        set_graph.move_second_y = m_defaultJsonArr[graph].toObject()["move_second_y"].toVariant().toDouble();

        set_graph.namefile = m_defaultJsonArr[graph].toObject()["namefile"].toString();

        set_graph.namefile_readed_from_folder = set_graph.namefile;

        set_graph.is_from_json = true;


        QStringList list = m_settings.targetPath.split("/");

        if(list[list.size()-1] == "")
        {
            QDir dir(m_settings.targetPath);
            if(!dir.exists())
            {
                dir.mkpath(".");
            }
            QString namefile = m_defaultJsonArr[graph].toObject()["namefile"].toString();

            set_graph.nameTargetFile = m_settings.targetPath + namefile  + ".png";
        }
        else
        {
            QDir dir(m_settings.targetPath);
            if(!dir.exists())
            {
                dir.mkpath(".");
            }
            QString namefile = m_defaultJsonArr[graph].toObject()["namefile"].toString();
            set_graph.nameTargetFile = m_settings.targetPath + "/" + namefile + ".png";
        }

        int size_graph = m_defaultJsonArr[graph].toObject()["graph"].toArray().size();
        for(int i = 0; i < size_graph; i++)
        {
            QString key_curve = m_defaultJsonArr[graph].toObject()["graph"].toArray()[i].toObject().keys()[0];
            QJsonObject param_curve = m_defaultJsonArr[graph].toObject()["graph"].toArray()[i].toObject()[key_curve].toObject();
//            qDebug() << "key_curve" << key_curve;

            QPair<QString, paramCurve> pair_key_param;

            paramCurve param;
            param.is_second_y = param_curve["second_y"].toBool();
            param.param_line = param_curve["param"].toString();
            param.param_x = param_curve["param_x"].toString();
            param.param_y = param_curve["param_y"].toString();
            param.name_line = param_curve["name"].toString();

            param.width_line = param_curve["width"].toInt();
            param.dash_line = param_curve["dash"].toString();
            param.color_line = param_curve["color"].toString();
            param.is_dots = param_curve["dots"].toBool();
            param.dots_size = param_curve["dots_size"].toInt();
            param.dots_style = param_curve["dots_style"].toString();
            param.is_Border = param_curve["is_Border"].toBool();

            pair_key_param = {key_curve, param};

//            vector_of_param.append(param);
//            qDebug() << "=============";
//            qDebug() << "param.is_second_y" << param.is_second_y;
//            qDebug() << "param.param_line" << param.param_line;
//            qDebug() << "param.param_x" << param.param_x;
//            qDebug() << "param.param_y" << param.param_y;
//            qDebug() << "param.name_line" << param.name_line;
//            qDebug() << "param.width_line" << param.width_line;
//            qDebug() << "param.dash_line" << param.dash_line;
//            qDebug() << "param.color_line" << param.color_line;
//            qDebug() << "param.is_dots" << param.is_dots;
//            qDebug() << "param.dots_size" << param.dots_size;
//            qDebug() << "param.dots_style" << param.dots_style;
//            qDebug() << "=============";

//            set_graph.map_params_curve.insert(key_curve, vector_of_param);
            set_graph.line_params_curve.push_back(pair_key_param);
        }

        QJsonArray vlinesArray;
        vlinesArray = m_defaultJsonArr[graph].toObject()["vlines"].toArray();
        for(int i = 0; i < vlinesArray.size(); i++)
        {
            paramVline data;
            data.text = vlinesArray[i].toObject()["text"].toString();
            data.text_orientation = vlinesArray[i].toObject()["text_orientation"].toString();
            data.text_font = vlinesArray[i].toObject()["text_font_size"].toVariant().toInt();
            data.text_x = vlinesArray[i].toObject()["text_coord"].toObject()["x"].toVariant().toDouble();
            data.text_y = vlinesArray[i].toObject()["text_coord"].toObject()["y"].toVariant().toDouble();

//            #ifdef Q_OS_WIN32
//                data.text = utf8tolocal(data.text);
//            #endif

            data.color = vlinesArray[i].toObject()["color"].toString();
            data.dash = vlinesArray[i].toObject()["dash"].toString();
            data.width = vlinesArray[i].toObject()["width"].toVariant().toInt();

            data.y_max = vlinesArray[i].toObject()["y_max"].toVariant().toDouble();
            data.y_min = vlinesArray[i].toObject()["y_min"].toVariant().toDouble();
            data.point_x = vlinesArray[i].toObject()["point_x"].toVariant().toDouble();

            set_graph.vlines.push_back(data);
        }
        m_default_vector_param_graph.append(set_graph);


    }
    m_vector_of_parameters.append(m_default_vector_param_graph);

    for(int graph = 0; graph < m_userJsonArr.size(); graph++)
    {
        paramGraph set_graph;
        set_graph.main_title = m_userJsonArr[graph].toObject()["title_main"].toString();
        set_graph.x_title = m_userJsonArr[graph].toObject()["title_x"].toString();
        set_graph.y_title = m_userJsonArr[graph].toObject()["title_y"].toString();
        set_graph.second_y_title = m_userJsonArr[graph].toObject()["second_title_y"].toString();

//        #ifdef Q_OS_WIN32
//            set_graph.main_title = utf8tolocal(set_graph.main_title);
//            set_graph.x_title = utf8tolocal(set_graph.x_title);
//            set_graph.y_title = utf8tolocal(set_graph.y_title);
//            set_graph.second_y_title = utf8tolocal(set_graph.second_y_title);
//        #endif

        set_graph.pos_legend = m_userJsonArr[graph].toObject()["legend_pos"].toString();


        set_graph.x_min_scale = m_userJsonArr[graph].toObject()["x_min_scale"].toVariant().toDouble();
        set_graph.x_max_scale = m_userJsonArr[graph].toObject()["x_max_scale"].toVariant().toDouble();

        set_graph.y_min_scale = m_userJsonArr[graph].toObject()["y_min_scale"].toVariant().toDouble();
        set_graph.y_max_scale = m_userJsonArr[graph].toObject()["y_max_scale"].toVariant().toDouble();

        set_graph.second_y_min_scale = m_userJsonArr[graph].toObject()["second_y_min_scale"].toVariant().toDouble();
        set_graph.second_y_max_scale = m_userJsonArr[graph].toObject()["second_y_max_scale"].toVariant().toDouble();

        set_graph.move_x = m_userJsonArr[graph].toObject()["move_x"].toVariant().toDouble();
        set_graph.move_y = m_userJsonArr[graph].toObject()["move_y"].toVariant().toDouble();
        set_graph.move_second_y = m_userJsonArr[graph].toObject()["move_second_y"].toVariant().toDouble();

        set_graph.namefile = m_userJsonArr[graph].toObject()["namefile"].toString();

        set_graph.namefile_readed_from_folder = set_graph.namefile;

        set_graph.is_from_json = true;

//        #ifdef Q_OS_WIN32
//            set_graph.namefile = utf8tolocal(set_graph.namefile);
//        #endif

        QStringList list = m_settings.targetPath.split("/");

        if(list[list.size()-1] == "")
        {
    //            qDebug() << "Есть";
            QDir dir(m_settings.targetPath);
            if(!dir.exists())
            {
                dir.mkpath(".");
            }
            QString namefile = m_userJsonArr[graph].toObject()["namefile"].toString();

//            #ifdef Q_OS_WIN32
//                namefile = utf8tolocal(namefile);
//            #endif
            set_graph.nameTargetFile = m_settings.targetPath + namefile  + ".png";
        }
        else
        {
    //            qDebug() << "Нет";
            QDir dir(m_settings.targetPath);
            if(!dir.exists())
            {
                dir.mkpath(".");
            }
            QString namefile = m_userJsonArr[graph].toObject()["namefile"].toString();
//            #ifdef Q_OS_WIN32
//                namefile = utf8tolocal(namefile);
//            #endif
            set_graph.nameTargetFile = m_settings.targetPath + "/" + namefile + ".png";
        }

        int size_graph = m_userJsonArr[graph].toObject()["graph"].toArray().size();
//        qDebug() << size_graph;
        for(int i = 0; i < size_graph; i++)
        {
            QString key_curve = m_userJsonArr[graph].toObject()["graph"].toArray()[i].toObject().keys()[0];
            QJsonObject param_curve = m_userJsonArr[graph].toObject()["graph"].toArray()[i].toObject()[key_curve].toObject();
//            qDebug() << "key_curve" << key_curve;
//            QVector<paramCurve> vector_of_param;

            QPair<QString, paramCurve> pair_key_param;

            paramCurve param;
            param.is_second_y = param_curve["second_y"].toBool();
            param.param_line = param_curve["param"].toString();
            param.param_x = param_curve["param_x"].toString();
            param.param_y = param_curve["param_y"].toString();
            param.name_line = param_curve["name"].toString();

            param.width_line = param_curve["width"].toInt();
            param.dash_line = param_curve["dash"].toString();
            param.color_line = param_curve["color"].toString();
            param.is_dots = param_curve["dots"].toBool();
            param.dots_size = param_curve["dots_size"].toInt();
            param.dots_style = param_curve["dots_style"].toString();
            param.is_Border = param_curve["is_Border"].toBool();


            pair_key_param = {key_curve, param};

//            vector_of_param.append(param);
//            qDebug() << "=============";
//            qDebug() << "param.is_second_y" << param.is_second_y;
//            qDebug() << "param.param_line" << param.param_line;
//            qDebug() << "param.param_x" << param.param_x;
//            qDebug() << "param.param_y" << param.param_y;
//            qDebug() << "param.name_line" << param.name_line;
//            qDebug() << "param.width_line" << param.width_line;
//            qDebug() << "param.dash_line" << param.dash_line;
//            qDebug() << "param.color_line" << param.color_line;
//            qDebug() << "param.is_dots" << param.is_dots;
//            qDebug() << "param.dots_size" << param.dots_size;
//            qDebug() << "param.dots_style" << param.dots_style;
//            qDebug() << "=============";

//            set_graph.map_params_curve.insert(key_curve, vector_of_param);
            set_graph.line_params_curve.push_back(pair_key_param);
        }
//        qDebug() << m_userJsonArr[graph];

        QJsonArray vlinesArray;
        vlinesArray = m_userJsonArr[graph].toObject()["vlines"].toArray();
        qDebug() << "vlinesArray size" << vlinesArray.size() << graph << set_graph.namefile;
        for(int i = 0; i < vlinesArray.size(); i++)
        {
            paramVline data;
            data.text = vlinesArray[i].toObject()["text"].toString();
            data.text_orientation = vlinesArray[i].toObject()["text_orientation"].toString();
            data.text_font = vlinesArray[i].toObject()["text_font_size"].toVariant().toInt();
            data.text_x = vlinesArray[i].toObject()["text_coord"].toObject()["x"].toVariant().toDouble();
            data.text_y = vlinesArray[i].toObject()["text_coord"].toObject()["y"].toVariant().toDouble();

//            #ifdef Q_OS_WIN32
//                data.text = utf8tolocal(data.text);
//            #endif

            data.color = vlinesArray[i].toObject()["color"].toString();
            data.dash = vlinesArray[i].toObject()["dash"].toString();
            data.width = vlinesArray[i].toObject()["width"].toVariant().toInt();

            data.y_max = vlinesArray[i].toObject()["y_max"].toVariant().toDouble();
            data.y_min = vlinesArray[i].toObject()["y_min"].toVariant().toDouble();
            data.point_x = vlinesArray[i].toObject()["point_x"].toVariant().toDouble();

            set_graph.vlines.append(data);
//            qDebug() << "text" << data.text;
        }
//        qDebug() << "set_graph.vlines size" << set_graph.vlines.size();
        m_user_vector_param_graph.append(set_graph);
    }
    m_vector_of_parameters.append(m_user_vector_param_graph);
//    qDebug() << "test";
}

void Painter::setParamGraph(bool is_at_time)
{
    paramGraph new_graph;
    int count_new_graph = 0;
    QString name_to_new = "Новый график";
    for(int i = 0; i < m_vector_of_parameters[1].size(); i++)
    {
        QString name = m_vector_of_parameters[1][i].main_title;
        name = name.toLower();
        if(name.indexOf(QRegExp("новый график*")) != -1)
        {
            count_new_graph++;
            name_to_new = "Новый график " + QString::number(count_new_graph);
            qDebug() << "count_new_graph" << count_new_graph;
        }
    }
    new_graph.main_title = name_to_new;
    if(is_at_time)
    {

        new_graph.x_title = "Время, с";
        new_graph.y_title = "";
    }
    else
    {
        new_graph.x_title = "X";
        new_graph.y_title = "Y";
    }

    new_graph.second_y_title = "";

    new_graph.line_params_curve = {};

//    #ifdef Q_OS_WIN32
//        new_graph.main_title = utf8tolocal(new_graph.main_title);
//        new_graph.x_title = utf8tolocal(new_graph.x_title);
//        new_graph.y_title = utf8tolocal(new_graph.y_title);
//        new_graph.second_y_title = utf8tolocal(new_graph.second_y_title);
//    #endif

    new_graph.pos_legend = "top_right";


    new_graph.x_min_scale = 0;
    new_graph.x_max_scale = 0;

    new_graph.y_min_scale = 0;
    new_graph.y_max_scale = 0;

    new_graph.second_y_min_scale = 0;
    new_graph.second_y_max_scale = 0;

    new_graph.move_x = 0;
    new_graph.move_y = 0;
    new_graph.move_second_y = 0;

    new_graph.namefile = new_graph.main_title;

    new_graph.namefile_readed_from_folder = "";

//    #ifdef Q_OS_WIN32
//        new_graph.namefile = utf8tolocal(new_graph.namefile);
//    #endif

    QStringList list = m_settings.targetPath.split("/");

    if(list[list.size()-1] == "")
    {
//            qDebug() << "Есть";
        QDir dir(m_settings.targetPath);
        if(!dir.exists())
        {
            dir.mkpath(".");
        }
        QString namefile = new_graph.namefile;

//        #ifdef Q_OS_WIN32
//            namefile = utf8tolocal(namefile);
//        #endif
        new_graph.nameTargetFile = m_settings.targetPath + namefile  + ".png";
    }
    else
    {
//            qDebug() << "Нет";
        QDir dir(m_settings.targetPath);
        if(!dir.exists())
        {
            dir.mkpath(".");
        }
        QString namefile = new_graph.namefile;
//        #ifdef Q_OS_WIN32
//            namefile = utf8tolocal(namefile);
//        #endif
        new_graph.nameTargetFile = m_settings.targetPath + "/" + namefile + ".png";
    }

    m_vector_of_parameters[1].append(new_graph);
}

void Painter::setParamGraph(int graph, bool isDefault)
{
    m_param_graph.line_params_curve.clear();
    int index_vector;
    if(isDefault)
    {
        index_vector = 0;
    }
    else
    {
        index_vector = 1;
    }

    m_param_graph.main_title = m_vector_of_parameters[index_vector][graph].main_title;
    m_param_graph.x_title = m_vector_of_parameters[index_vector][graph].x_title;
    m_param_graph.y_title = m_vector_of_parameters[index_vector][graph].y_title;
    m_param_graph.second_y_title = m_vector_of_parameters[index_vector][graph].second_y_title;


    QVectorIterator<QPair<QString, paramCurve>> set_curve(m_vector_of_parameters[index_vector][graph].line_params_curve);
    while (set_curve.hasNext()) {
        QPair<QString, paramCurve> curve = set_curve.next();

        QString key = curve.first;
        paramCurve paramStruct;

        paramStruct.color_line = curve.second.color_line;
        paramStruct.dash_line = curve.second.dash_line;
        paramStruct.name_line = curve.second.name_line;
        paramStruct.param_x = curve.second.param_x;
        paramStruct.param_y = curve.second.param_y;
        paramStruct.param_line = curve.second.param_line;


        paramStruct.is_dots = curve.second.is_dots;
        paramStruct.is_second_y = curve.second.is_second_y;
        paramStruct.dots_size = curve.second.dots_size;
        paramStruct.width_line = curve.second.width_line;
        paramStruct.dots_style = curve.second.dots_style;
        paramStruct.is_Border = curve.second.is_Border;


        if(paramStruct.dots_size == 0)
        {
            paramStruct.dots_size = 1;
        }

        if(paramStruct.width_line == 0)
        {
            paramStruct.width_line = 1;
        }

        if(paramStruct.dots_style.isEmpty())
        {
            paramStruct.dots_style = "SolidLine";
        }

        m_param_graph.line_params_curve.push_back({key, paramStruct});

//        if(m_param_graph.map_params_curve[key].size() > i)
//        {
//            m_param_graph.map_params_curve[key][i] = paramStruct;
//        }
//        else
//        {
//            m_param_graph.map_params_curve[key].append(paramStruct);
//        }


    }


    m_param_graph.pos_legend = m_vector_of_parameters[index_vector][graph].pos_legend;


    m_param_graph.x_min_scale = m_vector_of_parameters[index_vector][graph].x_min_scale;
    m_param_graph.x_max_scale = m_vector_of_parameters[index_vector][graph].x_max_scale;

    m_param_graph.y_min_scale = m_vector_of_parameters[index_vector][graph].y_min_scale;
    m_param_graph.y_max_scale = m_vector_of_parameters[index_vector][graph].y_max_scale;

    m_param_graph.second_y_min_scale = m_vector_of_parameters[index_vector][graph].second_y_min_scale;
    m_param_graph.second_y_max_scale = m_vector_of_parameters[index_vector][graph].second_y_max_scale;

    m_param_graph.move_x = m_vector_of_parameters[index_vector][graph].move_x;
    m_param_graph.move_y = m_vector_of_parameters[index_vector][graph].move_y;
    m_param_graph.move_second_y = m_vector_of_parameters[index_vector][graph].move_second_y;

    m_param_graph.namefile = m_vector_of_parameters[index_vector][graph].namefile;
    m_param_graph.nameTargetFile = m_vector_of_parameters[index_vector][graph].nameTargetFile;

}

void Painter::setParamGraph(int graph)
{
    m_param_graph.main_title = m_jsonArr[graph].toObject()["title_main"].toString();
    m_param_graph.x_title = m_jsonArr[graph].toObject()["title_x"].toString();
    m_param_graph.y_title = m_jsonArr[graph].toObject()["title_y"].toString();
    m_param_graph.second_y_title = m_jsonArr[graph].toObject()["second_title_y"].toString();


    for(int i = 0; i < m_lineArr.size(); i++)
    {
        for(int indTab = 0; indTab < m_tables.size(); indTab++)
        {
            if(m_tables[indTab]->m_group == m_lineArr[i].toObject().keys()[0])
            {
                QJsonArray paramArr = m_lineArr[i].toObject()[m_tables[indTab]->m_group].toArray();
//                qDebug() << "paramArr" << paramArr.size();
//                qDebug() << "test" << m_tables[indTab]->m_group;
                QString key = m_tables[indTab]->m_group;
                for(int indParam = 0; indParam < paramArr.size(); indParam++)
                {
                    paramCurve paramStruct;

                    paramStruct.color_line = paramArr[indParam].toObject()["color"].toString();
                    paramStruct.dash_line = paramArr[indParam].toObject()["dash"].toString();
                    paramStruct.name_line = paramArr[indParam].toObject()["name"].toString();
                    paramStruct.param_x = paramArr[indParam].toObject()["param_x"].toString();
                    paramStruct.param_y =  paramArr[indParam].toObject()["param_y"].toString();
                    paramStruct.param_line = paramArr[indParam].toObject()["param"].toString();


                    paramStruct.is_dots = paramArr[indParam].toObject()["dots"].toBool();
                    paramStruct.is_second_y = paramArr[indParam].toObject()["second_y"].toBool();
                    paramStruct.dots_size = paramArr[indParam].toObject()["dots_size"].toVariant().toInt();
                    paramStruct.width_line = paramArr[indParam].toObject()["width"].toVariant().toInt();
                    paramStruct.dots_style = paramArr[indParam].toObject()["dots_style"].toString();
                    paramStruct.is_Border = paramArr[indParam].toObject()["is_Border"].toBool();

                    if(paramStruct.dots_size == 0)
                    {
                        paramStruct.dots_size = 1;
                    }
                    if(paramStruct.width_line == 0)
                    {
                        paramStruct.width_line = 1;
                    }
                    if(paramStruct.dots_style.isEmpty())
                    {
                        paramStruct.dots_style = "SolidLine";
                    }

                    m_param_graph.line_params_curve.push_back({key, paramStruct});

//                    if(m_param_graph.map_params_curve[key].size() > indParam)
//                    {
//                        m_param_graph.map_params_curve[key][indParam] = paramStruct;
//                    }
//                    else
//                    {
//                        m_param_graph.map_params_curve[key].append(paramStruct);
//                    }
                }
            }
        }
    }



//    #ifdef Q_OS_WIN32
//        m_param_graph.main_title = utf8tolocal(m_param_graph.main_title);
//        m_param_graph.x_title = utf8tolocal(m_param_graph.x_title);
//        m_param_graph.y_title = utf8tolocal(m_param_graph.y_title);
//        m_param_graph.second_y_title = utf8tolocal(m_param_graph.second_y_title);
//    #endif

    m_param_graph.pos_legend = m_jsonArr[graph].toObject()["legend_pos"].toString();


    m_param_graph.x_min_scale = m_jsonArr[graph].toObject()["x_min_scale"].toVariant().toDouble();
    m_param_graph.x_max_scale = m_jsonArr[graph].toObject()["x_max_scale"].toVariant().toDouble();

    m_param_graph.y_min_scale = m_jsonArr[graph].toObject()["y_min_scale"].toVariant().toDouble();
    m_param_graph.y_max_scale = m_jsonArr[graph].toObject()["y_max_scale"].toVariant().toDouble();

    m_param_graph.second_y_min_scale = m_jsonArr[graph].toObject()["second_y_min_scale"].toVariant().toDouble();
    m_param_graph.second_y_max_scale = m_jsonArr[graph].toObject()["second_y_max_scale"].toVariant().toDouble();

    m_param_graph.move_x = m_jsonArr[graph].toObject()["move_x"].toVariant().toDouble();
    m_param_graph.move_y = m_jsonArr[graph].toObject()["move_y"].toVariant().toDouble();
    m_param_graph.move_second_y = m_jsonArr[graph].toObject()["move_second_y"].toVariant().toDouble();

    m_param_graph.namefile = m_jsonArr[graph].toObject()["namefile"].toString();

//    #ifdef Q_OS_WIN32
//        m_param_graph.namefile = utf8tolocal(m_param_graph.namefile);
//    #endif

    QStringList list = m_settings.targetPath.split("/");

    if(list[list.size()-1] == "")
    {
//            qDebug() << "Есть";
        QDir dir(m_settings.targetPath);
        if(!dir.exists())
        {
            dir.mkpath(".");
        }
        QString namefile = m_jsonArr[graph].toObject()["namefile"].toString();

//        #ifdef Q_OS_WIN32
//            namefile = utf8tolocal(namefile);
//        #endif
        m_param_graph.nameTargetFile = m_settings.targetPath + namefile  + ".png";
    }
    else
    {
//            qDebug() << "Нет";
        QDir dir(m_settings.targetPath);
        if(!dir.exists())
        {
            dir.mkpath(".");
        }
        QString namefile = m_jsonArr[graph].toObject()["namefile"].toString();
//        #ifdef Q_OS_WIN32
//            namefile = utf8tolocal(namefile);
//        #endif
        m_param_graph.nameTargetFile = m_settings.targetPath + "/" + namefile + ".png";
    }
}

QVector<Parcer::vlineSet> Painter:: setVLinesGraph(int graph, bool isDefault)
{
    QVector<Parcer::vlineSet> vlines;
//    QJsonArray vlinesArray;
    QVector<paramVline> vlinesArray;
    if(isDefault)
    {
//        vlinesArray = m_defaultJsonArr[graph].toObject()["vlines"].toArray();
        vlinesArray = m_vector_of_parameters[0][graph].vlines;
    }
    else
    {
//        vlinesArray = m_userJsonArr[graph].toObject()["vlines"].toArray();
        vlinesArray = m_vector_of_parameters[1][graph].vlines;
    }

    for(int i = 0; i < vlinesArray.size(); i++)
    {
        Parcer::vlineSet data;
        data.text = vlinesArray[i].text;
        data.text_orientation = vlinesArray[i].text_orientation;
        data.text_font = vlinesArray[i].text_font;
        data.text_x = vlinesArray[i].text_x;
        data.text_y = vlinesArray[i].text_y;

//        #ifdef Q_OS_WIN32
//            data.text = utf8tolocal(data.text);
//        #endif

        data.color = vlinesArray[i].color;
        data.dash = vlinesArray[i].dash;
        data.width = vlinesArray[i].width;

        double y_max = vlinesArray[i].y_max;
        double y_min = vlinesArray[i].y_min;
        double point_x = vlinesArray[i].point_x;

        data.data.push_back(y_max);
        data.time.push_back(point_x);

        data.data.push_back(y_min);
        data.time.push_back(point_x);

        data.text_x += point_x;
//            qDebug() << data.text_x << data.text;
        data.text_y += y_max;
        vlines.push_back(data);
//            qDebug() << "y_max" << y_max << "point_x" << point_x << "y_min" << y_min << "point_x" << point_x;

//            qDebug() << vlinesArray[i].toObject()["y_max"].toDouble();
    }

    return vlines;
}

QVector<Parcer::vlineSet> Painter::setVLinesGraph(int graph)
{
    QVector<Parcer::vlineSet> vlines;
    QJsonArray vlinesArray = m_jsonArr[graph].toObject()["vlines"].toArray();

    for(int i = 0; i < vlinesArray.size(); i++)
    {
        Parcer::vlineSet data;
        data.text = vlinesArray[i].toObject()["text"].toString();
        data.text_orientation = vlinesArray[i].toObject()["text_orientation"].toString();
        data.text_font = vlinesArray[i].toObject()["text_font_size"].toVariant().toInt();
        data.text_x = vlinesArray[i].toObject()["text_coord"].toObject()["x"].toVariant().toDouble();
        data.text_y = vlinesArray[i].toObject()["text_coord"].toObject()["y"].toVariant().toDouble();

//        #ifdef Q_OS_WIN32
//            data.text = utf8tolocal(data.text);
//        #endif

        data.color = vlinesArray[i].toObject()["color"].toString();
        data.dash = vlinesArray[i].toObject()["dash"].toString();
        data.width = vlinesArray[i].toObject()["width"].toVariant().toInt();

        double y_max = vlinesArray[i].toObject()["y_max"].toVariant().toDouble();
        double y_min = vlinesArray[i].toObject()["y_min"].toVariant().toDouble();
        double point_x = vlinesArray[i].toObject()["point_x"].toVariant().toDouble();

        data.data.push_back(y_max);
        data.time.push_back(point_x);

        data.data.push_back(y_min);
        data.time.push_back(point_x);

        data.text_x += point_x;
//            qDebug() << data.text_x << data.text;
        data.text_y += y_max;
        vlines.push_back(data);
//            qDebug() << "y_max" << y_max << "point_x" << point_x << "y_min" << y_min << "point_x" << point_x;

//            qDebug() << vlinesArray[i].toObject()["y_max"].toDouble();
    }

    return vlines;
}

void Painter::setFontSize(bool isInteractive)
{
    if(isInteractive)
    {
        m_fontTitle.setPointSize(13);
        m_fontLegend.setPointSize(11);
        m_fontAxis.setPointSize(8);
    }
    else
    {
        m_fontTitle.setPointSize(20);
        m_fontLegend.setPointSize(15);
        m_fontAxis.setPointSize(13);
    }

}

void Painter::setSettingsBackground(QwtPlot &plot, QwtPlotGrid &grid, bool isInteractive)
{
    int x_ticks_minor = 10;

    setFontSize(isInteractive);

    grid.enableXMin(true);
    grid.enableYMin(true);
    grid.setMajorPen(QPen(Qt::black, 1, Qt::SolidLine));
    grid.setMinorPen(QPen(Qt::gray, 2, Qt::DotLine));

    grid.attach( &plot );
    plot.setCanvasBackground(QColor("white"));

    if(m_param_graph.y_min_scale != m_param_graph.y_max_scale){
        plot.setAxisScale(QwtPlot::yLeft, m_param_graph.y_min_scale, m_param_graph.y_max_scale);
    }

    if(m_param_graph.second_y_min_scale != m_param_graph.second_y_max_scale){
        plot.setAxisScale(QwtPlot::yRight, m_param_graph.second_y_min_scale, m_param_graph.second_y_max_scale);
    }

    if(m_param_graph.x_min_scale != m_param_graph.x_max_scale)
    {
        plot.setAxisScale(QwtPlot::xBottom, m_param_graph.x_min_scale, m_param_graph.x_max_scale);
    }

    QPalette palette;

    palette.setColor(QPalette::Window, QColor(255, 255, 255));
    plot.setPalette(palette);

    plot.setAxisScaleDraw(QwtPlot::xTop, new QScaleDraw);
    plot.setAxisScaleDraw(QwtPlot::xBottom, new QScaleDraw);
    plot.setAxisScaleDraw(QwtPlot::yLeft, new QScaleDraw);
    plot.setAxisScaleDraw(QwtPlot::yRight, new QScaleDraw);

    plot.setAxisFont(QwtPlot::xTop, m_fontAxis);
    plot.setAxisFont(QwtPlot::xBottom, m_fontAxis);
    plot.setAxisFont(QwtPlot::yLeft, m_fontAxis);
    plot.setAxisFont(QwtPlot::yRight, m_fontAxis);

    QwtPlotLegendItem *legendItem = new QwtPlotLegendItem();
    legendItem->attach(&plot);
    legendItem->setRenderHint(QwtPlotItem::RenderAntialiased);

    setLegendPos(legendItem, m_param_graph.pos_legend);

    legendItem->setMaxColumns(1);

    QColor color(Qt::black);
    legendItem->setTextPen(color);
    legendItem->setBorderPen(color);

    QColor color2(Qt::white);
    color2.setAlpha(200);
    legendItem->setBackgroundBrush(color2);
    legendItem->setLegendIconSize(QSize(500, 10));

    plot.setAxisMaxMinor(QwtPlot::xBottom, x_ticks_minor);
//    plot.setAxisMaxMajor(QwtPlot::yLeft, 10);


    QwtText titletoPlot;
    titletoPlot.setFont(m_fontTitle);
    titletoPlot.setText(m_param_graph.main_title);
    plot.setTitle(titletoPlot);

    QwtText XTitle;
    QwtText YTitle;
    QwtText SecondYTitle;
    XTitle.setFont(m_fontTitle);
    YTitle.setFont(m_fontTitle);
    SecondYTitle.setFont(m_fontTitle);

    XTitle.setText(m_param_graph.x_title);
    YTitle.setText(m_param_graph.y_title);
    SecondYTitle.setText(m_param_graph.second_y_title);

    XTitle.setRenderFlags(XTitle.renderFlags() | Qt::TextWordWrap | Qt::TextWrapAnywhere);
    YTitle.setRenderFlags(YTitle.renderFlags() | Qt::TextWordWrap | Qt::TextWrapAnywhere);
    SecondYTitle.setRenderFlags(SecondYTitle.renderFlags() | Qt::TextWordWrap | Qt::TextWrapAnywhere);


    plot.setAxisTitle(QwtPlot::xBottom, XTitle);
    plot.setAxisTitle(QwtPlot::yLeft, YTitle);
    plot.setAxisTitle(QwtPlot::yRight, SecondYTitle);


    plot.axisWidget(QwtPlot::yRight)->setLayoutFlag(QwtScaleWidget::TitleInverted, false);

//    plot
//    QwtScaleEngine test;
//    test.set

//    CDis

//    QwtScaleDiv scaleDiv();

//    plot.axisScaleDraw(QwtPlot::yLeft)->label();// (QwtScaleDiv::MajorTick, 100);   //->enableComponent(QwtAbstractScaleDraw::ScaleComponent::Labels, false);
//    QwtScaleDraw test_x;
//    test_x.setTickLength();

//    plot.setAxisLabelRotation(QwtPlot::yRight, 90);
//    plot.setAxisScaleDraw(QwtPlot::yRight, m_secondYRota);
//    plot.setAx
}


void Painter::delete_json_file(int graph, int index_vector, QString path_to_file)
{
    if(!m_vector_of_parameters[index_vector][graph].namefile_readed_from_folder.isEmpty())
    {
        QFile::remove(path_to_file + "/" + m_vector_of_parameters[index_vector][graph].namefile_readed_from_folder + ".json");
    }
    m_vector_of_parameters[index_vector].remove(graph);
    m_userJsonArr.removeAt(graph);
}


void Painter::structToJson(int graph, bool isDefault, bool toSave, QString path_to_save)
{
    int index_vector;
    if(isDefault)
    {
        index_vector = 0;
    }
    else
    {
        index_vector = 1;
    }

    QJsonObject mainObj;
    QJsonArray mainArr;
    QVectorIterator<QPair<QString, paramCurve>> set_curve(m_vector_of_parameters[index_vector][graph].line_params_curve);

    QJsonArray keysArr;
    while(set_curve.hasNext()) {
        QJsonArray curveArr;
        QJsonObject keysObj;
        QJsonObject paramObj;

        QPair<QString, paramCurve> curve = set_curve.next();

        QString key = curve.first;

        QJsonObject curveObj;
        curveObj["color"] = curve.second.color_line;
        curveObj["dash"] = curve.second.dash_line;
        curveObj["name"] = curve.second.name_line;
        curveObj["param"] = curve.second.param_line;
        curveObj["param_x"] = curve.second.param_x;
        curveObj["param_y"] = curve.second.param_y;

        curveObj["width"] = curve.second.width_line;
        curveObj["second_y"] = curve.second.is_second_y;

        curveObj["dots"] = curve.second.is_dots;
        curveObj["dots_size"] = curve.second.dots_size;
        curveObj["dots_style"] = curve.second.dots_style;
        curveObj["is_Border"] = curve.second.is_Border;
//        curveArr.append(curveObj);

        keysObj[key] = curveObj;
        paramObj = keysObj;

//            qDebug() << "TEST" << set_curve.key() << curveArr;
        keysArr.append(paramObj);
    }
    mainObj["graph"] = keysArr;

    QJsonArray vlinesArray;
    QJsonObject vlinesObject;
    for(int i = 0; i < m_vector_of_parameters[index_vector][graph].vlines.size(); i++)
    {

        vlinesObject["text"] = m_vector_of_parameters[index_vector][graph].vlines[i].text;
        vlinesObject["text_orientation"] = m_vector_of_parameters[index_vector][graph].vlines[i].text_orientation;
        vlinesObject["text_font_size"] = m_vector_of_parameters[index_vector][graph].vlines[i].text_font;

        QJsonObject text_cord;
        text_cord["x"] = m_vector_of_parameters[index_vector][graph].vlines[i].text_x;
        text_cord["y"] = m_vector_of_parameters[index_vector][graph].vlines[i].text_y;
        vlinesObject["text_coord"] = text_cord;

        vlinesObject["color"] = m_vector_of_parameters[index_vector][graph].vlines[i].color;
        vlinesObject["dash"] = m_vector_of_parameters[index_vector][graph].vlines[i].dash;
        vlinesObject["width"] = m_vector_of_parameters[index_vector][graph].vlines[i].width;

        vlinesObject["point_x"] = m_vector_of_parameters[index_vector][graph].vlines[i].point_x;
        vlinesObject["y_max"] = m_vector_of_parameters[index_vector][graph].vlines[i].y_max;
        vlinesObject["y_min"] = m_vector_of_parameters[index_vector][graph].vlines[i].y_min;
        vlinesArray.append(vlinesObject);
    }

    mainObj["vlines"] = vlinesArray;
    mainObj["title_main"] = m_vector_of_parameters[index_vector][graph].main_title;
    mainObj["title_x"] = m_vector_of_parameters[index_vector][graph].x_title;
    mainObj["title_y"] = m_vector_of_parameters[index_vector][graph].y_title;
    mainObj["second_title_y"] = m_vector_of_parameters[index_vector][graph].second_y_title;

    mainObj["legend_pos"] = m_vector_of_parameters[index_vector][graph].pos_legend;

    mainObj["x_max_scale"] = 0;
    mainObj["x_min_scale"] = 0;
    mainObj["y_max_scale"] = 0;
    mainObj["y_min_scale"] = 0;
    mainObj["second_y_max_scale"] = 0;
    mainObj["second_y_min_scale"] = 0;

    mainObj["move_x"] = 0;
    mainObj["move_y"] = 0;
    mainObj["move_second_y"] = 0;

    mainObj["namefile"] = m_vector_of_parameters[index_vector][graph].main_title;


    mainArr.append(mainObj);
//        qDebug() << mainArr;

//        m_lineArr = mainArr[0].toObject()["graph"].toArray();

    if(toSave)
    {


        if(index_vector == 0)
        {

            qDebug() << "!!!!!Обновление памяти JSon default";
            m_defaultJsonArr[graph] = mainObj;
            m_all_jsons[0][graph] = mainObj;
        }
        else
        {
            if(m_userJsonArr.size() > graph)
            {
                qDebug() << "!!!!!!Обновление памяти JSon user";
                m_userJsonArr[graph] = mainObj;
                m_all_jsons[1][graph] = mainObj;
            }
            else
            {
                qDebug() << "!!!!!память graph add new" << graph;
                m_userJsonArr.append(mainObj);
                m_all_jsons[1].append(mainObj);
            }

        }

        if(m_vector_of_parameters[index_vector][graph].main_title != m_vector_of_parameters[index_vector][graph].namefile_readed_from_folder)
        {
            qDebug() << "old" << m_vector_of_parameters[index_vector][graph].namefile_readed_from_folder << "new" << m_vector_of_parameters[index_vector][graph].main_title;
            QFile::remove(path_to_save + "/" + m_vector_of_parameters[index_vector][graph].namefile_readed_from_folder + ".json");
            m_vector_of_parameters[index_vector][graph].namefile_readed_from_folder = m_vector_of_parameters[index_vector][graph].main_title;
        }

        QFile file;
        QString path = path_to_save + "/" + m_vector_of_parameters[index_vector][graph].main_title + ".json";
//        #ifdef Q_OS_WIN32
//            path = localtoutf8(path);
//        #endif
        file.setFileName(path);

        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Неудалось открыть Json файл" << file.fileName();
            return;
        }

        QByteArray jsonDoc = QJsonDocument(mainArr).toJson(QJsonDocument::Indented);

//        #ifdef Q_OS_WIN32
//            jsonDoc = jsonDoc.to;
//        #endif

        file.write(jsonDoc);
        file.close();
    }
    else
    {
        if(index_vector == 0)
        {

            qDebug() << "!!!!!Обновление JSon default";
            m_defaultJsonArr[graph] = mainObj;
//            m_all_jsons[0][graph] = mainObj;
        }
        else
        {
            if(m_userJsonArr.size() > graph)
            {
                qDebug() << "!!!!!!Обновление JSon user";
                m_userJsonArr[graph] = mainObj;
//                m_all_jsons[1][graph] = mainObj;
            }
            else
            {
                qDebug() << "!!!!!graph add new" << graph;
                m_userJsonArr.append(mainObj);
//                m_all_jsons[1].append(mainObj);
            }

        }
    }

}

bool Painter::assertPaint(int graph, bool isDefault)
{

    m_sizeVlines.y_max = -99999;
    m_sizeVlines.y_min = 99999;

    if(isDefault)
    {
        m_lineArr = m_defaultJsonArr[graph].toObject()["graph"].toArray();
//        qDebug() << "m_lineArr" << m_lineArr;
        QStringList allGroupFromThisJson;
        for(int i = 0; i < m_lineArr.size(); i++)
        {
            if(allGroupFromThisJson.contains(m_lineArr[i].toObject().keys()[0]))
            {
//                qDebug() << "Такой есть";
            }
            else
            {
                allGroupFromThisJson.push_back(m_lineArr[i].toObject().keys()[0]);
            }
        }

        QStringList result;
        for(auto &x : allGroupFromThisJson)
        {
            if(m_allGroupFromTables.count(x))
            {
                result.push_back(x);
            }
        }

        if(result.size() != allGroupFromThisJson.size())
        {
            qDebug() << "График №" << QString("%1").arg(graph, 3) << "нет требуемых групп. Совпадение:" << result << "Таблицы:" << m_allGroupFromTables << "Json:" << allGroupFromThisJson;
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        m_lineArr = m_userJsonArr[graph].toObject()["graph"].toArray();
//        qDebug() << "m_lineArr" << m_lineArr;
        QStringList allGroupFromThisJson;
        for(int i = 0; i < m_lineArr.size(); i++)
        {
            if(allGroupFromThisJson.contains(m_lineArr[i].toObject().keys()[0]))
            {
//                qDebug() << "Такой есть";
            }
            else
            {
                allGroupFromThisJson.push_back(m_lineArr[i].toObject().keys()[0]);
            }

        }

        QStringList result;
        for(auto &x : allGroupFromThisJson)
        {
            if(m_allGroupFromTables.count(x))
            {
                result.push_back(x);
            }
        }

        if(result.size() != allGroupFromThisJson.size())
        {
            qDebug() << "График №" << QString("%1").arg(graph, 3) << "нет требуемых групп. Совпадение:" << result << "Таблицы:" << m_allGroupFromTables << "Json:" << allGroupFromThisJson;
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool Painter::assertPaint(int graph)
{
    m_lineArr = m_jsonArr[graph].toObject()["graph"].toArray();
//        qDebug() << lineArr << "size" << lineArr.size();
    QStringList allGroupFromThisJson;
    for(int i = 0; i < m_lineArr.size(); i++)
    {
        allGroupFromThisJson.push_back(m_lineArr[i].toObject().keys()[0]);
    }

//        qDebug() << "Json:" << allGroupFromThisJson << "Tables:" << allGroupFromTables;
    QStringList result;
    for(auto &x : allGroupFromThisJson)
    {
        if(m_allGroupFromTables.count(x))
        {
            result.push_back(x);
        }
    }

    if(result.size() != allGroupFromThisJson.size())
    {
        qDebug() << "График №" << QString("%1").arg(graph, 3) << "нет требуемых групп. Совпадение:" << result << "Таблицы:" << m_allGroupFromTables << "Json:" << allGroupFromThisJson;
        return false;
    }
    else
    {
        return true;
    }
}



void Painter::add_vertical_line(int index_vector, int graph)
{
    paramVline data;
    data.text = "Вертикальная линия";
    data.text_orientation = "Horizontal";
    data.text_font = 14;
    data.text_x = 0;
    data.text_y = 0;

//    #ifdef Q_OS_WIN32
//        data.text = utf8tolocal(data.text);
//    #endif

    data.color = m_colors[5];
    data.dash = "SolidLine";
    data.width = 3;

    data.point_x = 0;
    data.y_min = 0;
    data.y_max = 0;

    m_vector_of_parameters[index_vector][graph].vlines.append(data);
}


void Painter::delete_vertical_line(int index_vector, int graph, int index_vline)
{
    m_vector_of_parameters[index_vector][graph].vlines.removeAt(index_vline);
}

void Painter::add_line_param_x_y(QString table_group, QString param_name, QString color, int index_vector, int graph)
{
    paramCurve paramStruct;

//    int index_table;

//    for(int i = 0; i < m_tables.size();)

    paramStruct.color_line = color;
    paramStruct.dash_line = "SolidLine";
    paramStruct.name_line = "Новый график";
    paramStruct.param_x = "";
    paramStruct.param_y = "";
    paramStruct.param_line = "";


    paramStruct.is_dots = true;
    paramStruct.is_second_y = false;
    paramStruct.dots_size = 3;
    paramStruct.width_line = 2;
    paramStruct.dots_style = "Ellipse";
    paramStruct.is_Border = true;



    m_vector_of_parameters[index_vector][graph].line_params_curve.push_back({table_group, paramStruct});
}


void Painter::add_line_param(QString table_group, QString param_name, QString name_of_legend, QString color, int index_vector, int graph)
{
    paramCurve paramStruct;

    paramStruct.color_line = color;
    paramStruct.dash_line = "SolidLine";
    paramStruct.name_line = name_of_legend;
    paramStruct.param_x = "";
    paramStruct.param_y = "";
    paramStruct.param_line = param_name;


    paramStruct.is_dots = true;
    paramStruct.is_second_y = false;
    paramStruct.dots_size = 3;
    paramStruct.width_line = 2;
    paramStruct.dots_style = "Ellipse";
    paramStruct.is_Border = true;


    m_vector_of_parameters[index_vector][graph].line_params_curve.push_back({table_group, paramStruct});

//    m_param_graph.map_params_curve[table_group].append(paramStruct);
//    if(m_vector_of_parameters[index_vector][graph].map_params_curve.contains(table_group))
//    {
//        m_vector_of_parameters[index_vector][graph].map_params_curve[table_group].append(paramStruct);
//    }
//    else
//    {
//        qDebug() << "table_group" << table_group << "param_name" << param_name << m_vector_of_parameters[index_vector][graph].map_params_curve.size();
//        m_vector_of_parameters[index_vector][graph].map_params_curve[table_group].push_back(paramStruct);
//        qDebug() << "table_group" << table_group << "param_name" << param_name << m_vector_of_parameters[index_vector][graph].map_params_curve.size();
//    }

}

void Painter::delete_all_param_at_group(QString table_group, int index_vector, int graph)
{
//    m_vector_of_parameters[index_vector][graph].map_params_curve.remove(table_group);

    for(int i = 0; i < m_vector_of_parameters[index_vector][graph].line_params_curve.size(); i++)
    {
        if(table_group == m_vector_of_parameters[index_vector][graph].line_params_curve[i].first)
        {
            m_vector_of_parameters[index_vector][graph].line_params_curve.removeAt(i);
            i--;
        }
    }
}

void Painter::delete_line_param_x_y(QString table_group, QString x, QString y, int index_vector, int graph)
{
    for(int index_line_param = 0; index_line_param < m_vector_of_parameters[index_vector][graph].line_params_curve.size(); index_line_param++)
    {
        if(m_vector_of_parameters[index_vector][graph].line_params_curve[index_line_param].first == table_group)
        {
            if(x == m_vector_of_parameters[index_vector][graph].line_params_curve[index_line_param].second.param_x && y == m_vector_of_parameters[index_vector][graph].line_params_curve[index_line_param].second.param_y)
            {
                m_vector_of_parameters[index_vector][graph].line_params_curve.removeAt(index_line_param);
            }
        }
    }
}


void Painter::delete_line_param(QString table_group, QString param_name, int index_vector, int graph)
{
    for(int index_line_param = 0; index_line_param < m_vector_of_parameters[index_vector][graph].line_params_curve.size(); index_line_param++)
    {
        if(m_vector_of_parameters[index_vector][graph].line_params_curve[index_line_param].first == table_group)
        {
            if(param_name == m_vector_of_parameters[index_vector][graph].line_params_curve[index_line_param].second.param_line)
            {
                m_vector_of_parameters[index_vector][graph].line_params_curve.removeAt(index_line_param);
            }
        }
    }

}

void Painter::paint_graph(QwtPlot &plot, int indTab, int indCurve, QVector<Parcer::vlineSet> &vlines, QVector<QwtPlotCurve*> &lines)
{
    m_is_at_time = true;
//    for(int indParam = 0; indParam < m_param_graph.line_params_curve.size(); indParam++)
//    {

//        paramArr = m_lineArr[i].toObject()[m_tables[indTab]->m_group].toArray();

        QwtPlotCurve *curve = new QwtPlotCurve();
        double curve_move_y = 0;
        qDebug() << "index curve" << indCurve;
        QString color_line = m_param_graph.line_params_curve[indCurve].second.color_line;
        QString dash_line = m_param_graph.line_params_curve[indCurve].second.dash_line;
        QString name_line = m_param_graph.line_params_curve[indCurve].second.name_line;
        QString param_x = m_param_graph.line_params_curve[indCurve].second.param_x;
        QString param_y = m_param_graph.line_params_curve[indCurve].second.param_y;
        QString param_line = m_param_graph.line_params_curve[indCurve].second.param_line;

        qDebug() << "indTab" << indTab << "indCurve" << indCurve << "param_x" << param_x;

//        QString color_line = paramArr[indParam].toObject()["color"].toString();
//        QString dash_line = paramArr[indParam].toObject()["dash"].toString();
//        QString name_line = paramArr[indParam].toObject()["name"].toString();
//        QString param_x = paramArr[indParam].toObject()["param_x"].toString();
//        QString param_y = paramArr[indParam].toObject()["param_y"].toString();
//        QString param_line = paramArr[indParam].toObject()["param"].toString();

//                        qDebug() << "name_line" << name_line << "param_x" << param_x << "tables[indTab]->m_group" << tables[indTab]->m_group;

//        #ifdef Q_OS_WIN32
//            name_line = utf8tolocal(name_line);
//        #endif

        bool is_dots = m_param_graph.line_params_curve[indCurve].second.is_dots;
        bool is_second_y = m_param_graph.line_params_curve[indCurve].second.is_second_y;
        int dots_size = m_param_graph.line_params_curve[indCurve].second.dots_size;
        int width_line = m_param_graph.line_params_curve[indCurve].second.width_line;
        QString dots_style = m_param_graph.line_params_curve[indCurve].second.dots_style;
        bool isBorder = m_param_graph.line_params_curve[indCurve].second.is_Border;


//        bool is_dots = paramArr[indParam].toObject()["dots"].toBool();
//        bool is_second_y = paramArr[indParam].toObject()["second_y"].toBool();
//        int dots_size = paramArr[indParam].toObject()["dots_size"].toVariant().toInt();
//        int width_line = paramArr[indParam].toObject()["width"].toVariant().toInt();
//        QString dots_style = paramArr[indParam].toObject()["dots_style"].toString();

        if(is_second_y)
        {
            plot.enableAxis(QwtPlot::yRight, true);
            curve->setYAxis(QwtPlot::yRight);
            curve_move_y = m_param_graph.move_second_y;
            m_is_second_y = true;
        }
        else
        {
            curve->setYAxis(QwtPlot::yLeft);
            curve_move_y = m_param_graph.move_y;
        }

        setCurvePen(curve, color_line, dash_line, width_line);

        curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased);


        QwtText titleText;
        titleText.setFont(m_fontLegend);
        titleText.setText(name_line);
        curve->setTitle(titleText);

        bool is_string_param = false;
        if(!param_line.isEmpty() && !m_tables[indTab]->m_data[param_line].isEmpty())
        {
            is_string_param = m_tables[indTab]->m_data[param_line][0].second.type() == QVariant::String;
        }
        if(is_string_param)
        {

            Parcer::ParamVecStr dataStr;
            dataStr = m_tables[indTab]->getVectorStr(param_line, m_param_graph.move_x);
//            qDebug() << "Строка" << dataStr.size << dataStr.time;
//                            qDebug() << "data str size" << data.size << param_line << tables[indTab]->m_data[param_line][0].second;
//                            qDebug() << "plot.axisScaleDiv" << plot.axisScaleDiv(QwtPlot::yLeft).interval().maxValue() -10 << plot.axisScaleDiv(QwtPlot::yLeft).interval().minValue() +10;

            for(int vlineInd = 0; vlineInd < dataStr.size; vlineInd++)
            {
                Parcer::vlineSet dataVline;
                dataVline.text = dataStr.data[vlineInd];
                dataVline.text_orientation = "Horizontal";
                dataVline.text_font = 17;

//                #ifdef Q_OS_WIN32
//                    dataVline.text = utf8tolocal(dataVline.text);
//                #endif

                dataVline.color = color_line;
                dataVline.dash = dash_line;
                dataVline.width = width_line;

//                                double percent = 10;

                double point_x = dataStr.time[vlineInd];

                dataVline.text_x = point_x;
                dataVline.text_y = 0;

                dataVline.data.push_back(0);
                dataVline.time.push_back(point_x);

                dataVline.data.push_back(0);
                dataVline.time.push_back(point_x);
    //            qDebug() << data.text_x << data.text;
                vlines.push_back(dataVline);
            }

        }
        else
        {
            if(!param_x.isEmpty() && !param_y.isEmpty())
            {
                qDebug() << "Явно указаная X";
                Parcer::ParamXYVec data;
                data = m_tables[indTab]->getXYVectorD(param_x, param_y, m_param_graph.move_x, curve_move_y);
                curve->setSamples(data.data_x, data.data_y);
                m_is_at_time = false;
            }
            else if(!param_line.isEmpty())
            {
//                qDebug() << "Не явно указаная X";
                Parcer::ParamVec data;
                data = m_tables[indTab]->getVectorD(param_line, m_param_graph.move_x, curve_move_y);

//                qDebug() << "data.y_max" << QString::number(data.y_max, 'f', 9) << "data.y_min" <<  QString::number(data.y_min, 'f', 9);

                if(data.y_max > m_sizeVlines.y_max)
                {
                    m_sizeVlines.y_max = data.y_max;
                }

                if(data.y_min < m_sizeVlines.y_min)
                {
                    m_sizeVlines.y_min = data.y_min;
                }


                curve->setSamples(data.time, data.data);
                m_is_at_time = true;
            }
            else
            {
                qDebug() << "Не корректные параметры";
                m_is_at_time = false;
            }
        }

        if(is_dots)
        {
            QwtSymbol *symb = new QwtSymbol();
            setPointSymbol(symb, color_line, dots_size, dots_style, isBorder);
            curve->setSymbol(symb);
        }
        curve->setLegendIconSize(QSize(50, 10));
        curve->attach(&plot);
        lines.push_back(curve);

        qDebug() << "Replot";
        plot.replot();

//    }
}


void Painter::drawLinesInGraph(QwtPlot &plot, QVector<Parcer::vlineSet> &vlines, QVector<QwtPlotCurve*> &lines)
{
    for(int i = 0; i < m_lineArr.size(); i++)
    {
        qDebug() << "m_lineArr.size()" << m_lineArr.size();
        for(int indTab = 0; indTab < m_tables.size(); indTab++)
        {
            if(m_tables[indTab]->m_group == m_lineArr[i].toObject().keys()[0])
            {
//                QJsonArray paramArr = m_lineArr[i].toObject()[m_tables[indTab]->m_group].toArray();
                paint_graph(plot, indTab, i, vlines, lines);
            }
            else
            {
//                    qDebug() << "Не совпали: Таблица:" << tables[indTab]->m_group << "Json" << m_lineArr[i].toObject().keys()[0];
            }
        }
    }
}



void Painter::drawVerticalLines(QwtPlot &plot, QVector<Parcer::vlineSet> &vlines, QVector<QwtPlotCurve*> &lines, QVector<QwtPlotMarker*> &markers)
{
    for(int i = 0; i < vlines.size(); i++)
    {
        QwtPlotCurve *curve = new QwtPlotCurve();
        QwtPlotMarker *mark = new QwtPlotMarker();
        QString text = vlines[i].text;
        QString text_orient = vlines[i].text_orientation;
        QString color_line = vlines[i].color;
        QString dash_line = vlines[i].dash;
        int width_line = vlines[i].width;
        setCurvePen(curve, color_line, dash_line, width_line);

        if(vlines[i].data[0] == 0 && vlines[i].data[1] == 0)
        {

            text = " " + text;
//            double y_maxInterval = plot.axisScaleDiv(QwtPlot::yLeft).interval().maxValue();
//            double y_minInterval = plot.axisScaleDiv(QwtPlot::yLeft).interval().minValue();

            double y_maxInterval = m_sizeVlines.y_max;
            double y_minInterval = m_sizeVlines.y_min;

//                                double x_maxInterval = plot.axisScaleDiv(QwtPlot::xBottom).interval().maxValue();

//            double y_max = y_maxInterval * 0.8;
//            double y_min = y_minInterval * 0.8;

            double y_max = y_maxInterval;
            double y_min = y_minInterval;


//            vlines[i].text_y += y_max + (y_max * -0.04);
            vlines[i].text_y += y_max;

//                vlines[i].text_x += vlines[i].time[0];
            qDebug() << "y_max" << QString::number(y_max, 'f', 9)  << "y_min" << QString::number(y_min, 'f', 9) << "delta" <<  QString::number(y_max - y_min, 'f', 9) << "y_maxInterval" << QString::number(y_maxInterval, 'f', 9) << "y_minInterval" << QString::number(y_minInterval, 'f', 9) << "delta" << QString::number(y_maxInterval - y_minInterval, 'f', 9);


            vlines[i].data[0] = y_min;
            vlines[i].data[1] = y_max;

        }

        QFont font;
        font.setPointSize(vlines[i].text_font);

        mark->setYAxis(QwtPlot::yLeft);

        QwtText t;
        t.setFont(font);
        t.setText(text);
        mark->setLabel(t);

        mark->setValue(vlines[i].text_x, vlines[i].text_y);

        if(m_map_orientation.contains(text_orient))
        {
            mark->setLabelOrientation(m_map_orientation[text_orient].qt_orientation);
        }
        else
        {
            mark->setLabelOrientation(Qt::Horizontal);
        }


        mark->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
        mark->attach(&plot);
        markers.push_back(mark);

//            curve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol, false);
        curve->setItemAttribute(QwtPlotItem::Legend, false);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve->setYAxis(QwtPlot::yLeft);
        vlines[i].xAxis = curve->xAxis();
        vlines[i].yAxis = curve->yAxis();

        curve->setSamples(vlines[i].time, vlines[i].data);
        curve->attach(&plot);
        lines.push_back(curve);
    }
}


void Painter::finalPaint()
{
    qDebug() << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << "\nПостоение графиков";
    for(int graph = 0; graph < m_jsonArr.size(); graph++)
    {
        // Настройки графика
        setParamGraph(graph);

        // Настройка Вертикальных линий
        QVector<Parcer::vlineSet> vlines = setVLinesGraph(graph);

        // Настройки фона
        QwtPlot plot;
        QwtPlotGrid grid;
        setSettingsBackground(plot, grid, false);

        // Поиск. Содержат ли сурсы нужные ключи для графика
        if(!assertPaint(graph))
        {
            continue;
        }

        qDebug() << "Создание графика №" << QString("%1").arg(graph, 3) << m_param_graph.namefile;

        // Создание линий
        QVector<QwtPlotCurve*> lines;
        drawLinesInGraph(plot, vlines, lines);

        // Отрисовка вертикальных линий
        QVector<QwtPlotMarker*> markers;
        drawVerticalLines(plot, vlines, lines, markers);

        // TODO Фильтровать точки для постороения красивого графика (Брать минимальную и максимальную точку в чанке)
        ////        QwtSplineCurveFitter *fitter = new QwtSplineCurveFitter;
        ////        fitter->setFitMode(QwtSplineCurveFitter::ParametricSpline);
        ////        fitter->setSplineSize(1000);
        ////        curve.setCurveFitter(fitter);
        ////        curve.setCurveAttribute(QwtPlotCurve::Fitted, true);

        m_app->processEvents();

        // Отрисовка графика
        output(&plot, m_param_graph.nameTargetFile);

        // ***
        for(int i = 0; i < lines.size(); i++)
        {
            delete lines[i];
        }

        for(int i = 0; i < markers.size(); i++)
        {
            delete markers[i];
        }
    }
}


void Painter::processPaint()
{
    // Парсим Json
    parceJson();

    // Парсим таблицы и сохроняем все в QMap
    parceSource();

    // Рисовка графиков
    finalPaint();
}

