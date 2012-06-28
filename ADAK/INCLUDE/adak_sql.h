#ifndef ADAKLIB_ADAK_SQL_H
#define ADAKLIB_ADAK_SQL_H

#include "adak_sql_abstract.h"
#include "adak_sql_query.h"
#include "sql_plugins.h"

class ADAK_SQL : public ADAK_SQL_ABSTRACT {
    public:
        ADAK_SQL     (QList<ADAK_SQL_STRUCT *> adak_sql_struct, const QString& db_name);
        ~ADAK_SQL    ();

        int           SQL_FIND_NUM_OF_TABLES_IN_DB ( );
        int           UPDATE_TABLES                ( QList<ADAK_SQL_STRUCT *> adak_sql_struct = QList<ADAK_SQL_STRUCT *>(),QString db_name = "");

private:
};


#endif // ADAKLIB_ADAK_SQL_H
