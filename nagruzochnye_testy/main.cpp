#include <vector>
#include <QDebug>
#include <queue>
#include <boost/circular_buffer.hpp>
#include <ctime>

#include <iostream>

int glob = 0;

namespace Numerator
{
    template <typename T>
    struct Range
    {
        Range() {}

        Range(T start, T end) : start_{start}, end_{end} {}

        Range(const Range &rhs) : start_{rhs.start_}, end_{rhs.end_} {}

        T start() const { return start_; }
        T end() const { return end_; }

        void setStart(const T &s) { start_ = s; }
        void setEnd(const T &e) { end_ = e; }

        bool isValid() const
        {
            return start_ > 0 && start_ <= end_;
        }

        bool contains(T number) const
        {
            return number >= start_ && number <= end_;
        }

        bool contains(const Range &rhs) const
        {
            return contains(rhs.start_) && contains(rhs.end_);
        }

        bool intersects(const Range& rhs) const
        {
            return contains(rhs.start_) || contains(rhs.end_);
        }

        bool operator==(const Range &rhs) const
        {
            return (start_ == rhs.start_) && (end_ == rhs.end_);
        }

        bool operator!=(const Range &rhs) const
        {
            return !(*this == rhs);
        }

        bool operator<(const Range &rhs) const
        {
            if (start_ == rhs.start_)
            {
                return end_ < rhs.end_;
            }
            else
            {
                return start_ < rhs.start_;
            }
        }

        friend QDebug operator<<(QDebug debug, const Range<T> &r)
        {
            QDebugStateSaver saver(debug);
            debug.nospace() << '[' << r.start_ << " " << r.end_ << ']';

            return debug;
        }

    private:
        T start_ = 0;
        T end_ = 0;
    };

    template <typename T>
    class NumberPool
    {
        using BufferT = std::queue<T, boost::circular_buffer<T>>;
        using PoolT = std::vector<Range<T>>;
        using iterator = typename PoolT::iterator;
        using const_iterator = typename PoolT::const_iterator;

    public:
        NumberPool() : buffer{boost::circular_buffer<T>{BUFFER_LEN}} {}

        NumberPool(const Range<T> &range) : NumberPool()
        {
            pool.reserve(32);
            pool.emplace_back(range);

            for (auto i = range.start(); i < range.end(); i++)
            {
                buffer.push(i);
            }
        }

        auto isEmpty() const
        {
            return pool.empty();
        }

        auto take(T &number = nullptr)
        {
            auto pickRange = [&](T number, iterator it)
            {
                it->setStart(number + 1);
                if (it->start() > it->end())
                {
                    pool.erase(it);
                }
                return true;
            };

            while (!pool.empty())
            {
                number = pool[0].start();
                return pickRange(number, pool.begin());
            }

            return false;
        }

        auto exclude(const T &number)
        {
            return exclude(Range<T>{number, number});
        }

        auto exclude(const Range<T> &excluded)
        {
            auto oldPool {pool};
            pool.clear();

            for (const auto &range : oldPool)
            {

                if (range.intersects(excluded))
                {
                    if (excluded.contains(range))
                    {
                        continue;
                    }
                    else if (range.contains(excluded))
                    {
                        auto r1 = Range<T>{range.start(), excluded.start() - 1};
                        if (r1.isValid())
                        {
                            pool.emplace_back(r1);
                        }
                        auto r2 = Range<T>{excluded.end() + 1, range.end()};
                        if (r2.isValid())
                        {
                            pool.emplace_back(r2);
                        }
                    }
                    else
                    {
                        auto s = range.start() > excluded.start() ?
                                    excluded.end() + 1 :
                                    range.start();
                        auto e = range.end() > excluded.end() ?
                                    range.end() :
                                    excluded.start() - 1;
                        auto r = Range<T>{s, e};
                        pool.emplace_back(r);
                    }
                }
                else
                {
                    pool.emplace_back(range);
                }
            }


            if (!pool.empty())
            {
                std::sort(pool.begin(), pool.end());
            }
        }

        auto add(const T &number)
        {
            return add(Range<T>{number, number});
        }

        auto add(const Range<T> &range)
        {
            auto found = std::lower_bound(begin(), end(), range);
            pool.insert(found, range);
            normalize();

            for (auto i = range.start(); i < range.end(); i++)
            {
                buffer.push(i);
            }
            return false;
        }
        const_iterator begin() const { return pool.cbegin(); }
        const_iterator end() const { return pool.cend(); }

    private:
        auto normalize()
        {
            size_t curRange = 0;
            for (size_t i = 0; i < pool.size() - 1; ++i)
            {
                auto &range = pool.at(curRange);
                const auto &nextRange = pool.at(i + 1);
                if (nextRange.start() <= range.end() + 1)
                {
                    range.setEnd(nextRange.end());
                }
                else
                {
                    curRange++;
                    pool[curRange] = nextRange;
                }
            }
            auto poolSize = pool.size();
            while (curRange < poolSize - 1)
            {
                pool.pop_back();
                curRange++;
            }
        }

    private:
        static constexpr auto BUFFER_LEN = 30;

        PoolT pool;
        BufferT buffer;
    };

    template <typename T>
    QDebug operator<<(QDebug debug, const NumberPool<T> &numerator)
    {
        QDebugStateSaver saver(debug);

        for (const auto &r : numerator)
        {
            debug << r;
        }

        return debug;
    }

}



using namespace Numerator;



void load_test(int min, int max)
{
    NumberPool<qint16> np;
    std::vector<qint16> v;
    qint16 tmp;
    np.add(Range<qint16>(min, max));
    int i = 0;
    int start = clock();

    while(i <= 72000)
    {
        if(clock() - start >= 200)
        {
            qDebug() << "======================\n";
            int r = rand();
            if(r % 2 == 0 && !np.isEmpty())
            {
                np.take(tmp);
                v.push_back(tmp);
                std::sort(v.begin(), v.end());
                qDebug() << np;
                qDebug() << v;
            }
            else
            {
                if(v.empty())
                {
                    np.take(tmp);
                    v.push_back(tmp);
                    std::sort(v.begin(), v.end());
                    qDebug() << np;
                    qDebug() << v;
                }
                else
                {
                    r = rand() %  v.size();
                    np.add(v[r]);
                    v.erase(v.begin() + r);
                    std::sort(v.begin(), v.end());
                    qDebug() << np;
                    qDebug() << v;
                }
            }

            i++;
            start = clock();
        }
    }
}

int main()
{
    srand(clock());
    load_test(1, 9999);
    return 0;
}
