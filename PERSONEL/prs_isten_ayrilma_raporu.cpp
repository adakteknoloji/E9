#include <QStringList>
#include <QDate>
#include "prs_isten_ayrilma_raporu_class.h"
#include "prs_isten_ayrilma_raporu_open.h"
#include "adak_utils.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "cari_struct.h"
#include "cari_console_utils.h"
#include "prs_con_utils.h"
 

extern ADAK_SQL   *     DB;
extern PRS_RAPOR_VARS    PRS_RV;

/**************************************************************************************
                   SHOW_ISTEN_AYRILMA_RAPORU
***************************************************************************************/

QString SHOW_ISTEN_AYRILMA_RAPORU ( int personel_id, int isten_ayrilan_id )
{
    ISTEN_AYRILMA_RAPORU * R = new ISTEN_AYRILMA_RAPORU ( personel_id, isten_ayrilan_id );

    return R->CREATE_HTML_PAGES();

}

/**************************************************************************************
                   ISTEN_AYRILMA_RAPORU::ISTEN_AYRILMA_RAPORU
**************************************************************************************/

ISTEN_AYRILMA_RAPORU::ISTEN_AYRILMA_RAPORU ( int personel_id, int isten_ayrilan_id ) : REPORT_KERNEL ("PRS_ISTEN_AYRILMA_RAPORU")
{
    m_personel_id      = personel_id;
    m_isten_ayrilan_id = isten_ayrilan_id;

    m_rapor_bitti_mi   = false;

    SET_WRAP( WRAP );

    SET_HEIGHTS ( 0, 0 );
}

/**************************************************************************************
                   ISTEN_AYRILMA_RAPORU::GET_HEADER
***************************************************************************************/

QString ISTEN_AYRILMA_RAPORU::GET_HEADER ()
{
    QString         header;

    header += "<table width=1250 cellspacing=0 cellpadding=0 border=0>"
               "<tr>"
                    "<td align=center><font style=\"font-size:17pt;\"><b><br>İBRANAME&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b><br><br> &nbsp;</font>"
                    "</td>"
                "</tr>"
             "</table>";

    return header;
}

/**************************************************************************************
                   ISTEN_AYRILMA_RAPORU::GET_BODY
**************************************************************************************/

