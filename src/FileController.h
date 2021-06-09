#pragma once

#include <QMutex>
#include <QObject>
#include <QQueue>

#include <map>

class FileController : public QObject
{
    Q_OBJECT

    using Dictionary =
        std::map< QString, quint64 >; ///< Ключем является слово, значением -
                                      ///< количество его вхождений в тексте

    using DictionaryVector = std::vector< std::pair< QString, quint64 > >; ///< Структура для сортировки и хранения top15

public:
    explicit FileController( QObject *parent = nullptr );
    ~FileController();

public:
    ///
    /// \brief Прекращение работы потока
    ///
    void abort();

signals:
    ///
    /// \brief Сигнал прекращение завершения потока
    ///
    void finished();

    ///
    /// \brief getProgress сигнал об изменении прогресса чтения файла
    /// \param progress
    ///
    void getProgress( const qint64 progress );

    ///
    /// \brief getDict сигнал о том, что мы получили из файла top15
    /// \param dict top15
    ///
    void getDict( const DictionaryVector &dict );

    void getEmptyFile();

public slots:
    ///
    /// \brief Основная функция класса
    ///        Извлекает из очереди запросов элемент и производит необходимые вычисления
    ///
    void calculate();

    ///
    /// \brief slotGetFilePath Добавление в очередь путей к файлам
    /// \param filePath путь к выбранному пользователем файла
    ///
    void slotGetFilePath( const QUrl &filePath );

private:
    QMutex m_mutex;
    std::atomic< bool > m_abort; ///< Флаг прекращения потока
    QQueue< QUrl > m_fileRequests; ///< Очередь запросов

private:
    ///
    /// \brief readFile Функция чтения файла из очереди
    /// \param filePath
    /// \return Возвращает словарь уникальных слов, встреченных в файле
    ///
    Dictionary readFile( const QUrl &filePath );

    ///
    /// \brief getTop Трансформируем мапу в top15
    /// \param dictionary мапа со словами их количеством вхождений
    ///
    DictionaryVector getTop( const Dictionary &dictionary );

    ///
    /// \brief filterString Отфильтруем строку файла в необходимый формат
    /// \param str
    ///
    QString filterString( const QString &str );
};
