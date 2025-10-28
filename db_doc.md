# Database Module Documentation (db.c)

## Overview

This module provides a SQLite database interface for storing and retrieving chat messages. It includes comprehensive error handling, input validation, and resource management to ensure safe and reliable database operations.

## Dependencies

```c
#include "db.h"
```

## Constants

```c
#define MAX_USERNAME_LENGTH 50
#define MAX_MESSAGE_LENGTH 1000
```

These constants define the maximum allowed lengths for usernames and messages to prevent buffer overflows and excessive memory usage.

---

## Database Schema

### Table: `messages`

| Column   | Type    | Constraints              | Description                    |
|----------|---------|--------------------------|--------------------------------|
| id       | INTEGER | PRIMARY KEY AUTOINCREMENT | Unique identifier for each message |
| username | TEXT    | NOT NULL                 | Name of the user who sent the message |
| message  | TEXT    | NOT NULL                 | Content of the message         |

---

## Functions

### `db_init(sqlite3 **db)`

Initializes the database by creating and opening a connection to `chat.db`.

**Parameters:**
- `db`: Double pointer to an sqlite3 database object that will store the connection

**Returns:**
- `SQLITE_OK` on success
- `SQLITE_ERROR` if the db pointer is NULL
- SQLite error code on failure

**Validation:**
- ✅ Checks if db pointer is valid before proceeding

**Behavior:**
- Validates the database pointer reference
- Creates a file named `chat.db` if it doesn't exist
- Opens a connection to the database
- Prints success or error messages to stdout/stderr

**Example Usage:**
```c
sqlite3 *db;
if (db_init(&db) != SQLITE_OK) {
    fprintf(stderr, "Failed to initialize database\n");
    return EXIT_FAILURE;
}
```

**Important Notes:**
- If initialization fails, do NOT call `db_close()` as the database pointer may be in an invalid state
- The database file is created in the current working directory

---

### `db_close(sqlite3 *db)`

Closes the database connection and cleans up resources.

**Parameters:**
- `db`: Pointer to the sqlite3 database object to close

**Returns:**
- None (void function)

**Validation:**
- ✅ Checks if db pointer is NULL before attempting to close
- ✅ Uses `sqlite3_close_v2()` as fallback to force close and prevent resource leaks

**Behavior:**
- Silently returns if db pointer is NULL
- Attempts to close the database connection
- If normal close fails, forces close using `sqlite3_close_v2()` to prevent resource leaks
- Prints error message if close operation fails

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
- `SQLITE_ERROR` if db pointer is NULL
- SQLite error code on failure

**Validation:**
- ✅ Checks if db pointer is valid

**Behavior:**
- Validates the database pointer
- Creates the `messages` table with the schema described above
- Uses `IF NOT EXISTS` clause to avoid errors if table already exists
- Prints success or error messages to stdout/stderr
- Automatically frees error message memory on failure

**Example Usage:**
```c
if (db_create_tables(db) != SQLITE_OK) {
    fprintf(stderr, "Failed to create tables\n");
    db_close(db);
    return EXIT_FAILURE;
}
```

---

### `db_insert_message(sqlite3 *db, const char *username, const char *message)`

Inserts a new message into the database with comprehensive validation.

**Parameters:**
- `db`: Pointer to an open sqlite3 database connection
- `username`: Name of the user sending the message
- `message`: Content of the message

**Returns:**
- `SQLITE_OK` on success
- `SQLITE_ERROR` if validation fails
- SQLite error code on database operation failure

**Validation:**
- ✅ Checks if db pointer is valid
- ✅ Checks if username is not NULL or empty
- ✅ Checks if message is not NULL or empty
- ✅ Validates username length (max 50 characters)
- ✅ Validates message length (max 1000 characters)
- ✅ Checks return values for all bind operations
- ✅ Properly finalizes statement on any error

**Security Features:**
- Uses prepared statements with parameter binding to prevent SQL injection attacks
- `SQLITE_TRANSIENT` flag ensures SQLite makes internal copies of the strings
- All user input is validated before processing

**Error Handling:**
- Validates all input parameters before database operations
- Checks return value of each `sqlite3_bind_text()` call
- Properly cleans up resources (finalizes statement) on any error
- Provides descriptive error messages for each failure case

**Example Usage:**
```c
if (db_insert_message(db, "Alice", "Hello, World!") != SQLITE_OK) {
    fprintf(stderr, "Failed to insert message\n");
}
```

