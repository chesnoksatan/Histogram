#include "MainWindow.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>

namespace {
constexpr auto MAIN_WINDOW = "qrc:/main.qml";
constexpr auto IMPORT = "qrc:/";
} // namespace

MainWindow::MainWindow( QObject *parent )
    : QObject( parent ),
      m_engine( std::make_shared< QQmlApplicationEngine >(
          new QQmlApplicationEngine( this ) ) ),
      m_mainWindow( nullptr )
{
    m_engine->rootContext()->setContextProperty( "Histogram", this );
    m_engine->addImportPath( IMPORT );

    QQmlComponent component( m_engine.get(), QUrl( MAIN_WINDOW ) );
    m_mainWindow = component.create();
}

MainWindow::~MainWindow()
{
    if ( m_mainWindow != nullptr )
        delete m_mainWindow;
}

void MainWindow::getDict( const std::vector< std::pair< QString, quint64 > > &dict )
{
    QJsonObject tmp;

    std::for_each( dict.cbegin(), dict.cend(), [&tmp]( const auto &elem ) {
        tmp.insert( elem.first, QJsonValue::fromVariant( elem.second ) );
    } );

    emit getValue( tmp );
}
