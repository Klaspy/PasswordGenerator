#include "workersmodel.h"

WorkersModel::WorkersModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    dbWorker->addLine(Worker("Тихонов", "Дмитрий", "", 101));
    workers = dbWorker->getWorkersList();
}

QVariant WorkersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role)
    if (orientation == Qt::Vertical) return QVariant();

    switch (section) {
    case 0: return QStringLiteral("ФИО сотрудника");
    case 1: return QStringLiteral("Кабинет");
    case 2: return QStringLiteral("Пароль");
    case 3: return QStringLiteral("Дейтсвия");
    }

    return QVariant();
}

int WorkersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return workers.size() + 1;
}

int WorkersModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4;
}

QVariant WorkersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row() - 1;
    if (row < 0) return QVariant();

    int col = index.column();
    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case 0: return workers.at(row).secondName + " " + workers.at(row).name + " " + workers.at(row).surname;
        case 1: return workers.at(row).cabinet;
        case 2: return workers.at(row).password;
        }
    }

    return QVariant();
}

bool WorkersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

bool WorkersModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool WorkersModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

ProxyWorkersModel::ProxyWorkersModel(WorkersModel *sourceModel)
    : QSortFilterProxyModel {sourceModel}
{
    setSourceModel(sourceModel);

    invalidate();
}

bool ProxyWorkersModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)
    return sourceRow > 0;
}
