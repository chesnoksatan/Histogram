#pragma once

#include "FileController.h"
#include "MainWindow.h"
#include "TopController.h"

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
    TopController *m_topController;
    QThread *m_fileThread;
    QThread *m_topThread;

private:
    void setConnections() const noexcept;
    void startThread() const noexcept;
};
