#include "../backend/include/db.h"

int main()
{
    sqlite3 *db;
    
    // Initialize database
    if (db_init(&db) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to initialize database\n");
        return EXIT_FAILURE; // Don't call db_close here, db might not be initialized
    }
    
    // Create tables
    if (db_create_tables(db) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to create tables\n");
        db_close(db);
        return EXIT_FAILURE;
    }
    
    // Test database functions - insert message
    if (db_insert_message(db, "Aryan", "Welcome to Pgin Bitches") != SQLITE_OK)
    {
        fprintf(stderr, "Failed to insert test message\n");
    }
    
    // Retrieve and display messages
    printf("--- Chat History ---\n");
    if (db_get_messages(db) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to retrieve messages\n");
    }
    printf("--------------------\n");
    
    // Clean up
    db_close(db);
    return 0;
}