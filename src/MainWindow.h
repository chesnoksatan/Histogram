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

public slots:
    ///
    /// \brief getDict слот для обработки входящего топ15
    /// \param dict
    ///
    void getDict( const std::vector< std::pair< QString, quint64 > > &dict );

signals:
    ///
    /// \brief setFile сигнал обозначающий, что пользователь выбрал файл
    /// \param filePath Путь к выбранному файлу
    ///
    void setFile( const QUrl &filePath );
    ///
    /// \brief getProgress сигнал об изменении прогресса чтения файла
    /// \param progress
    ///
    void getProgress( const qint64 progress );

    ///
    /// \brief getValue отправляем на гуй топ15
    /// \param value
    ///
    void getValue( const QJsonObject &value );

    void getEmptyFile();

private:
    QQmlApplicationEngine *m_engine;
    QObject *m_mainWindow;
};
