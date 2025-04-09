#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

class DbWorker : public QObject
{
    Q_OBJECT

public:
    explicit DbWorker(QObject *parent = nullptr);

private:
    QSqlDatabase db;
    QSqlTableModel *workers;

    void init();
};

#endif // DBWORKER_H
