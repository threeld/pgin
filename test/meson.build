project('pgin-test', 'c', default_options: ['c_std=c2x'])

# Find the sqlite3 dependency
sqlite_dep = dependency('sqlite3')

# Include directories
inc_dir = include_directories('../backend/include')

# Source files
src_files = files('test_db.c', '../backend/src/db.c')

# Define the test executable
executable('test_db', src_files, include_directories: inc_dir, dependencies: [sqlite_dep])
