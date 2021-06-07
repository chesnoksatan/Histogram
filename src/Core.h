#pragma once

#include "MainWindow/MainWindow.h"

#include <QObject>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core( QObject *parent = nullptr );
    ~Core();

private:
    MainWindow *m_mainWindow;
};
