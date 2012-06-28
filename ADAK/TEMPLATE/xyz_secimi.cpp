#include <QTableWidget>
#include "adak_sql.h"
#include "xyz_secimi_class.h"
#include "xyz_secimi_open.h"
#include "secim_kerneli.h"

extern ADAK_SQL *           DB;


// Not : Tum fonksiyonlar ve kullanim detaylari ayni zamanda secim_kerneli.h dosyasindada mevcuttur.


/**************************************************************************************
                   XYZ_SEC
***************************************************************************************/

int XYZ_SEC ( QWidget * parent )
{
    int          secilen_id = -1;
    XYZ_SECIMI * F = new XYZ_SECIMI ( parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC();
    return ( secilen_id );
}

/**************************************************************************************
                   XYZ_SECIMI::XYZ_SECIMI
***************************************************************************************/

XYZ_SECIMI::XYZ_SECIMI( QWidget * parent ) : SECIM_KERNELI ( parent )
{
    // Donen adresi BUTTON_CLICKED kullandik.Eger icon ve size eklenmezse default olarak ekle ikonu gelir ve size da
    // layouta uygun olarak ayarlanir.
    //
    // Ornek; ADD_BUTTON ("Yeni Kayit",Qt::ToolButtonTextUnderIcon,":/kernel_icons/kernel_ekle_ikonu.png",30,30);

    button_yeni_xxyyzz = ADD_BUTTON ("Yeni XXXXX");

    // otomatik sorting ayarlar.Default deger true dur.False gonderilirse otomatik olarak siralama yapilmaz.
    // SET_SORTING(false);
    SET_WINDOW_SIZE ( 300, 800 );
    SET_HEADERS (QStringList() << "Fis Id" << "Fis No" << "Fis Tarihi" << "Aciklama" << "Toplam Borc" << "Toplam Alacak" << "Hesap Kodu" << "Fis Turu" );

    SET_PAGE_TITLE    ( tr ( "BLA BLA SEÇİMİ" ) );
    SET_SETTING_NAME  ( "BLA_BLA_SECIMI" );
    SET_HELP_PAGE     ( tr("bla_bla_secimi.html") );

    INIT_KERNEL ( DB );

    SET_COLUMN_WIDTH ( 2, 200 );
    /*
     * SET_COLUMN_WIDTH fonksiyonu init kernel dan sonra kullanilmali.Cunku atanan degerlere gore table
     * init kernel da yaratiliyor.Eger init kerneldan once bu fonksiyon cagrilirsa table henuz
     * yaratilmadigindan crash olur.
     */
}

/**************************************************************************************
                   XYZ_SECIMI::FILL_TABLE
***************************************************************************************/

void XYZ_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT ( "muhasebe_fisleri","fis_id, fis_no, fis_tarihi ,aciklama, toplam_borc, toplam_alacak, fis_turu" );

    if( query.SELECT() EQ 0 ) {
        return;
    }

    while (query.NEXT()) {

        current_row = ADD_NEW_ROW();

        int     fis_turu = result.value ( "fis_turu" ).toInt();
        QString fis_turu_str ;

        table_widget->item(current_row,0)->setText ( query.VALUE ( "fis_id" ).toString());
        table_widget->item(current_row,1)->setText ( query.VALUE ( "fis_no" ).toString());
        table_widget->item(current_row,1)->setTextAlignment ( Qt::AlignRight );
        table_widget->item(current_row,2)->setText ( query.VALUE ( "fis_tarihi" ).toDate().toString("dd MMMM yyyy dddd"));
        table_widget->item(current_row,3)->setText ( query.VALUE ( "aciklama").toString());
        table_widget->item(current_row,4)->setText ( VIRGUL_EKLE ( query.VALUE( "toplam_borc").toString()));
        table_widget->item(current_row,5)->setText ( VIRGUL_EKLE ( query.VALUE ( "toplam_alacak" ).toString()));
        table_widget->item(current_row,6)->setText ( query.VALUE ( "kasa_hesabi_id" ).toString());
        table_widget->item(current_row,7)->setText ( fis_turu_str );

    }
}

/**************************************************************************************
                   XYZ_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int XYZ_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number, column_number )->text().toInt();
    return ADAK_EXIT;
}

/**************************************************************************************
                     XYZ_SECIMI::BUTTON_CLICKED
***************************************************************************************/

void XYZ_SECIMI::BUTTON_CLICKED(QToolButton * button)
{
    if (button EQ button_yeni_xxyyzz) {
        OPEN_HIZMET_FORMU (this);
        // Kayit eklendikten sonra table refresh edilmeli
        REFRESH_TABLE();
    }
}
