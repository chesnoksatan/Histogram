#include "FileController.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QUrl>

namespace {
struct
{
    bool operator()( std::pair< QString, quint64 > a,
                     std::pair< QString, quint64 > b ) const
    {
        return a.second > b.second;
    }
} maxCountSort;
} // namespace

FileController::FileController( QObject *parent ) : QObject( parent )
{
    qRegisterMetaType< DictionaryVector >( "DictionaryVector" );
}

FileController::~FileController()
{
}

void FileController::abort()
{
    QMutexLocker locker( &m_mutex );
    m_abort = true;
}

void FileController::slotGetFilePath( const QUrl &filePath )
{
    QMutexLocker locker( &m_mutex );
    m_fileRequests.enqueue( filePath );
}

FileController::Dictionary FileController::readFile( const QUrl &filePath )
{
    QFile currentFile( filePath.toLocalFile() );

    Dictionary container;

    int j = 0;

    if ( currentFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        const auto size = currentFile.size();

        while ( !currentFile.atEnd() )
        {
            const auto &line = trUtf8( currentFile.readLine() );
            emit getProgress(
                ( ( size - currentFile.bytesAvailable() ) * 100 ) / size );

            const auto &list = filterString( line ).split( " " );
            std::for_each( list.begin(), list.end(),
                           [&container]( const auto &word ) {
                               if ( word != "" )
                                   container[word]++;
                           } );
            //            emit getDict( getTop( container ) );
        }

        currentFile.close();
    }
    else
    {
        qDebug() << QString( "Невозможно открыть файл %1. Причина: %2" )
                        .arg( filePath.toLocalFile(), currentFile.errorString() );
    }

    return container;
}

FileController::DictionaryVector
FileController::getTop( const FileController::Dictionary &dictionary )
{
    DictionaryVector container;
    for ( auto &pair : dictionary )
        container.push_back( pair );

    /// Отсортируем элементы вектора по убыванию количества вхождений
    std::sort( container.begin(), container.end(), maxCountSort );
    /// Оставим в массиве топ 15 слов
    if ( container.size() > 15 )
        container.erase( container.begin() + 15, container.end() );
    /// Отсортируем в алфавитном порядке
    std::sort( container.begin(), container.end() );

    return container;
}

QString FileController::filterString( const QString &str )
{
    return str.toLower()
        .remove( "\n" )
        .remove( QRegExp( "\\d" ) )
        .remove( QRegExp( " [^a-zа-я\\d\\s] " ) );
}

void FileController::calculate()
{
    m_abort = false;
    forever
    {
        QCoreApplication::instance()->processEvents();

        if ( m_abort )
            break;

        m_mutex.lock();
        if ( !m_fileRequests.isEmpty() )
        {
            const auto &dict = readFile( m_fileRequests.dequeue() );
            emit getDict( getTop( dict ) );
        }
        m_mutex.unlock();

        QThread::sleep( 1 );
    }

    emit finished();
}
