#include "adak_utils.h"
#include "stok_urun_birimleri_batch_class.h"
#include "ui_stok_urun_birimleri_batch.h"
 
 
#include "e9_log.h"

#include "stok_enum.h"

extern ADAK_SQL * DB;


/**************************************************************************************
                   OPEN_STOK_URUN_BIRIMLERI_BATCH
***************************************************************************************/


void OPEN_STOK_URUN_BIRIMLERI_BATCH ( int p_urun_id, QWidget * p_parent )
{
    STOK_URUN_BIRIMLERI_BATCH * urun_birimleri = new STOK_URUN_BIRIMLERI_BATCH ( p_urun_id, p_parent );
    urun_birimleri->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::STOK_URUN_BIRIMLERI_BATCH
***************************************************************************************/

STOK_URUN_BIRIMLERI_BATCH::STOK_URUN_BIRIMLERI_BATCH( int p_urun_id, QWidget * p_parent) :  BATCH_KERNEL (p_parent), m_ui(new Ui::STOK_URUN_BIRIMLERI_BATCH)
{
    m_urun_id = p_urun_id;

    m_ui->setupUi                   ( this );
    START_BATCH_KERNEL              ( this, DB );
}

/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::SETUP_FORM
***************************************************************************************/

void STOK_URUN_BIRIMLERI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ("stok_urun_birimleri.html");
    SET_PAGE_TITLE      ( tr ( "STK - ÜRÜN BİRİMLERİ" ) );

    SET_AUTO_EXIT_BATCH             ( true );

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME    ( "STOK_URUN_BIRIMLERI_BATCH" );

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunler","urun_temel_birimi","urun_id = :urun_id");
    sql_query.SET_VALUE(":urun_id", m_urun_id );
    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Stok id bulunamadı"), NULL );
        return;
    }
    sql_query.NEXT();
    m_temel_urun_birimi = sql_query.VALUE(0).toString();

    SET_WIDGET_ARRAYS               ();
    URUN_BIRIMI_BILGILERINI_OKU     ();
    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr("Kaydet") );
    SET_FIRST_FOCUS_WIDGET          ( m_ui->lineEdit_birim_adi_1 );
    SET_ENTER_KEY_FOR_RUN_BATCH     ( true );
}

/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::CHECK_VAR
***************************************************************************************/

