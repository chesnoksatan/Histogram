#include "Core.h"

Core::Core( QObject *parent )
    : QObject( parent ), m_mainWindow( new MainWindow() )
{
}

Core::~Core()
{
    delete m_mainWindow;
}
