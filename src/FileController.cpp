#include "FileController.h"

#include <QCoreApplication>
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
    m_fileRequests.enqueue( filePath.toLocalFile() );
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
            const auto currentFile = m_fileRequests.dequeue();
        }
        m_mutexRequests.unlock();

        QThread::sleep( 1 );
    }

    emit finished();
}
