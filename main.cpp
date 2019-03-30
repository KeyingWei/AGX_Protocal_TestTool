#include "widget.h"
#include <QApplication>
#include <qstylefactory.h>
//#include "commomhelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



       Widget w;
       w.show();



    return a.exec();
}
