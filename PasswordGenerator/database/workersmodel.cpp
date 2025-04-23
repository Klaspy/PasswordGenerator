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
        default: return "";
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

void WorkersModel::resetPassword(int index)
{
    if (index > -1 && index < workers.count())
    {
        Worker toUpdate = workers.at(index);
        toUpdate.password = generatePassword(toUpdate);
        toUpdate.passwordGenDate = QDate::currentDate();

        QPair<bool, std::optional<Worker>> updated = dbWorker->modifyLine(toUpdate);
        if (updated.first)
        {
            workers[index].password        = updated.second.value().password;
            workers[index].passwordGenDate = updated.second.value().passwordGenDate;

            emit dataChanged(this->index(index, 2), this->index(index, 2), {Qt::DisplayRole});
        }
    }
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

void WorkersModel::resetAllPasswords()
{
    for (int i = 0; i < workers.size(); i++)
    {
        resetPassword(i);
    }

    emit dataChanged(index(0, 2), index(workers.size() - 1, 2), {Qt::DisplayRole});
}

void WorkersModel::copyStringToClipboard(QString string) const
{
    QGuiApplication::clipboard()->setText(string);
}

void WorkersModel::exportWorkers(QUrl filePath)
{
    exporter->exportWorkers(workers, filePath.toString(QUrl::PreferLocalFile));
}

QString WorkersModel::generatePassword(const Worker &worker)
{
    QString password;
    password.append(QString::number(worker.cabinet).rightJustified(3, '0'));

    QRandomGenerator rng(QTime::currentTime().msecsSinceStartOfDay());
    int letterIndex = rng.bounded(32);
    password.append(QChar(0x410 + letterIndex));

    for (int i = 0; i < 8; i++)
    {
        int letterIndex = rng.bounded(32);
        password.append(QChar(0x430 + letterIndex));
    }

    return password;
}

ProxyWorkersModel::ProxyWorkersModel(WorkersModel *sourceModel)
    : QSortFilterProxyModel {sourceModel}
{
    setSourceModel(sourceModel);
    sort(0, Qt::AscendingOrder);

    invalidate();
}

void ProxyWorkersModel::resetPassword(int index)
{
    QModelIndex idx = this->index(index, 0);
    if (idx.isValid())
    {
        qobject_cast<WorkersModel*>(sourceModel())->resetPassword(mapToSource(idx).row());
    }
}

bool ProxyWorkersModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (!source_left.isValid() || !source_right.isValid()) return false;
    auto source = qobject_cast<WorkersModel*>(sourceModel());
    switch (sortColumn())
    {
    case 0:
    {
        int res = lessThanZeroCol(source_left, source_right);
        if (res != 0)
        {
            return res < 0;
        }
        else
        {
            return source->data(source->index(source_left.row(), 1)).toInt() < source->data(source->index(source_right.row(), 1)).toInt();
        }
    }
    case 1:
    {
        if (source->data(source_left).toInt() == source->data(source_right).toInt())
        {
            return lessThanZeroCol(source_left, source_right) < 0;
        }
        else
        {
            return source->data(source_left).toInt() < source->data(source_right).toInt();
        }
    }
    default: return false;
    }
}

int ProxyWorkersModel::lessThanZeroCol(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    auto source = qobject_cast<WorkersModel*>(sourceModel());
    QStringList fullName_left;
    fullName_left.append(source->data(source_left, WorkersModel::SecondNameRole).toString());
    fullName_left.append(source->data(source_left, WorkersModel::NameRole).toString());
    fullName_left.append(source->data(source_left, WorkersModel::SurnameRole).toString());

    QStringList fullName_right;
    fullName_right.append(source->data(source_right, WorkersModel::SecondNameRole).toString());
    fullName_right.append(source->data(source_right, WorkersModel::NameRole).toString());
    fullName_right.append(source->data(source_right, WorkersModel::SurnameRole).toString());
    qDebug() << fullName_left << fullName_right;

    if (fullName_left.at(0) == fullName_right.at(0))
    {
        if (fullName_left.at(1) == fullName_right.at(1))
        {
            if (fullName_left.at(2) == fullName_right.at(2))
            {
                return 0;
            }
            else
            {
                return fullName_left.at(2) < fullName_right.at(2) ? -1 : 1;
            }
        }
        else
        {
            return fullName_left.at(1) < fullName_right.at(1) ? -1 : 1;
        }
    }
    else
    {
        return fullName_left.at(0) < fullName_right.at(0) ? -1 : 1;
    }
}

void ProxyWorkersModel::sort(int column, Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(column, order);

    emit sortChanged();
}
