#include "dbworker.h"

DbWorker::DbWorker(QObject *parent)
    : QObject{parent}
{
    db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    db.setDatabaseName("database.sqlite");

    db.open();

    init();
}

void DbWorker::init()
{
    if (db.isOpen())
    {
        QSqlQuery query("CREATE TABLE IF NOT EXISTS \"Workers\" ("
                        "\"Id\"	INTEGER NOT NULL UNIQUE, "
                        "\"SecondName\"	TEXT NOT NULL, "
                        "\"Name\"	TEXT NOT NULL, "
                        "\"Surname\"	TEXT, "
                        "\"Cabinet\"	INTEGER NOT NULL, "
                        "PRIMARY KEY(\"Id\" AUTOINCREMENT)"
                        ")");
        if (!query.exec())
        {
            qDebug() << "Ошибка при попытке создания таблицы работников" << query.lastError();
        }
    }
}
