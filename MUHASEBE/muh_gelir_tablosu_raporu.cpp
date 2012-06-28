#include "muh_gelir_tablosu_raporu_class.h"
#include "muh_gelir_tablosu_raporu_open.h"
#include "adak_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_console_utils.h"
#include "e9_enum.h"
#include "yonetim.h"
#include "muh_console_utils.h"

extern ADAK_SQL *       DB;


/**************************************************************************************
                    GET_GELIR_TABLOSU_RAPORU
*****************************************************************/

QString GET_GELIR_TABLOSU_RAPORU (MUH_RAPOR_VARS * rapor_vars)
{
    GELIR_TABLOSU_RAPORU    * R = new GELIR_TABLOSU_RAPORU(rapor_vars);
    return R->CREATE_HTML_PAGES();
}

/*****************************************************************
          GELIR_TABLOSU_RAPORU::GELIR_TABLOSU_RAPORU
*****************************************************************/

GELIR_TABLOSU_RAPORU::GELIR_TABLOSU_RAPORU(MUH_RAPOR_VARS * rapor_vars) : REPORT_KERNEL ("MUH_GELIR_TABLOSU_RAPORU")
{
    M_MUH_RV = rapor_vars;

    SET_HEIGHTS (30 , 10);

    m_max_yazdirilabilir_satir_sayisi =  GET_MAX_LINE_COUNT (9);
    m_yazdirilan_kayit_sayisi         = 0;
    m_last_hesap_indisi               = 0;
    m_gelir_tablosu_hesap_sayisi      = 0;

    GELIR_TABLOSU_HESAPLARI    = new MUH_GELIR_TABLOSU_HESAPLARI_STRUCT [GELIR_TABLOSU_ARRAY_SIZE];
    GELIR_TABLOSU_OLUSTUR();
}

/*****************************************************************
              GELIR_TABLOSU_RAPORU::GET_HEADER
*****************************************************************/

QString GELIR_TABLOSU_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:85%;\" BORDER=0>");
    SET_TD_TAG_STRING    ( QStringList() << "style=\"width:25%;\"" << "style=\"width:50%;\" ALIGN=CENTER"
                          << "style=\"width:25%;\" ALIGN=RIGHT");

    CREATE_TABLE         ( QStringList() << "" << "" << "" , 0 , 0);

    ADD_ROW_TO_TABLE ( QStringList() << E9_GET_FIRMA_ISMI() << "<b> GELİR TABLOSU </b>" << "" );

    ADD_ROW_TO_TABLE ( QStringList() << "" << M_MUH_RV->baslangic_tarihi.toString("dd MMMM yyyy dddd") +  "-" +
                    M_MUH_RV->bitis_tarihi.toString("dd MMMM yyyy dddd") << "");

    return GET_TABLE_HTML_STRING();
}

/*****************************************************************
             GELIR_TABLOSU_RAPORU::GET_BODY
*****************************************************************/

