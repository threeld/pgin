# Database Module Documentation

## Overview

This module provides a simple SQLite database interface for storing and retrieving chat messages. It handles database initialization, table creation, message insertion, and message retrieval operations.

## Database Schema

### Table: `messages`

| Column   | Type    | Constraints              | Description                    |
|----------|---------|--------------------------|--------------------------------|
| id       | INTEGER | PRIMARY KEY AUTOINCREMENT | Unique identifier for each message |
| username | TEXT    | NOT NULL                 | Name of the user who sent the message |
| message  | TEXT    | NOT NULL                 | Content of the message         |

## Functions

### `db_init(sqlite3 **db)`

Initializes the database by creating and opening a connection to `chat.db`.

**Parameters:**
- `db`: Double pointer to an sqlite3 database object that will store the connection

**Returns:**
- `SQLITE_OK` on success
- SQLite error code on failure

**Behavior:**
- Creates a file named `chat.db` if it doesn't exist
- Opens a connection to the database
- Prints success or error messages to stdout/stderr

**Example Usage:**
```c
sqlite3 *db;
if (db_init(&db) != SQLITE_OK) {
    // Handle initialization error
}
```

---

### `db_close(sqlite3 *db)`

Closes the database connection and cleans up resources.

**Parameters:**
- `db`: Pointer to the sqlite3 database object to close

**Returns:**
- None (void function)

**Example Usage:**
```c
db_close(db);
```

---

### `db_create_tables(sqlite3 *db)`

Creates the necessary database tables if they don't already exist.

**Parameters:**
- `db`: Pointer to an open sqlite3 database connection

**Returns:**
- `SQLITE_OK` on success
- SQLite error code on failure

**Behavior:**
- Creates the `messages` table with the schema described above
- Uses `IF NOT EXISTS` clause to avoid errors if table already exists
- Prints success or error messages to stdout/stderr
- Automatically frees error message memory on failure

**Example Usage:**
```c
if (db_create_tables(db) != SQLITE_OK) {
    // Handle table creation error
}
```

---

### `db_insert_message(sqlite3 *db, const char *username, const char *message)`

Inserts a new message into the database.

**Parameters:**
- `db`: Pointer to an open sqlite3 database connection
- `username`: Name of the user sending the message
- `message`: Content of the message

**Returns:**
- `SQLITE_OK` on success
- SQLite error code on failure

**Security Features:**
- Uses prepared statements with parameter binding to prevent SQL injection attacks
- `SQLITE_TRANSIENT` flag ensures SQLite makes internal copies of the strings

**Example Usage:**
```c
if (db_insert_message(db, "Alice", "Hello, World!") != SQLITE_OK) {
    // Handle insertion error
}
```

---

### `db_get_messages(sqlite3 *db)`

Retrieves and prints all messages from the database to the console.

**Parameters:**
- `db`: Pointer to an open sqlite3 database connection

**Returns:**
- `SQLITE_OK` on success
- SQLite error code on failure

**Behavior:**
- Retrieves messages ordered by ID in ascending order (oldest first)
- Prints each message in the format: `[username]: message`
- Processes rows one at a time using `sqlite3_step()`

**Example Usage:**
```c
printf("Chat History:\n");
if (db_get_messages(db) != SQLITE_OK) {
    // Handle retrieval error
}
```

**Output Example:**
```
[Alice]: Hello, World!
[Bob]: Hi Alice!
[Alice]: How are you?
```

## Key Concepts

### Prepared Statements

This module uses prepared statements for database operations, which provide several benefits:

1. **Security**: Prevents SQL injection by separating SQL logic from data
2. **Performance**: Compiled statements can be reused efficiently
3. **Type Safety**: Explicit binding of parameters with proper types

### Resource Management

The module follows proper resource management practices:

- `sqlite3_prepare_v2()` compiles SQL into a statement object
- `sqlite3_finalize()` releases statement resources
- Error messages are freed with `sqlite3_free()`

### Error Handling

Each function checks return codes and provides detailed error messages using `sqlite3_errmsg()` when operations fail.

## Usage Example

```c
#include "db.h"

int main() {
    sqlite3 *db;
    
    // Initialize database
    if (db_init(&db) != SQLITE_OK) {
        return 1;
    }
    
    // Create tables
    if (db_create_tables(db) != SQLITE_OK) {
        db_close(db);
        return 1;
    }
    
    // Insert messages
    db_insert_message(db, "Alice", "Hello!");
    db_insert_message(db, "Bob", "Hi there!");
    
    // Display all messages
    db_get_messages(db);
    
    // Clean up
    db_close(db);
    
    return 0;
}
```

## Dependencies

- **SQLite3**: This module requires the SQLite3 library to be installed and linked
- **db.h**: Header file containing function declarations and necessary includes

## Notes

- The database file `chat.db` is created in the current working directory
- All string parameters are copied by SQLite (using `SQLITE_TRANSIENT`), so the caller can safely free or modify them after function calls
- Messages are stored and retrieved in chronological order based on their auto-incrementing ID