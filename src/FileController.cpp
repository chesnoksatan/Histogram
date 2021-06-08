#include "FileController.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QUrl>

FileController::FileController( QObject *parent ) : QObject( parent )
{
}

FileController::~FileController()
{
}

void FileController::slotGetFilePath( const QUrl &filePath )
{
    QMutexLocker locker( &m_mutexRequests );
    m_fileRequests.enqueue( filePath );
}

FileController::Dictionary FileController::readFile( const QUrl &filePath )
{
    QFile currentFile( filePath.toLocalFile() );

    Dictionary container;

    if ( currentFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        const auto size = currentFile.size();

        while ( !currentFile.atEnd() )
        {
            const auto &line = trUtf8( currentFile.readLine() );
            emit getProgress(
                ( ( size - currentFile.bytesAvailable() ) * 100 ) / size );

            const auto &list = line.toLower().remove( "\n" ).split( " " );
            std::for_each(
                list.begin(), list.end(),
                [&container]( const auto &word ) { container[word]++; } );
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

void FileController::getTop( const FileController::Dictionary &dictionary )
{
    Q_UNUSED( dictionary )
}

void FileController::calculate()
{
    m_abort = false;
    forever
    {
        QCoreApplication::instance()->processEvents();

        if ( m_abort )
            break;

        m_mutexRequests.lock();
        if ( !m_fileRequests.isEmpty() )
        {
            const auto &dict = readFile( m_fileRequests.dequeue() );
            getTop( dict );
        }
        m_mutexRequests.unlock();

        QThread::sleep( 1 );
    }

    emit finished();
}
