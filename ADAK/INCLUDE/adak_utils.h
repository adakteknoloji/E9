#ifndef ADAK_BASE_H
#define ADAK_BASE_H
    #include "adak_defines.h"

    #define             NUM_OF_ADAK_PROGRAMS      100

    class  QString;
    class  QPushButton;
    class  QByteArray;
    class  QWidget;
    class  QPixmap;
    class  QObject;

    struct ADAK_SQL_STRUCT;

    //return_empty flag i eger true sa gonderilen deger 0.00 veya 0 ise ""(empty) dondurur.
    //false olursa eger bu islevi yapmaz
    bool                FILE_EXISTS        ( const QString& file_path );
    bool                DELETE_FILE        ( const QString& file_path );



    // malloc ve free yerine MALLOC ve FREE kullanmaliyiz.
    // Bu fonksiyonlar alinan hafizayi tamamen temizliyor
    //
    // Ayrica FREE ederken de ezilip ezilmedigini kontrol ediyor
    //

    char *              MALLOC           (unsigned int request_size);
    void                FREE             (void * q);


    QString             GET_ADAK_MODULE_NAME(int modul_id);

    QPixmap             ADAK_BYTE_ARRAY_2_PIXMAP  (QByteArray byte_array);
    QByteArray          ADAK_PIXMAP_2_BYTE_ARRAY  (QString p_pixmap_path);
    bool                ADAK_ADD_PIXMAP_TO_BUTTON (QPushButton * push_button, QString * file_path, QWidget * parent);

    QString             ADAK_CREATE_TEMP_FILE (QByteArray byte_array);
    void                ADAK_REMOVE_TEMP_FILES ();

    QString             SQL_USERNAME        ();
    QString             SQL_DEFAULT_USERNAME();
    void                SET_SQL_USERNAME    (QString sql_username);

    QString             SQL_PASSWORD        ();
    QString             SQL_DEFAULT_PASSWORD();
    void                SET_SQL_PASSWORD    (QString sql_password);

    QString             SQL_HOSTNAME        ();
    void                SET_SQL_HOSTNAME    (QString sql_hostname);

    QString             SQL_MOTORU          ();
    void                SET_SQL_MOTORU      (QString sql_motoru);
    int                 SQL_PORT            ();
    void                SET_SQL_PORT        (int sql_port);
    QString             SQL_DBNAME_ONEK     ();
    void                SET_SQL_DBNAME_ONEK (QString sql_motoru);

    QString             VIRGUL_EKLE         (const QString &cevrilecek_sayi,int precision = 2,
                                             bool eksi_deger_girilsin = true ,bool return_empty = true);
    QChar               DECIMAL_SEPERATOR   ();
    QChar               THOUSAND_SEPERATOR  ();


    QString             ADAK_ISLETIM_SISTEMI     (void);
    void                SET_ADAK_VERITABANI_ID   (int adak_veritabani_id);
    void                SET_ADAK_DISPLAY_ID      (int adak_display_id);
    int                 ADAK_VERITABANI_ID       (void);
    int                 ADAK_DISPLAY_ID          (void);
    QString             ADAK_PROGRAM_SHORTNAME   (int adak_program_id);
    QString             ADAK_PROGRAM_LONGNAME    (int adak_program_id);
    QString             ADAK_PROGRAM_EXE         (int adak_program_id);
    QString             ADAK_PROGRAM_EMAIL       (int adak_program_id);
    QString             ADAK_PROGRAM_WEB         (int adak_program_id);
    QString             ADAK_PROGRAM_VERSION     (int adak_program_id);
    bool                ADAK_PROGRAM_NEED_DONGLE (void);
    QString             GET_REPORT_TOP_RIGHT_HEADER (void);

    QStringList         GET_MAC_ADDRESS    (void);
    QString             GET_DONGLE_IP      (void);

    bool                PROGRAMA_UYGUN_DONGLE_VARMI (void);


    void                UNPACK_INT_ARRAY    ( const QString& array_string  ,
                                              int * int_array              , int array_size );
    void                UNPACK_DOUBLE_ARRAY ( const QString& array_string  ,
                                              double * double_array        , int array_size );
    void                UNPACK_STRING_ARRAY ( const QString& array_string  ,
                                              QString * string_array       , int array_size );

    QByteArray          PACK_INT_ARRAY      ( int * int_array, int array_size );
    QByteArray          PACK_DOUBLE_ARRAY   ( double * double_array, int array_size );
    QByteArray          PACK_STRING_ARRAY   ( QString * str_array, int array_size );




    class QDate;

    QDate              MALI_YIL_FIRST_DATE  ();
    QDate              MALI_YIL_365_DATE    ();
    QDate              MALI_ARRAY_LAST_DATE ();
    int                MALI_YIL             ();

    // MALI YIL ILK TARIHI SETLENDIGINDE OTOMATIK OLARAK
    // MALI YIL SONTARIH VE ARRAY_LAST TARIH HESAPLANILIR
    void               SET_MALI_YIL_FIRST_DATE( QDate p_date  );

    // GONDERMIS OLDUGUNUZ TARIHIN GUN_NO SUNU YANI ARRAY_INDISINI GERI DONDERIR
    int                MALI_YIL_ARRAY_INDIS   ( QDate p_date );




    // ROUND (1234.21 , 1) ==> 1234.2
    // ROUND (1234.21 , 2) ==> 1234.21
    // ROUND (1234.21 , 3) ==> 1234.210

    double              ROUND               ( double deger, int precision = 2);
    double              ROUND_UP            ( double deger, int precision = 2);
    double              ROUND_DOWN          ( double deger, int precision = 2);

    /* Ornek :
       a=1.5021
       b=1.5021
       c=1.5021 iken
      
      
       ROUND (a+b+c) bize 4.51 
       ROUND (a) + ROUND (b) + ROUND (c) bize 4.50 verir.
     
     */

    QString             ADAK_YAZIYLA_YAZ   ( double yazdirilacak_sayi, QString para_birimi = "");



    void                ADAK_WARNING             (QString message,QObject * focus_widget, QWidget * parent);
    void                ADAK_ERROR               (QString message,QObject * focus_widget, QWidget * parent);
    void                ADAK_INFO                (QString message,QObject * focus_widget, QWidget * parent);
    ADAK_MSG_ENUM       ADAK_YES_NO              (QString message,QObject * focus_widget, QWidget * parent);
    ADAK_MSG_ENUM       ADAK_YES_NO_CANCEL       (QString message,QObject * focus_widget, QWidget * parent);


    void                ADAK_CURSOR_BUSY       ();
    void                ADAK_CURSOR_NORMAL     ();

    void                ADAK_SET_FONT_SIZE     (QWidget * widget, double oran);

#endif // ADAK_BASE_H
