#include <QApplication>
#include "yatfen.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    qsrand(QDateTime::currentDateTime().toTime_t());

    Yatfen w;
    w.show();

    return a.exec();
}

