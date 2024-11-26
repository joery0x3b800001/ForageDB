#ifndef TABLE_H
#define TABLE_H

#include <QString>
#include <QVector>
#include <QList>
#include <QDataStream>
#include "Record.h"

class Table {
public:
    // Constructor with name and optional columns
    Table(const QString& name = "", const QStringList& columns = QStringList());

    // Getter and setter methods
    void setName(const QString& name);
    QString getName() const;

    // Column management
    void addColumn(const QString& column);
    QStringList getColumns() const;  // Return columns as QStringList

    // Record management
    void addRecord(const Record& record);
    QList<Record> getRecords() const;

private:
    QString name;               // Table name
    QStringList columns;        // Columns of the table
    QList<Record> records;      // List of records (data)
};

// Serialization stream operators for Table
QDataStream& operator<<(QDataStream& out, const Table& table);
QDataStream& operator>>(QDataStream& in, Table& table);

#endif // TABLE_H
