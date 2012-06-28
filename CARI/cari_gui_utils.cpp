#include <QComboBox>
#include "adak_utils.h"
#include "adres_console_utils.h"
#include "adak_sql.h"
#include "adak_gruplar.h"
#include "cari_console_utils.h"
#include "adres_email_fisi_open.h"
#include "adres_kayitlari_raporu_open.h"
#include "adres_not_defteri_fisi_open.h"
#include "adres_telefon_fisi_open.h"
#include "adres_yetkililer_fisi_open.h"
#include "adres_zarf_yazdir_batch_open.h"


extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_CARI_NOT_DEFTERI
***************************************************************************************/

void OPEN_CARI_NOT_DEFTERI (int cari_hesap_id , int modul_id, int program_id , QWidget * parent)
{
    int adres_id = CARI_GET_ADRES_RECORD_ID(cari_hesap_id);

    if (adres_id > 0) {
        OPEN_ADRES_NOT_DEFTERI_FISI(adres_id,modul_id,program_id,parent);
    }
}

/**************************************************************************************
                   OPEN_CARI_ZARF_YAZDIR
***************************************************************************************/

void OPEN_CARI_ZARF_YAZDIR (int cari_hesap_id , QString printer_name , QWidget * parent)
{
    int adres_id = CARI_GET_ADRES_RECORD_ID(cari_hesap_id);

    if (adres_id > 0) {
        OPEN_ADRES_ZARF_YAZDIR_BATCH(adres_id,printer_name,parent);
    }
}

/**************************************************************************************
                   OPEN_CARI_EMAIL_FISI
***************************************************************************************/

void OPEN_CARI_EMAIL_FISI (int cari_hesap_id , QWidget * parent)
{
    int adres_id = CARI_GET_ADRES_RECORD_ID(cari_hesap_id);

    if (adres_id > 0) {
        OPEN_ADRES_EMAIL_FISI(adres_id,parent);
    }
}

/**************************************************************************************
                   OPEN_CARI_TELEFON_FISI
***************************************************************************************/

void OPEN_CARI_TELEFON_FISI (int cari_hesap_id , QWidget * parent)
{
    int adres_id = CARI_GET_ADRES_RECORD_ID(cari_hesap_id);

    if (adres_id > 0) {
        OPEN_ADRES_TELEFON_FISI(adres_id,parent);
    }
}


/**************************************************************************************
                   OPEN_CARI_FIRMA_YETKILILERI
***************************************************************************************/

void OPEN_CARI_FIRMA_YETKILILERI(int cari_hesap_id , QWidget * parent)
{
    int adres_id = CARI_GET_ADRES_RECORD_ID(cari_hesap_id);

    if (adres_id > 0) {
        OPEN_ADRES_YETKILILER_FISI( adres_id,parent );
    }
}

/**************************************************************************************
                         CARI_GET_NEXT_CARI_KODU
***************************************************************************************/

QString CARI_GET_NEXT_CARI_KODU (QString program_kod_oneki, int program_id, int module_id , bool start_transaction )
{
    SQL_QUERY query (DB);

    query.PREPARE_SELECT("e9_sabit_degerler","cari_hesap_kodu_counter");

    if ( query.SELECT() EQ 0 ) {
        return "";
    }

    query.NEXT();

    int current_value = query.VALUE(0).toInt() + 1;

    QString siradaki_deger = program_kod_oneki + QString("%1").arg(QVariant(current_value).toString(),5,'0');

    QStringList adres_id_list = ADR_GET_ADRES_ID_LIST( -1 ,module_id , program_id);

    query.PREPARE_SELECT("car_hesaplar","cari_hesap_kodu","cari_hesap_kodu LIKE :cari_hesap_kodu");

    query.SET_VALUE     (":cari_hesap_kodu", siradaki_deger                       );

    if ( adres_id_list.isEmpty() EQ false ) {
        query.AND_MULTI_EKLE("adres_id" , query.TO_QVARIANT(adres_id_list),ADAK_OR);
    }

    if ( query.SELECT("cari_hesap_kodu",0,1) NE 0 ) {
        ADAK_ERROR("Sistem tarafından yaratılan cari hesap kodu daha önce elle girilen kodla çakıştı.\n"
                  "Üretilen kod atlandı.Elle yeni bir kod girin yada tekrar deneyin",NULL,NULL);
        siradaki_deger.clear();
    }

    if ( start_transaction EQ true ) {
        DB->START_TRANSACTION();
    }

    query.PREPARE_UPDATE("e9_sabit_degerler","sabit_deger_id","cari_hesap_kodu_counter","");
    query.SET_VALUE     (":cari_hesap_kodu_counter",QVariant(current_value).toString()    );
    query.UPDATE        ();

    if ( start_transaction EQ true ) {
        DB->COMMIT_TRANSACTION();
    }


    return siradaki_deger;
}
/**************************************************************************************
                         CARI_FILL_CARI_KART_TIPI_COMBO_BOX
***************************************************************************************/

void CARI_FILL_CARI_KART_TIPI_COMBO_BOX(QComboBox *p_combo_box)
{
    QStringList cari_kart_tipleri;

    cari_kart_tipleri << "Alıcı" << "Satıcı" << "Personel" << "Firma Ortağı";
    p_combo_box->addItems( cari_kart_tipleri );
}


/**************************************************************************************
                        CARI_FILL_CARI_ENT_FIS_TURU_COMBO_BOX
***************************************************************************************/

void CARI_FILL_CARI_ENT_FIS_TURU_COMBO_BOX ( QComboBox * p_combo_box  )
{
    QStringList cari_ent_fis_turleri;

    cari_ent_fis_turleri<< QObject::tr("Tüm Entegrasyon Fişleri")
                        << QObject::tr("Banka Entegrasyon Fişi")
                        << QObject::tr("Çek-Senet Entegrasyon Fişi ( Ödeme )")
                        << QObject::tr("Çek-Senet Entegrasyon Fişi")
                        << QObject::tr("Fatura Entegrasyon Fişi")
                        << QObject::tr("Personel Entegrasyon Fişi")
                        << QObject::tr("SM Makbuz Entegrasyon Fişi")
                        << QObject::tr("Diğer Entegrasyon Fişleri");

    p_combo_box->addItems( cari_ent_fis_turleri );

}




