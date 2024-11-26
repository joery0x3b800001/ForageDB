#include "table.h"

Table::Table(const QString& name, const QStringList& columns)
    : name(name), columns(columns) {}

void Table::setName(const QString& name) {
    this->name = name;
}

QString Table::getName() const {
    return name;
}

void Table::addColumn(const QString& column) {
    columns.append(column);
}

QStringList Table::getColumns() const {
    return columns;
}

void Table::addRecord(const Record& record) {
    records.append(record);  // Add the record to the table
}

QList<Record> Table::getRecords() const {
    return records;  // Return all records
}

// Serialization: Write Table to a QDataStream
QDataStream& operator<<(QDataStream& out, const Table& table) {
    out << table.getName();               // Write table name
    out << table.getColumns();            // Write columns (QStringList)

    int recordCount = table.getRecords().size();
    out << recordCount;                   // Write number of records
    for (const Record& record : table.getRecords()) {
        out << record;                    // Write each record (assuming Record has proper stream operators)
    }

    return out;
}

// Serialization: Read Table from a QDataStream
QDataStream& operator>>(QDataStream& in, Table& table) {
    QString tableName;
    QStringList columns;
    int recordCount;

    in >> tableName;                   // Read table name
    in >> columns;                     // Read columns (QStringList)
    in >> recordCount;                 // Read number of records

    table.setName(tableName);          // Set the table name
    for (const QString& column : columns) {
        table.addColumn(column);       // Add columns to the table
    }

    for (int i = 0; i < recordCount; ++i) {
        Record record;
        in >> record;                  // Read each record (assuming Record has proper stream operators)
        table.addRecord(record);       // Add record to the table
    }

    return in;
}
