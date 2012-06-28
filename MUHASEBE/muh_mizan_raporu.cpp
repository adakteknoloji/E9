#include "adak_utils.h"
#include "muh_mizan_raporu_class.h"
#include "muh_mizan_raporu_open.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "yonetim.h"
#include "adak_gruplar.h"
#include "muh_console_utils.h"
#include "e9_console_utils.h"

extern ADAK_SQL *          DB;

/**************************************************************************************
                   GET_MIZAN_RAPORU
***************************************************************************************/

QString GET_MIZAN_RAPORU ( QList<int> grup_idleri,MUH_RAPOR_VARS * rapor_vars )
{
    MIZAN_RAPORU  *  R = new MIZAN_RAPORU ( grup_idleri,rapor_vars );
    return R->CREATE_HTML_PAGES();
}

/**************************************************************************************
               MIZAN_RAPORU::MIZAN_RAPORU
***************************************************************************************/

MIZAN_RAPORU::MIZAN_RAPORU ( QList<int> grup_idleri,MUH_RAPOR_VARS * rapor_vars ) : REPORT_KERNEL ( "MUH_MIZAN_RAPORU" )
{
    M_MUH_RV = rapor_vars;

    SET_HEIGHTS ( 30, 5 );

    M_MUH_HESAPLAR = MUH_GET_HESAPLAR ( M_MUH_RV->bas_hesap_kodu, M_MUH_RV->bts_hesap_kodu );

    m_hesap_idleri = GRP_GRUBA_EKLI_KAYITLARI_BUL(E9_PROGRAMI,MUHASEBE_MODULU,grup_idleri);

    YAZDIRILACAK_MUHASEBE_HESAPLARINI_BELIRLE ();

    m_max_line_count             =  GET_MAX_LINE_COUNT (11);
    m_footer_toplam_borc         =  0.00;
    m_footer_toplam_alacak       =  0.00;
    m_footer_borc_bakiye         =  0.00;
    m_footer_alacak_bakiye       =  0.00;
    m_hesap_indisi               =  0;
    m_hesaplar_bitti_mi          =  false;
    m_sayfa_numarasi             =  0;
}

/**************************************************************************************
                   MIZAN_RAPORU::GET_HEADER
***************************************************************************************/

QString MIZAN_RAPORU::GET_HEADER()
{
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% style=\"font-size:80%;\" BORDER=0 >" );

    SET_TD_TAG_STRING    ( QStringList()<<"WIDTH = 30%"<<"WIDTH=40% ALIGN=CENTER"<<"WIDTH=30% ALIGN=RIGHT" );

    CREATE_TABLE         ( QStringList()<<""<<""<<"",0,0 );

    ADD_ROW_TO_TABLE     ( QStringList() << E9_GET_FIRMA_ISMI()<< "" << GET_REPORT_TOP_RIGHT_HEADER() );

    m_sayfa_numarasi++;

    ADD_ROW_TO_TABLE     ( QStringList()<< "" << "" << "Sayfa No : " + QVariant ( m_sayfa_numarasi ).toString() );

    QString filters_1 = "";
    QString filters_2 = "";

    if ( filters_1.isEmpty() EQ true ) {
        if ( M_MUH_RV->bakiyesiz_hesaplar NE 0 ) {
            filters_1.append ( "Bakiyesiz" );
        }
        if ( M_MUH_RV->calismamis_hesaplar NE 0 ) {
            if ( filters_1.isEmpty() EQ false ) {
                filters_1.append (  "-" );
            }
            filters_1.append (  "Çalışmamış" );
        }
        if ( filters_1.isEmpty() EQ false ) {
            filters_1.append ( " Hesaplar" );
        }

        if ( M_MUH_RV->nakli_yekun EQ 1 ) {
            filters_1.append ( " Nakli Yekün" );
        }

        filters_2.append ( QString (  " Seviye : %1" ).arg ( M_MUH_RV->mizan_seviyesi )  );
        filters_2.append ( QString ( " Aralık : %1 - %2 " ).arg ( M_MUH_RV->bas_hesap_kodu, M_MUH_RV->bts_hesap_kodu )  );

        if ( m_grup_adi.isEmpty() EQ false ) {

            filters_2.append ( m_grup_adi );
        }
    }


    ADD_ROW_TO_TABLE ( QStringList()<< "" << "<b>MİZAN RAPORU<b>" << filters_1 );

    ADD_ROW_TO_TABLE ( QStringList() << "" << M_MUH_RV->baslangic_tarihi.toString ( "dd MMMM yyyy dddd" ) + " - "
                     + M_MUH_RV->bitis_tarihi.toString ( "dd MMMM yyyy dddd" ) << filters_2 );

    return GET_TABLE_HTML_STRING();
   
}

