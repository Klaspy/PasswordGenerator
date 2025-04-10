#include "dbworker.h"

DbWorker::DbWorker(QObject *parent)
    : QObject{parent}
{
    db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    db.setDatabaseName("database.sqlite");

    db.open();

    init();
}

QPair<bool, std::optional<Worker>> DbWorker::addLine(Worker newWorker)
{
    if (db.isOpen())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO Workers (SecondName, Name, Surname, Cabinet) "
                        "VALUES (:secondName, :name, :surname, :cabinet) "
                        "RETURNING *");

        query.bindValue(":secondName", newWorker.secondName);
        query.bindValue(":name",       newWorker.name);
        query.bindValue(":surname",    newWorker.surname);
        query.bindValue(":cabinet",    newWorker.cabinet);

        if (query.exec())
        {
            if (query.next())
            {
                Worker worker(query.value("Id").toUInt(),
                              query.value("SecondName").toString(),
                              query.value("Name"      ).toString(),
                              query.value("Surname"   ).toString(),
                              query.value("Cabinet"   ).toUInt(),
                              "", QDate());

                return {true, worker};
            }
        }
        else
        {
            qWarning() << "Ошибка при добавлении нового работника" << query.lastError();
        }
    }

    return {false, std::nullopt};
}

QList<Worker> DbWorker::getWorkersList()
{
    QList<Worker> workers;
    if (db.isOpen())
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM Workers");

        if (query.exec())
        {
            while (query.next())
            {
                Worker worker(query.value("Id").toUInt());
                worker.secondName      = query.value("SecondName"     ).toString();
                worker.name            = query.value("Name"           ).toString();
                worker.surname         = query.value("Surname"        ).toString();
                worker.cabinet         = query.value("Cabinet"        ).toUInt();
                worker.password        = query.value("Password"       ).toString();
                worker.passwordGenDate = QDate::fromJulianDay(query.value("PasswordGenDate").toInt());

                workers.append(worker);
            }
        }
        else
        {
            qWarning() << "Ошибка при запросе списка работников" << query.lastError();
        }
    }

    return workers;
}

void DbWorker::init()
{
    if (db.isOpen())
    {
        QSqlQuery query("CREATE TABLE IF NOT EXISTS \"Workers\" ("
                        "\"Id\"	INTEGER NOT NULL UNIQUE, "
                        "\"SecondName\"         TEXT NOT NULL, "
                        "\"Name\"               TEXT NOT NULL, "
                        "\"Surname\"            TEXT, "
                        "\"Cabinet\"            INTEGER NOT NULL, "
                        "\"Password\"           TEXT, "
                        "\"PasswordGenDate\"	INTEGER, "
                        "PRIMARY KEY(\"Id\" AUTOINCREMENT)"
                        ")");
        if (!query.exec())
        {
            qWarning() << "Ошибка при попытке создания таблицы работников" << query.lastError();
        }
    }
}
