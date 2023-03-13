#ifndef TEST_NUMBERPOOL_H
#define TEST_NUMBERPOOL_H
#include <QObject>

class test_NumberPool : public QObject
{
    Q_OBJECT
public:
    explicit test_NumberPool(QObject *parent = 0);
private slots:
    void test_exclude();
    void test_take();
};

#endif // TEST_NUMBERPOOL_H