QString GELIR_TABLOSU_RAPORU::GET_BODY()
{
    QStringList headers;
    QStringList string_list;

    double      donem_bitis_bakiyesi;

    if (m_last_hesap_indisi > m_gelir_tablosu_hesap_sayisi ) {
        return NULL;
    }

    headers.clear();

    headers << "<b><u>Hesap/Açıklama"
            << "";


    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:75%;\" BORDER=0>");
    SET_TD_TAG_STRING    ( QStringList() << "style=\"width:50%;\"" << "style=\"width:50%;\"");
    CREATE_TABLE         ( headers , 1 , 13);

    m_yazdirilan_kayit_sayisi = 0;

    for ( ; ; ) {

        if (m_last_hesap_indisi > m_gelir_tablosu_hesap_sayisi ) {
            break;
        }

        QString hesap_ismi = QObject::tr( GELIR_TABLOSU_HESAPLARI[m_last_hesap_indisi].hesap_ismi);

        int hesap_id       = GELIR_TABLOSU_HESAPLARI [m_last_hesap_indisi].hesap_id;

        if (hesap_id > 0 ) {
            hesap_ismi.prepend("-----------");
        }

        if ( GELIR_TABLOSU_HESAPLARI[m_last_hesap_indisi].bitis_borc_bakiyesi >
             GELIR_TABLOSU_HESAPLARI[m_last_hesap_indisi].bitis_alacak_bakiyesi ) {
            donem_bitis_bakiyesi = GELIR_TABLOSU_HESAPLARI[m_last_hesap_indisi].bitis_borc_bakiyesi -
                                   GELIR_TABLOSU_HESAPLARI[m_last_hesap_indisi].bitis_alacak_bakiyesi;
        }
        else {
            donem_bitis_bakiyesi = GELIR_TABLOSU_HESAPLARI[m_last_hesap_indisi].bitis_alacak_bakiyesi -
                                   GELIR_TABLOSU_HESAPLARI[m_last_hesap_indisi].bitis_borc_bakiyesi;
        }

        if (hesap_id EQ HESAPLAMA_SATIRI ) {
            string_list.clear();
            string_list << "<u>" + hesap_ismi;

            if (donem_bitis_bakiyesi EQ 0 ) {
                string_list << "";
            }
            else {
                string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND (donem_bitis_bakiyesi ) ).toString());
            }
        }
        else {
            string_list.clear();
            string_list << hesap_ismi;

            if (donem_bitis_bakiyesi EQ 0 ) {
                string_list << "";
            }
            else {
                string_list << VIRGUL_EKLE ( QVariant ( ROUND (donem_bitis_bakiyesi )).toString());
            }
        }

        ADD_ROW_TO_TABLE(string_list);

        m_yazdirilan_kayit_sayisi++;
        m_last_hesap_indisi++;

        if (m_yazdirilan_kayit_sayisi EQ m_max_yazdirilabilir_satir_sayisi) {
            return GET_TABLE_HTML_STRING();
        }
    }
    return GET_TABLE_HTML_STRING();
}

/*****************************************************************
           GELIR_TABLOSU_RAPORU::GET_FOOTER
*****************************************************************/

QString GELIR_TABLOSU_RAPORU::GET_FOOTER()
{
    return NULL;
}
/*****************************************************************
         GELIR_TABLOSU_RAPORU::GELIR_TABLOSU_OLUSTUR();
*****************************************************************/

