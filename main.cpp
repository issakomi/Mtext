#include <QApplication>
#include "charswidget.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    CharsWidget w(1);
    w.show();
    return app.exec();
}
