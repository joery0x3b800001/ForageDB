#include "database.h"
#include "sqlparser.h"
#include <QFile>
#include <QTextStream>

Database::Database() {}

void Database::createTable(const QString& sql) {
    SQLParser parser;
    QString tableName = parser.extractTableName(sql);
    QVector<QString> columns = parser.extractColumns(sql);

    Table newTable(tableName);
    for (const auto& column : columns) {
        newTable.addColumn(column);
    }

    tables.insert(tableName, newTable);
    QTextStream(stdout) << "Table \"" << tableName << "\" created successfully!\n";
}

void Database::insertRecord(const QString& sql) {
    SQLParser parser;
    QString tableName = parser.extractTableName(sql);
    QVector<QString> values = parser.extractValues(sql);

    if (tables.contains(tableName)) {
        Record newRecord(values);
        tables[tableName].addRecord(newRecord);
        QTextStream(stdout) << "Inserted record into \"" << tableName << "\"\n";
    } else {
        QTextStream(stdout) << "Table \"" << tableName << "\" not found!\n";
    }
}

void Database::selectData(const QString& sql) {
    SQLParser parser;
    QString tableName = parser.extractTableName(sql);

    if (tables.contains(tableName)) {
        displayTable(tableName);
    } else {
        QTextStream(stdout) << "Table \"" << tableName << "\" does not exist.\n";
    }
}

void Database::saveDatabase(const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << tables.size();
        for (auto it = tables.begin(); it != tables.end(); ++it) {
            out << it.key() << it.value();
        }
        QTextStream(stdout) << "Database saved to \"" << filename << "\"\n";
    } else {
        QTextStream(stdout) << "Error opening file for saving!\n";
    }
}

void Database::loadDatabase(const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        int tableCount;
        in >> tableCount;
        for (int i = 0; i < tableCount; ++i) {
            QString tableName;
            Table table("");
            in >> tableName >> table;
            tables.insert(tableName, table);
        }
        QTextStream(stdout) << "Database loaded from \"" << filename << "\"\n";
    } else {
        QTextStream(stdout) << "Error opening file for loading!\n";
    }
}

void Database::displayTable(const QString& tableName) {
    Table table = tables[tableName];
    QTextStream(stdout) << "Table \"" << tableName << "\" contents:\n";

    QVector<QString> columns = table.getColumns();
    for (const auto& column : columns) {
        QTextStream(stdout) << column << "\t";
    }
    QTextStream(stdout) << "\n";

    QVector<Record> records = table.getRecords();
    for (const auto& record : records) {
        for (const auto& value : record.getValues()) {
            QTextStream(stdout) << value << "\t";
        }
        QTextStream(stdout) << "\n";
    }
}