**Error Messages:**
- "Database pointer is NULL"
- "Username is NULL or empty"
- "Message is NULL or empty"
- "Username exceeds maximum length of 50 characters"
- "Message exceeds maximum length of 1000 characters"
- "Failed to prepare statement: [SQLite error]"
- "Failed to bind username: [SQLite error]"
- "Failed to bind message: [SQLite error]"
- "Execution failed: [SQLite error]"

---

### `db_get_messages(sqlite3 *db)`

Retrieves and prints all messages from the database to the console.

**Parameters:**
- `db`: Pointer to an open sqlite3 database connection

**Returns:**
- `SQLITE_OK` on success
- `SQLITE_ERROR` if db pointer is NULL
- SQLite error code on failure

**Validation:**
- ✅ Checks if db pointer is valid
- ✅ Checks for NULL column values before printing

**Behavior:**
- Validates the database pointer
- Retrieves messages ordered by ID in ascending order (oldest first)
- Checks each row for NULL values before printing
- Prints each message in the format: `[username]: message`
- Processes rows one at a time using `sqlite3_step()`
- Skips rows with NULL values and prints a warning
- Properly finalizes the statement after use

**Example Usage:**
```c
printf("--- Chat History ---\n");
if (db_get_messages(db) != SQLITE_OK) {
    fprintf(stderr, "Failed to retrieve messages\n");
}
printf("--------------------\n");
```

**Output Example:**
```
[Alice]: Hello, World!
[Bob]: Hi Alice!
[Alice]: How are you?
```

---

## Key Concepts

### Prepared Statements

This module uses prepared statements for all database operations, which provide several benefits:

1. **Security**: Prevents SQL injection by separating SQL logic from data
2. **Performance**: Compiled statements can be reused efficiently
3. **Type Safety**: Explicit binding of parameters with proper types

### Resource Management

The module follows strict resource management practices:

- `sqlite3_prepare_v2()` compiles SQL into a statement object
- `sqlite3_finalize()` releases statement resources (called even on errors)
- `sqlite3_close_v2()` forces database closure to prevent leaks
- Error messages are freed with `sqlite3_free()`

### Input Validation

Every function validates its inputs:

- NULL pointer checks for all pointer parameters
- Empty string checks for text inputs
- Length validation against defined maximums
- Return value checks for all SQLite operations

### Error Handling

Comprehensive error handling throughout:

- Every function checks return codes
- Descriptive error messages using `sqlite3_errmsg()`
- Proper cleanup on error paths
- Resources are always freed, even when operations fail

---

## Complete Usage Example

```c
#include "../backend/include/db.h"
#include <stdlib.h>

int main()
{
    sqlite3 *db;
    
    // Initialize database
    if (db_init(&db) != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize database\n");
        return EXIT_FAILURE;
    }
    
    // Create tables
    if (db_create_tables(db) != SQLITE_OK) {
        fprintf(stderr, "Failed to create tables\n");
        db_close(db);
        return EXIT_FAILURE;
    }
    
    // Insert messages
    db_insert_message(db, "Alice", "Hello!");
    db_insert_message(db, "Bob", "Hi there!");
    db_insert_message(db, "Alice", "How are you?");
    
    // Display all messages
    printf("--- Chat History ---\n");
    db_get_messages(db);
    printf("--------------------\n");
    
    // Clean up
    db_close(db);
    
    return 0;
}
```

---

## Error Prevention Features

### Memory Safety
- NULL pointer checks prevent segmentation faults
- All resources are properly freed on error paths
- `SQLITE_TRANSIENT` ensures safe string copying

### Input Validation
- Maximum length checks prevent buffer overflows
- Empty string detection prevents invalid data
- Return value validation catches operation failures

### Resource Leak Prevention
- Statement finalization on all code paths
- Forced database closure as fallback
- Proper error message memory management

---

## Important Notes

1. **Database Location**: The `chat.db` file is created in the current working directory

2. **String Safety**: All string parameters are copied by SQLite using `SQLITE_TRANSIENT`, so callers can safely free or modify them after function calls

3. **Message Order**: Messages are stored and retrieved in chronological order based on their auto-incrementing ID

4. **Error Recovery**: On any error, functions clean up resources before returning, preventing memory leaks

5. **Thread Safety**: This implementation is not thread-safe. For multi-threaded applications, additional synchronization is required