void GELIR_TABLOSU_RAPORU::GELIR_TABLOSU_OLUSTUR()
{

     MUH_HESAP_GRUPLARI_STRUCT      MUH_HESAP_GRUPLARI[] = {
        {"60", "BRÜT SATIŞLAR"},
        {"61", "SATIŞ İNDİRİMLERİ (-)"},
        {"62", "SATIŞLARIN MALİYETİ (-)"},
        {"63", "FAALİYET GİDERLERİ (-)"},
        {"64", "DİĞER FAALİYETLERDEN OLAĞAN GELİR VE KARLAR"},
        {"65", "DİĞER FAALİYETLERDEN OLAĞAN GİDER VE ZARARLAR (-)"},
        {"66", "FİNANSMAN GİDERLERİ (-)"},
        {"67", "OLAĞANDIŞI GELİR VE KARLARI"},
        {"68", "OLAĞANDIŞI GİDER VE ZARARLAR (-)"},
        {"69", "DÖNEM NET KARI (ZARARI)"}
    };

    int grup_indis_no          = 0;
    m_gelir_hesaplari_indis_no = 0;

    for ( int j = 0 ; j < GELIR_TABLOSU_GRUP_SAYISI ; j++ ) {

        grup_indis_no = m_gelir_hesaplari_indis_no;

        strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu , MUH_HESAP_GRUPLARI[j].hesap_kodu);
        strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "");
        strcat (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , MUH_HESAP_GRUPLARI[j].hesap_kodu);
        strcat (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "-");
        strcat (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , MUH_HESAP_GRUPLARI[j].hesap_ismi);

        GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_id = GRUP_SATIRI;

        GRUBUN_HESAPLARINI_GELIR_TABLOSUNA_EKLE ( grup_indis_no , MUH_HESAP_GRUPLARI[j].hesap_kodu);

        if (strncmp (MUH_HESAP_GRUPLARI[j].hesap_kodu , "61",2) EQ 0 ) {

            m_gelir_hesaplari_indis_no++;

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu , "NS");
            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "<b>NET SATIŞLAR");
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_id  = HESAPLAMA_SATIRI;

            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       = 0.00;
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     = 0.00;

            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "60");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "61");
        }
        if (strncmp (MUH_HESAP_GRUPLARI[j].hesap_kodu ,"62",2 ) EQ 0 ) {

            m_gelir_hesaplari_indis_no++;

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu , "BS");
            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "<b>BRÜT SATIŞ KARI/ZARARI");
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_id  = HESAPLAMA_SATIRI;

            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       = 0.00;
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     = 0.00;

            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "NS");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "62");

        }
         if (strncmp ( MUH_HESAP_GRUPLARI[j].hesap_kodu ,"63",2 ) EQ 0 ) {

            m_gelir_hesaplari_indis_no++;

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu , "FK");
            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "<b>FAALİYET KARI/ZARARI");
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_id  = HESAPLAMA_SATIRI;

            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       = 0.00;
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     = 0.00;

            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "BS");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "63");

        }
        if (strncmp (MUH_HESAP_GRUPLARI[j].hesap_kodu ,"66",2 ) EQ 0 ) {

            m_gelir_hesaplari_indis_no++;

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu , "OK");
            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "<b>OLAĞAN KAR/ZARAR");
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_id  = HESAPLAMA_SATIRI;

            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       = 0.00;
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     = 0.00;

            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "FK");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "64");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "65");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "66");

        }
        if (strncmp ( MUH_HESAP_GRUPLARI[j].hesap_kodu ,"68",2 ) EQ 0 ) {

            m_gelir_hesaplari_indis_no++;

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu , "DK");
            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "<b>DÖNEM BRÜT KARI/ZARARI");
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_id  = HESAPLAMA_SATIRI;

            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       = 0.00;
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     = 0.00;

            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "OK");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "67");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "68");

        }

        if (strncmp(MUH_HESAP_GRUPLARI[j].hesap_kodu ,"69",2 ) EQ 0 ) {

            m_gelir_hesaplari_indis_no++;

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu , "DN");
            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "<b>DÖNEM NET KARI/ZARARI");
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_id  = HESAPLAMA_SATIRI;

            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       = 0.00;
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     = 0.00;

            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "DK");
            GRUP_BAKIYESI (m_gelir_hesaplari_indis_no , "69");

        }
        m_gelir_hesaplari_indis_no++;
    }

    if (m_gelir_hesaplari_indis_no > 1 ) {

        m_gelir_tablosu_hesap_sayisi = m_gelir_hesaplari_indis_no -1;
    }
    else {
        m_gelir_tablosu_hesap_sayisi = m_gelir_hesaplari_indis_no;
    }

}

/***********************************************************/
/*            GELIR_TABLOSU_RAPORU::GRUP_BAKIYESI          */
/***********************************************************/

void GELIR_TABLOSU_RAPORU::GRUP_BAKIYESI(int p_grup_indis_no , const char * p_grup_kodu)
{
    if (strlen(p_grup_kodu) EQ 0 ) {
        return;
    }
    for (int i = 0 ; i < m_gelir_hesaplari_indis_no ; i++ ) {

        if (strlen(GELIR_TABLOSU_HESAPLARI[i].hesap_kodu) NE 2 ) {
            continue;
        }
        if (strncmp (GELIR_TABLOSU_HESAPLARI[i].hesap_kodu , p_grup_kodu , strlen(p_grup_kodu)) NE 0 ) {
            continue;
        }
        GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_borc_bakiyesi       += GELIR_TABLOSU_HESAPLARI[i].bitis_borc_bakiyesi;
        GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_alacak_bakiyesi     += GELIR_TABLOSU_HESAPLARI[i].bitis_alacak_bakiyesi;
    }

}
/******************************************************************/
/*         GELIR_TABLOSU_RAPORU::HESAP_BAKIYELERI                 */
/******************************************************************/

