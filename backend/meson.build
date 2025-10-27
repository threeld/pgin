project('pgin', 'c', default_options: ['c_std=c2x'])

sqlite_dep = dependency('sqlite3')

subdir('src')
inc_dir = include_directories('include')

executable('pgin', src_dir, include_directories: inc_dir, dependencies: sqlite_dep)
