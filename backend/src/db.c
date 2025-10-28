#include "db.h"

#define MAX_USERNAME_LENGTH 50
#define MAX_MESSAGE_LENGTH 1000

// This function initializes a file named chat.db and opens a database connection to it.
int db_init(sqlite3 **db)
{
    // Check if db pointer is valid
    if (!db)
    {
        fprintf(stderr, "Database pointer reference is NULL\n");
        return SQLITE_ERROR;
    }

    int response = sqlite3_open("chat.db", db); // Tries to create a chat.db file and returns a response if failed
    if (response != SQLITE_OK)
    {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(*db)); // Print error
        return response;
    }
    printf("Database opened successfully.\n");
    return SQLITE_OK; // Return SQLITE_OK status code
}

// This is the function for closing the database connection.
void db_close(sqlite3 *db)
{
    if (!db)
    {
        return;
    }

    int response = sqlite3_close(db);
    if (response != SQLITE_OK)
    {
        fprintf(stderr, "Failed to close database properly: %s\n", sqlite3_errmsg(db));
        sqlite3_close_v2(db); // Force close to prevent resource leaks
    }
}

// This function is for creating tables in the database
int db_create_tables(sqlite3 *db)
{
    // Check if db pointer is valid
    if (!db)
    {
        fprintf(stderr, "Database pointer is NULL\n");
        return SQLITE_ERROR;
    }

    // Creating a table named messages in the db if it doesnt exist
    const char *sql =
        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "message TEXT NOT NULL);";

    char *err_msg = NULL;   // Creates a err_msg pointer and set it to NULL so that SQL can use it when needed.
    int response = sqlite3_exec(db, sql, 0, 0, &err_msg); // Execution of sql command with no callbacks being 0 and 0.
    if (response != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg); // Free up the memory of err_msg
        return response;
    }

    printf("Tables created successfully.\n");
    return SQLITE_OK;
}

// This function is to write messages into the database table.
int db_insert_message(sqlite3 *db, const char *username, const char *message)
{
    // Validate input parameters
    if (!db)
    {
        fprintf(stderr, "Database pointer is NULL\n");
        return SQLITE_ERROR;
    }

    // Check if username is not present or empty.
    if (!username || strlen(username) == 0)
    {
        fprintf(stderr, "Username is NULL or empty\n");
        return SQLITE_ERROR;
    }

    // Check if message is not present or empty.
    if (!message || strlen(message) == 0)
    {
        fprintf(stderr, "Message is NULL or empty\n");
        return SQLITE_ERROR;
    }

    // Check length limits
    if (strlen(username) > MAX_USERNAME_LENGTH)
    {
        fprintf(stderr, "Username exceeds maximum length of %d characters\n", MAX_USERNAME_LENGTH);
        return SQLITE_ERROR;
    }

    if (strlen(message) > MAX_MESSAGE_LENGTH)
    {
        fprintf(stderr, "Message exceeds maximum length of %d characters\n", MAX_MESSAGE_LENGTH);
        return SQLITE_ERROR;
    }

    sqlite3_stmt *stmt;  // stmt stands for statement object which is like a compiled version of your query.
    const char *sql = "INSERT INTO messages (username, message) VALUES (?, ?);"; // '?' prevents SQL Injection and variable inputs

    // sqlite3_prepare_v2 compiles the SQL statement into a prepared statement object. More efficient for repeated queries and crucial for preventing SQL injection by separating logic from data.
    int response = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (response != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return response;
    }

    // Binds the username to the first '?' in the SQL statement. SQLITE_TRANSIENT makes a copy of the string.
    response = sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
    if (response != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind username: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt); // finalize the statement to free resources.
        return response;
    }

    // Binds the message to the second '?' in the SQL statement. SQLITE_TRANSIENT makes a copy of the string.
    response = sqlite3_bind_text(stmt, 2, message, -1, SQLITE_TRANSIENT);
    if (response != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind message: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt); // finalize the statement to free resources.
        return response;
    }

    response = sqlite3_step(stmt); // Executes the prepared statement. For an INSERT statement, it should return SQLITE_DONE on success.
    if (response != SQLITE_DONE)
    {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt); // finalize the statement to free resources.
        return response;
    }

    sqlite3_finalize(stmt); // finalizes the statement to free up resources.
    return SQLITE_OK;
}

// This function retrieves all messages from the database and prints them to the console.
int db_get_messages(sqlite3 *db)
{
    // Check if db pointer is valid
    if (!db)
    {
        fprintf(stderr, "Database pointer is NULL\n");
        return SQLITE_ERROR;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT username, message FROM messages ORDER BY id ASC;";

    int response = sqlite3_prepare_v2(db, sql, -1, &stmt, 0); // Prepares the SQL statement for execution.
    if (response != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return response;
    }

    // Loop through each row returned by the SELECT statement.
    // sqlite3_step() returns SQLITE_ROW for each row found, and SQLITE_DONE when no more rows are available.
    while ((response = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        // sqlite3_column_text() retrieves the text value from a specific column of the current row.
        // The first column (index 0) is 'username', and the second (index 1) is 'message'.
        const unsigned char *username = sqlite3_column_text(stmt, 0);
        const unsigned char *message = sqlite3_column_text(stmt, 1);

        // Check for NULL values
        if (!username || !message)
        {
            fprintf(stderr, "Warning: NULL value encountered in database row\n");
            continue;
        }

        printf("[%s]: %s\n", username, message); // Prints the retrieved message in a chat-like format.
    }

    // After the loop, if sqlite3_step() did not return SQLITE_DONE, it means an error occurred.
    if (response != SQLITE_DONE)
    {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return response;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}