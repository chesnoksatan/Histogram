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

void MainWindow::getDict( const std::vector< std::pair< quint64, QString > > &dict )
{
    //    qDebug() << dict;

    QJsonObject tmp;

    for ( const auto &elem : dict )
    {
        tmp.insert( elem.second, QJsonValue::fromVariant( elem.first ) );
    }
    emit getValue( tmp );
    //    emit getVector(
    //        QVector< std::pair< quint64, QString > >::fromStdVector( dict ) );
}
