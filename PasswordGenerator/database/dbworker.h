#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDate>
#include <QStandardPaths>

struct Worker {
    quint32 id;
    QString secondName;
    QString name;
    QString surname;
    quint32 cabinet;
    QString password;
    QDate passwordGenDate;

    explicit Worker(const quint32 &id) : id {id}
    {}

    Worker(const QString &secondName, const QString &name, const QString &surname, const quint32 &cabinet)
        : id {0}, secondName {secondName}, name {name}, surname {surname}, cabinet {cabinet}
    {}

    Worker(const quint32 &id, const QString &secondName, const QString &name, const QString &surname,
           const quint32 &cabinet, const QString &password, const QDate &passwordDate)
        : id {id}, secondName {secondName}, name {name}, surname {surname}, cabinet {cabinet}
        , password {password}, passwordGenDate {passwordDate}
    {}

    Worker(const Worker &other)
        : id {other.id}, secondName {other.secondName}, name {other.name}, surname {other.surname},
        cabinet {other.cabinet}, password {other.password}, passwordGenDate {other.passwordGenDate}
    {}

    Worker operator = (const Worker &other)
    {
        id = other.id;
        secondName = other.secondName;
        name = other.name;
        surname = other.surname;
        cabinet = other.cabinet;
        password = other.password;
        passwordGenDate = other.passwordGenDate;

        return *this;
    }
};

class DbWorker : public QObject
{
    Q_OBJECT

public:
    explicit DbWorker(QObject *parent = nullptr);

    QPair<bool, std::optional<Worker>> addLine(Worker newWorker);
    QPair<bool, std::optional<Worker>> modifyLine(Worker worker);
    bool deleteLine(int id);

    bool beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    QList<Worker> getWorkersList();

private:
    QSqlDatabase db;

    void init();
};

#endif // DBWORKER_H
