#ifndef ENUM_TYPES_H
#define ENUM_TYPES_H


enum COLUMN_TYPE {
    INTEGER,
    TEXT,
    REAL,
    BLOB
};


enum INDEX_TYPE {
     NONE,
     PRIMARY,
     UNIQUE,
     INDEX
};

enum SQL_QUERY_TYPE {
    INSERT_QUERY,
    UPDATE_QUERY,
    DELETE_QUERY,
    SELECT_QUERY
};


#endif // ENUM_TYPES_H
