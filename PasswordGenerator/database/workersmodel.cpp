#include "workersmodel.h"

WorkersModel::WorkersModel(QObject *parent)
    : QAbstractTableModel(parent)
{
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

    int row = index.row();
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
    else if (role == AllEditableRoles)
    {
        return QVariantList({workers.at(row).secondName, workers.at(row).name, workers.at(row).surname, workers.at(row).cabinet});
    }

    return QVariant();
}

bool WorkersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ((Roles)role != AllEditableRoles)
    {
        return false;
    }

    if (data(index, role) != value) {

        switch (role)
        {
        case AllEditableRoles:
        {
            QVariantList listValue = value.toList();
            if (listValue.size() != 4) return false;

            Worker toEdit(workers.at(index.row()));
            toEdit.secondName = listValue.at(0).toString();
            toEdit.name       = listValue.at(1).toString();
            toEdit.surname    = listValue.at(2).toString();
            toEdit.cabinet    = listValue.at(3).toUInt();

            QPair<bool, std::optional<Worker>> updated = dbWorker->modifyLine(toEdit);
            if (updated.first)
            {
                workers[index.row()].secondName = updated.second.value().secondName;
                workers[index.row()].name       = updated.second.value().name;
                workers[index.row()].surname    = updated.second.value().surname;
                workers[index.row()].cabinet    = updated.second.value().cabinet;
                emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 3), {Qt::DisplayRole});
            }
            else
            {
                return false;
            }

            break;
        }
        default: break;
        }

        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

QHash<int, QByteArray> WorkersModel::roleNames() const
{
    QHash<int, QByteArray> result = QAbstractTableModel::roleNames();

    result.insert(AllEditableRoles, "allEditableRoles_");
    // result.insert(SecondNameRole,   "secondName_");
    // result.insert(NameRole,         "name_");
    // result.insert(SurnameRole,      "surname_");
    // result.insert(PasswordRole,     "password_");
    // result.insert(PasswordGenDate,  "passwordDate_);

    return result;
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
    qDebug() << row;
    return false;
    if (count + row > workers.size()) count = workers.size() - row;
    if (count < 1) return false;

    bool success {true};
    if (dbWorker->beginTransaction())
    {
        for (int i = 0; i < count; i++)
        {
            if (!dbWorker->deleteLine(workers.at(row + i).id))
            {
                success = false;
            }
        }

        if (!success)
        {
            dbWorker->rollbackTransaction();
        }
    }
    else
    {
        success = false;
    }

    if (success)
    {
        dbWorker->commitTransaction();
        beginRemoveRows(parent, row, row + count - 1);

        workers.remove(row, count);

        endRemoveRows();

        return true;
    }


    return false;
}

int WorkersModel::roleKey(QByteArray roleName) const
{
    return roleNames().key(roleName, -1);
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
    return true;
    return sourceRow > 0;
}
