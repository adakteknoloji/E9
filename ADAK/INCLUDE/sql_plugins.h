#ifndef SQL_PLUGINS_H
#define SQL_PLUGINS_H

#include "adak_defines.h"

class  ADAK_SQL;
struct ADAK_SQL_STRUCT;

enum DB_DRIVER { UNKNOWN, SQLITE, PSQL, MYSQL, MSSQL, ORACLE , ODBC };

DB_DRIVER       GET_SQL_DB_DRIVER_ENUM       ( QString driver_name );
int             GET_DEFAULT_SQL_PORT_NUMBER  ( DB_DRIVER db_driver);
QStringList     GET_SQL_DRIVER_LIST          ();

ADAK_SQL *      CREATE_ADAK_SQL_CONNECTION  ( QList<ADAK_SQL_STRUCT *> adak_sql_struct , QString db_name , DB_DRIVER driver );
void            SQL_SET_DB_SID               (QString sid);
QString         SQL_GET_DB_SID               ();

//! VERITABANI OTOMATIK BAGLANIR.
ADAK_SQL *     CONNECT_DATABASE             ( QList<ADAK_SQL_STRUCT *> adak_sql_struct , QString db_name , DB_DRIVER driver,  bool show_error_screen );

#endif // SQL_PLUGINS_H
