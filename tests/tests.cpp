#include "tests.h"

#include "bill.h"
#include "savefile.h"
#include "transaction.h"

#include <billviewmodel.h>
#include <contentmanager.h>

void Tests::toUpper()
{
    QVERIFY(true);
}

void Tests::testTransactionToJson()
{
    Transaction tr{"Pomathios", 10.0};

    const auto json = tr.to_json();

    Transaction tr_back = Transaction::from_json(json);

    QVERIFY(tr == tr_back);
}

void Tests::testBills()
{
    Bill b{};

    b.insert_transaction("Pomathios", 10.0f);
    b.insert_transaction("Emrah", 15.0f);
    b.insert_transaction("Fromages", 12.0f);

    QVERIFY(b.sum() == 37.0f);

    b.set_sum(40.0f);

    QVERIFY(b.sum() == 40.0f);
}

void Tests::testSaveFile()
{
    auto compare_vectors = [](QVector<Bill> const &vec1, QVector<Bill> const &vec2) {
        if (vec1.size() != vec2.size())
            return false;

        for (int i = 0; i < vec1.size(); ++i) {
            if (vec1[i] != vec2[i])
                return false;
        }

        return true;
    };

    Bill b{"revolut", QDate::currentDate()};

    b.insert_transaction("Pomathios", 10.0f);
    b.insert_transaction("Emrah", 15.0f);
    b.insert_transaction("Fromages", 12.0f);

    QVector<Bill> bills = {b};

    SaveFile file("saved_bills.json");

    file.save_bills(bills);
    auto bills_back = file.load_bills();

    QVERIFY(compare_vectors(bills, bills_back));

    b.set_sum(40.0f);

    file.save_bills(bills);
    bills_back = file.load_bills();

    QVERIFY(compare_vectors(bills, bills_back));
}

void Tests::testBillViewModel() {}

QTEST_MAIN(Tests)
#include "tests.moc"
