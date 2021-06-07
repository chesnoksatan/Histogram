#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>

/**
 * @brief MainWindow класс для работы с qml частью приложения
 */
class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow( QObject *parent = nullptr );
    ~MainWindow() override;

signals:
    void setFile( const QUrl &filePath );

private:
    QQmlApplicationEngine *m_engine;
    QObject *m_mainWindow;
};
