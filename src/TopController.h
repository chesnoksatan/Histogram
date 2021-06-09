#pragma once

#include <QMutex>
#include <QObject>
#include <QQueue>

#include <map>

class TopController : public QObject
{
    Q_OBJECT

    using Dictionary =
        std::map< QString, quint64 >; ///< Ключем является слово, значением -
                                      ///< количество его вхождений в тексте

    using DictionaryVector = std::vector< std::pair< QString, quint64 > >; ///< Структура для сортировки и хранения top15

public:
    explicit TopController( QObject *parent = nullptr );
    ~TopController() {};

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
    /// \brief getDict сигнал о том, что мы получили из файла top15
    /// \param dict top15
    ///
    void getDict( const DictionaryVector &dict );

public slots:
    ///
    /// \brief Основная функция класса
    ///        Извлекает из очереди запросов элемент и производит необходимые вычисления
    ///
    void calculate();

    void slotGetDictionary( const Dictionary &dictionary );

private:
    QMutex m_mutex;
    std::atomic< bool > m_abort; ///< Флаг прекращения потока
    QQueue< Dictionary > m_dictionaryRequests; ///< Очередь запросов

private:
    ///
    /// \brief getTop Трансформируем мапу в top15
    /// \param dictionary мапа со словами их количеством вхождений
    ///
    void getTop( const Dictionary &dictionary );
};
