#ifndef ADAK_SQL_STRUCT_H
#define ADAK_SQL_STRUCT_H

#define NUM_OF_INDEX     20
#define NUM_OF_COLUMNS  400
#define NUM_OF_TABLES   100

#include <adak_sql_enums.h>


struct COLUMN_STRUCT {
    const char *  name;
    COLUMN_TYPE   type;
    int           length;
    bool          is_array;
    int           array_length;
    bool          is_auto_increment;
    bool          is_not_null;
};

struct INDEX_STRUCT {
    INDEX_TYPE    type;
    int           column_count;
    const char *  columns[NUM_OF_COLUMNS];
};


struct TABLE_STRUCT {
    const char *   name;
    int            column_count;
    COLUMN_STRUCT  columns[NUM_OF_COLUMNS];
    int            index_count;
    INDEX_STRUCT   indexes[NUM_OF_INDEX];
};


struct ADAK_SQL_STRUCT {
    int             adak_program_id;
    int             table_count;
    TABLE_STRUCT    tables[NUM_OF_TABLES];
};


#endif // ADAK_SQL_STRUCT_H 
