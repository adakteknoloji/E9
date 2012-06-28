#include "cari_makbuz_belgesi_class.h"
#include "cari_makbuz_belgesi_open.h"
#include "adak_defines.h"
#include "yonetim.h"
#include "adak_sql.h"
#include "belge_kernel_open.h"
#include "cek_enum.h"
#include "belge_kernel_utils.h"
#include "e9_belge.h"
#include "adak_utils.h"
#include "dvz_enum.h"
#include "adres_console_utils.h"
#include "cek_senet_utils.h"
#include "dvz_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   CARI_MAKBUZ_BELGESI::CARI_MAKBUZ_BELGESI
***************************************************************************************/

void PRINT_CARI_MAKBUZ_BELGESI (int cari_fis_id, QWidget * parent )
{
    int tasarim_id = TASARIM_SEC(e9_belge_struct[CARI_MAKBUZLAR].belge_id, parent );

    if (tasarim_id EQ -1) {
        return;
    }

    SET_BELGE_TASARIM( CARI_MAKBUZLAR, tasarim_id, e9_belge_struct );

    CARI_MAKBUZ_BELGESI * tahsilat_makbuzu = new CARI_MAKBUZ_BELGESI( cari_fis_id );
    Q_UNUSED ( tahsilat_makbuzu );
}

/**************************************************************************************
                   CARI_MAKBUZ_BELGESI::CARI_MAKBUZ_BELGESI
***************************************************************************************/

CARI_MAKBUZ_BELGESI::CARI_MAKBUZ_BELGESI(int cari_fis_id)
{

    m_fis_id        = cari_fis_id;
    m_line_index    = 0;

    SQL_QUERY f_query (DB);
    SQL_QUERY s_query (DB);

    m_tahsil_eden   = KULLANICI_ADI();

    f_query.PREPARE_SELECT("car_fisler","cari_hesap_id,fis_tarihi,doviz_tutar_bilgileri,belge_seri,belge_numarasi",
                           "fis_id = :fis_id");

    f_query.SET_VALUE(":fis_id",m_fis_id);

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    f_query.NEXT();

    m_seri_sira = f_query.VALUE(3).toString() + " " + f_query.VALUE(4).toString();

    s_query.PREPARE_SELECT("car_hesaplar", "adres_id","cari_hesap_id = :cari_hesap_id" );
    s_query.SET_VALUE(":cari_hesap_id",f_query.VALUE(0).toInt());
    if ( s_query.SELECT() NE 0 ) {
        s_query.NEXT();

        QString adres_str = ADR_GET_ADRES_BILGILERI( s_query.VALUE(0).toInt() );

        m_unvan           = ADR_GET_FIRMA_SAHIS_ADI(s_query.VALUE(0).toInt()) + "<br>" + adres_str;

        m_tarih = QDate::fromString(f_query.VALUE(0).toString(),"yyyy.MM.dd").toString("dd.MM.yyyy");

        m_toplam_tutar    = 0.00;

        f_query.PREPARE_SELECT("car_fis_satirlari",
                               "cek_senet_id,borc_tutari,alacak_tutari,doviz_id,aciklama",
                               "fis_id = :fis_id");
        f_query.SET_VALUE(":fis_id",m_fis_id);
        if ( f_query.SELECT( "cek_senet_id,borc_tutari,alacak_tutari,doviz_id ASC " ) NE 0 ) {
            while ( f_query.NEXT() EQ true ) {
                if ( f_query.VALUE(0).toInt() > 0 ) {

                    CEK_SENET_STRUCT CEK_BILG;

                    int result = CEK_GET_TEMEL_BILGILER( &CEK_BILG, f_query.VALUE(0).toInt() );

                    if ( result EQ 0 ) {
                        continue;
                    }
                    if ( CEK_BILG.cek_senet_turu EQ ENUM_CEK ) {
                        m_odeme_sekli       << "Çek";
                    }
                    else {
                        m_odeme_sekli       << "Senet";
                    }

                    m_banka             << CEK_BILG.banka_ismi;
                    m_sube              << CEK_BILG.sube_kodu +"/"+CEK_BILG.sube_ismi;
                    m_cek_no            << CEK_BILG.cek_senet_numarasi;
                    m_cek_tarihi        << CEK_BILG.vade_tarihi;
                    m_aciklama          << f_query.VALUE(4).toString();
                    m_para_birimi       << DVZ_GET_DOVIZ_KODU( f_query.VALUE(3).toInt() );
                    m_tutar             << QVariant ( CEK_BILG.cek_senet_tutari ).toString();
                    m_toplam_tutar      += CEK_BILG.cek_senet_tutari;
                    continue;
                }

                m_doviz_kodu = f_query.VALUE(3).toString();

                m_odeme_sekli       << "Nakit";
                m_aciklama          << f_query.VALUE(4).toString();
                m_para_birimi       << " ";

                if (f_query.VALUE(1).toDouble() > 0) {
                    m_tutar             << f_query.VALUE(1).toString() + " " + f_query.VALUE(3).toString();
                    m_toplam_tutar      += f_query.VALUE(1).toDouble();
                }
                else {
                    m_tutar             << f_query.VALUE(2).toString() + " " + f_query.VALUE(3).toString();
                    m_toplam_tutar      += f_query.VALUE(2).toDouble();
                }
            }
        }
    }


    START_PRINT();
}

