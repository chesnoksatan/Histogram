#include "MainWindow.h"

namespace {
constexpr auto MAIN_WINDOW = "qrc:/main.qml";
constexpr auto IMPORT = "qrc:/";
} // namespace

MainWindow::MainWindow( QObject *parent )
    : QObject( parent ),
      m_engine( new QQmlApplicationEngine( this ) ),
      m_mainWindow( nullptr )
{
    m_engine->rootContext()->setContextProperty( "Histogram", this );
    m_engine->addImportPath( IMPORT );

    QQmlComponent component( m_engine, QUrl( MAIN_WINDOW ) );
    m_mainWindow = component.create();
}

MainWindow::~MainWindow()
{
    delete m_engine;
    if ( m_mainWindow != nullptr )
        delete m_mainWindow;
}