QString ISTEN_AYRILMA_RAPORU::GET_BODY ()
{
    SQL_QUERY       sql_query_kisisel      ( DB );
    SQL_QUERY       sql_query_sgk          ( DB );
    SQL_QUERY       query                  ( DB );
    SQL_QUERY       sql_query(DB);
    QString         body;

    if ( m_rapor_bitti_mi EQ true ) {

        return NULL;
    }

    int cari_hesap_id = 0;
    query.PREPARE_SELECT( "prs_personeller","cari_hesap_id", "personel_id = :personel_id");
    query.SET_VALUE     ( ":personel_id", m_personel_id );

    if ( query.SELECT() NE 0 ) {
         query.NEXT();
        cari_hesap_id   = query.VALUE( 0 ).toInt();
    }

    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri  );

    sql_query.PREPARE_SELECT("prs_sabit_degerler","ibraname_maddeleri  ",
                             "sabit_degerler_id = :sabit_degerler_id");
    sql_query.SET_VALUE(":sabit_degerler_id",1);

    if ( sql_query.SELECT() EQ 0 ) {
        return "";
    }
    sql_query.NEXT();
    QString ibraname_madddeleri = sql_query.VALUE(0).toString();

    sql_query.PREPARE_SELECT("e9_sabit_degerler","firma_ismi,firma_adres_satiri_1,firma_adres_satiri_2,firma_adres_satiri_3 ",
                             "sabit_deger_id =:sabit_deger_id");
    sql_query.SET_VALUE(":sabit_deger_id",1);

    QString firma_ismi;
    QString firma_adresi;

    if(sql_query.SELECT() > 0) {
        sql_query.NEXT();
        firma_ismi = sql_query.VALUE(0).toString();
        firma_adresi = sql_query.VALUE(1).toString() + sql_query.VALUE(2).toString() + sql_query.VALUE(3).toString();
    }



    sql_query_sgk.PREPARE_SELECT("prs_personeller","sigorta_no ",
                                 "personel_id = :personel_id");

    sql_query_sgk.SET_VALUE( ":personel_id", m_personel_id );

    if ( sql_query_sgk.SELECT() EQ 0 ) {
        return "";
    }
    sql_query_sgk.NEXT();


    sql_query_kisisel.PREPARE_SELECT("prs_personel_kisisel_blgler, prs_isten_ayrilanlar",
                                     "ise_giris_tarihi, isten_ayrilma_tarihi",
                                     "prs_personel_kisisel_blgler.personel_id = :personel_id "
                                     "AND prs_isten_ayrilanlar.personel_id = prs_personel_kisisel_blgler.personel_id ");

    sql_query_kisisel.SET_VALUE( ":personel_id", m_personel_id );

    if ( sql_query_kisisel.SELECT() EQ 0 ) {
        return "";
    }
    sql_query_kisisel.NEXT();


    body += "<br><br><br><table width=1250 cellspacing=0 cellpadding=0 border=0>"
                "<tr>"
                "<td align=left><font style=\"font-size:13pt;\"><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>"                    "</td>"
                "<td align=left><font style=\"font-size:13pt;\"><b>&nbsp;&nbsp;&nbsp;&nbsp;</b>"                    "</td>"
                    "</tr>"
                "<tr>"
                "<td align=left><font style=\"font-size:13pt;\"><b>Adı&nbsp;&nbsp;Soyadı&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;</b>"+
                    hesap_bilgileri.cari_hesap_ismi +"</font>"
                    "</td>"
                    "<td align=left><font style=\"font-size:13pt;\"><b>İşverenin Ünvanı  :&nbsp;&nbsp;&nbsp;&nbsp;</b>"+
                    firma_ismi +"&nbsp;&nbsp;&nbsp;" +"</font>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td align=left><font style=\"font-size:13pt;\"><b>SSK &nbsp;Sicil No&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;</b>"+
                    PRS_GET_PERSONEL_SICIL_NO( m_personel_id ) +"&nbsp;&nbsp;&nbsp;</font>"
                    "</td>"
                    "<td align=left><font style=\"font-size:13pt;\"><b>SSK &nbsp;&nbsp;İşyeri Numarası  :&nbsp;&nbsp;&nbsp;&nbsp;</b></font>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td align=left><font style=\"font-size:13pt;\"><b>İşe Giriş&nbsp;Tarihi&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;</b>" +
                        QDate::fromString( sql_query_kisisel.VALUE("ise_giris_tarihi").toString(), "yyyy.MM.dd" )
                        .toString("dd MMMM yyyy")+ "&nbsp;</font>"
                    "</td>"
                     "<td align=left><font style=\"font-size:13pt;\"><b>Adresi   :&nbsp;&nbsp;&nbsp;&nbsp;</b>&nbsp;</font>"+
                      firma_adresi +"&nbsp;&nbsp;&nbsp;</font>"
                    "</td>"
                "</tr>"
                "<tr>"
                "<td align=left><font style=\"font-size:13pt;\"><b>İşten Çıkış Tarihi: &nbsp;&nbsp;&nbsp;&nbsp;</b>" +
                        QDate::fromString(  sql_query_kisisel.VALUE("isten_ayrilma_tarihi").toString(), "yyyy.MM.dd" ).toString("dd MMMM yyyy")+ "&nbsp;</font>"
                    "</td>"
                "</tr>"
                "<tr>";


    query.PREPARE_SELECT("prs_isten_ayrilanlar",
                         "personel_id, isten_ayrilma_tarihi, "
                         "isten_ayrilma_sekli_text, calisma_suresi_yil, calisma_suresi_ay, "
                         "calisma_suresi_gun, ihbar_taz_var_mi, "
                         "net_ihbar_taz_tutari, brut_ihbar_taz_tutari, ihbar_taz_gelir_vergisi_tutari, "
                         "ihbar_taz_damga_vergisi_tutari, kidem_taz_var_mi, net_kidem_taz_tutari, "
                         "brut_kidem_taz_tutari, kidem_taz_damga_vergisi_tutari ",
                         "isten_ayrilan_id = :isten_ayrilan_id");

    query.SET_VALUE( ":isten_ayrilan_id", m_isten_ayrilan_id );

    if ( query.SELECT() NE 0 ) {

        query.NEXT();

        body += "<td align=left><font style=\"font-size:13pt;\"><b>İşten Ayrılış&nbsp;Şekli&nbsp;&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;</b>"+
                                query.VALUE("isten_ayrilma_sekli_text").toString()+
                                   "</font>"
                                "</td>"
                             "</tr>"
                          "</table>";

        body += "<br><font style=\"font-size:13pt;\"><b>Açıklama&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                                "&nbsp;&nbsp;&nbsp;:&nbsp;</b><br><br></font>"
                    "<table width=900 cellspacing=0 cellpadding=0 border=1>"
                    "<tr height=25 >"
                        "<td width=150 align=center><font style=\"font-size:11pt;\"><b>ÖDEMELER      </b></font></td>"
                        "<td width=150 align=center><font style=\"font-size:11pt;\"><b>BRÜT KAZANÇ   </b></font></td>"
                        "<td width=130 align=center><font style=\"font-size:11pt;\"><b>Gelir Vergisi </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:11pt;\"><b>Damga V       </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:11pt;\"><b>KESİNTİ       </b></font></td>"
                        "<td width=170 align=center><font style=\"font-size:11pt;\"><b>NET ÖDEME     </b></font></td>"
                     "</tr>"
                   "</table>";

        body +=
                    "<table width=900 cellspacing=0 cellpadding=0 border=1>"
                    "<tr height=25 >"
                        "<td width=150 align=center><font style=\"font-size:11pt;\"><b>Fazla Mesai Ücreti      </b></font></td>"
                        "<td width=150 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=130 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=170 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                    "</tr>"
                    "</table>"

                    "<table width=900 cellspacing=0 cellpadding=0 border=1>"
                    "<tr height=25 >"
                        "<td width=150 align=center><font style=\"font-size:11pt;\"><b>Ücreti      </b></font></td>"
                        "<td width=150 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=130 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                        "<td width=170 align=center><font style=\"font-size:11pt;\"><b> </b></font></td>"
                     "</tr>"
                   "</table>"

                    "<table width=900 cellspacing=0 cellpadding=0 border=1>"
                    "<tr height=25 >"
                        "<td width=150 align=center><font style=\"font-size:13pt;\"><b>Temsilcilik Ücreti      </b></font></td>"
                        "<td width=150 align=center><font style=\"font-size:13pt;\"><b> </b></font></td>"
                        "<td width=130 align=center><font style=\"font-size:13pt;\"><b> </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:13pt;\"><b> </b></font></td>"
                        "<td width=100 align=center><font style=\"font-size:13pt;\"><b> </b></font></td>"
                        "<td width=170 align=center><font style=\"font-size:13pt;\"><b> </b></font></td>"
                     "</tr>"
                   "</table>"

                   "<table width=900 cellspacing=0 cellpadding=0 border=1>"
                   "<tr height=25 >"
                       "<td width=150 align=center><font style=\"font-size:11pt;\"><b>İhbar Tazminatı      </b></font></td>"
                       "<td width=150 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("brut_ihbar_taz_tutari").toString(), 2, false) +"</b></font></td>"
                       "<td width=130 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("ihbar_taz_gelir_vergisi_tutari").toString(), 2, false) +" </b></font></td>"
                       "<td width=100 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("ihbar_taz_damga_vergisi_tutari").toString(), 2, false) +" </b></font></td>"
                       "<td width=100 align=center><font style=\"font-size:11pt;\"><b>"" </b></font></td>"
                       "<td width=170 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("net_ihbar_taz_tutari").toString(), 2,false) +" </b></font></td>"
                    "</tr>"
                  "</table>"

                  "<table width=900 cellspacing=0 cellpadding=0 border=1>"
                  "<tr height=25 >"
                      "<td width=150 align=center><font style=\"font-size:11pt;\"><b>Kıdem Tazminatı      </b></font></td>"
                      "<td width=150 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("brut_kidem_taz_tutari").toString(), 2, false) +" </b></font></td>"
                      "<td width=130 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(QVariant(0).toString(), 2, false) +" </b></font></td>"
                      "<td width=100 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("kidem_taz_damga_vergisi_tutari").toString(), 2, false) +" </b></font></td>"
                      "<td width=100 align=center><font style=\"font-size:11pt;\"><b>"" </b></font></td>"
                      "<td width=170 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("net_kidem_taz_tutari").toString(), 2, false) + " </b></font></td>"
                   "</tr>"
                 "</table>"

                 "<table width=900 cellspacing=0 cellpadding=0 border=1>"
                 "<tr height=25 >"
                     "<td width=150 align=center><font style=\"font-size:11pt;\"><b>TOPLAM      </b></font></td>"
                     "<td width=150 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(QVariant(query.VALUE("brut_ihbar_taz_tutari").toDouble() +
                                                                                      query.VALUE("brut_kidem_taz_tutari").toDouble() ).toString(), 2, false) +
                                                                                     "</b></font></td>"
                     "<td width=130 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(query.VALUE("ihbar_taz_gelir_vergisi_tutari").toString(), 2, false)+" </b></font></td>"
                     "<td width=100 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(QVariant(query.VALUE("ihbar_taz_damga_vergisi_tutari").toDouble() +
                                                                                       query.VALUE("kidem_taz_damga_vergisi_tutari").toDouble() ).toString(), 2, false) +
                                                                                     "</b></font></td>"
                     "<td width=100 align=center><font style=\"font-size:11pt;\"><b></b></font></td>"
                     "<td width=170 align=center><font style=\"font-size:11pt;\"><b>"+ VIRGUL_EKLE(QVariant(query.VALUE("net_ihbar_taz_tutari").toDouble() +
                                                                                       query.VALUE("net_kidem_taz_tutari").toDouble() ).toString(), 2, false) +" </b></font></td>"
                  "</tr>"
                "</table>";
    }

    body += "<table width=1250 height=100 cellspacing=0 cellpadding=0 border=0>"
                    "<tr>"
                        "<td>"
                            "<font style=\"font-size:13pt;\">""<br>""<br>" +QDate::fromString( sql_query_kisisel.VALUE("ise_giris_tarihi")
                             .toString(), "yyyy.MM.dd" ).toString("dd MMMM yyyy")+"&nbsp;&nbsp;&nbsp;"+
                              ibraname_madddeleri.replace("\n", "<br>").replace("\t", "&nbsp;&nbsp;&nbsp;") +
                            "</font>"
                        "</td>"
                    "</tr>"
                "</table><br>";

    m_rapor_bitti_mi = true;

    return body;
}

