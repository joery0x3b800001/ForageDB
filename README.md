## Forage DB

The architecture of the database system we’ve described, we can break down the system into key components and interactions. This will be a high-level design that shows how the various parts of the system work together. Here's an architecture diagram description and explanation of each component.

---

### **Architecture Diagram: ForageDB**

```plaintext
                               +--------------------+
                               |     Command Line    |
                               |    Interface (CLI)  |
                               +--------------------+
                                       |
                                       v
                      +----------------------------------------+
                      |                Application Logic       |
                      |    (Handles commands, database logic)  |
                      +----------------------------------------+
                                       |
                                       v
             +---------------------------------------------------------+
             |                       Database Layer                    |
             |  +-------------------+    +------------------------+    |
             |  |     Table Class    |    |      Record Class      |    |
             |  |                   |    |                        |    |
             |  | - Columns          |    | - Values               |    |
             |  | - Records          |    |                        |    |
             |  | - CRUD Operations  |    | - CRUD Operations      |    |
             |  |                   |    |                        |    |
             |  +-------------------+    +------------------------+    |
             +---------------------------------------------------------+
                                       |
                                       v
                           +-----------------------------+
                           |     File Persistence Layer  |
                           |   (Load and Save Database)  |
                           +-----------------------------+
```

### **Component Breakdown:**

---

#### **1. Command Line Interface (CLI):**
- The **CLI** is the entry point where users interact with the database system. It accepts commands from the user such as `CREATE TABLE`, `INSERT INTO`, `SELECT`, `SHOW TABLES`, etc.
- User inputs are parsed and passed to the application logic layer for further processing.

---

#### **2. Application Logic Layer:**
- This is the core of the system, where commands entered via the CLI are handled.
- It contains the **command handler** that interprets and validates commands, and then calls the appropriate methods in the **Database Layer**.
- Key functionalities in this layer:
  - **Command Parsing:** Parse and validate user input.
  - **Table and Record Operations:** Invoke database methods to create tables, insert records, query data, etc.
  - **Help & Exit:** Provides help information and handles exiting the program.
  - **File Persistence:** Interacts with the file system to save/load the database state to/from a file.
  
---

#### **3. Database Layer:**
- **Table Class:**
  - This class represents a **table** in the database. Each table has columns and records.
  - **Columns:** Store the names of the columns in a table (e.g., `id`, `name`, `age`).
  - **Records:** Stores the actual data rows. Each record is typically a collection of values corresponding to the columns.
  - **CRUD Operations:** Methods to create tables, insert records, and perform other operations (like drop table, update, etc.).
  
- **Record Class:**
  - Represents a **record** (row) in the table.
  - Contains values for each column and provides operations to manage those values.
  
- The **Table** and **Record** classes work together to simulate a relational database. Each table holds a collection of records, and each record contains the values for a particular entry.

---

#### **4. File Persistence Layer:**
- The **File Persistence Layer** manages saving and loading the database state to and from a file.
- The database is serialized and deserialized using file operations:
  - **Save Database:** The state of the database (all tables and records) is written to a file using serialization (likely using `QDataStream`).
  - **Load Database:** The state of the database is read from a file and restored into memory when the program starts.
- This allows for persistent storage of data, so the database can retain its state across sessions.

---

### **Detailed Flow of Operations:**

1. **User Command Input (CLI):**
   - The user inputs a command, e.g., `CREATE TABLE users (id, name, age)`.
   - The **CLI** sends this command to the **Application Logic Layer** for processing.

2. **Command Parsing and Execution (Application Logic):**
   - The application logic processes the input command:
     - If it’s a `CREATE TABLE` command, the logic will check if the table exists and create a new table in the **Database Layer**.
     - For an `INSERT INTO` command, it will validate the column values and insert a new record into the relevant table in the **Database Layer**.
     - For `SELECT` and other queries, the application will fetch records from the **Table** class in the **Database Layer**.

3. **Database Layer (Table & Record Classes):**
   - The **Table Class** manages the columns and records.
   - Each **Record** is stored in a list or a similar data structure inside the **Table Class**.
   - CRUD operations on tables and records are carried out here.

4. **Saving and Loading the Database (File Persistence):**
   - The **Save Database** and **Load Database** operations are invoked by the user, and the data is saved or loaded from a file.
   - This layer handles all the file I/O operations for persistent data storage.

---

### **How the Components Interact:**

- **CLI** → **Application Logic Layer**: The user types commands in the terminal (CLI), which are passed to the application logic for interpretation.
- **Application Logic Layer** → **Database Layer**: The application logic interacts with the database (Table and Record classes) to manipulate the data based on user commands.
- **Database Layer** → **File Persistence Layer**: When the database is saved or loaded, the database layer communicates with the file persistence layer to store or retrieve the serialized database state.

---

### **Example:**

> LOAD DATABASE my_database.dat
Database loaded from file: my_database.dat

> CREATE TABLE users (id, name, age);
Table users created successfully!

> INSERT INTO users VALUES ('1', 'Alice', 30);
Record inserted into users.

> SELECT users;
Table: users
Columns: ("id", "name", "age")
Records:
("1", "Alice", "30")

> SAVE DATABASE new_database.dat
Database saved to file: new_database.dat

> EXIT
Database saved to file: database.dat
Exiting program.

---

### **Conclusion:**

This architecture is modular and easily extendable. The core logic of handling tables and records is separated from the file persistence and user interaction layers, which keeps the system maintainable. Each component interacts with others in a clear and direct way, ensuring that commands are parsed, executed, and stored or retrieved from files in a structured and efficient manner.