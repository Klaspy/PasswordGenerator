#include "xlsxworker.h"
#include "database/dbworker.h"

bool sortByFullName(const Worker &w1, const Worker &w2)
{
    if (w1.secondName == w2.secondName)
    {
        if (w1.name == w2.name)
        {
            return w1.surname < w2.surname;
        }
        else
        {
            return w1.name < w2.name;
        }
    }
    else
    {
        return w1.secondName < w2.secondName;
    }
}

XlsxWorker::XlsxWorker(QObject *parent)
    : QObject{parent}
{}

bool XlsxWorker::exportWorkers(QList<Worker> workers, QString fileName)
{
    if (workers.isEmpty()) return false;
    QXlsx::Document doc;
    // doc.setColumnWidth(2, 2, 100);
    doc.setColumnWidth(3, 3, 18);

    std::sort(workers.begin(), workers.end(), &sortByFullName);

    QXlsx::Format format;
    format.setBorderStyle(QXlsx::Format::BorderThin);

    QXlsx::Format headerFormat;
    headerFormat.setPatternBackgroundColor(QColor(0x9f, 0xc4, 0xff));
    headerFormat.setBorderStyle(QXlsx::Format::BorderMedium);

    int row {1};
    doc.write(row, 1, QStringLiteral("ФИО"),     headerFormat);
    doc.write(row, 2, QStringLiteral("Кабинет"), headerFormat);
    doc.write(row, 3, QStringLiteral("Пароль"),  headerFormat);
    row++;

    int fullNameColWidth {3};
    foreach (const Worker &worker, workers) {
        QString fullName = worker.secondName + " " + worker.name + " " + worker.surname;
        fullNameColWidth = qMax(fullNameColWidth, fullName.size());
        doc.write(row, 1, fullName,                        format);
        doc.write(row, 2, QString::number(worker.cabinet), format);
        doc.write(row, 3, worker.password,                 format);
        row++;
    }


    doc.setColumnWidth(1, 1, fullNameColWidth * 1.2);

    return doc.saveAs(fileName);
}