/**************************************************************************************
                   ISTEN_AYRILMA_RAPORU::GET_FOOTER
***************************************************************************************/

QString ISTEN_AYRILMA_RAPORU::GET_FOOTER ()
{
    SQL_QUERY       sql_query_adres        ( DB );
    QString         footer;



    CARI_HESAP_STRUCT hesap_bilgileri;
    CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

    CARI_KART_BILGILERINI_OKU( PRS_GET_CARI_HESAP_ID(m_personel_id), &hesap_bilgileri  );





    footer += "<table width=900 cellspacing=0 cellpadding=0 border=0>"
                    "<tr>"
                        "<td width=150 align=left>"
                        "<font style=\"font-size:13pt;\"> IBRA EDEN <br><br>""Adı Soyadı:<br> İmzası:<br> <br>"
                        "</font>"
                        "</td>"
                        "<td width=150 align=right>"
                        "<font style=\"font-size:13pt;\"> ""İşten Ayrılmadan İtibaren 6 Ay İçerisinde<br> Bulunacağı Tebligat Adresi""<br>Adresi:    "
                      //  sql_query_adres.VALUE("adres").toString().replace("\n", "<br>") +"<br>Telefon No:   <br>"

                        "</font>"
                        "</td>"
                    "</tr>"
                "</table><br>";

    return footer;
}
