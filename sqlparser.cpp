#include "sqlparser.h"
#include <QRegularExpression>  // Include QRegularExpression header

QString SQLParser::extractTableName(const QString& sql) {
    // Use QRegularExpression instead of QRegExp
    static QRegularExpression regex("CREATE TABLE (\\w+)\\s*\\(");
    QRegularExpressionMatch match = regex.match(sql);
    if (match.hasMatch()) {
        return match.captured(1);
    }
    return QString();
}

QVector<QString> SQLParser::extractColumns(const QString& sql) {
    QVector<QString> columns;
    static QRegularExpression regex("(\\w+)");  // Match words (column names)
    QRegularExpressionMatchIterator i = regex.globalMatch(sql);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        columns.append(match.captured(1));  // Add matched column name to the list
    }

    return columns;
}

QVector<QString> SQLParser::extractValues(const QString& sql) {
    QVector<QString> values;
    static QRegularExpression regex("'([^']+)'");  // Match values inside single quotes
    QRegularExpressionMatchIterator i = regex.globalMatch(sql);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        values.append(match.captured(1));  // Add matched value to the list
    }

    return values;
}
