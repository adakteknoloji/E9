#ifndef ADAK_SQL_ABSTRACT_H
#define ADAK_SQL_ABSTRACT_H

#include <QStringList>
#include <QSqlDatabase>

#include "adak_sql_enums.h"
#include "sql_plugins.h"

class  QSqlDatabase;

struct TABLE_STRUCT;

struct ADAK_SQL_STRUCT;

enum SQL_OPERATIONS {
    SQL_SELECT = 0,
    SQL_INSERT = 1,
    SQL_UPDATE = 2,
    SQL_DELETE = 3
};

class ADAK_SQL_ABSTRACT
{
public:
                ADAK_SQL_ABSTRACT                        ( QList<ADAK_SQL_STRUCT *> adak_sql_struct ,QString db_name );
                ~ADAK_SQL_ABSTRACT                       ( );

    virtual int         CONNECT_TO_DATABASE               ( bool show_error_screen );
    virtual int         DISCONNECT_TO_DATABASE            ( );
    virtual int         LOCK_ROW                          ( const QString id_column_name,const QString table_name, const QString where_info);
    virtual void        UNLOCK_ROW                        ( const QString id_column_name,const QString table_name, const QString where_info);

    virtual void        SET_CLIENT_CODEC                  ( );

    virtual void        START_TRANSACTION                 ( );
    virtual void        COMMIT_TRANSACTION                ( );
    virtual void        CANCEL_TRANSACTION                ( );

    virtual bool        SQL_CREATE_TABLES                 ( ADAK_SQL_STRUCT * adak_sql_struct , QString db_name = "");
    virtual bool        SQL_DROP_TABLES                   ( ADAK_SQL_STRUCT * adak_sql_struct , QString db_name = "");

    virtual bool        SQL_CREATE_TABLES                 ( QList<ADAK_SQL_STRUCT *> adak_sql_struct , QString db_name = "");
    virtual bool        SQL_DROP_TABLES                   ( QList<ADAK_SQL_STRUCT *> adak_sql_struct , QString db_name = "");

    virtual int         SQL_FIND_NUM_OF_TABLES_IN_DB      ( );

    //
    // Dikkat !..      YIL SONU DEVIR ISLEMLERI disinda bu fonksiyon gerekmemeli
    // Bu fonksiyon sadece YIL SONU DEVIR ISLEMLERI icin yazilmistir.
    //
    // PostgreSQL ile calisirken AUTO_INCREMENT alanlara deger atamasi yapilacak olursa
    // en buyuk degerin hangisi oldugunu PostgreSQL ' e soylemek gerekiyor.
    // Bu islem atamalarin sonunda bir defa yapilirsa yeterli olacaktir.
    virtual void        SET_TABLE_AUTO_INCREMENT_ID_VALUE ( const QString p_table_name, const QString p_id_column_name, int p_last_auto_inc_value );

    virtual bool        TRANSACTION_IS_ON                 ( );
    virtual bool        SQL_TABLES_EXISTS                 ( const QString db_name );

    virtual QString     GET_SQL_DRIVER_DEFAULT_DB_NAME    ( );
    virtual QString     GET_SQL_DRIVER_NAME               ( );
    virtual DB_DRIVER   GET_SQL_DRIVER_ENUM               ( );

    virtual QString     OPEN_CREATE_TABLE_SQL             ( const QString &table_name  );
    virtual QString     CLOSE_CREATE_TABLE_SQL            ( TABLE_STRUCT current_table );
    virtual QString     CREATE_COLUMN_SQL                 ( QString const& column_name, COLUMN_TYPE column_type, int length, bool is_auto_increment, bool is_not_null,bool is_array,INDEX_TYPE index_type );

    virtual QString     DROP_TABLE_SQL                    ( const QString& table_name );
    virtual QString     GET_TABLE_NAMES_SQL               ( );
    virtual QString     GET_COLUMN_NAMES_SQL              ( const QString &table_name, const QString& database_name);
    virtual QString     ADD_ONE_COLUMN_SQL                ( int column_number, TABLE_STRUCT current_table );
    virtual QString     GET_COLUMN_TYPE_NAME              ( COLUMN_TYPE p_column_type, int is_array = 0 );

    virtual int         GET_ID_USING_DRIVER               ( QSqlQuery * query );

    virtual void        SQL_PREPARE_QUERY                 (QString * table_name, QString * id_column_name,  QString * column_names       ,QString * where_condition    , int sql_operation);

    virtual QString     CREATE_SELECT_QUERY               (QString * table_name, QString * column_names   , QString * where_condition    ,QString * group_by,QString * order_by, int baslanilacak_satir , int secilecek_satir_sayisi);
    virtual QString     CREATE_INSERT_QUERY               (QString * table_name, QString * id_column_name , QString * insert_column_names,QString * insert_column_values);
    virtual QString     CREATE_UPDATE_QUERY               (QString * table_name, QString * id_column_name , QString * update_column_names,QString * update_column_values,QString * where_condition);
    virtual QString     CREATE_DELETE_QUERY               (QString * table_name, QString * where_condition);

    virtual void        SQL_FINALIZE_QUERY                ( int sql_operation );

    virtual void        REMOVE_CONNECT                    ( QString connection_name );

    virtual bool        IS_DB_HAS_MULTIPLE_INSERT         () = 0;
    virtual QString     GET_MULTIPLE_INSERT_DIVIDER       ();
    virtual QString     CREATE_MULTIPLE_INSERT_HEADER     (const QString &table_name,const QString &id_column_name, const QString &insert_column_names) = 0;
    virtual QString     CREATE_MULTIPLE_INSERT_BODY       (const QString &table_name,const QString &id_column_name, const QString &insert_column_names,const QString &insert_column_values) = 0;
    virtual QString     CREATE_MULTIPLE_INSERT_FOOTER     (const QString &table_name,const QString &id_column_name, const QString &insert_column_names) = 0;

    ADAK_SQL_STRUCT *   GET_SQL_STRUCT                    ( int program_id );
    QList<int>          GET_DB_PROGRAM_IDS                ( );

    QString             GET_CONNECTION_LAST_ERROR         ( );

    QSqlDatabase        m_db_connection;

    int                 m_sql_transaction_counter;
    int                 m_sql_active_query_counter;
    int                 m_database_index_count;
    QString             m_db_name;
    QStringList         m_sql_trigger_list;

protected:
    QStringList         CREATE_ALL_TABLES_SQL             ( ADAK_SQL_STRUCT * adak_sql_struct );
    QString             CREATE_ONE_TABLE_SQL              ( int   table_number,      ADAK_SQL_STRUCT * adak_sql_struct );
    QStringList         DROP_ALL_TABLES_SQL               ( ADAK_SQL_STRUCT * adak_sql_struct );

    QString             CLEAR_UNWANTED_CHARACTERS         ( QString string_name );

    QList<ADAK_SQL_STRUCT *> m_adak_sql_struct;

private:
    QList<int>          m_db_program_ids;



};

#endif // ADAK_SQL_ABSTRACT_H
