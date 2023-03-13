#include <QTest>
#include "test_numberpool.h"
QTEST_MAIN(test_NumberPool)
#include "C:\Qt_projects\Let_Pruct\Numerator_1.cpp"

using namespace Numerator;

test_NumberPool::test_NumberPool(QObject* parent) : QObject(parent) {}

void test_NumberPool::test_exclude()
{
        qint16 temp, temp1;
        NumberPool<qint16>* np;
        std::vector<qint16> v;

        np = new NumberPool<qint16>;
        np->add(Range<qint16>(1,5));
        np->exclude(1);
        np->take(temp);
        QCOMPARE(temp, 2);
        delete np;

        np = new NumberPool<qint16>;
        np->add(1);
        np->exclude(1);
        QCOMPARE( np->take(temp), false);
        delete np;

        np = new NumberPool<qint16>;
        np->add(Range<qint16>(1,5));
        np->add(Range<qint16>(10,15));
        np->add(Range<qint16>(20,25));
        np->exclude(13);
        np->take(temp);
        QCOMPARE(temp, 1);
        delete np;

        np = new NumberPool<qint16>;
        np->add(Range<qint16>(1,5));
        np->add(Range<qint16>(10,15));
        np->add(Range<qint16>(20,25));
        np->exclude(Range<qint16>(12,13));
        np->take(temp);
        QCOMPARE(temp, 1);
        delete np;
}

void test_NumberPool::test_take()
{
    qint16 temp;
    NumberPool<qint16>* np;
    std::vector<qint16> v;

    np = new NumberPool<qint16>;//умирает когда заканчивается последовательность
    np->add(1);
    np->take(temp);
    QCOMPARE(temp, 1);
    delete np;

    np = new NumberPool<qint16>;
    np->add(Range<qint16>(1,10));
    np->take(temp);
    np->take(temp);
    np->take(temp);
    np->take(temp);
    np->take(temp);
    QCOMPARE(temp, 5);
    np->take(temp);
    np->take(temp);
    np->take(temp);
    np->take(temp);
    np->take(temp);
    QCOMPARE(temp, 10);
    delete np;

    np = new NumberPool<qint16>;
    np->add(Range<qint16>(1,30));
    np->take(temp);
    v.push_back(temp);
    np->take(temp);
    v.push_back(temp);
    np->take(temp);
    v.push_back(temp);
    np->add(v[0]);
    v.erase(v.begin());
    np->add(v[0]);
    v.erase(v.begin());
    np->take(temp);
    v.push_back(temp);
    np->take(temp);
    v.push_back(temp);
    temp = 0;
    for(int i = 0; i < v.size(); i++)
    {
        temp += v[i];
    }
    QCOMPARE(temp, 3 + 1 + 2);
    delete np;

    np = new NumberPool<qint16>;
    np->add(Range<qint16>(1,100));
    np->take(temp);
    v.push_back(temp);
    np->take(temp);
    v.push_back(temp);
    np->take(temp);
    v.push_back(temp);
    np->add(v[0]);
    v.erase(v.begin());
    np->add(v[0]);
    v.erase(v.begin());
    np->take(temp);
    v.push_back(temp);
    np->take(temp);
    v.push_back(temp);
    temp = 0;
    for(int i = 0; i < v.size(); i++)
    {
        temp += v[i];
    }
    QCOMPARE(temp, 3 + 4 + 5);
    delete np;

}


