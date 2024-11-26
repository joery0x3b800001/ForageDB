#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <QString>
#include <QVector>
#include <QRegularExpression>  // Use QRegularExpression instead of QRegExp

class SQLParser {
public:
    QString extractTableName(const QString& sql);
    QVector<QString> extractColumns(const QString& sql);
    QVector<QString> extractValues(const QString& sql);
};

#endif // SQLPARSER_H