void GELIR_TABLOSU_RAPORU::GRUBUN_HESAPLARINI_GELIR_TABLOSUNA_EKLE(int p_grup_indis_no , const char * p_grup_kodu)
{
    SQL_QUERY sql_query(DB);

    double borc_bakiyesi   [E9_ARRAY_SIZE];
    double alacak_bakiyesi [E9_ARRAY_SIZE];

    double donem_kari   = 0.0;
    double donem_zarari = 0.0;

    int bts_gun_no          = MALI_YIL_ARRAY_INDIS(M_MUH_RV->bitis_tarihi);

    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,tam_hesap_kodu,hesap_ismi,"
                             "toplam_borc_array,toplam_alacak_array " ,
                             "hesap_seviyesi = :hesap_seviyesi");

    sql_query.SET_VALUE(":hesap_seviyesi" , 1 );

    GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_borc_bakiyesi         = 0.00;
    GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_alacak_bakiyesi       = 0.00;

    if (sql_query.SELECT() > 0 ) {

        while (sql_query.NEXT()) {

            QString tam_hesap_kodu          = sql_query.VALUE(1).toString();
            const char * tam_hesap_kodu_str = tam_hesap_kodu.toStdString().c_str();
            if ( strncmp ( tam_hesap_kodu_str , p_grup_kodu , strlen (p_grup_kodu)) NE 0 ){
                continue;
            }
            int hesap_id        = sql_query.VALUE(0).toInt();
            QString hesap_ismi  = sql_query.VALUE(2).toString();

            UNPACK_DOUBLE_ARRAY (sql_query.VALUE(3).toString() , borc_bakiyesi   , E9_ARRAY_SIZE);
            UNPACK_DOUBLE_ARRAY (sql_query.VALUE(4).toString() , alacak_bakiyesi , E9_ARRAY_SIZE);

            double borc_bakiye  = 0.0;
            double alacak_bakiye = 0.0;

            int gun_no = MALI_YIL_ARRAY_INDIS(M_MUH_RV->bitis_tarihi);

            if ( M_MUH_RV->bakiyesiz_hesaplar EQ 0 ) {

                if ( borc_bakiyesi[gun_no] EQ 0 AND alacak_bakiyesi[gun_no] EQ 0 ) {
                    continue;
                }
            }


            m_gelir_hesaplari_indis_no++;

            GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no].hesap_id = hesap_id;

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_kodu ,
                    tam_hesap_kodu.toStdString().c_str());

            strcpy (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi,"   ");

            strcat (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , tam_hesap_kodu.toStdString().c_str());
            strcat (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , "-");
            strcat (GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].hesap_ismi , hesap_ismi.toStdString().c_str());


            MUH_GET_FROM_GIDER_HESABI_ISLENMEMIS_BAKIYE(hesap_id ,borc_bakiye , alacak_bakiye , gun_no);

            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       = borc_bakiyesi[bts_gun_no] + borc_bakiye;
            GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     = alacak_bakiyesi[bts_gun_no] + alacak_bakiye;

            if ( tam_hesap_kodu EQ "690") {

                MUH_GET_FROM_GELIR_TABLOSU_DONEM_NET_KARI_ZARARI(donem_kari,donem_zarari ,bts_gun_no);

                GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi       += donem_zarari;
                GELIR_TABLOSU_HESAPLARI [m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi     += donem_kari;

                GELIR_TABLOSU_HESAPLARI[p_grup_indis_no].bitis_borc_bakiyesi += donem_zarari;
                GELIR_TABLOSU_HESAPLARI[p_grup_indis_no].bitis_alacak_bakiyesi += donem_kari;

            }
            else if ( tam_hesap_kodu EQ "692") {

                //691 hesabinin bakiyesi bulunur.

                double vergi_gideri = 0.0;

                if ( GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no-1].bitis_borc_bakiyesi >
                     GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no-1].bitis_alacak_bakiyesi) {
                    //Donem net zarari
                    vergi_gideri = GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no-1].bitis_borc_bakiyesi -
                                   GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no-1].bitis_alacak_bakiyesi;

                    GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi = donem_kari - vergi_gideri;

                }
                else {
                    //Donem net kari
                    vergi_gideri = GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no-1].bitis_alacak_bakiyesi -
                                   GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no-1].bitis_borc_bakiyesi;
                    //692 hesabina donem net zarari islenir.
                    GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi = donem_zarari - vergi_gideri;
                }

                GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_borc_bakiyesi         += GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no].bitis_borc_bakiyesi;
                GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_alacak_bakiyesi       += GELIR_TABLOSU_HESAPLARI[m_gelir_hesaplari_indis_no].bitis_alacak_bakiyesi;
            }

            GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_borc_bakiyesi         += borc_bakiyesi[bts_gun_no] + borc_bakiye;
            GELIR_TABLOSU_HESAPLARI [p_grup_indis_no].bitis_alacak_bakiyesi       += alacak_bakiyesi[bts_gun_no] + alacak_bakiye;



        }
    }
}



