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
    switch (role)
    {
    case Qt::DisplayRole:
    {
        switch (col)
        {
        case 0: return workers.at(row).secondName + " " + workers.at(row).name + " " + workers.at(row).surname;
        case 1: return workers.at(row).cabinet;
        case 2: return workers.at(row).password;
        }
    }
    case AllEditableRoles:
    {
        return QVariantList({workers.at(row).secondName, workers.at(row).name, workers.at(row).surname, workers.at(row).cabinet});
    }
    case SecondNameRole: return workers.at(row).secondName;
    case NameRole:       return workers.at(row).name;
    case SurnameRole:    return workers.at(row).surname;
    default: return QVariant();
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
    result.insert(SecondNameRole,   "secondName_");
    result.insert(NameRole,         "name_");
    result.insert(SurnameRole,      "surname_");
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

void WorkersModel::addNewWorker(QVariantList workerData)
{
    if (workerData.size() != 4) return;
    auto dbResult = dbWorker->addLine(Worker(workerData.at(0).toString(),
                                             workerData.at(1).toString(),
                                             workerData.at(2).toString(),
                                             workerData.at(3).toInt()));
    if (dbResult.first)
    {
        beginInsertRows({}, workers.size(), workers.size());

        workers.append(dbResult.second.value());

        endInsertRows();
    }
}

ProxyWorkersModel::ProxyWorkersModel(WorkersModel *sourceModel)
    : QSortFilterProxyModel {sourceModel}
{
    setSourceModel(sourceModel);
    sort(0, Qt::AscendingOrder);

    invalidate();
}

bool ProxyWorkersModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    auto source = qobject_cast<WorkersModel*>(sourceModel());
    // qDebug() << sortOrder();
    switch (sortColumn())
    {
    case 0:
    {
            QStringList fullName_left;
            fullName_left.append(source->data(source_left, WorkersModel::SecondNameRole).toString());
            fullName_left.append(source->data(source_left, WorkersModel::SecondNameRole).toString());
            fullName_left.append(source->data(source_left, WorkersModel::SecondNameRole).toString());

            QStringList fullName_right;
            fullName_right.append(source->data(source_right, WorkersModel::SecondNameRole).toString());
            fullName_right.append(source->data(source_right, WorkersModel::SecondNameRole).toString());
            fullName_right.append(source->data(source_right, WorkersModel::SecondNameRole).toString());

            if (fullName_left.at(0) == fullName_right.at(0))
            {
                if (fullName_left.at(1) == fullName_right.at(1))
                {
                    return fullName_left.at(2) < fullName_right.at(2);
                }
                else
                {
                    return fullName_left.at(1) < fullName_right.at(1);
                }
            }
            else
            {
                return fullName_left.at(0) < fullName_right.at(0);
            }
    }
    case 1:
    {
            return source->data(source_left).toInt() < source->data(source_right).toInt();
    }
    default: return false;
    }
}

void ProxyWorkersModel::sort(int column, Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(column, order);

    emit sortChanged();
}
