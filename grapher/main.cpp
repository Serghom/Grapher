#include <QtCore>
#include <QApplication>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextCodec>


#include "graphergui.h"

#include <painter.h>


int main(int argc, char *argv[])
{

//    QCoreApplication a(argc, argv);
    QApplication a(argc, argv);
    QApplication::setApplicationVersion("2.6.0");
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
//    QLocale::setNumberOptions(QLocale::NumberOptions)



    #ifdef Q_OS_WIN32
        std::setlocale(LC_ALL, "Russian_Russia.1251");
    #endif


    QCommandLineParser parser;
    parser.setApplicationDescription("Grapher");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption sourceOption(QStringList() << "source" << "source_file",
                                  QCoreApplication::translate("main", "source"),
                                  QCoreApplication::translate("main", "path to source file")
                                  );
    parser.addOption(sourceOption);

    QCommandLineOption jsonOption(QStringList() << "json" << "json_file",
                                  QCoreApplication::translate("main", "json"),
                                  QCoreApplication::translate("main", "path to json file")
                                  );
    parser.addOption(jsonOption);

    QCommandLineOption targetOption(QStringList() << "target" << "target_path",
                                  QCoreApplication::translate("main", "target"),
                                  QCoreApplication::translate("main", "path to target path")
                                  );
    parser.addOption(targetOption);

    QCommandLineOption settingsOption(QStringList() << "settings" << "settings_file",
                                  QCoreApplication::translate("main", "settings"),
                                  QCoreApplication::translate("main", "path to settings file")
                                  );
    parser.addOption(settingsOption);

    QCommandLineOption jsonFolder("jfolder", QCoreApplication::translate("main", "Read jsons from folder"));
    parser.addOption(jsonFolder);


    parser.process(a);
    Painter::ParamPainter set;

    set.isSource = parser.isSet(sourceOption);
    set.isJsonFile = parser.isSet(jsonOption);
    set.isTargetPath = parser.isSet(targetOption);

//    set.settingsFile = parser

    set.is_interactive = !set.isSource && !set.isJsonFile && !set.isTargetPath;

    if(!set.is_interactive)
    {
        if(!set.isSource)
        {
            qDebug() << "Не указан source";
            return 0;
        }
        else
        {
            set.source = parser.values(sourceOption).at(0);
            qDebug() << "Source File:" << set.source;
        }

        // --json "/home/lebedenko/Desktop/pythonParcer/json/jsonFolder"
        if(!set.isJsonFile)
        {
            qDebug() << "Не указан json";
            return 0;
        }
        else
        {
            set.jsonFile = parser.values(jsonOption).at(0);
            qDebug() << "Json File:" << set.jsonFile;
        }

        // --target "/home/lebedenko/Desktop/test/reports/graph"
        if(!set.isTargetPath)
        {
            qDebug() << "Не указан target";
            return 0;
        }
        else
        {
            set.targetPath = parser.values(targetOption).at(0);
            qDebug() << "Target Path:" << set.targetPath;
        }

        //  --jfolder
        set.is_jFol = parser.isSet(jsonFolder);
        qDebug() << "Read Json from folder:" << set.is_jFol;

//        QApplication app(argc, argv, true);
        Painter paint(a, set);
        paint.processPaint();

//        --json "/home/lebedenko/Desktop/test/grapher/usersJsonFolder" --source "sau|/home/lebedenko/Desktop/test/reports/sau-bk_kittp-flight-32002-01.11.2022_17.14.06.302.txt;gnss|/home/lebedenko/Desktop/test/reports/gnss-flight-32002-01.11.2022_17.14.06.335.txt" --target "/home/lebedenko/Desktop/test/reports/graph"  --jfolder

        return 0;
    }
    else
    {
        qDebug() << "Интерактивный режим";
        /* load the system translations provided by Qt */
        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(),
                QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        a.installTranslator(&qtTranslator);

        if(parser.isSet(settingsOption))
        {
            set.settingsFile = parser.values(settingsOption).at(0);
        }
        GrapherGui w(a, set);
        w.show();

        return a.exec();
    }
}

/*

             ┌m
               ▒,       ,▄▌
                ▒@▌  ▐▌ ▓▓▓ █L
                 ▐▒▒▄█▓▓▓▓▓▓▓▌░░╖  ▄▒@m
                ,║▒▒▒▒▒▓▒░░░░░╢╢▒▒█▒▒▒▌,
               @░░░▒░░░░░░░░░░░║▒▒▒▒▒▒▒▒@
             |▒░░░░░░░░░░░║▒▒▒▒▒▒▒▒▒▒▒▒▒░k▄▒▒▄m ▄▄▄▄
            ▒Ñ░░░░░░░░░▒▒▒▒▒░░░▄░╢▒▒▒▒▒▀░▒▒▒▒▒▓█▒▒▒▒
            ▒░░░░░K░║▒▒▒░░░████▓█▓▒▒▒▒▒░▒▒▒▒▒▒▒▒▒▒▌`
            ▒░░░░░░╓╓▒▒▒▓▓▓▀▒h  ▓▓▓▒▒▒░░▒▒▒▒▒▒▒▒▒▄gg
            ▒░░░░░░░█▒▒▒▓▓▓,▒h ,▒░▒▒▒░░░▒▒▒▒▒▒▒▓▒▒▒▒
             ├▒░░░░▄▒▒▒▒▓▓▓▓▒@@▒▒▒▒░░░╟▒▒▒▒▒▒▒▒▒▒▒▒"
             |▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒░░▒▒▒▒▒▒▒▓▒▒L
               ║░░░░▀▀▒▒▒▒▒▒▒▒▒▒▒░░║░░░▒▒▒▒▒█▒▒▒▒Γ ,,,,,,,,,
               `║░░░║░░░░░░░░▒▒▒░░║░░░▒▒▒▒▒▒▒▒▒▓▒╗m░░░░░░░░░╗
             ,~ ▒▒░▒░░░░▒░░░░▒▓▄╓▒▒░░░▒▒▒▒▒▒▓▓▓▀▓▌▄░░░░░░░░░░░░╓
               N░░░░░░▒▒ÑÑÑÑ ▐▓▓ ╢▒░░░░▒▒▒▒▓▓▌,,▓▓▓,Ñ░░░░░░░░░░▒
                `````        '"▄▄█▒▒▒▒▒▒▒▒██▒▓▓▓▓▓▓▓ '╙║░░░░░░░▒░km
                            µµ▄▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓   ╝░░░░░░░▒░║▒,
                         ▐██▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒▒    ▒░░░░║▒░║░░▒
                         ▐▒▒▒▒▒▒▒▒▒▒▒▒▌"""▀▒▒▒▒▒▒▒▒▒    ▒░░░╖░░░▒░░▒░km
                         ▐▒▒▒▒▒▒▒▓▓▀▀▀▀     ▒▓▒▒▒▒▒▒▒▒▒▒░░░▒░░░▒░░▒░░║▒
                         ▐▒▒▒▒▒▒▒▒▌         ▓▒▒▒▒▒▒▒▒▒▒░║║░░░░▒░░║▒░░░░@
                          "░▒▒▒▒▒▒▒W░        ▐▒▒▒▒▒▒▒▒▒▒▄░░╖░░░░╖░░░░░░▒
                            ░░▒░▒▒▒▒░         ░▒░▒▒▒▒▒▒▒▒▒▒░░░░╢░░░░░░░▒

*/