/**************************************************************************************
                   MIZAN_RAPORU::GET_BODY
***************************************************************************************/

QString MIZAN_RAPORU::GET_BODY()
{

    QStringList          STRING_LIST;
    QStringList          HEADERS;
    QStringList          HESAP_BILGILERI;


    if ( m_hesaplar_bitti_mi EQ true ) {
        return NULL;
    }

    if ( m_hesap_indisi >= M_YAZDIRILACAK_HESAPLAR.size() ) {
        return NULL;
    }
    
    SET_TABLE_TAG_STRING ( "<TABLE WIDTH = 100% BORDER=1 style=\"font-size:75%;\">" );
    HEADERS                         << "Hesap Kodu" << "Hesap İsmi" << "Borç" << "Alacak"
                                    << "Borç Bakiye" << "Alacak Bakiye";

    SET_TD_TAG_STRING    ( QStringList() << "WIDTH = 16%" << "WIDTH = 34%" << "WIDTH=12%" << "WIDTH=12%"
                                         << "WIDTH=12%" << "WIDTH=14%" );

    CREATE_TABLE ( HEADERS, 1, 13 );

    m_gonderilen_kayit_sayisi = 0;

    if ( M_YAZDIRILACAK_HESAPLAR.size() EQ 0 ) {
        return NULL;
    }

    for ( int i = m_hesap_indisi; i < M_YAZDIRILACAK_HESAPLAR.size(); i++ ) {

        SET_TD_TAG_STRING ( QStringList() << "" << "" << "align = RIGHT" << "align = RIGHT"
                                          << "align = RIGHT" << "align = RIGHT" );


        HESAP_BILGILERI = QVariant ( M_YAZDIRILACAK_HESAPLAR.at(i) ).toString().split ( "\t" );


        int         hesap_seviyesi          =   QVariant ( HESAP_BILGILERI.at(1) ).toInt();
        QString     tam_hesap_kodu          =   QVariant ( HESAP_BILGILERI.at(3) ).toString();
        QString     hesap_ismi              =   QVariant ( HESAP_BILGILERI.at(4) ).toString();
        double      hesap_toplam_borc       =   QVariant ( HESAP_BILGILERI.at(5) ).toDouble();
        double      hesap_toplam_alacak     =   QVariant ( HESAP_BILGILERI.at(6) ).toDouble();
        double      hesap_borc_bakiye       =   QVariant ( HESAP_BILGILERI.at(7) ).toDouble();
        double      hesap_alacak_bakiye     =   QVariant ( HESAP_BILGILERI.at(8) ).toDouble();
        int         toplam_bilgisine_ekle   =   QVariant ( HESAP_BILGILERI.at(9) ).toInt();



        if ( hesap_seviyesi > 1 ) {
            for ( int i = 1 ; i < hesap_seviyesi ; i++ ) {
                tam_hesap_kodu.prepend ( "&nbsp;&nbsp;" );
                tam_hesap_kodu.prepend ( "&nbsp;&nbsp;" );
            }
        }

        STRING_LIST.clear();

        if ( toplam_bilgisine_ekle EQ 1 ) {

        STRING_LIST << tam_hesap_kodu << hesap_ismi << VIRGUL_EKLE ( QVariant ( ROUND ( hesap_toplam_borc     ) ).toString() )
                                                    << VIRGUL_EKLE ( QVariant ( ROUND ( hesap_toplam_alacak   ) ).toString() )
                                                    << VIRGUL_EKLE ( QVariant ( ROUND ( hesap_borc_bakiye     ) ).toString() )
                                                    << VIRGUL_EKLE ( QVariant ( ROUND ( hesap_alacak_bakiye   ) ).toString() );
        }
        else {

            double  toplam_borc     = 0.00;
            double  toplam_alacak   = 0.00;
            double  borc_bakiyesi   = 0.00;
            double  alacak_bakiyesi = 0.00;

            UST_HESAPLARIN_BORC_ALACAGINI_HESAPLA ( tam_hesap_kodu, toplam_borc, toplam_alacak );

            if ( toplam_borc > toplam_alacak ) {
                borc_bakiyesi = toplam_borc - toplam_alacak;
            }
            else if ( toplam_alacak > toplam_borc ) {
                alacak_bakiyesi = toplam_alacak - toplam_borc ;
            }


            STRING_LIST << tam_hesap_kodu << hesap_ismi << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_borc     ) ).toString() )
                                                        << VIRGUL_EKLE ( QVariant ( ROUND ( toplam_alacak   ) ).toString() )
                                                        << VIRGUL_EKLE ( QVariant ( ROUND ( borc_bakiyesi   ) ).toString() )
                                                        << VIRGUL_EKLE ( QVariant(  ROUND ( alacak_bakiyesi ) ).toString() );

        }

        m_gonderilen_kayit_sayisi++;

        if ( hesap_seviyesi EQ 1 ) {
            M_BASILMIS_UST_HESAPLAR << tam_hesap_kodu;
        }

        ADD_ROW_TO_TABLE ( STRING_LIST );

        STRING_LIST.clear();

        m_hesap_indisi++;

        if ( toplam_bilgisine_ekle EQ 1 ) {

            m_footer_toplam_borc   += hesap_toplam_borc;
            m_footer_toplam_alacak += hesap_toplam_alacak;
            m_footer_borc_bakiye   += hesap_borc_bakiye;
            m_footer_alacak_bakiye += hesap_alacak_bakiye;
        }

        if ( m_gonderilen_kayit_sayisi EQ m_max_line_count ) {
           return GET_TABLE_HTML_STRING();
        }

        if ( m_hesap_indisi >= M_YAZDIRILACAK_HESAPLAR.size() ) {
            m_hesaplar_bitti_mi = true;
        }

    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   MIZAN_RAPORU::GET_FOOTER
