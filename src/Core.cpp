#include "Core.h"

Core::Core( QObject *parent )
    : QObject( parent ),
      m_mainWindow( new MainWindow() ),
      m_fileController( new FileController() ),
      m_fileThread( new QThread() )
{
    setConnections();
    startThread();
}

Core::~Core()
{
    if ( m_fileThread->isRunning() )
    {
        m_fileController->abort();
        m_fileThread->quit();
        m_fileThread->wait();
    }

    delete m_fileThread;
    delete m_mainWindow;
    delete m_fileController;
}

void Core::setConnections() const noexcept
{
    QObject::connect( m_mainWindow, &MainWindow::setFile, m_fileController,
                      &FileController::slotGetFilePath );

    QObject::connect( m_fileController, &FileController::getEmptyFile,
                      m_mainWindow, &MainWindow::getEmptyFile );

    QObject::connect( m_fileController, &FileController::getProgress,
                      m_mainWindow, &MainWindow::getProgress );

    QObject::connect( m_fileController, &FileController::getDict, m_mainWindow,
                      &MainWindow::getDict );
}

void Core::startThread() const noexcept
{
    m_fileController->moveToThread( m_fileThread );

    /// В момент запуска второстепенного потока вызвается функция calculate
    QObject::connect( m_fileThread, &QThread::started, m_fileController,
                      &FileController::calculate );

    /// При завершении работы функции calculate потоку передается сигнал
    /// finished, который вызывает срабатывание слота quit
    QObject::connect( m_fileController, &FileController::finished, m_fileThread,
                      &QThread::quit );

    m_fileThread->start();
}
