#include "mainwindow.h"

#include <atp/atp.hpp>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        if (ATP::initialize_translator(locale))
            break;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
