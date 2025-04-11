#ifndef WORKERSMODEL_H
#define WORKERSMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

#include "dbworker.h"

class WorkersModel : public QAbstractTableModel
{
    Q_OBJECT

    enum Roles {
        AllEditableRoles = Qt::UserRole + 1, //0x101
        // SecondNameRole,
        // NameRole,
        // SurnameRole,
        // PasswordRole,
        // PasswordGenDate,
    };

public:
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

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

public slots:
    int roleKey(QByteArray roleName) const;

private:
    DbWorker *dbWorker {new DbWorker(this)};

    QList<Worker> workers;
};

class ProxyWorkersModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ProxyWorkersModel(WorkersModel *sourceModel);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // WORKERSMODEL_H
