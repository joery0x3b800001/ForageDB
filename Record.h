#ifndef RECORD_H
#define RECORD_H

#include <QVector>
#include <QString>

class Record {
public:
    Record();  // Default constructor
    Record(const QVector<QString>& data);  // Constructor with data

    QVector<QString> getValues() const;
    void setValues(const QVector<QString>& newValues);

private:
    QVector<QString> values;  // Store record values
};

// Stream operators for Record
QDataStream& operator<<(QDataStream& out, const Record& record);
QDataStream& operator>>(QDataStream& in, Record& record);

#endif // RECORD_H
