#include "adak_utils.h"
#include "dmr_demirbas_listesi_class.h"
#include "dmr_demirbas_listesi_open.h"
#include "e9_console_utils.h"
#include "e9_enum.h"
#include "dmr_con_utils.h"
#include "dmr_struct.h"
#include "yonetim.h"
#include "adak_gruplar.h"
#include "adak_sql_query.h"
#include "dmr_enum.h"
#include "prs_con_utils.h"
#include "cari_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   F_DMR_DEMIRBAS_LISTESI
***************************************************************************************/

QString OPEN_DMR_DEMIRBAS_LISTESI (DMR_RAPOR_VARS * P_DMR_RV )
{
    DMR_DEMIRBAS_LISTESI  *  R = new DMR_DEMIRBAS_LISTESI( P_DMR_RV);
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
          DMR_DEMIRBAS_LISTESI::DMR_DEMIRBAS_LISTESI
***************************************************************************************/

DMR_DEMIRBAS_LISTESI::DMR_DEMIRBAS_LISTESI ( DMR_RAPOR_VARS * P_DMR_RV ) : REPORT_KERNEL ( "DMR_DEMIRBAS_LISTESI" )
{
    SET_HEIGHTS (30,30);

    m_max_line_count     =  GET_MAX_LINE_COUNT (14);

    m_degerleme_oncesi_demirbas_toplami             = 0.0;
    m_degerleme_oncesi_birikmis_amortisman_toplami  = 0.0;
    m_degerleme_oncesi_net_deger_toplami            = 0.0;

    m_degerleme_sonrasi_demirbas_toplami            = 0.0;
    m_degerleme_sonrasi_birikmis_amortisman_toplami = 0.0;
    m_degerleme_sonrasi_net_deger_toplami           = 0.0;

    m_birikmis_amortisman_toplami                   = 0.0;
    m_donem_amortisman_toplami                      = 0.0;

    m_ayrilmamis_amortisman_toplami                 = 0.0;

    m_tum_satirlar_basildi                          = false;

    m_sira_no                                       = 0;
    m_last_satir_indisi                             = 0;

    M_DMR_RV                                        = P_DMR_RV;

    FILL_YAZDIRILACAK_SATIRLAR_LIST();
}


/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI::GET_HEADER
***************************************************************************************/

QString DMR_DEMIRBAS_LISTESI::GET_HEADER()

{
    QString grup_kodu_adi = "";

    SET_TABLE_TAG_STRING ( "<TABLE style = \"width:100%;font-size:75%;\" BORDER=0 >" );

    SET_TD_TAG_STRING ( QStringList()<<"style = \"width:30%; font-size:100%;\""
                        <<"style = \"width:40%; font-size:100%;\" ALIGN=CENTER"
                        <<"style = \"width:30%; font-size:100%;\"ALIGN=RIGHT" );

    CREATE_TABLE     ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE ( QStringList() << E9_GET_FIRMA_ISMI() << "" << GET_REPORT_TOP_RIGHT_HEADER() );
    ADD_ROW_TO_TABLE ( QStringList() << "" << "<b>" + QVariant ( MALI_YIL_FIRST_DATE().year()).toString() + ADD_HTML_SPACE("   ") + "SABİT KIYMET LİSTESİ"<< "");
    ADD_ROW_TO_TABLE ( QStringList() << "" << grup_kodu_adi << "" );

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI::GET_BODY
***************************************************************************************/

QString DMR_DEMIRBAS_LISTESI::GET_BODY()
{

    if ( m_tum_satirlar_basildi EQ true ) {
        return NULL;
    }

    QStringList headers;
    QStringList YAZDIRILACAK_SATIR;
    QStringList EKLENEN_SATIR;

    m_eklenen_kayit_sayisi = 0;

    headers << "Sıra" <<"Sabit Kıymet <br>Cinsi"<< "Yılı" << "Sabit Kıymet <br>Değeri" << "Birikmiş<br> Amortisman" << "Net Değer"
            << "Amortisman<br> Oranı" << "Yen.Değer.<br>Oranı"<<"Sabit Kıymet <br> Yeni Değeri"<<"Bir.Amortisman <br>Yeni Değeri"
            <<"Yeni Net<br> Değer"<< QVariant ( MALI_YIL_FIRST_DATE().year()).toString() +  "<br>Amortismanı"<<"Birikmiş<br> Amortisman"
            <<"Ayrılmamış <br>Amortismanı"<<"Zimmetlendiği<br>Personel";


    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:65%; \" BORDER = 1>" );

    SET_TD_TAG_STRING(QStringList() << "style = \"width:3%;\"" << "style = \"width:7%;\""<< "style = \"width:3%;\""
                                    << "style = \"width:7%;\"" << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                    << "style = \"width:7%;\"" << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                    << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                    << "style = \"width:7%;\"" << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                    << "style = \"width:10%;\"");

    CREATE_TABLE ( headers,1,14 );

    SET_TD_TAG_STRING ( QStringList()<< "" << "" << "" <<"align = RIGHT "<<"align = RIGHT "
                                                 <<"align = RIGHT "<<"align=CENTER"<<"align=CENTER"<<"align = RIGHT"
                                                 <<"align = RIGHT " << "align=RIGHT" << "align=RIGHT" << "align=RIGHT"
                                                 << "align=RIGHT" << "");



    for ( ; ;  ) {

        if ( m_last_satir_indisi >= M_YAZDIRILACAK_SATIRLAR.size() ) {
            m_tum_satirlar_basildi = true;
            break;
        }

        YAZDIRILACAK_SATIR = M_YAZDIRILACAK_SATIRLAR.at(m_last_satir_indisi).split("\t");

        EKLENEN_SATIR.clear();

        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(0) << YAZDIRILACAK_SATIR.at(1) ;
        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(2) << YAZDIRILACAK_SATIR.at(3);
        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(4) << YAZDIRILACAK_SATIR.at(5);
        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(6) << YAZDIRILACAK_SATIR.at(7);
        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(8) << YAZDIRILACAK_SATIR.at(9);
        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(10) << YAZDIRILACAK_SATIR.at(11);
        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(12) << YAZDIRILACAK_SATIR.at(13);
        EKLENEN_SATIR << YAZDIRILACAK_SATIR.at(14);

        ADD_ROW_TO_TABLE(EKLENEN_SATIR);

        m_last_satir_indisi++;
        m_eklenen_kayit_sayisi++;


        if ( m_eklenen_kayit_sayisi EQ m_max_line_count ) {
            break;
        }
    }

    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI::GET_FOOTER
***************************************************************************************/

QString DMR_DEMIRBAS_LISTESI::GET_FOOTER()
{


    SET_TABLE_TAG_STRING ( "<TABLE style=\"width:100%; font-size:65%; \" BORDER = 0>");

    SET_TD_TAG_STRING    ( QStringList() << "style = \"width:13%;\""
                                         << "style = \"width:7%;\"" << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                         << "style = \"width:7%;\"" << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                         << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                         << "style = \"width:7%;\"" << "style = \"width:7%;\"" << "style = \"width:7%;\""
                                         << "style = \"width:10%;\"");

    CREATE_TABLE ( QStringList() << ""<<"" << "" << "" << "" << "" << "" << "" << ""  << "" << ""<<"" << "",0,0 );

    SET_TD_TAG_STRING ( QStringList()<< ""<<"align = RIGHT "<<"align = RIGHT "
                                     <<"align = RIGHT "<<""<<""<<"align = RIGHT"
                                     <<"align = RIGHT " << "align=RIGHT" << "align=RIGHT" << "align=RIGHT" << "align=RIGHT" << "");

    QStringList footer_satiri;

    footer_satiri << "";
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_degerleme_oncesi_demirbas_toplami ) ).toString(),2,false );
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_degerleme_oncesi_birikmis_amortisman_toplami)).toString(),2,false );
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_degerleme_oncesi_net_deger_toplami ) ).toString(),2,false);
    footer_satiri << "" << "";
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_degerleme_sonrasi_demirbas_toplami)).toString(),2,false);
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_degerleme_sonrasi_birikmis_amortisman_toplami)).toString(),2,false);
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_degerleme_sonrasi_net_deger_toplami)).toString(),2,false);
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_donem_amortisman_toplami)).toString(),2,false);
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_birikmis_amortisman_toplami)).toString(),2,false);
    footer_satiri << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_ayrilmamis_amortisman_toplami)).toString(),2,false) << "";

    ADD_ROW_TO_TABLE ( footer_satiri );

    return GET_TABLE_HTML_STRING();

}

