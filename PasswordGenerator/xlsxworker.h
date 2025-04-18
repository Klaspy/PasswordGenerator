#ifndef XLSXWORKER_H
#define XLSXWORKER_H

#include <QObject>
#include "QXlsx/header/xlsxdocument.h"
#include "QXlsx/header/xlsxchartsheet.h"
#include "QXlsx/header/xlsxcellrange.h"
#include "QXlsx/header/xlsxchart.h"
#include "QXlsx/header/xlsxrichstring.h"
#include "QXlsx/header/xlsxworkbook.h"

struct Worker;

class XlsxWorker : public QObject
{
    Q_OBJECT
public:
    explicit XlsxWorker(QObject *parent = nullptr);

    bool exportWorkers(QList<Worker> workers, QString fileName);

signals:
};

#endif // XLSXWORKER_H
