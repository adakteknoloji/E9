#include "smm_makbuz_belgesi_class.h"
#include "adak_defines.h"
#include "yonetim.h"
#include "adak_sql.h"
#include "belge_kernel_utils.h"
#include "e9_belge.h"
#include "adak_utils.h"
#include "cari_console_utils.h"
#include "adres_console_utils.h"
#include "cek_senet_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "adres_struct.h"
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   SMM_MAKBUZ_BELGESI::SMM_MAKBUZ_BELGESI
***************************************************************************************/

void PRINT_SMM_MAKBUZ_BELGESI (int fis_id)
{
    int tasarim_id = TASARIM_SEC(e9_belge_struct[SM_MAKBUZU].belge_id,NULL);

    if (tasarim_id EQ -1) {
        return;
    }

    SET_BELGE_TASARIM(SM_MAKBUZU,tasarim_id,e9_belge_struct);

    SMM_MAKBUZ_BELGESI * sm_makbuzu = new SMM_MAKBUZ_BELGESI(fis_id);

    sm_makbuzu->START_PRINT();
}

/**************************************************************************************
                   SMM_MAKBUZ_BELGESI::SMM_MAKBUZ_BELGESI
***************************************************************************************/

SMM_MAKBUZ_BELGESI::SMM_MAKBUZ_BELGESI(int makbuz_id)
{


    SQL_QUERY   query (DB);

    int cari_hesap_id = 0;

    query.PREPARE_SELECT("smm_makbuzlar,smm_hizmetler" , "cari_hesap_id,cari_hesap_adresi,"
                         "vergi_dairesi,vergi_numarasi,belge_seri,belge_numarasi,"
                         "makbuz_tarihi,hizmet_adi,brut_tutar,vergi_stopaj_tutari,"
                         "net_tutar,kdv_tutari,hizmet_tutari,aciklama",
                         "makbuz_id = :makbuz_id AND "
                         "smm_hizmetler.hizmet_id = smm_makbuzlar.hizmet_id");
    query.SET_VALUE(":makbuz_id" , makbuz_id);

    if ( query.SELECT() NE 0 ) {

        query.NEXT();

        cari_hesap_id     = query.VALUE(0).toInt();
        m_adresi          = query.VALUE(1).toString();
        m_vergi_dairesi   = query.VALUE(2).toString();
        m_vergi_no        = query.VALUE(3).toString();
        m_belge_seri_sira = query.VALUE(4).toString() + " " + query.VALUE(5).toString();
        m_tarih           = QDate::fromString(query.VALUE(6).toString() , "yyyy.MM.dd" ).toString("dd MM yyyy");
        m_yapilan_hizmet  = query.VALUE(7).toString() + "\n" + query.VALUE("aciklama").toString();
        m_brut_ucret      = VIRGUL_EKLE(query.VALUE(8).toString());
        m_stopaj          = VIRGUL_EKLE(query.VALUE(9).toString());
        m_net_ucret       = VIRGUL_EKLE(query.VALUE(10).toString());
        m_kdv             = VIRGUL_EKLE(query.VALUE(11).toString());
        m_hizmet_tutari   = VIRGUL_EKLE(query.VALUE(12).toString());

        int adr_record_id = CARI_GET_ADRES_RECORD_ID(cari_hesap_id);

        ADRES_HESAP_STRUCT * ADR_RECORD = new ADRES_HESAP_STRUCT;

        ADR_KART_BILGILERINI_OKU(ADR_RECORD , adr_record_id);

        m_unvan = ADR_RECORD->unvan;

    }
}



/**************************************************************************************
                   SMM_MAKBUZ_BELGESI::GET_HEADER_INFO
***************************************************************************************/

void SMM_MAKBUZ_BELGESI::GET_HEADER_INFO ()
{
    SET_HEADER_DATA(SMM_UNVAN         , m_unvan      );
    SET_HEADER_DATA(SMM_TARIH         , m_tarih      );
    SET_HEADER_DATA(SMM_ADRES         , m_adresi     );
    SET_HEADER_DATA(SMM_SERI_SIRA     , m_belge_seri_sira );
    SET_HEADER_DATA(SMM_VERGI_DAIRESI , m_vergi_dairesi);
    SET_HEADER_DATA(SMM_VERGI_NO      , m_vergi_no);
}

/**************************************************************************************
                   SMM_MAKBUZ_BELGESI::GET_FOOTER_INFO
***************************************************************************************/

void SMM_MAKBUZ_BELGESI::GET_FOOTER_INFO ()
{
    SET_FOOTER_DATA(SMM_BRUT_UCRET          , m_brut_ucret );
    SET_FOOTER_DATA(SMM_HIZMET_TUTARI_R     , m_hizmet_tutari);
    SET_FOOTER_DATA(SMM_KDV                 , m_kdv);
    SET_FOOTER_DATA(SMM_NET_UCRET           , m_net_ucret);
    SET_FOOTER_DATA(SMM_VERGI_STOPAJ        , m_stopaj);
    SET_FOOTER_DATA(SMM_HIZMET_TUTARI_Y     , ADAK_YAZIYLA_YAZ(QVariant(m_hizmet_tutari).toDouble() ,
                                                             DVZ_GET_TEMEL_PARA_BIRIMI_KODU()));
}

/**************************************************************************************
                   SMM_MAKBUZ_BELGESI::GET_LINE_INFO
***************************************************************************************/

void SMM_MAKBUZ_BELGESI::GET_LINE_INFO ()
{
    SET_LINE_DATA(SMM_YAPILAN_HIZMET      , m_yapilan_hizmet       );
}
