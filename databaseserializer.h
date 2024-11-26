#ifndef DATABASESERIALIZER_H
#define DATABASESERIALIZER_H

#include <QMap>
#include <QString>
#include <QDataStream>
#include "table.h"

class DatabaseSerializer {
public:
    void saveToFile(const QMap<QString, Table>& tables, const QString& filename);
    QMap<QString, Table> loadFromFile(const QString& filename);
};

#endif // DATABASESERIALIZER_H
