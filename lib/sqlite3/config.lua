add_c99object("lib/sqlite3/sqlite3")
add_c99object("lib/sqlite3/shell")

add_cflags {
    "-DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_JSON1",
    "-Ilib/sqlite3/",

    -- recommended SQLite build flags for CLI
    "-DSQLITE_THREADSAFE=0",
    "-DSQLITE_ENABLE_EXPLAIN_COMMENTS",
    "-DSQLITE_INTROSPECTION_PRAGMAS",
    "-DSQLITE_ENABLE_UNKNOWN_SQL_FUNCTION",
    "-DSQLITE_ENABLE_STMTVTAB",
    "-DSQLITE_ENABLE_DBPAGE_VTAB",
    "-DSQLITE_ENABLE_DBSTAT_VTAB",
    "-DSQLITE_ENABLE_OFFSET_SQL_FUNC",
    "-DSQLITE_ENABLE_RTREE",
    "-DSQLITE_ENABLE_FTS4"
}

if use_readline == true then
    add_cflags {
        "-DHAVE_READLINE=1"
    }
end
