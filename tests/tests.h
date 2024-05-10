#ifndef TESTS_H
#define TESTS_H

#include <QtTest/QTest>

class Tests : public QObject
{
    Q_OBJECT
private slots:
    void toUpper();

    void testTransactionToJson();

    void testBills();

    void testSaveFile();

    void testBillViewModel();
};

#endif // TESTS_H
