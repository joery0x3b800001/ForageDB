#include "databaseserializer.h"
#include <QFile>
#include <QDataStream>

void DatabaseSerializer::saveToFile(const QMap<QString, Table>& tables, const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15);  // Ensure compatibility with Qt 5.x or later

        // Save the number of tables
        out << tables.size();
        for (auto it = tables.begin(); it != tables.end(); ++it) {
            out << it.key();  // Save table name
            out << it.value();  // Save table data (columns and records)
        }
    }
}

QMap<QString, Table> DatabaseSerializer::loadFromFile(const QString& filename) {
    QMap<QString, Table> tables;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);  // Ensure compatibility with Qt 5.x or later

        // Read the number of tables
        int tableCount;
        in >> tableCount;
        for (int i = 0; i < tableCount; ++i) {
            QString tableName;
            Table table;
            in >> tableName;  // Read table name
            in >> table;  // Read table data (columns and records)
            tables.insert(tableName, table);
        }
    }
    return tables;
}