/**************************************************************************************
                   DMR_DEMIRBAS_LISTESI::FILL_YAZDIRILACAK_SATIRLAR_LIST();
***************************************************************************************/

void DMR_DEMIRBAS_LISTESI::FILL_YAZDIRILACAK_SATIRLAR_LIST()
{

    double amortisman_tutari_array [ E9_ARRAY_SIZE ];



    M_DEMIRBASLAR_ID_LISTESI = GRP_GRUBA_EKLI_KAYITLARI_BUL(E9_PROGRAMI, DEMIRBAS_GRUP_MODULU, M_DMR_RV->grup_id_list );


    SQL_QUERY select_query ( DB );

     select_query.PREPARE_SELECT("dmr_demirbaslar","demirbas_adi,alis_tarihi,demirbasin_degeri,amortisman_orani,"
                                 "demirbasin_yeni_degeri,yeni_brkms_amortisman_tutari,amortisman_yontemi,"
                                 "muh_yili_ayrilacak_amor_tutari,demirbas_id,amortisman_tutari_array,personel_id" , "") ;

     if (M_DEMIRBASLAR_ID_LISTESI.isEmpty() EQ false) {

        select_query.AND_MULTI_EKLE("demirbas_id" ,  select_query.TO_QVARIANT(M_DEMIRBASLAR_ID_LISTESI) , ADAK_OR);;
     }

     if ( M_DMR_RV->sube_id > 0 ) {
         select_query.AND_EKLE("sube_id = :sube_id");
         select_query.SET_VALUE(":sube_id" , M_DMR_RV->sube_id );
     }

     if ( M_DMR_RV->bas_dmr_kodu.isEmpty() EQ false ) {
         select_query.AND_EKLE("demirbas_kodu BETWEEN :bas_dmr_kodu AND :bts_dmr_kodu");
         select_query.SET_VALUE(":bas_dmr_kodu" , M_DMR_RV->bas_dmr_kodu);
         select_query.SET_VALUE(":bts_dmr_kodu" , M_DMR_RV->bts_dmr_kodu);
     }

     if ( select_query.SELECT() EQ 0 ) {
         return;
     }

     QStringList YAZDIRILACAK_SATIR;

     while ( select_query.NEXT() EQ true ) {

         int demirbas_id                      = select_query.VALUE(8).toInt();
         int demirbas_giris_yili              = QDate::fromString ( select_query.VALUE(1).toString(),"yyyy.MM.dd").year();

         double demirbasin_degeri             = select_query.VALUE(2).toDouble(); // demirbasin degerleme oncesi degerini gosterir
         m_degerleme_oncesi_demirbas_toplami  += demirbasin_degeri;


         double amortisman_orani                 = select_query.VALUE(3).toDouble();

         double demirbasin_yeni_degeri          = select_query.VALUE(4).toDouble();
         m_degerleme_sonrasi_demirbas_toplami   += demirbasin_yeni_degeri;

         double birikmis_amortisman_yeni_degeri           = select_query.VALUE(5).toDouble();
         m_degerleme_sonrasi_birikmis_amortisman_toplami += birikmis_amortisman_yeni_degeri;

         int amortisman_sekli = select_query.VALUE(6).toInt();

         if ( amortisman_sekli EQ AZALAN_BAKIYE ) {
              amortisman_orani = amortisman_orani * 2;
         }

         int personel_id = select_query.VALUE("personel_id").toInt();

         int cari_hesap_id = PRS_GET_PERSONEL_CARI_HESAP_ID( personel_id );

         QString cari_hesap_kodu, cari_hesap_ismi;

         CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

         //Yeniden Degerleme oncesi birikmis amortismanlari,amortisman tutari arrayinin 0.indisinden aliyoruz.

         UNPACK_DOUBLE_ARRAY ( select_query.VALUE("amortisman_tutari_array").toString(),amortisman_tutari_array,E9_ARRAY_SIZE);

         double degerleme_oncesi_birikmis_amortisman = amortisman_tutari_array[0];

         double degerleme_oncesi_net_deger           = demirbasin_degeri - degerleme_oncesi_birikmis_amortisman;
         m_degerleme_oncesi_net_deger_toplami        += degerleme_oncesi_net_deger;


         double degerleme_sonrasi_net_deger          = demirbasin_yeni_degeri - birikmis_amortisman_yeni_degeri;
         m_degerleme_sonrasi_net_deger_toplami       += degerleme_sonrasi_net_deger;


         //Amortisman tutarini,amortisman tutari arrayinin rapor tarihine denk gelen gun indisinden 0.gunu(devir bakiyesini) cikararak buluruz.

         double amortisman_tutari     = amortisman_tutari_array[MALI_YIL_ARRAY_INDIS(QDate::currentDate() ) ] - degerleme_oncesi_birikmis_amortisman;
         m_donem_amortisman_toplami += amortisman_tutari;



         SQL_QUERY sql_query ( DB );
         sql_query.PREPARE_SELECT ( "dmr_fisler,dmr_fis_satirlari","degerleme_orani",
                                    "demirbas_id = :demirbas_id AND dmr_fisler.fis_id = dmr_fis_satirlari.fis_id "
                                    "AND fis_turu = :fis_turu" );
         sql_query.SET_VALUE      ( ":demirbas_id" , demirbas_id );
         sql_query.SET_VALUE      ( ":fis_turu"    , YENIDEN_DEGERLEME_FISI );


         double degerleme_orani = 1.0;
         if ( sql_query.SELECT() NE 0 ) {
              sql_query.NEXT();
             degerleme_orani = sql_query.VALUE(0).toDouble();
         }

         m_sira_no++;



         //Birikmis amortisman tutari ,amortisman tutari arrayinin muhasebe yilinin son gunune denk gelen indisinden alinir.(31 Aralik)

         double birikmis_amortisman_tutari = amortisman_tutari_array [ MALI_YIL_ARRAY_INDIS ( MALI_YIL_FIRST_DATE())];

         m_birikmis_amortisman_toplami += birikmis_amortisman_tutari;
         //Ayrilmamis amortismani gostermek icin muhasebe yili icin ayrilacak toplam amortismandan, ayrilmis amortismani cikaririz.

         double muhasebe_yili_ayrilacak_toplam_amortisman_tutari = select_query.VALUE(7).toDouble();

         double muhasebe_yili_ayrilmamis_amortisman_tutari = muhasebe_yili_ayrilacak_toplam_amortisman_tutari - amortisman_tutari;


         m_ayrilmamis_amortisman_toplami += muhasebe_yili_ayrilmamis_amortisman_tutari;

         YAZDIRILACAK_SATIR.clear();

         YAZDIRILACAK_SATIR.append(QVariant ( m_sira_no ).toString() +
         "\t" + select_query.VALUE(0).toString()+
         "\t" + QVariant ( demirbas_giris_yili).toString() +
         "\t" +VIRGUL_EKLE ( QVariant (demirbasin_degeri ).toString(),2,false )+
         "\t" + VIRGUL_EKLE ( QVariant ( ROUND(degerleme_oncesi_birikmis_amortisman) ).toString(),2,false )+
         "\t" + VIRGUL_EKLE ( QVariant ( ROUND(degerleme_oncesi_net_deger        ) ).toString(),2,false )+
         "\t % " + VIRGUL_EKLE ( QVariant ( ROUND ( amortisman_orani        ) ).toString()) +
         "\t % " + VIRGUL_EKLE ( QVariant ( ROUND(degerleme_orani           ) ).toString() ) +
         "\t" + VIRGUL_EKLE ( QVariant ( ROUND ( demirbasin_yeni_degeri          ) ).toString(),2,false )+
         "\t" + VIRGUL_EKLE ( QVariant ( ROUND ( birikmis_amortisman_yeni_degeri ) ).toString(),2,false )+
         "\t" + VIRGUL_EKLE ( QVariant ( ROUND ( degerleme_sonrasi_net_deger     ) ).toString(),2,false )+
         "\t" + VIRGUL_EKLE ( QVariant ( ROUND ( amortisman_tutari               ) ).toString(),2,false )
         + "\t" + VIRGUL_EKLE ( QVariant ( ROUND ( birikmis_amortisman_tutari   ) ).toString(),2,false)
         + "\t" + VIRGUL_EKLE ( QVariant ( ROUND ( muhasebe_yili_ayrilmamis_amortisman_tutari ) ).toString(),2,false )
         + "\t" + cari_hesap_ismi );

         M_YAZDIRILACAK_SATIRLAR << YAZDIRILACAK_SATIR;

     }

}
