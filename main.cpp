#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QCommandLineParser>
#include <QStringList>

#include "table.h"  // Assuming you have Table.h and Record.h for your classes
#include "Record.h"

// Global database instance
QMap<QString, Table> database;
bool saved = false;

const char* asciiArt = R"(
,------.                                   ,------.  ,-----.
|  .---',---. ,--.--. ,--,--. ,---.  ,---. |  .-.  \ |  |) /_
|  `--,| .-. ||  .--'' ,-.  || .-. || .-. :|  |  \  :|  .-.  \
|  |`  ' '-' '|  |   \ '-'  |' '-' '\   --.|  '--'  /|  '--' /
`--'    `---' `--'    `--`--'.`-  /  `----'`-------' `------'
                             `---'
)";

// ASCII art for ForageDB
void printForageDBArt() {
    qDebug() << asciiArt;
}

// Load the database from a file
void loadDatabaseFromFile(const QString& fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> database;
        file.close();
        qDebug() << "Database loaded from file: " << fileName;
    } else {
        qDebug() << "Error: Could not load database from file: " << fileName;
    }
}

// Save the database to a file
void saveDatabaseToFile(const QString& fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << database;
        file.close();
        qDebug() << "Database saved to file: " << fileName;
    } else {
        qDebug() << "Error: Could not save database to file: " << fileName;
    }
}

// Function to create a new table
void createTable(const QString& tableName, const QStringList& columns) {
    if (database.contains(tableName)) {
        qDebug() << "Error: Table" << tableName << "already exists.";
        return;
    }

    if (columns.isEmpty()) {
        qDebug() << "Error: A table must have at least one column.";
        return;
    }

    Table newTable(tableName, columns);
    database.insert(tableName, newTable);
    qDebug() << "Table" << tableName << "created successfully!";
}

// Function to insert a record into a table
void insertIntoTable(const QString& tableName, const QStringList& values) {
    if (!database.contains(tableName)) {
        qDebug() << "Error: Table" << tableName << "does not exist.";
        return;
    }

    Table& table = database[tableName];

    // Ensure the number of values matches the number of columns
    if (values.size() != table.getColumns().size()) {
        qDebug() << "Error: Number of values does not match number of columns.";
        return;
    }

    // Clean the input values (remove quotes and trim whitespace)
    QStringList cleanedValues;
    for (QString value : values) {
        value = value.trimmed();  // Trim leading and trailing spaces
        value = value.remove('\'');  // Remove surrounding single quotes (if any)
        value = value.remove(';');   // Remove any semicolon characters (if any)
        cleanedValues.append(value);
    }

    // Create a new record with the cleaned values
    Record newRecord(cleanedValues);
    table.addRecord(newRecord);
    qDebug() << "Record inserted into" << tableName;
}


// Function to select and display data from a table
void selectFromTable(const QString& tableName) {
    if (!database.contains(tableName)) {
        qDebug() << "Error: Table" << tableName << "does not exist.";
        return;
    }

    Table& table = database[tableName];
    qDebug() << "Table:" << tableName;
    qDebug() << "Columns:" << table.getColumns();

    const QList<Record>& records = table.getRecords();
    qDebug() << "Records:";
    for (const Record& record : records) {
        qDebug() << record.getValues();
    }
}

// Function to list all columns
void showColumns() {
    if (database.isEmpty()) {
        qDebug() << "No columns available.";
    } else {
        qDebug() << "Columns:";
        for (const QString& tableName : database.keys()) {
            Table& table = database[tableName];
            qDebug() << tableName << ":" << table.getColumns();
        }
    }
}

// Function to list all tables
void showTables() {
    if (database.isEmpty()) {
        qDebug() << "No tables available.";
    } else {
        qDebug() << "Tables:";
        for (const QString& tableName : database.keys()) {
            qDebug() << tableName;
        }
    }
}

// Function to delete a table
void dropTable(const QString& tableName) {
    if (!database.contains(tableName)) {
        qDebug() << "Error: Table" << tableName << "does not exist.";
        return;
    }

    database.remove(tableName);
    qDebug() << "Table" << tableName << "deleted successfully!";
}

// Function to handle the commands entered by the user
void handleCommand(const QString& command) {
    if (command.startsWith("CREATE TABLE", Qt::CaseInsensitive)) {
        // CREATE TABLE <table_name> (<col1>, <col2>, ...)
        QStringList parts = command.mid(12).trimmed().split('(');
        if (parts.size() != 2) {
            qDebug() << "Error: Invalid CREATE TABLE syntax.";
            return;
        }

        QString tableName = parts[0].trimmed();
        QStringList columns = parts[1].remove(')').split(',');

        // Clean each column name (remove semicolons and trim spaces)
        for (int i = 0; i < columns.size(); ++i) {
            columns[i] = columns[i].trimmed();  // Trim whitespace
            columns[i] = columns[i].remove(';'); // Remove semicolons
        }

        createTable(tableName, columns);

    } else if (command.startsWith("INSERT INTO", Qt::CaseInsensitive)) {
        // INSERT INTO <table_name> VALUES (<val1>, <val2>, ...)
        QStringList parts = command.mid(12).trimmed().split("VALUES");
        if (parts.size() != 2) {
            qDebug() << "Error: Invalid INSERT INTO syntax.";
            return;
        }

        QString tableName = parts[0].trimmed();
        QString valuesStr = parts[1].trimmed();
        valuesStr.remove('(').remove(')');  // Remove parentheses
        QStringList values = valuesStr.split(',');

        // Clean the values (remove quotes and semicolons)
        for (int i = 0; i < values.size(); ++i) {
            values[i] = values[i].trimmed();   // Trim whitespace
            values[i] = values[i].remove('\''); // Remove quotes
            values[i] = values[i].remove(';');  // Remove semicolons
        }

        insertIntoTable(tableName, values);

    } else if (command.startsWith("SELECT", Qt::CaseInsensitive)) {
        // SELECT <table_name>
        QString tableName = command.mid(7).trimmed();

        // Remove semicolon if it exists at the end of the table name
        tableName = tableName.remove(';').trimmed();  // Clean table name

        selectFromTable(tableName);

    } else if (command.startsWith("SHOW TABLES", Qt::CaseInsensitive)) {
        QString tableName = command.mid(11).trimmed();

        // Remove semicolon if it exists at the end of the table name
        tableName = tableName.remove(';').trimmed();  // Clean table name
        // SHOW TABLES
        showTables();

    } else if (command.startsWith("SHOW COLUMNS", Qt::CaseInsensitive)) {
        QString tableName = command.mid(12).trimmed();

        // Remove semicolon if it exists at the end of the column name
        tableName = tableName.remove(';').trimmed(); // Clean column name
        // SHOW COLUMNS
        showColumns();

    } else if (command.startsWith("DROP TABLE", Qt::CaseInsensitive)) {
        // DROP TABLE <table_name>
        QString tableName = command.mid(10).trimmed();
        dropTable(tableName);

    } else if (command.startsWith("SAVE DATABASE", Qt::CaseInsensitive)) {
        // SAVE DATABASE <filename>
        saved = true;
        QStringList parts = command.mid(13).trimmed().split(' ');
        if (parts.size() != 1) {
            qDebug() << "Error: Invalid SAVE DATABASE syntax.";
            return;
        }
        QString filename = parts[0].trimmed();
        saveDatabaseToFile(filename);

    } else if (command.startsWith("LOAD DATABASE", Qt::CaseInsensitive)) {
        // LOAD DATABASE <filename>
        QStringList parts = command.mid(13).trimmed().split(' ');
        if (parts.size() != 1) {
            qDebug() << "Error: Invalid LOAD DATABASE syntax.";
            return;
        }
        QString filename = parts[0].trimmed();
        loadDatabaseFromFile(filename);

    } else if (command == "EXIT") {
        if (!saved) {
            saveDatabaseToFile("database.dat");  // Save the database to a file before exiting
        }
        qDebug() << "Exiting program.";

    } else {
        qDebug() << "Error: Unknown command.";
    }
}



// Display the help message
void showHelp() {
    qDebug() << "Help - Available Commands:";
    qDebug() << "1. CREATE TABLE <table_name> (<col1>, <col2>, ...)";
    qDebug() << "   - Creates a new table with the specified columns.";
    qDebug() << "2. INSERT INTO <table_name> VALUES (<val1>, <val2>, ...)";
    qDebug() << "   - Inserts a new record into the specified table.";
    qDebug() << "3. SELECT <table_name>";
    qDebug() << "   - Displays all records from the specified table.";
    qDebug() << "4. SHOW TABLES";
    qDebug() << "   - Lists all available tables.";
    qDebug() << "5. DROP TABLE <table_name>";
    qDebug() << "   - Deletes the specified table.";
    qDebug() << "6. SHOW COLUMNS";
    qDebug() << "   - Lists all available columns.";
    qDebug() << "7. SAVE DATABASE <filename>";
    qDebug() << "   - Saves the current database to the specified file.";
    qDebug() << "8. LOAD DATABASE <filename>";
    qDebug() << "   - Loads the database from the specified file.";
    qDebug() << "9. EXIT";
    qDebug() << "   - Exits the program and saves the database.";
    qDebug() << "10. --help";
    qDebug() << "   - Shows this help message.";
}

// Main function - entry point of the application
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QTextStream in(stdin);
    QTextStream out(stdout);

    // Set up command-line arguments parser
    QCommandLineParser parser;
    printForageDBArt();  // Display the ASCII art here
    parser.setApplicationDescription("ForageDB SQL-like Command Line Tool");
    parser.addHelpOption();
    parser.addVersionOption();

    // Command-line parsing for load at startup
    parser.addPositionalArgument("load-file", "The database file to load at startup.");
    parser.process(app);

    // Load database if a file is provided as argument
    if (parser.positionalArguments().size() > 0) {
        QString fileName = parser.positionalArguments().first();
        loadDatabaseFromFile(fileName);
    } else {
        qDebug() << "No database file provided. Starting with an empty database.";
    }

    // Command-line interface loop
    while (true) {
        out << "> ";  // Prompt for user input
        out.flush();

        QString command = in.readLine().trimmed();

        if (command == "EXIT") {
            if (!saved) {
                saveDatabaseToFile("database.dat");  // Save the database to a file before exiting
            }
            qDebug() << "Exiting program.";
            break;
        }

        if (command == "--help") {
            showHelp();
        } else {
            handleCommand(command);  // Handle the input command
        }
    }

    return 0;
}
