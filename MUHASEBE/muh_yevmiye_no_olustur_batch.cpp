#include "muh_yevmiye_no_olustur_batch_class.h"
#include "muh_yevmiye_no_olustur_batch_open.h"
#include "ui_muh_yevmiye_no_olustur_batch.h"
#include "e9_log.h"
#include "e9_console_utils.h"
#include "muh_enum.h"
#include "e9_gui_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_MUH_YEVMIYE_NO_OLUSTUR_BATCH
***************************************************************************************/

void OPEN_MUH_YEVMIYE_NO_OLUSTUR_BATCH ( QWidget * parent )
{
    MUH_YEVMIYE_NO_OLUSTUR_BATCH * B = new MUH_YEVMIYE_NO_OLUSTUR_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::MUH_YEVMIYE_NO_OLUSTUR_BATCH
***************************************************************************************/

MUH_YEVMIYE_NO_OLUSTUR_BATCH::MUH_YEVMIYE_NO_OLUSTUR_BATCH ( QWidget *parent ) : BATCH_KERNEL ( parent ), m_ui(new Ui::MUH_YEVMIYE_NO_OLUSTUR_BATCH)
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_YEVMIYE_NO_OLUSTUR_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE  ( "genel-muhasebe_yevmiye-numaralari-olustur" );
    SET_PAGE_TITLE ( tr ( "MUH - YEVMİYE NUMARALARINI OLUŞTUR" ) );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Oluştur" ) );
    SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_bas_tarihi );

    son_yevmiye_numarasi = -1;

    QDate rapor_baslangic_tarihi = SON_YEVMIYE_TARIHI();
    QDate bitis_tarihi           = MALI_YIL_FIRST_DATE() ;

    m_ui->adakDate_bas_tarihi->SET_DATE(rapor_baslangic_tarihi );
    m_ui->adakDate_bitis_tarihi->SET_DATE( bitis_tarihi );

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("muh_fisler" , "yevmiye_numarasi,fis_id,fis_tarihi" , "yevmiye_numarasi > :yevmiye_numarasi");
    query.SET_VALUE(":yevmiye_numarasi" , 0);

    if ( query.SELECT("yevmiye_numarasi DESC" , 0 , 1) EQ 0 ) {
        m_bas_date = m_ui->adakDate_bas_tarihi->QDATE();
        return;
    }
    query.NEXT();

    son_yevmiye_numarasi = query.VALUE(0).toInt();

    m_ui->line_edit_yevmiye_no->setText ( QVariant ( son_yevmiye_numarasi ).toString() );
    m_ui->lineEdit_olusturulan_son_fis->setText ( query.VALUE(1).toString() );

    m_ui->adakDate_bas_tarihi->SET_DATE( ( QDate::fromString ( query.VALUE(2).toString() , "yyyy.MM.dd" ) ).addDays(1) );

    m_bas_date = m_ui->adakDate_bas_tarihi->QDATE();

    son_yevmiye_numarasi++;

    SET_SETTING_NAME        ( "MUH_YEVMIYE_NO_OLUSTUR_BATCH" );
}