***************************************************************************************/

QString MIZAN_RAPORU::GET_FOOTER()
{

    QStringList string_list;

    SET_TABLE_TAG_STRING ( "<TABLE WIDTH=100% BORDER=0 style=\"font-size:75%;\">" );

    SET_TD_TAG_STRING   ( QStringList()<<"WIDTH = 50%"<<"WIDTH=12% ALIGN = RIGHT"
                        <<"WIDTH=12% ALIGN=RIGHT"<<"WIDTH=12% ALIGN=RIGHT"<<"WIDTH=14% ALIGN=RIGHT" );

    CREATE_TABLE        ( QStringList()<<""<<""<<""<<""<<"",0,0 );

    string_list << "";
    string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_toplam_borc   ) ).toString() );
    string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_toplam_alacak ) ).toString() );
    string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_borc_bakiye   ) ).toString() );
    string_list << "<b>" + VIRGUL_EKLE ( QVariant ( ROUND ( m_footer_alacak_bakiye ) ).toString() );

    ADD_ROW_TO_TABLE ( string_list );

    return GET_TABLE_HTML_STRING();
}


/**************************************************************************************
                   MIZAN_RAPORU::YAZDIRILACAK_MUHASEBE_HESAPLARINI_BELIRLE
***************************************************************************************/

