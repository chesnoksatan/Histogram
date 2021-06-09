#include "FileController.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QUrl>

FileController::FileController( QObject *parent ) : QObject( parent )
{
    qRegisterMetaType< Dictionary >( "Dictionary" );
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
    qDebug() << "В очередь загрузки поступил новый файл" << filePath;
}

FileController::Dictionary FileController::readFile( const QUrl &filePath )
{
    QFile currentFile( filePath.toLocalFile() );

    Dictionary container;

    if ( currentFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        const auto size = currentFile.size();

        if ( size == 0 )
        {
            qDebug() << "На обработку пришел пустой файл";
            emit getEmptyFile();
            currentFile.close();
            return container;
        }

        bool isSend = false;
        while ( !currentFile.atEnd() )
        {
            const auto &line = trUtf8( currentFile.readLine() );

            const auto progress =
                ( ( size - currentFile.bytesAvailable() ) * 100 ) / size;
            emit getProgress( progress );

            const auto &list = filterString( line ).split( " " );
            std::for_each( list.begin(), list.end(),
                           [&container]( const auto &word ) {
                               if ( word.length() > 1 )
                                   container[word]++;
                           } );

            /// Для обновления гистограммы в условно реальном времени
            /// Если посылать при каждой итерации цикла программа сходит с ума и
            /// все перестает работать (для больших файлов)
            if ( ( progress % 5 == 0 ) && progress )
            {
                if ( !isSend )
                    emit getDict( container );
                isSend = true;
            }
            else
                isSend = false;
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
            readFile( m_fileRequests.dequeue() );

        m_mutex.unlock();

        QThread::sleep( 1 );
    }

    emit finished();
}
