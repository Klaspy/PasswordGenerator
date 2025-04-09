#ifndef WORKERSMODEL_H
#define WORKERSMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

class WorkersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit WorkersModel(QObject *parent = nullptr);

    struct Worker {
        const quint32 id;
        QString secondName;
        QString name;
        QString surname;
        quint32 cabinet;

        Worker(const int &id) : id {id}
        {}
    };

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QList<Worker> workers;
};

#endif // WORKERSMODEL_H
