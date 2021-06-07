#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow( QObject *parent = nullptr );
    ~MainWindow() override;

private:
    QQmlApplicationEngine *m_engine;
    QObject *m_mainWindow;
};
