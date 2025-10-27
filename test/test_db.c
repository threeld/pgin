#include "../backend/include/db.h"

int main() {
    sqlite3 *db;
    if (db_init(&db) != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize database\n");
        exit(EXIT_FAILURE);
    }

    if (db_create_tables(db) != SQLITE_OK) {
        fprintf(stderr, "Failed to create tables\n");
        db_close(db);
        exit(EXIT_FAILURE);
    }

    // Test database functions
    if (db_insert_message(db, "Aryan", "Welcome to Pgin bitches") != SQLITE_OK) {
        fprintf(stderr, "Failed to insert test message\n");
    }

    printf("--- Chat History ---\n");
    db_get_messages(db);
    printf("--------------------\n");

    db_close(db);
    return 0;
}
