#include "Core.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    Core core;

    return app.exec();
}