void MIZAN_RAPORU::YAZDIRILACAK_MUHASEBE_HESAPLARINI_BELIRLE ()
{
    double               hesap_toplam_borc      = 0.0;
    double               hesap_toplam_alacak    = 0.0;
    double               hesap_borc_bakiye      = 0.0;
    double               hesap_alacak_bakiye    = 0.0;

    QString              hesap_bilgisi;
    QString              muh_hesap;
    QStringList          HESAP_BILGILERI;

    double borc_bakiyesi   [ E9_ARRAY_SIZE ];
    double alacak_bakiyesi [ E9_ARRAY_SIZE ];

    int  bas_gun_no   = M_MUH_RV->bas_gun_no;
    int  bts_gun_no   = M_MUH_RV->bts_gun_no;


    for ( int i = 0; i < M_MUH_HESAPLAR.size(); i++ ) {

        muh_hesap                   = M_MUH_HESAPLAR.at(i);
        HESAP_BILGILERI             = muh_hesap.split("\t");


        int         hesap_id            = QVariant ( HESAP_BILGILERI.at(0) ).toInt();
        QString     tam_hesap_kodu      = QVariant ( HESAP_BILGILERI.at(1) ).toString();
        QString     hesap_ismi          = QVariant ( HESAP_BILGILERI.at(2) ).toString();
        int         parent_id           = QVariant ( HESAP_BILGILERI.at(3) ).toInt();
        int         hesap_seviyesi      = QVariant ( HESAP_BILGILERI.at(4) ).toInt();
        int         tali_hesap_mi       = QVariant ( HESAP_BILGILERI.at(5) ).toInt();

        if ( hesap_seviyesi > M_MUH_RV->mizan_seviyesi ) {
            continue;
        }

        if (m_hesap_idleri.isEmpty() EQ false) {
            if (m_hesap_idleri.contains(hesap_id) EQ false) {
                continue;
            }
        }

        UNPACK_DOUBLE_ARRAY ( QVariant ( HESAP_BILGILERI.at(6)).toString() , borc_bakiyesi   , E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( QVariant ( HESAP_BILGILERI.at(7)).toString() , alacak_bakiyesi , E9_ARRAY_SIZE );

        hesap_toplam_borc      = 0.0;
        hesap_toplam_alacak    = 0.0;
        hesap_borc_bakiye      = 0.0;
        hesap_alacak_bakiye    = 0.0;

        if ( M_MUH_RV->nakli_yekun EQ true) {
            hesap_toplam_borc   = borc_bakiyesi   [ bts_gun_no ];
            hesap_toplam_alacak = alacak_bakiyesi [ bts_gun_no ];
        }
        else {
            hesap_toplam_borc   = borc_bakiyesi   [ bts_gun_no ]   - borc_bakiyesi   [ bas_gun_no-1 ];
            hesap_toplam_alacak = alacak_bakiyesi [ bts_gun_no ]   - alacak_bakiyesi [ bas_gun_no-1 ];
        }

        if ( hesap_toplam_borc > hesap_toplam_alacak ) {
            hesap_borc_bakiye   = hesap_toplam_borc - hesap_toplam_alacak;
            hesap_alacak_bakiye = 0.00;
        }
        else if  ( hesap_toplam_alacak > hesap_toplam_borc ) {
            hesap_alacak_bakiye = hesap_toplam_alacak - hesap_toplam_borc;
            hesap_borc_bakiye   = 0.00;
        }

        if ( M_MUH_RV->bakiyesiz_hesaplar EQ false ) {
            if ( hesap_toplam_borc EQ hesap_toplam_alacak ) {
                if ( hesap_toplam_borc NE 0 AND hesap_toplam_alacak NE 0 )
                continue;
            }
        }

        if ( M_MUH_RV->calismamis_hesaplar EQ 0 ) {
            if ( alacak_bakiyesi [ bas_gun_no-1 ] EQ alacak_bakiyesi [ bts_gun_no ]
                AND borc_bakiyesi[ bas_gun_no-1 ] EQ borc_bakiyesi [ bts_gun_no ] ) {
                if ( ( hesap_toplam_borc EQ 0 ) AND ( hesap_toplam_alacak EQ 0 ) ) {
                    continue;
                    // Bu ikinci if çalışmamış hesaplar seçili değilken rapor alıdığında
                    // calışmamış olduğu halde bakiyesi olan hesapları rapora dahil etmek için kullanıldı.
                    // Aksi halde bu hesaplar rapora yansımayacağından mizanda sapma oluyor.
                }
            }
        }

        int toplam_bilgisine_ekle = 0;

        if ( ( hesap_seviyesi EQ M_MUH_RV->mizan_seviyesi ) OR ( tali_hesap_mi EQ 1 ) OR (M_MUH_HESAPLAR.size() EQ 1) ) {
            toplam_bilgisine_ekle = 1;
        }

        hesap_bilgisi = QVariant ( hesap_id).toString() + "\t" + QVariant ( hesap_seviyesi).toString() + "\t" +
                        QVariant ( parent_id).toString() + "\t" + tam_hesap_kodu + "\t" + hesap_ismi + "\t" +
                        QVariant ( hesap_toplam_borc).toString() + "\t" + QVariant ( hesap_toplam_alacak ).toString()
                        + "\t" + QVariant ( hesap_borc_bakiye ).toString() + "\t" + QVariant ( hesap_alacak_bakiye ).toString()
                        + "\t" + QVariant ( toplam_bilgisine_ekle ).toString();

        M_YAZDIRILACAK_HESAPLAR << hesap_bilgisi;
    }
}

/**************************************************************************************
                   MIZAN_RAPORU::UST_HESAPLARIN_BORC_ALACAGINI_HESAPLA
***************************************************************************************/

void MIZAN_RAPORU::UST_HESAPLARIN_BORC_ALACAGINI_HESAPLA ( QString p_tam_hesap_kodu, double &toplam_borc, double &toplam_alacak )
{
    QStringList     ALT_HESAPLAR;
    QStringList     HESAP_BILGILERI;

    QString tam_hesap_kodu = p_tam_hesap_kodu.remove ( "&nbsp;" );

    for ( int i = 0; i < M_YAZDIRILACAK_HESAPLAR.size(); i++ ) {

        HESAP_BILGILERI = M_YAZDIRILACAK_HESAPLAR.at(i).split ( "\t" );

        if ( QVariant(HESAP_BILGILERI.at(9)).toInt() EQ 0 ) {
            continue;
        }

        if ( QString ( HESAP_BILGILERI.at(3) ) EQ tam_hesap_kodu ) {
            continue;
        }


        if ( QString(HESAP_BILGILERI.at(3)).mid ( 0, tam_hesap_kodu.size() ) EQ tam_hesap_kodu ) {
            if ( ILK_HESAP_IKINCI_HESABIN_ALT_HESABI_MI ( HESAP_BILGILERI.at(3), p_tam_hesap_kodu ) EQ true ) {
                ALT_HESAPLAR.append ( M_YAZDIRILACAK_HESAPLAR.at(i) );
            }
        }
    }

    for ( int i = 0; i < ALT_HESAPLAR.size(); i++ ) {

        HESAP_BILGILERI =   QVariant(ALT_HESAPLAR.at(i)).toString().split("\t");

        if ( HESAP_BILGILERI.at(3) EQ tam_hesap_kodu ) {
            continue;
        }

        toplam_borc    +=   QVariant ( HESAP_BILGILERI.at(5) ).toDouble();
        toplam_alacak  +=   QVariant ( HESAP_BILGILERI.at(6) ).toDouble();
    }

}

/**************************************************************************************
                   MIZAN_RAPORU::ILK_HESAP_IKINCI_HESABIN_ALT_HESABI_MI
***************************************************************************************/

bool MIZAN_RAPORU::ILK_HESAP_IKINCI_HESABIN_ALT_HESABI_MI ( QString p_alt_hesap_tam_hesap_kodu, const QString p_parent_hesap_tam_hesap_kodu )
{

    QString     alt_hesap_parent_tam_hesap_kodu = "";
    int         alt_hesap_parent_id             = 0;
    int         parent_hesap_id                 = 0;

    QString     alt_hesap                       = MUH_FIND_AND_GET_HESAP_BILGILERI( p_alt_hesap_tam_hesap_kodu,    M_MUH_HESAPLAR );
    QString     parent_hesap                    = MUH_FIND_AND_GET_HESAP_BILGILERI( p_parent_hesap_tam_hesap_kodu, M_MUH_HESAPLAR );

    QStringList ALT_HESAP_BILGILERI    = alt_hesap.split ( "\t" );
    QStringList PARENT_HESAP_BILGILERI = parent_hesap.split ( "\t" );

    if ( QVariant ( ALT_HESAP_BILGILERI.at(4) ).toInt() <= QVariant ( PARENT_HESAP_BILGILERI.at(4) ).toInt() ) {
        return false;
    }

    if ( PARENT_HESAP_BILGILERI.size() > 0 ) {
        parent_hesap_id = QVariant( PARENT_HESAP_BILGILERI.at(0)).toInt();
    }
    else {
        return false;
    }

    if ( ALT_HESAP_BILGILERI.size() > 0 ) {
        alt_hesap_parent_id             = QVariant (  ALT_HESAP_BILGILERI.at(3) ).toInt();
        alt_hesap                       = MUH_FIND_AND_GET_HESAP_BILGILERI( alt_hesap_parent_id, M_MUH_HESAPLAR );
        ALT_HESAP_BILGILERI             = alt_hesap.split ( "\t" );
        alt_hesap_parent_tam_hesap_kodu = ALT_HESAP_BILGILERI.at(1);

        if ( parent_hesap_id EQ alt_hesap_parent_id ) {
            return true;
        }
    }
    else {
        return false;
    }

    if ( alt_hesap_parent_id EQ 0 ) {
        return false;
    }

    return ILK_HESAP_IKINCI_HESABIN_ALT_HESABI_MI ( alt_hesap_parent_tam_hesap_kodu, p_parent_hesap_tam_hesap_kodu );

}
