#ifndef DB_H
#define DB_H

#include <stdio.h> // For printf and perror
#include <stdlib.h> // 
#include <sqlite3.h> // For SQLite

int db_init(sqlite3 **db); // Initialize the database connection
int db_create_tables(sqlite3 *db); // For creating tables in the database
int db_insert_message(sqlite3 *db, const char *username, const char *message); // For saving msgs in database
int db_get_messages(sqlite3 *db); // For getting msgs from the database.
void db_close(sqlite3 *db); // For closing the database connection

#endif