/**************************************************************************************
                   CARI_MAKBUZ_BELGESI::GET_HEADER_INFO
***************************************************************************************/

void CARI_MAKBUZ_BELGESI::GET_HEADER_INFO ()
{
    SET_HEADER_DATA(ISLEM_UNVAN       ,m_unvan      );
    SET_HEADER_DATA(ISLEM_TARIH       ,m_tarih      );
    SET_HEADER_DATA(ISLEM_SERI_SIRA   ,m_seri_sira  );
}

/**************************************************************************************
                   CARI_MAKBUZ_BELGESI::GET_FOOTER_INFO
***************************************************************************************/

void CARI_MAKBUZ_BELGESI::GET_FOOTER_INFO ()
{
    SET_FOOTER_DATA(ISLEM_TOP_TUTAR_Y , ADAK_YAZIYLA_YAZ(m_toplam_tutar));
    SET_FOOTER_DATA(ISLEM_TOP_TUTAR_R , QString(VIRGUL_EKLE(QVariant(m_toplam_tutar).toString()) + m_doviz_kodu));
    SET_FOOTER_DATA(ISLEM_TAHSIL_EDEN , m_tahsil_eden );
}

/**************************************************************************************
                   CARI_MAKBUZ_BELGESI::GET_LINE_INFO
***************************************************************************************/

void CARI_MAKBUZ_BELGESI::GET_LINE_INFO ()
{
    QString ISLEM_ODEME_SEKLI_STR = " ";
    QString ISLEM_ACIKLAMA_STR    = " ";
    QString ISLEM_TUTAR_STR       = " ";
    QString ISLEM_PARA_BIRIMI_STR = " ";

    QString ISLEM_BANKA_STR       = " ";
    QString ISLEM_SUBE_STR        = " ";
    QString ISLEM_CEK_NO_STR      = " ";
    QString ISLEM_CEK_TARIHI_STR  = " ";



    if (m_line_index < m_odeme_sekli.size()) {

        ISLEM_BANKA_STR       = m_banka.at(m_line_index);
        ISLEM_SUBE_STR        = m_sube.at(m_line_index);
        ISLEM_CEK_NO_STR      = m_cek_no.at(m_line_index);
        ISLEM_CEK_TARIHI_STR  = m_cek_tarihi.at(m_line_index);
        ISLEM_ODEME_SEKLI_STR = m_odeme_sekli.at(m_line_index);
        ISLEM_ACIKLAMA_STR    = m_aciklama.at(m_line_index);
        QStringList tutar_bilgileri = m_tutar.at(m_line_index).split(" ");
        ISLEM_TUTAR_STR       = VIRGUL_EKLE(tutar_bilgileri.at(0));
        if (tutar_bilgileri.size() EQ 2) {
            ISLEM_PARA_BIRIMI_STR = tutar_bilgileri.at(1);
        }
        m_line_index++;
    }

    SET_LINE_DATA(ISLEM_ODEME_SEKLI , ISLEM_ODEME_SEKLI_STR );
    SET_LINE_DATA(ISLEM_ACIKLAMA    , ISLEM_ACIKLAMA_STR    );
    SET_LINE_DATA(ISLEM_TUTAR       , ISLEM_TUTAR_STR       );
    SET_LINE_DATA(ISLEM_PARA_BIRIMI , ISLEM_PARA_BIRIMI_STR );
    SET_LINE_DATA(ISLEM_BANKA       , ISLEM_BANKA_STR       );
    SET_LINE_DATA(ISLEM_SUBE        , ISLEM_SUBE_STR        );
    SET_LINE_DATA(ISLEM_CEK_NO      , ISLEM_CEK_NO_STR      );
    SET_LINE_DATA(ISLEM_CEK_TARIHI  , ISLEM_CEK_TARIHI_STR  );
}
