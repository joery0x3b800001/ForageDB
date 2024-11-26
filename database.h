#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QMap>
#include <QVector>
#include "table.h"

class Database {
public:
    Database();
    void createTable(const QString& sql);
    void insertRecord(const QString& sql);
    void selectData(const QString& sql);
    void saveDatabase(const QString& filename);
    void loadDatabase(const QString& filename);
    void displayTable(const QString& tableName);

private:
    QMap<QString, Table> tables;  // Store tables by their names
};

#endif // DATABASE_H
