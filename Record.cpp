#include "Record.h"
#include <QDataStream>

// Constructor for Record that takes a QVector<QString>
Record::Record(const QVector<QString>& data) : values(data) {}

// Default constructor
Record::Record() {}

QVector<QString> Record::getValues() const {
    return values;
}

void Record::setValues(const QVector<QString>& newValues) {
    values = newValues;
}

// Serialize Record to QDataStream
QDataStream& operator<<(QDataStream& out, const Record& record) {
    out << record.getValues();  // Serialize the QVector<QString> (values)
    return out;
}

// Deserialize Record from QDataStream
QDataStream& operator>>(QDataStream& in, Record& record) {
    QVector<QString> values;
    in >> values;  // Deserialize QVector<QString> (values)
    record.setValues(values);  // Set the values into the record
    return in;
}
