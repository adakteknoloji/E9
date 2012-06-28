#ifndef ADAK_SQL_QUERY_H
#define ADAK_SQL_QUERY_H

#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QSqlRecord>

class ADAK_SQL;

class QSqlRecord;

enum ADAK_QUERY_SPLITTER_TYPES {
    ADAK_OR  =   1,
    ADAK_AND =   2
};

//
//  Class ' in amaci ; kullaniclarin girdigi her turlu veri ile calisabilmek. Ozellikle ' , " , \ gibi karakterler girildiginde
//  SQL sorgusu bozulur. Bu yapi sayesinde bozulma olamaz.
//

class SQL_QUERY : private QSqlQuery
{
    public :
        // Bir SQL sorgusu yaratmak icin kullanilir.
        //
        // Ornek : SQL_QUERY query (E9_DB);
                       SQL_QUERY        (ADAK_SQL * XXX_DB);
                      ~SQL_QUERY ();


        // SQL ' de INSERT islemi yapmak icin kullanilir.
        // id_column_name : Tablonun PRIMARY_KEY olan xxxx_id ' si olmalidir.
        // query          : ODBC standartlarinda bir SQL sorgusu olmalidir.
        //
        // Ornek :  SQL_QUERY  query (E9_DB);
        //          query.PREPARE_INSERT ("hesap_id", "INSERT INTO muhasebe_hesaplari "
        //                                            "( hesap_kodu,   hesap_ismi,   hesap_seviyesi)
        //                                     "VALUES (:hesap_kodu,  :hesap_ismi,  :hesap_seviyesi)";
        //
        // Degiskenlerin degerleri burada verilmeyecektir. Sadece :hesap_kodu seklinde belirtilecektir. Degerler
        // daha sonra SET_VALUE ile verilecektir.


        // PREPARE_INSERT ve SET_VALUE'larla hazirlanmis islemi yaptirir
        // Donus degeri olarak PREPARE_INSERT te verilmis PRIMARY_KEY olan ID ' yi dondurur.
        //
        // Ornek : hesap_id = query.INSERT ();


        void             PREPARE_INSERT       (const QString& table_name,const QString& primary_key , const QString& insert_column_names);
        int              INSERT               (void);


        // PREPARE_INSERT ile aynilar
        void             PREPARE_UPDATE       (const QString& table_name,const QString& primary_key , const QString& update_column_names , const QString& where_condition);
        void             UPDATE               (void);



        // PREPARE_DELETE ve SET_VALUE'larla hazirlanmis islemi yaptirir
        void             PREPARE_DELETE       (const QString& table_name, const QString& where_condition);
        void             DELETE               (void);

        // PREPARE_SELECT ve SET_VALUE'larla hazirlanmis islemi yaptirir
        // Donus degeri bulunan KAYIT_SAYISI ' dir
        //
        // Ornek : if (query.SELECT() EQ 0) {
        //             return;
        //         }
        //
        void             PREPARE_SELECT       (const QString& table_name, const QString& column_names);
        void             PREPARE_SELECT       (const QString& table_name, const QString& column_names, const QString& where_condition);
        void             PREPARE_SELECT       (const QString& table_name, const QString& column_names, const QString& where_condition , const QString& group_by);
        int              SELECT               (void);
        int              SELECT               (const QString& order_by);
        int              SELECT               (const QString& order_by, int baslanilacak_satir , int secilecek_satir_sayisi);




        // PREPARE_INSERT, PREPARE_UPDATE, PREPARE_DELETE ve PREPARE_SELECT icin gonderilen sorgu dizisindeki
        // degiskenlerin degerlerini atamak icin kullanilir.
        //
        // Ornek : query.SET_VALUE (":hesap_kodu", hesap_kodu);
        //         query.SET_VALUE (":hesap_ismi", hesap_ismi);
        //         query.SET_VALUE (":hesap_seviyesi, 1);

        void             SET_LIKE             (const QString& definition, const QVariant& value);

        void             SET_VALUE            (const QString& definition, const QVariant& value);
        void             SET_VALUE            (const int insert_sira, const QVariant& value);

        // Sorguyu bitirip veritabanini OZGUR birakir. Bazi SQL ler ozgur birakilmadigi surece COMMIT etmezler.
        //
        // Ornek : query.FINISH();
        void             FINISH           (void);


        // Sorgu bir SELECT ise kac satir bilgi oldugunu gosterir. Bilgi bulunamadiysa 0 dondurur.
        //
        // Ornek : query.NUM_OF_ROWS ();
        int              NUM_OF_ROWS      (void);


        // Sorguda bulunan kayitlari sirasi ile almak icin kullanilir.
        // Kayit varsa "true" , kalmadiya "false" dondurur.
        // Once NUM_OF_ROWS() ile kayit bulunup bulunmadigi kontrol edilmelidir.
        bool             NEXT             (void);


        // NEXT islemi icin sayaci sifirlar , tekrar ilk kayitttan baslatir
        //
        // Ornek : query.RESTART_NEXT();
        void             RESTART_NEXT     (void);


        // Sorguda bulunan rowlarin sutunlarina erismek icin kullanilir.
        //
        // Ilk sutun 0 nolu sutundur. Sonra 1,2,3,.. vs
        QVariant         VALUE (int sutun_no);

        // Sorguda bulunan rowlarin sutunlarina erismek icin kullanilir.
        //
        // Sutun numarasi yerine degisken_ismi ile erismeyi saglar.
        //
        // DIKKAT !..
        //       Degisken ismi ile erismek sutun_no ile erismeye gore cok YAVASTIR.
        QVariant         VALUE (const QString& column_name);





        //////////////////////

        void             PARANTEZ_AC                     ();
        void             PARANTEZ_KAPA                   ();

        void             AND_EKLE                        ( QString where_string );
        void             OR_EKLE                         ( QString where_string );

        void             AND_MULTI_EKLE                  ( QString column_id_name, QList<QVariant> values, ADAK_QUERY_SPLITTER_TYPES splitter);
        void             OR_MULTI_EKLE                   ( QString column_id_name, QList<QVariant> values, ADAK_QUERY_SPLITTER_TYPES splitter);

        QList<QVariant>  TO_QVARIANT                     (QList<int> values);
        QList<QVariant>  TO_QVARIANT                     (QStringList values);

    protected:
        ADAK_SQL    *   m_db;

        bool             EXEC                     (const QString& query);
        bool             EXEC                     (void);

    private :
        int              p_struct_id;
        int              p_table_id;

        int              TRY_TO_GET_ID_USING_QT ();

        void             BIND_ALL_VALUES();

        void             ADD_MULTI                        (QString column_id_name, QList<QVariant> values, ADAK_QUERY_SPLITTER_TYPES splitter );

        int              m_current_sql_operation;

        QStringList      m_bind_values_names;
        QList<QVariant>  m_bind_values;

        QSqlRecord       p_record;

        QString          m_query;
        QString          m_table_name;
        QString          m_data_column_names;
        QString          m_data_column_values;
        QString          m_id_column_name;
        QString          m_group_by;
        QString          m_where_condition;
};

#endif // ADAK_SQL_QUERY_H
