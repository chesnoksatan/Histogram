#pragma once

#include <QMutex>
#include <QObject>
#include <QQueue>

class FileController : public QObject
{
    Q_OBJECT
public:
    explicit FileController( QObject *parent = nullptr );
    ~FileController();

public:
    /*!
     *   \brief Прекращение работы потока
     */
    void abort() { m_abort = true; }

public slots:
    ///
    /// \brief slotGetFilePath Добавление в очередь путей к файлам
    /// \param filePath путь к выбранному пользователем файла
    ///
    void slotGetFilePath( const QUrl &filePath );

signals:
    /*!
     *   \brief Сигнал прекращение завершения потока
     */
    void finished();

public slots:
    /*!
     * \brief Основная функция класса
     *        Извлекает из очереди запросов элемент и производит необходимые вычисления
     */
    void calculate();

private:
    QMutex m_mutexRequests; ///< Мьютекс для очереди запросов
    std::atomic< bool > m_abort; ///< Флаг прекращения потока
    QQueue< QString > m_fileRequests; ///< Очередь запросов
};
