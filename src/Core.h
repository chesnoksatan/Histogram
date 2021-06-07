#pragma once

#include "FileController.h"
#include "MainWindow.h"

#include <QObject>
#include <QThread>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core( QObject *parent = nullptr );
    ~Core();

private:
    MainWindow *m_mainWindow;
    FileController *m_fileController;
    QThread *m_fileThread;

private:
    void setConnections() const noexcept;
    void startThread() const noexcept;
};
