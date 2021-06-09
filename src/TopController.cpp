#include "TopController.h"

#include <QCoreApplication>
#include <QThread>

namespace {
struct
{
    bool operator()( std::pair< QString, quint64 > a,
                     std::pair< QString, quint64 > b ) const
    {
        return a.second > b.second;
    }
} maxCountSort; ///< Для сортировки вектора по количеству вхождений

constexpr auto TOP = 15; ///< Количество слов, которые должны входить в топ
} // namespace

TopController::TopController( QObject *parent ) : QObject( parent )
{
    qRegisterMetaType< DictionaryVector >( "DictionaryVector" );
}

void TopController::abort()
{
    QMutexLocker locker( &m_mutex );
    m_abort = true;
}

void TopController::slotGetDictionary( const TopController::Dictionary &dictionary )
{
    QMutexLocker locker( &m_mutex );
    m_dictionaryRequests.enqueue( dictionary );
}

void TopController::getTop( const TopController::Dictionary &dictionary )
{
    DictionaryVector container;

    for ( auto &pair : dictionary )
        container.push_back( pair );

    /// Отсортируем элементы вектора по убыванию количества вхождений
    std::sort( container.begin(), container.end(), maxCountSort );
    /// Оставим в массиве топ 15 слов
    if ( container.size() > TOP )
        container.erase( container.begin() + TOP, container.end() );
    /// Отсортируем в алфавитном порядке
    std::sort( container.begin(), container.end() );

    emit getDict( container );
}

void TopController::calculate()
{
    m_abort = false;
    forever
    {
        QCoreApplication::instance()->processEvents();

        if ( m_abort )
            break;

        m_mutex.lock();

        if ( !m_dictionaryRequests.isEmpty() )
            getTop( m_dictionaryRequests.dequeue() );

        m_mutex.unlock();

        QThread::usleep( 500 );
    }

    emit finished();
}