int STOK_URUN_BIRIMLERI_BATCH::CHECK_VAR ( QObject * object )
{
    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( object EQ M_URUN_BIRIM_ADI_LINEEDIT[i] ) {

            if ( M_URUN_BIRIM_ADI_LINEEDIT[i]->text() EQ m_temel_urun_birimi ) {
                MSG_WARNING(  tr("Bu birim temel stok birimi olarak tanımlanmış. "
                                                            "Farklı bir stok birimi tanımlamalısınız."), NULL );
                return ADAK_FAIL;
            }
            FILL_ALT_BIRIM_COMBOBOXES();
            return ADAK_OK;
        }
        else if ( object EQ M_URUN_BIRIM_KATSAYI_COMMAEDIT[i]
                  OR object EQ M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]
                  OR object EQ M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[i]
                  OR object EQ M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[i]
                  OR object EQ M_URUN_KARTINDA_GOSTER_CHECKBOX[i] ) {
            if ( M_URUN_BIRIM_ADI_LINEEDIT[i]->text().isEmpty() EQ true ) {
                MSG_WARNING(  tr( "%n nolu satır için önce Birim Adı bilgisini girmelisiniz.", "", i +1 ), M_URUN_BIRIM_ADI_LINEEDIT[ i ] );

                return ADAK_FAIL;
            }
        }
        else {
            if ( object EQ M_URUN_BIRIMI_TEMIZLE_BUTTON[i] ) {
                M_URUN_BIRIM_ADI_LINEEDIT[i]->blockSignals(true);
                M_URUN_BIRIM_KATSAYI_COMMAEDIT[i]->blockSignals(true);
                M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->blockSignals(true);
                M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[i]->blockSignals(true);
                M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[i]->blockSignals(true);
                M_URUN_KARTINDA_GOSTER_CHECKBOX[i]->blockSignals(true);

                M_URUN_BIRIM_ADI_LINEEDIT[i]->clear();
                M_URUN_BIRIM_KATSAYI_COMMAEDIT[i]->clear();
                M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->setCurrentIndex(0);
                M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[i]->clear();
                M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[i]->clear();
                M_URUN_KARTINDA_GOSTER_CHECKBOX[i]->setChecked(false);

                M_URUN_BIRIM_ADI_LINEEDIT[i]->blockSignals(false);
                M_URUN_BIRIM_KATSAYI_COMMAEDIT[i]->blockSignals(false);
                M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->blockSignals(false);
                M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[i]->blockSignals(false);
                M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[i]->blockSignals(false);
                M_URUN_KARTINDA_GOSTER_CHECKBOX[i]->blockSignals(false);

            }
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::CHECK_RUN
***************************************************************************************/

int STOK_URUN_BIRIMLERI_BATCH::CHECK_RUN()
{
    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( M_URUN_BIRIM_ADI_LINEEDIT[i]->text().isEmpty() EQ false  ) {
            if ( i > 0 ) {
                if ( M_URUN_BIRIM_ADI_LINEEDIT[i-1]->text().isEmpty() EQ true ) {
                    MSG_WARNING(  tr("%n nolu satır boş bırakılmış. Satır atlayarak ürün birimi kaydı yapamazsınız. Sırayla kayıt yapmalısınız.", "", i ), M_URUN_BIRIM_ADI_LINEEDIT[ i -1 ] );

                    return ADAK_FAIL;
                }
            }
            if ( M_URUN_BIRIM_KATSAYI_COMMAEDIT[i]->GET_DOUBLE() EQ 0.0 ) {
                MSG_WARNING(  tr( "%n nolu satırda katsayı bilgisi girilmemiş.","", i + 1 ), M_URUN_BIRIM_KATSAYI_COMMAEDIT[i] );

                return ADAK_FAIL;
            }
            if ( M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->currentText().isEmpty() EQ true ) {
                MSG_WARNING(  tr( "%n nolu satırda alt birim bilgisi girilmemiş.","", i + 1 ), M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[ i ] );

                return ADAK_FAIL;
            }
//            if ( M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[ i ]->GET_DOUBLE() EQ 0.0 ) {
//                MSG_WARNING(  tr( "%n nolu satırda alış fiyatı bilgisi girilmemiş.","", i + 1 ), M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[ i ] );

//                return ADAK_FAIL;
//            }
//            if ( M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[ i ]->GET_DOUBLE() EQ 0.0 ) {
//                MSG_WARNING(  tr( "%n nolu satırda satış fiyatı bilgisi girilmemiş.","", i + 1 ), M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[ i ] );

//                return ADAK_FAIL;
//            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::RUN_BATCH
***************************************************************************************/

void STOK_URUN_BIRIMLERI_BATCH::RUN_BATCH()
{
    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {

        M_URUN_BIRIM_ADLARI         [i] = M_URUN_BIRIM_ADI_LINEEDIT[i]->text();
        M_URUN_BIRIM_KATSAYILARI    [i] = M_URUN_BIRIM_KATSAYI_COMMAEDIT[i]->GET_DOUBLE();
        M_URUN_BIRIM_ALIS_FIYATLARI [i] = M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[i]->GET_DOUBLE();
        M_URUN_BIRIM_SATIS_FIYATLARI[i] = M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[i]->GET_DOUBLE();
        M_GORUNECEK_URUN_BIRIMLERI  [i] = QVariant( M_URUN_KARTINDA_GOSTER_CHECKBOX[i]->isChecked()).toInt();
        M_URUN_CAPRAZ_BIRIMLER      [i] = M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->currentIndex();
    }

    QString     urun_birim_adlari_str               = PACK_STRING_ARRAY ( M_URUN_BIRIM_ADLARI,          URUN_BIRIMLERI_SAYISI );
    QString     urun_birim_katsayilari_str          = PACK_DOUBLE_ARRAY ( M_URUN_BIRIM_KATSAYILARI,     URUN_BIRIMLERI_SAYISI );
    QString     urun_capraz_birimler_str            = PACK_INT_ARRAY    ( M_URUN_CAPRAZ_BIRIMLER,       URUN_BIRIMLERI_SAYISI );
    QString     urun_birim_alis_fiyatlar_str       = PACK_DOUBLE_ARRAY ( M_URUN_BIRIM_ALIS_FIYATLARI,  URUN_BIRIMLERI_SAYISI );
    QString     urun_birim_sts_fiyatlar_str      = PACK_DOUBLE_ARRAY ( M_URUN_BIRIM_SATIS_FIYATLARI, URUN_BIRIMLERI_SAYISI );
    QString     goruntulenecek_urun_birimleri_str   = PACK_INT_ARRAY    ( M_GORUNECEK_URUN_BIRIMLERI,   URUN_BIRIMLERI_SAYISI );



    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi","urun_id = :urun_id");
    sql_query.SET_VALUE(":urun_id" , m_urun_id );

    QString urun_kodu;
    QString urun_adi;

    if(sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        urun_kodu = sql_query.VALUE(0).toString();
        urun_adi  = sql_query.VALUE(1).toString();
    }

    QString log_detaylari = "Ürün Kodu / Adı : " + urun_kodu + " " + urun_adi;

    DB->START_TRANSACTION();

    E9_LOG_KAYDI_EKLE(STOK_MODULU , LOG_STK_URUN_BIRIMLERI , -1 , log_detaylari);


    sql_query.PREPARE_UPDATE("stk_urunler","stk_urunler","urun_birimleri_array, "
                             "urun_birim_katsayilari_array                , "
                             "urun_capraz_birimler_array               , "
                             "urun_birim_alis_fiyatlar_array           , "
                             "urun_birim_sts_fiyatlar_array            , "
                             "gorunecek_urn_birimleri_array            ","urun_id = :urun_id");

    sql_query.SET_VALUE ( ":urun_birimleri_array",              urun_birim_adlari_str               );
    sql_query.SET_VALUE ( ":urun_birim_katsayilari_array",      urun_birim_katsayilari_str          );
    sql_query.SET_VALUE ( ":urun_capraz_birimler_array",        urun_capraz_birimler_str            );
    sql_query.SET_VALUE ( ":urun_birim_alis_fiyatlar_array",   urun_birim_alis_fiyatlar_str       );
    sql_query.SET_VALUE ( ":urun_birim_sts_fiyatlar_array",  urun_birim_sts_fiyatlar_str      );
    sql_query.SET_VALUE ( ":gorunecek_urn_birimleri_array",     goruntulenecek_urun_birimleri_str   );
    sql_query.SET_VALUE ( ":urun_id",                           m_urun_id                           );

    sql_query.UPDATE();

    DB->COMMIT_TRANSACTION();
}


/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::SET_WIDGET_ARRAYS
***************************************************************************************/

void STOK_URUN_BIRIMLERI_BATCH::SET_WIDGET_ARRAYS ()
{
    M_URUN_BIRIM_ADI_LINEEDIT               = new QLineEdit * [URUN_BIRIMLERI_SAYISI];

    M_URUN_BIRIM_ADI_LINEEDIT[0]            = m_ui->lineEdit_birim_adi_1;
    M_URUN_BIRIM_ADI_LINEEDIT[1]            = m_ui->lineEdit_birim_adi_2;
    M_URUN_BIRIM_ADI_LINEEDIT[2]            = m_ui->lineEdit_birim_adi_3;
    M_URUN_BIRIM_ADI_LINEEDIT[3]            = m_ui->lineEdit_birim_adi_4;

    M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX        = new QComboBox * [URUN_BIRIMLERI_SAYISI];

    M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[0]     = m_ui->comboBox_alt_birim_1;
    M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[1]     = m_ui->comboBox_alt_birim_2;
    M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[2]     = m_ui->comboBox_alt_birim_3;
    M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[3]     = m_ui->comboBox_alt_birim_4;

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->setCurrentIndex(-1);
    }

    M_URUN_BIRIM_KATSAYI_COMMAEDIT          = new QCommaEdit * [URUN_BIRIMLERI_SAYISI];

    M_URUN_BIRIM_KATSAYI_COMMAEDIT[0]       = m_ui->commaEdit_katsayi_1;
    M_URUN_BIRIM_KATSAYI_COMMAEDIT[1]       = m_ui->commaEdit_katsayi_2;
    M_URUN_BIRIM_KATSAYI_COMMAEDIT[2]       = m_ui->commaEdit_katsayi_3;
    M_URUN_BIRIM_KATSAYI_COMMAEDIT[3]       = m_ui->commaEdit_katsayi_4;


    M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT      = new QCommaEdit * [URUN_BIRIMLERI_SAYISI];

    M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[0]   = m_ui->commaEdit_alis_fiyati_1;
    M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[1]   = m_ui->commaEdit_alis_fiyati_2;
    M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[2]   = m_ui->commaEdit_alis_fiyati_3;
    M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[3]   = m_ui->commaEdit_alis_fiyati_4;

    M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT     = new QCommaEdit * [URUN_BIRIMLERI_SAYISI];

    M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[0]  = m_ui->commaEdit_satis_fiyati_1;
    M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[1]  = m_ui->commaEdit_satis_fiyati_2;
    M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[2]  = m_ui->commaEdit_satis_fiyati_3;
    M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[3]  = m_ui->commaEdit_satis_fiyati_4;

    M_URUN_KARTINDA_GOSTER_CHECKBOX         = new QCheckBox * [URUN_BIRIMLERI_SAYISI];

    M_URUN_KARTINDA_GOSTER_CHECKBOX[0]      = m_ui->checkBox_urun_kartinda_goster_1;
    M_URUN_KARTINDA_GOSTER_CHECKBOX[1]      = m_ui->checkBox_urun_kartinda_goster_2;
    M_URUN_KARTINDA_GOSTER_CHECKBOX[2]      = m_ui->checkBox_urun_kartinda_goster_3;
    M_URUN_KARTINDA_GOSTER_CHECKBOX[3]      = m_ui->checkBox_urun_kartinda_goster_4;

    M_URUN_BIRIMI_TEMIZLE_BUTTON            = new QPushButton * [URUN_BIRIMLERI_SAYISI];

    M_URUN_BIRIMI_TEMIZLE_BUTTON[0]         = m_ui->button_temizle_1;
    M_URUN_BIRIMI_TEMIZLE_BUTTON[1]         = m_ui->button_temizle_2;
    M_URUN_BIRIMI_TEMIZLE_BUTTON[2]         = m_ui->button_temizle_3;
    M_URUN_BIRIMI_TEMIZLE_BUTTON[3]         = m_ui->button_temizle_4;

}

/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::FILL_ALT_BIRIM_COMBOBOXES
***************************************************************************************/

void STOK_URUN_BIRIMLERI_BATCH::FILL_ALT_BIRIM_COMBOBOXES()
{

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        int current_comboBox_index = M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->currentIndex();
        if ( current_comboBox_index EQ -1 ) {
            current_comboBox_index = 0;
        }
        M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->blockSignals(true);
        M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->clear();
        M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->addItem(m_temel_urun_birimi);
        for ( int j = 0; j < i; j++ ) {
            if ( M_URUN_BIRIM_ADI_LINEEDIT[j]->text().isEmpty() EQ false ) {
                M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->addItem(M_URUN_BIRIM_ADI_LINEEDIT[j]->text());

            }
        }
        if ( M_URUN_BIRIM_ADI_LINEEDIT[i]->text().isEmpty() EQ false ) {
            M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->setCurrentIndex(current_comboBox_index);
        }
        M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->blockSignals(false);
    }
}

/**************************************************************************************
                   STOK_URUN_BIRIMLERI_BATCH::URUN_BIRIMI_BILGILERINI_OKU
***************************************************************************************/


void STOK_URUN_BIRIMLERI_BATCH::URUN_BIRIMI_BILGILERINI_OKU ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunler","urun_birimleri_array, urun_birim_katsayilari_array, "
                             "urun_capraz_birimler_array, urun_birim_alis_fiyatlar_array, "
                             "urun_birim_sts_fiyatlar_array, gorunecek_urn_birimleri_array ",
                             "urun_id = :urun_id");

    sql_query.SET_VALUE ( ":urun_id", m_urun_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    QString     urun_birim_adlari_str               = sql_query.VALUE(0).toString();
    QString     urun_birim_katsayilari_str          = sql_query.VALUE(1).toString();
    QString     urun_capraz_birimler_str            = sql_query.VALUE(2).toString();
    QString     urun_birim_alis_fiyatlar_str       = sql_query.VALUE(3).toString();
    QString     urun_birim_sts_fiyatlar_str      = sql_query.VALUE(4).toString();
    QString     goruntulenecek_urun_birimleri_str   = sql_query.VALUE(5).toString();

    M_URUN_BIRIM_ADLARI                             = new QString [URUN_BIRIMLERI_SAYISI];
    M_URUN_BIRIM_KATSAYILARI                        = new double  [URUN_BIRIMLERI_SAYISI];
    M_URUN_CAPRAZ_BIRIMLER                          = new int     [URUN_BIRIMLERI_SAYISI];
    M_URUN_BIRIM_ALIS_FIYATLARI                     = new double  [URUN_BIRIMLERI_SAYISI];
    M_URUN_BIRIM_SATIS_FIYATLARI                    = new double  [URUN_BIRIMLERI_SAYISI];
    M_GORUNECEK_URUN_BIRIMLERI                      = new int     [URUN_BIRIMLERI_SAYISI];

    UNPACK_STRING_ARRAY ( urun_birim_adlari_str,             M_URUN_BIRIM_ADLARI,            URUN_BIRIMLERI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( urun_birim_katsayilari_str,        M_URUN_BIRIM_KATSAYILARI,       URUN_BIRIMLERI_SAYISI );
    UNPACK_INT_ARRAY    ( urun_capraz_birimler_str,          M_URUN_CAPRAZ_BIRIMLER,         URUN_BIRIMLERI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( urun_birim_alis_fiyatlar_str,     M_URUN_BIRIM_ALIS_FIYATLARI,    URUN_BIRIMLERI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( urun_birim_sts_fiyatlar_str,    M_URUN_BIRIM_SATIS_FIYATLARI,   URUN_BIRIMLERI_SAYISI );
    UNPACK_INT_ARRAY    ( goruntulenecek_urun_birimleri_str, M_GORUNECEK_URUN_BIRIMLERI,     URUN_BIRIMLERI_SAYISI );

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( M_URUN_BIRIM_ADLARI[i].isEmpty() EQ true ) {
            break;
        }
        M_URUN_BIRIM_ADI_LINEEDIT[i]->setText(M_URUN_BIRIM_ADLARI[i]);
        M_URUN_BIRIM_KATSAYI_COMMAEDIT[i]->SET_DOUBLE(M_URUN_BIRIM_KATSAYILARI[i]);
        M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->addItem(m_temel_urun_birimi);
        for ( int j = 0; j < i; j++ ) {
            M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->addItem(M_URUN_BIRIM_ADLARI[j]);
        }
        M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT[i]->SET_DOUBLE(M_URUN_BIRIM_ALIS_FIYATLARI[i]);
        M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT[i]->SET_DOUBLE(M_URUN_BIRIM_SATIS_FIYATLARI[i]);
        M_URUN_KARTINDA_GOSTER_CHECKBOX[i]->setChecked(M_GORUNECEK_URUN_BIRIMLERI[i]);
    }

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX[i]->setCurrentIndex(M_URUN_CAPRAZ_BIRIMLER[i]);
    }
}