/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_YEVMIYE_NO_OLUSTUR_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED( object );
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_YEVMIYE_NO_OLUSTUR_BATCH::CHECK_RUN()
{

    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_bas_tarihi, m_ui->adakDate_bitis_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }

    int msg_secim = MSG_YES_NO( tr ( "Bu işlem uzun sürebilir,devam edilsin mi?" ), NULL );

    if ( msg_secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_YEVMIYE_NO_OLUSTUR_BATCH::RUN_BATCH()
{
    bool yevmiye_numaralari_olusturuldu = false;
    QString son_olusturulan_fis_bilgileri;


    DB->START_TRANSACTION();

    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("muh_fisler" , "fis_id, fis_tarihi, fis_turu, yevmiye_numarasi, yvm_defterine_basildi",
                             "fis_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi ");

    sql_query.SET_VALUE      ( ":baslangic_tarihi", m_ui->adakDate_bas_tarihi->DATE() );
    sql_query.SET_VALUE      ( ":bitis_tarihi"    , m_ui->adakDate_bitis_tarihi->DATE() );

    if ( son_yevmiye_numarasi EQ -1 ) {
        son_yevmiye_numarasi = ACILIS_FISINE_NUMARA_VER();
    }
    else if ( m_ui->adakDate_bas_tarihi->QDATE() NE m_bas_date ) {
        SQL_QUERY query             ( DB );
        SQL_QUERY acilis_fisi_query ( DB );

        query.PREPARE_SELECT("muh_fisler" , "yevmiye_numarasi" , "fis_tarihi < :baslangic_tarihi");

        query.SET_VALUE ( ":baslangic_tarihi", m_ui->adakDate_bas_tarihi->DATE() );

        if ( query.SELECT("yevmiye_numarasi DESC" , 0, 1) NE 0 ) {
            query.NEXT();
            son_yevmiye_numarasi = query.VALUE(0).toInt();
            son_yevmiye_numarasi++;
        }
        else {
            son_yevmiye_numarasi = 1;
        }

        query.PREPARE_SELECT("muh_fisler" , "fis_id" , "(fis_tarihi >= :baslangic_tarihi AND "
                             "fis_tarihi <= :bitis_tarihi) AND fis_turu = :fis_turu ");

        query.SET_VALUE (  ":baslangic_tarihi", m_ui->adakDate_bas_tarihi->DATE()  );
        query.SET_VALUE (  ":bitis_tarihi"    , m_ui->adakDate_bitis_tarihi->DATE() );
        query.SET_VALUE (  ":fis_turu"        , ENUM_ACILIS_FISI );

        if ( query.SELECT("yevmiye_numarasi ASC" , 0 , 1) NE 0) {
            son_yevmiye_numarasi = 1;
            while ( query.NEXT() ) {

                acilis_fisi_query.PREPARE_UPDATE (  "muh_fisler","fis_id", "yevmiye_numarasi", "fis_id = :fis_id");
                acilis_fisi_query.SET_VALUE      (  ":yevmiye_numarasi",son_yevmiye_numarasi );
                acilis_fisi_query.SET_VALUE      (  ":fis_id"          ,query.VALUE(0).toInt() );
                acilis_fisi_query.UPDATE();
                son_yevmiye_numarasi++;
            }
        }
    }



    if ( sql_query.SELECT("fis_tarihi ASC") EQ 0 ) {
        DB->CANCEL_TRANSACTION();
        MSG_WARNING(  tr ( "Uygun fiş bulunamadı" ), NULL );
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        int     fis_id                = sql_query.VALUE(0).toInt();
        QString fis_tarihi            = sql_query.VALUE(1).toString();
        int     fis_turu              = sql_query.VALUE(2).toInt();
        int     yevmiye_numarasi      = sql_query.VALUE(3).toInt();
        int     yev_defterine_basildi = sql_query.VALUE(4).toInt();

        if ( QString::compare ( fis_tarihi ,m_ui->adakDate_bitis_tarihi->DATE() ) > 0  ) {
            break;
        }
        if ( yevmiye_numarasi NE 0 AND yev_defterine_basildi EQ 1 ) {
            continue;
        }
        if ( fis_turu EQ ENUM_ACILIS_FISI OR fis_turu EQ ENUM_KAPANIS_FISI ) {
            continue;
        }

        QString fis_no     = sql_query.VALUE(0).toString();
        QDate fisin_tarihi = QDate::fromString ( sql_query.VALUE(1).toString(),"yyyy.MM.dd" );

        son_olusturulan_fis_bilgileri = QString ( "Fiş No: " ) + fis_no;
        son_olusturulan_fis_bilgileri+= QString ( " Fiş Tarihi : " ) + fisin_tarihi.toString ( "dd.MM.yyyy" );

        SQL_QUERY update_query      ( DB );


        update_query.PREPARE_UPDATE ( "muh_fisler","fis_id","yevmiye_numarasi", "fis_id = :fis_id" );
        update_query.SET_VALUE      ( ":yevmiye_numarasi",son_yevmiye_numarasi );
        update_query.SET_VALUE      ( ":fis_id",fis_id );
        update_query.UPDATE();
        son_yevmiye_numarasi++;
        yevmiye_numaralari_olusturuldu = true;

        switch ( fis_turu ) {
            case ENUM_MAHSUP_FISI  :
                son_olusturulan_fis_bilgileri += " Fiş Türü: Mahsup Fişi";
                break;
            case ENUM_TAHSIL_FISI :
                son_olusturulan_fis_bilgileri += " Fiş Türü: Tahsil Fişi";
                break;
            case ENUM_TEDIYE_FISI  :
                son_olusturulan_fis_bilgileri += " Fiş Türü: Tediye Fişi";
                break;
            default :
                break;
        }
    }
    m_ui->line_edit_yevmiye_no->setText ( QVariant ( (son_yevmiye_numarasi - 1 ) ).toString() );
    m_ui->lineEdit_olusturulan_son_fis->setText ( son_olusturulan_fis_bilgileri );

    sql_query.PREPARE_SELECT("muh_fisler" , "fis_id" , "(fis_tarihi BETWEEN "
                             ":baslangic_tarihi AND :bitis_tarihi) AND fis_turu = :fis_turu ");

    sql_query.SET_VALUE      ( ":fis_turu"          , ENUM_KAPANIS_FISI );
    sql_query.SET_VALUE      ( ":baslangic_tarihi"  , m_ui->adakDate_bas_tarihi->DATE()  );
    sql_query.SET_VALUE      ( ":bitis_tarihi"      , m_ui->adakDate_bitis_tarihi->DATE()  );

    if ( sql_query.SELECT() NE 0 ) {
        KAPANIS_FISINE_NUMARA_VER();
    }

    QString log_detaylari = QObject::tr ( "Yevmiye Numaraları Oluşturuldu , \n" ) +
                            QObject::tr ( "Tarih Aralığı    : " )  +
                            m_ui->adakDate_bas_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                            m_ui->adakDate_bitis_tarihi->QDATE().toString("dd MMMM yyyy") ;

    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_YVM_NO_OLUSTUR,LOG_ISLEM_UPDATE,log_detaylari );
    DB->COMMIT_TRANSACTION();

    if ( yevmiye_numaralari_olusturuldu EQ true ) {
        MSG_INFO( tr ( "Yevmiye numaraları oluşturuldu!.." ), NULL );
    }

}


