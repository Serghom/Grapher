#include "parcer.h"
#include <stdio.h>

Parcer::Parcer(QString filename, QString splitter, QString group, QMap<QString, QStringList> paramsList, int time_ind)
{
    m_splitter = splitter;
    m_group = group;
    m_file.setFileName(filename);

    QVector<QString> paramsV = paramsList[m_group].toSet().toList().toVector();



//    qDebug() << paramsList[m_group].toSet().toList().toVector().indexOf("F") << paramsList[m_group].toSet().toList().toVector();


    qDebug() <<"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << "\nПарсинг таблицы группы:" << m_group;
    qDebug() << "Кол-во параметров:" << paramsV.size();

    int size_parameters_of_key = paramsV.size();

    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << m_file.errorString() << m_file.fileName();
        is_success = false;
        return;
    }
    int counter_lines = 0;
    if(size_parameters_of_key != 0)
    {
        qDebug() << "====\nПоиск заголовка по заданным параметрам";
        QTextStream in(&m_file);

        while(!in.atEnd())
        {
            counter_lines++;
            QString line = in.readLine();
            QStringList fields = line.split(m_splitter);
            bool is_title_founded = false;
            QStringList trimed_fields = {};

            if(!is_all_title_writen && fields.size() == 1)
            {
                QStringList str = fields[0].split(": ");
                if(str[0] == "Номер изделия")
                {
                    m_meta.rm_number = str[1];
                    qDebug() << "Номер изделия" << m_meta.rm_number;
                }

                if(str[0] == "Дата начала")
                {
                    m_meta.start_date = str[1];
                    qDebug() << "Дата начала" << m_meta.start_date;
                }

                if(str[0] == "Время начала")
                {
                    m_meta.start_time = str[1];
                    qDebug() << "Время начала" << m_meta.start_time;
                }

            }

            if(!is_all_title_writen && fields.size() > 1)
            {

                for(int i = 0; i < fields.size(); i++)
                {
                    trimed_fields.append(fields[i].trimmed());
                }

                for(int i = 0; i < size_parameters_of_key; i++)
                {

                    bool ref;
                    fields[0].trimmed().replace(",", ".").toDouble(&ref);
    //                qDebug() << "Данные:" << fields[0] <<  "удалось ли перевести в число:" << ref;
                    if(ref)
                    {
                        break;
                    }
                    qDebug() << paramsV[i] << "fields.size()" << trimed_fields.size() << "fields[0]" << trimed_fields[0];
                    if(trimed_fields.indexOf(paramsV[i]) != -1)
                    {
        //                qDebug() << "Тут шапка" << trimed_fields;
        //                qDebug() << fields;
                        m_all_titles.append(trimed_fields);
                        is_title_founded = true;
                        qDebug() << "Есть совпадение";
                        break;
                    }
                }


                if(fields.size() > 1 && !is_title_founded && !m_all_titles.isEmpty())
                {
                    is_all_title_writen = true;
                }
            }


            if(is_all_title_writen && !is_title_found)
            {
                qDebug() << "===\nУдаление лишних заголовков";
                qDebug() << "Кол-во заголовков:" << m_all_titles.size();



                int indParam = 0;
                while (m_all_titles.size() > 1 && indParam +1 < m_all_titles[0].size())
                {
                    for(int indTitle = 0; indTitle < m_all_titles.size(); indTitle++)
                    {
                        qDebug() << "Заголовок #" << indTitle << "param #" << indParam << m_all_titles[indTitle][indParam];

//                        qDebug() << "Рег" << m_all_titles[indTitle][indParam].indexOf(QRegExp("[А-яЁё]")) << m_all_titles[indTitle][indParam];

                        if(m_all_titles[indTitle][indParam].indexOf(QRegExp("[А-яЁё]")) != -1)
                        {
                            m_all_rus_title.append(m_all_titles[indTitle]);
                            m_all_titles.remove(indTitle);

                            indTitle--;
                            qDebug() << "Есть русский\nУдаление лишнего заголовка. Осталось заголовков:" << m_all_titles.size();
                        }


                    }
                    indParam++;
                }



                indParam = 0;
                while (m_all_titles.size() > 1 && size_parameters_of_key - 1 > indParam)
                {
                    for(int indTitle = 0; indTitle < m_all_titles.size(); indTitle++)
                    {
                        qDebug() << "Заголовок #" << indTitle << "param #" << indParam << paramsV[indParam];
                        if(m_all_titles[indTitle].indexOf(paramsV[indParam]) == -1)
                        {
                            bool is_delite_title = false;
                            for(int j = 0; j < m_all_titles.size(); j++)
                            {
                                if(m_all_titles[j].contains(paramsV[indParam]))
                                {
                                    is_delite_title = true;
                                }
                            }
                            if(is_delite_title)
                            {
                                m_all_titles.remove(indTitle);
                                indTitle--;
                                qDebug() << "Есть расхождение\nУдаление лишнего заголовка. Осталось заголовков:" << m_all_titles.size();
                            }
    //                        break;
                        }
                    }
                    indParam++;
                }
    //            qDebug() << "Шапка:" << m_all_titles[0];
                is_title_found = true;
            }




            if(is_title_found && !is_title_writen)
            {
                for(int i = 0; i < m_all_rus_title.size(); i++)
                {
                    if(m_all_rus_title[i].size() != m_all_titles[0].size())
                    {
                        m_all_rus_title.remove(i);
                        i--;
                    }
                }

//                if(m_all_rus_title.size() > 0 && m_all_titles[0].size() == m_all_rus_title[0].size())
//                {
//                    for(int i = 0; i < m_all_rus_title[0].size(); i++)
//                    {
//                        qDebug() << "RUS" <<  m_all_rus_title[0][i] << "ENG" << m_all_titles[0][i];
//                    }
//                }



                QStringList title_from_file = m_all_titles[0];
                for(int i = 0; i < title_from_file.size(); i++)
                {
                    if(i == title_from_file.size() -1)
                    {
                        if(title_from_file[i].trimmed() == "")
                        {
                            is_last_void = true;
                            break;
                        }
                    }
                    TitleInd title;
                    title.name = title_from_file[i].trimmed();
                    title.ind = i;
                    m_title["eng"].append(title);

                }
                if(m_all_rus_title.size() > 0)
                {
                    QStringList title_from_file_rus = m_all_rus_title[0];
                    for(int i = 0; i < title_from_file_rus.size(); i++)
                    {
                        if(i == title_from_file_rus.size() -1)
                        {
                            if(title_from_file_rus[i].trimmed() == "")
                            {
                                is_last_void = true;
                                break;
                            }
                        }
                        TitleInd title;
                        title.name = title_from_file_rus[i].trimmed();
                        title.ind = i;
                        m_title["rus"].append(title);

                    }
                }

                is_title_writen = true;
            }

            if(is_title_writen && fields.size() > 1)
            {
                int undo = 0;
                if(is_last_void)
                {
                    undo = 1;
                }

    //            m_last_data_string = "";
                for(int i = 0; i < m_title["eng"].size(); i++)
                {
                    if(fields[m_title["eng"][i].ind].trimmed() != "-")
                    {
                        QPair<double, QVariant> data_pair;

    //                    data_pair.second = fields[i].trimmed();
                        // TODO После первого заполнения запомнить типы параметров, в случае если они не совпали - не добовлять. Чтобы в тип дабл не попал тип строки и так далее
                        data_pair.first = fields[time_ind].trimmed().replace(",", ".").toDouble();

                        bool ref;
                        double d_data = fields[m_title["eng"][i].ind].trimmed().replace(",", ".").toDouble(&ref);

                        if(ref)
                        {
                            data_pair.second = d_data;
                        }
                        else
                        {
                            data_pair.second = fields[m_title["eng"][i].ind].trimmed();
                        }
                        m_data[m_title["eng"][i].name].append(data_pair);
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "Нет параметров, поиск шапки";
        QTextStream in(&m_file);

        while(!in.atEnd())
        {
            counter_lines++;
            QString line = in.readLine();
            QStringList fields = line.split(m_splitter);
            bool is_title_founded = false;
            QStringList trimed_fields = {};

            if(!is_all_title_writen && fields.size() > 1)
            {

                for(int i = 0; i < fields.size(); i++)
                {
                    trimed_fields.append(fields[i].trimmed());
                }

                for(int i = 0; i < trimed_fields.size(); i++)
                {

                    bool ref;
                    fields[0].trimmed().replace(",", ".").toDouble(&ref);
    //                qDebug() << "Данные:" << fields[0] <<  "удалось ли перевести в число:" << ref;
                    if(ref)
                    {
                        break;
                    }
//                    qDebug() << paramsV[i] << "fields.size()" << trimed_fields.size() << "fields[0]" << trimed_fields[0];


                    m_all_titles.append(trimed_fields);
                    is_title_founded = true;
                    qDebug() << "Есть совпадение";
                    break;
                }


                if(fields.size() > 1 && !is_title_founded && !m_all_titles.isEmpty())
                {
                    is_all_title_writen = true;
                }
            }

            if(is_all_title_writen && !is_title_found)
            {
                qDebug() << "===\nУдаление лишних заголовков";
                qDebug() << "Кол-во заголовков:" << m_all_titles.size();



                int indParam = 0;
                while (m_all_titles.size() > 1 && indParam +1 < m_all_titles[0].size())
                {
                    for(int indTitle = 0; indTitle < m_all_titles.size(); indTitle++)
                    {
                        qDebug() << "Заголовок #" << indTitle << "param #" << indParam << m_all_titles[indTitle][indParam];

//                        qDebug() << "Рег" << m_all_titles[indTitle][indParam].indexOf(QRegExp("[А-яЁё]")) << m_all_titles[indTitle][indParam];

                        if(m_all_titles[indTitle][indParam].indexOf(QRegExp("[А-яЁё]")) != -1)
                        {
                            m_all_rus_title.append(m_all_titles[indTitle]);
                            m_all_titles.remove(indTitle);

                            indTitle--;
                            qDebug() << "Есть расхождение\nУдаление лишнего заголовка. Осталось заголовков:" << m_all_titles.size();
                        }


                    }
                    indParam++;
                }
    //            qDebug() << "Шапка:" << m_all_titles[0];
                is_title_found = true;
            }


            if(is_title_found && !is_title_writen)
            {

                for(int i = 0; i < m_all_rus_title.size(); i++)
                {
                    if(m_all_rus_title[i].size() != m_all_titles[0].size())
                    {
                        m_all_rus_title.remove(i);
                        i--;
                    }
                }

                QStringList title_from_file = m_all_titles[0];
                for(int i = 0; i < title_from_file.size(); i++)
                {
                    if(i == title_from_file.size() -1)
                    {
                        if(title_from_file[i].trimmed() == "")
                        {
                            is_last_void = true;
                            break;
                        }
                    }
                    TitleInd title;
                    title.name = title_from_file[i].trimmed();
                    title.ind = i;
                    m_title["eng"].append(title);

                }

                if(m_all_rus_title.size() > 0)
                {
                    QStringList title_from_file_rus = m_all_rus_title[0];
                    for(int i = 0; i < title_from_file_rus.size(); i++)
                    {
                        if(i == title_from_file_rus.size() -1)
                        {
                            if(title_from_file_rus[i].trimmed() == "")
                            {
                                is_last_void = true;
                                break;
                            }
                        }
                        TitleInd title;
                        title.name = title_from_file_rus[i].trimmed();
                        title.ind = i;
                        m_title["rus"].append(title);

                    }
                }

                is_title_writen = true;
            }

            if(is_title_writen && fields.size() > 1)
            {
                int undo = 0;
                if(is_last_void)
                {
                    undo = 1;
                }

    //            m_last_data_string = "";
                for(int i = 0; i < m_title["eng"].size(); i++)
                {
                    if(fields[m_title["eng"][i].ind].trimmed() != "-")
                    {
                        QPair<double, QVariant> data_pair;

    //                    data_pair.second = fields[i].trimmed();
                        // TODO После первого заполнения запомнить типы параметров, в случае если они не совпали - не добовлять. Чтобы в тип дабл не попал тип строки и так далее
                        data_pair.first = fields[time_ind].trimmed().replace(",", ".").toDouble();

                        bool ref;
                        double d_data = fields[m_title["eng"][i].ind].trimmed().replace(",", ".").toDouble(&ref);

                        if(ref)
                        {
                            data_pair.second = d_data;
                        }
                        else
                        {
                            data_pair.second = fields[m_title["eng"][i].ind].trimmed();
                        }
                        m_data[m_title["eng"][i].name].append(data_pair);
                    }
                }
            }
        }

    }


//    qDebug() << "m_data.keys().size()" << m_data.keys().size();
    qDebug() << m_data.keys();
    qDebug() << "Кол-во строк:" << counter_lines;
    qDebug() << "Группа:" << m_group;

    m_file.close();
}

Parcer::ParamVec Parcer::getVectorD(QString key, double move_x, double move_y)
{
    ParamVec output;
    output.y_max = output.y_min = m_data[key][0].second.toDouble() + move_y;
    for(int i = 0; i < m_data[key].size(); i++)
    {
        double y = m_data[key][i].second.toDouble() + move_y;
        output.time.push_back(m_data[key][i].first + move_x);
        output.data.push_back(y);
        if(y > output.y_max)
        {
            output.y_max = y;
        }

        if(y < output.y_min)
        {
            output.y_min = y;
        }
    }

    output.size = output.time.size();

    return output;
}

Parcer::ParamVecStr Parcer::getVectorStr(QString key, double move_x)
{

    ParamVecStr output;
    for(int i = 0; i < m_data[key].size(); i++)
    {
        QVariant var_data_string = m_data[key][i].second;

        QString data_string = var_data_string.toString();

        bool ref;
        double d_data = m_data[key][i].second.toDouble(&ref);

        if(data_string != m_last_data_string && !ref)
        {
//            qDebug() << "time" << m_data[key][i].first + move_x << "data_string" << data_string;
            output.data.push_back(data_string);
            output.time.push_back(m_data[key][i].first + move_x);
        }

        m_last_data_string = var_data_string.toString();

    }

    output.size = output.time.size();
    m_last_data_string = "";
    return output;
}



Parcer::ParamXYVec Parcer::getXYVectorD(QString key_x, QString key_y, double move_x, double move_y)
{
    ParamXYVec output;
    int x_size = m_data[key_x].size();
    int y_size = m_data[key_y].size();


    if(x_size == y_size)
    {
        for(int i = 0; i < x_size; i++)
        {
            if(m_data[key_x][i].first == m_data[key_y][i].first)
            {
                output.data_x.push_back(m_data[key_x][i].second.toDouble() + move_x);
                output.data_y.push_back(m_data[key_y][i].second.toDouble() + move_y);
            }
        }
    }
    else
    {
        if(x_size > y_size)
        {
            {
                int last_x_ind = 0;
                for(int i = 0; i < y_size; i++)
                {
                    double time_x = m_data[key_x][last_x_ind].first;
                    double time_y = m_data[key_y][i].first;
                    if(time_x == time_y)
                    {
                        output.data_x.push_back(m_data[key_x][last_x_ind].second.toDouble() + move_x);
                        output.data_y.push_back(m_data[key_y][i].second.toDouble() + move_y);
    //                    last_y_ind = i;
                    }
                    else if(time_y > time_x)
                    {
                        while(time_x != time_y)
                        {
                            last_x_ind++;
                            time_x = m_data[key_x][last_x_ind].first;
                            time_y = m_data[key_y][i].first;
                            if(time_x > time_y)
                            {
    //                            qDebug() << "Разрыв time_x:" << time_x << "time_y" << time_y;
                                break;
                            }
                            if(time_x == time_y)
                            {
                                output.data_x.push_back(m_data[key_x][last_x_ind].second.toDouble() + move_x);
                                output.data_y.push_back(m_data[key_y][i].second.toDouble() + move_y);
    //                            qDebug() << "x sec" << m_data[key_x][i].first << "y sec" << m_data[key_y][last_y_ind].first;
                            }

                            if(m_data[key_x].size()-1 == last_x_ind)
                            {
    //                            qDebug() << "Сработало";
                                break;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            int last_y_ind = 0;
            for(int i = 0; i < x_size; i++)
            {

                double time_x = m_data[key_x][i].first;
                double time_y = m_data[key_y][last_y_ind].first;
                if(time_x == time_y)
                {
                    output.data_x.push_back(m_data[key_x][i].second.toDouble() + move_x);
                    output.data_y.push_back(m_data[key_y][last_y_ind].second.toDouble() + move_y);
//                    last_y_ind = i;
                }
                else if(time_x > time_y)
                {
                    while(time_x != time_y)
                    {
                        last_y_ind++;
                        time_x = m_data[key_x][i].first;
                        time_y = m_data[key_y][last_y_ind].first;
                        if(time_y > time_x)
                        {
//                            qDebug() << "Разрыв time_x:" << time_x << "time_y" << time_y;
                            break;
                        }
                        if(time_x == time_y)
                        {
                            output.data_x.push_back(m_data[key_x][i].second.toDouble() + move_x);
                            output.data_y.push_back(m_data[key_y][last_y_ind].second.toDouble() + move_y);
//                            qDebug() << "x sec" << m_data[key_x][i].first << "y sec" << m_data[key_y][last_y_ind].first;
                        }

                        if(m_data[key_y].size()-1 == last_y_ind)
                        {
//                            qDebug() << "Сработало";
                            break;
                        }
                    }
                }
            }
        }
    }
    output.size = output.data_x.size();
    return output;
}

QString Parcer::engTitleToRus(QString eng_name)
{

    if(m_title.contains("rus") && !eng_name.isEmpty())
    {
        for(int i = 0; i < m_title["eng"].size(); i++)
        {
            if(m_title["eng"][i].name == eng_name)
            {
                return m_title["rus"][m_title["eng"][i].ind].name;
            }
        }
        return eng_name;
    }
    else
    {
        return eng_name;
    }


}

QString Parcer::rusTitleToEng(QString rus_name)
{
    if(m_title.contains("eng") && !rus_name.isEmpty())
    {
        for(int i = 0; i < m_title["rus"].size(); i++)
        {
            if(m_title["rus"][i].name == rus_name)
            {
                return m_title["eng"][m_title["rus"][i].ind].name;
            }
        }
        return rus_name;
    }
    else
    {
        return rus_name;
    }


}

