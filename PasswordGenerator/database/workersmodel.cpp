#include "workersmodel.h"

WorkersModel::WorkersModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant WorkersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
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

    return workers.size();
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

    // FIXME: Implement me!
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
