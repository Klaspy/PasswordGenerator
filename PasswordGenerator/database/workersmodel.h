#ifndef WORKERSMODEL_H
#define WORKERSMODEL_H

#include <QGuiApplication>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QRandomGenerator>
#include <QTime>
#include <QClipboard>

#include "dbworker.h"
#include "../xlsxworker.h"

class WorkersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles {
        AllEditableRoles = Qt::UserRole + 1, //0x101
        SecondNameRole,
        NameRole,
        SurnameRole,
        // PasswordRole,
        // PasswordGenDate,
    };

    explicit WorkersModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void resetPassword(int index);

public slots:
    int roleKey(QByteArray roleName) const;
    void addNewWorker(QVariantList workerData);

    void resetAllPasswords();

    void copyStringToClipboard(QString string) const;

    void exportWorkers(QUrl filePath);

private:
    DbWorker *dbWorker {new DbWorker(this)};
    XlsxWorker *exporter {new XlsxWorker(this)};

    QList<Worker> workers;

    QString generatePassword(const Worker &worker);
};

class ProxyWorkersModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int sortCol  READ sortCol  NOTIFY sortChanged FINAL)
    Q_PROPERTY(int sortOrdr READ sortOrdr NOTIFY sortChanged FINAL)

public:
    explicit ProxyWorkersModel(WorkersModel *sourceModel);

    int sortCol() const {return sortColumn();}
    int sortOrdr() const {return (int)sortOrder();}

public slots:
    void resetPassword(int index);

signals:
    void sortChanged();

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
    int lessThanZeroCol(const QModelIndex &source_left, const QModelIndex &source_right) const;

    void sort(int column, Qt::SortOrder order) override;
};

#endif // WORKERSMODEL_H