/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::SON_YEVMIYE_TARIHI
***************************************************************************************/

QDate MUH_YEVMIYE_NO_OLUSTUR_BATCH::SON_YEVMIYE_TARIHI()
{
    QDate baslangic_tarihi ( MALI_YIL_FIRST_DATE() );
    QDate bitis_tarihi     ( MALI_YIL_FIRST_DATE() );

    SQL_QUERY sql_query      ( DB );

    sql_query.PREPARE_SELECT("muh_fisler" , "fis_tarihi, yevmiye_numarasi, yvm_defterine_basildi", "fis_tarihi <= :fis_tarihi");

    sql_query.SET_VALUE      ( ":fis_tarihi"       , bitis_tarihi.toString ( "yyyy.MM.dd" ) );


    if( sql_query.SELECT() EQ 0 ) {
        return baslangic_tarihi;
    }

    while ( sql_query.NEXT() EQ true ) {
        QString fis_tarihi        = sql_query.VALUE(0).toString();
        int yevmiye_numarasi      = sql_query.VALUE(1).toInt();
        int yev_defterine_basildi = sql_query.VALUE(2).toInt();

        if ( yevmiye_numarasi EQ 0 ) {
            continue;
        }
        if ( yev_defterine_basildi EQ 1 ) {
            baslangic_tarihi = QDate::fromString ( fis_tarihi,"yyyy.MM.dd" );
            break;
        }

    }

    return baslangic_tarihi;
}

/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::ACILIS_FISINE_NUMARA_VER
***************************************************************************************/

int MUH_YEVMIYE_NO_OLUSTUR_BATCH::ACILIS_FISINE_NUMARA_VER()
{
    int acilis_fisi_numarasi = 1;

    SQL_QUERY sql_query      ( DB );
    SQL_QUERY update_query   ( DB );

    sql_query.PREPARE_SELECT("muh_fisler" , "fis_id" , "fis_turu = :fis_turu ");

    sql_query.SET_VALUE      ( ":fis_turu"         , ENUM_ACILIS_FISI );


    if ( sql_query.SELECT() EQ 0 ) {
        return acilis_fisi_numarasi;
    }

    while ( sql_query.NEXT() EQ true ) {
        int fis_id = sql_query.VALUE ( "fis_id" ).toInt();

        update_query.PREPARE_UPDATE  ( "muh_fisler","fis_id", "yevmiye_numarasi", "fis_id = :fis_id");
        update_query.SET_VALUE       ( ":yevmiye_numarasi",acilis_fisi_numarasi );
        update_query.SET_VALUE       ( ":fis_id"          ,fis_id               );
        update_query.UPDATE();
        acilis_fisi_numarasi++;
    }
    return acilis_fisi_numarasi;
}

/**************************************************************************************
                   MUH_YEVMIYE_NO_OLUSTUR_BATCH::KAPANIS_FISINE_NUMARA_VER
***************************************************************************************/

void MUH_YEVMIYE_NO_OLUSTUR_BATCH::KAPANIS_FISINE_NUMARA_VER()
{
    QDate bitis_tarihi ( MALI_YIL_FIRST_DATE() );

        SQL_QUERY sql_query      ( DB );
        SQL_QUERY update_query   ( DB );

        sql_query.PREPARE_SELECT("muh_fisler" , "fis_id ,fis_turu" , "fis_tarihi = :bitis_tarihi  AND fis_turu = :fis_turu");

        sql_query.SET_VALUE      ( ":bitis_tarihi"     , bitis_tarihi.toString ( "yyyy.MM.dd") );
        sql_query.SET_VALUE      ( ":fis_turu"         , ENUM_KAPANIS_FISI );

        if ( sql_query.SELECT() EQ 0 ) {
            return;
        }

        while ( sql_query.NEXT() EQ true ) {
            int fis_id = sql_query.VALUE(0).toInt();
            update_query.PREPARE_UPDATE ( "muh_fisler", "fis_id" , "yevmiye_numarasi","fis_id = :fis_id " );
            update_query.SET_VALUE      ( ":yevmiye_numarasi", son_yevmiye_numarasi );
            update_query.SET_VALUE      ( ":fis_id", fis_id );
            update_query.UPDATE();
        }
}
