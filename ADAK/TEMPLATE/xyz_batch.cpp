#include "xyz_batch_open.h"
#include "xyz_batch_class.h"
#include "ui_xyz_batch.h"
#include "print.h"

extern ADAK_SQL *           DB;

// Not - 1: Tum fonksiyonlar ve kullanim detaylari ayni zamanda batch_kernel.h dosyasindada mevcuttur.

// Not - 2: Eger pencerelerin tum isletim sistemlerinde duzgun tam ekran olmasini istiyorsak xxx.show() komutundan sonra
// setWindowState(Qt::WindowMaximized); komutunu cagirmaliyiz.Mdi kullaniyorsak ise SET_MDI_WINDOW dan sonra setWindowState(Qt::WindowMaximized);
// cagirmaliyiz.Tam ekran olmayanlarda ise max min sizelari designer dan ve ya koddan sabitleyerek resize olmasini engellemeliyiz.

// Not - 3: Arama ve Batch ekranlarinda kullanilan kernel button widgetlarin sizepolicyleri vertical ve horizontal icin fixed olmali.

////Bu fonksiyonla CLEAR_ALL_WIDGETS da clear lanmicak widgetlar setlenir.
//void                   DISABLE_CLEAR_ON_WIDGET              (QObject * widget);

/**************************************************************************************
                   OPEN_XYZ_BATCH
***************************************************************************************/

void OPEN_XYZ_BATCH ( QWidget * parent )
{
    XYZ_BATCH * F = new XYZ_BATCH ( parent );
    F->EXEC();
}

/**************************************************************************************
                   XYZ_BATCH::XYZ_BATCH
***************************************************************************************/

XYZ_BATCH::XYZ_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::XYZ_BATCH )
{
    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );
}

// TableWidget larda item changed yada double clicked eventlari check var da kontrol edilebilir.
// Gelen object daima table widget olacaktir.Bu yuzden objectname kontrol etmeliyiz mesela check var dusen bir table widget icin;
//    if (object EQ m_ui->tableWidget ) {
//        if (object.objectName() EQ "ITEM_CHANGED") {
//            item changed gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//        }
//        if (object.objectName() EQ "ITEM_DOUBLE_CLICKED") {
//            item double clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//        }
//        if (object.objectName() EQ "ITEM_CLICKED") {
//            item clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//        }
//    }

/**************************************************************************************
                   XYZ_BATCH::SETUP_FORM
***************************************************************************************/

void XYZ_BATCH::SETUP_FORM ()
{
    // Buton ismini setliyoruz.
    SET_NAME_OF_RUN_BATCH_BUTTON ( "xxxxyyyzzz" );

    // Programa ilk girildiginde focuslanacak widget setlenmelidir.
    SET_FIRST_FOCUS_WIDGET ( m_ui->xyz_widget );

    // Butonumuzun eklenecegi widgeti register ediyoruz.
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_PAGE_TITLE    (tr("BLA BLA EKRANI"));
    SET_SETTING_NAME  ("BLA_BLA_BATCH");
    SET_HELP_PAGE     ("bla_bla.html");

    //Tum lineeditlerin ve limitedtexteditlerin uzunlugu INIT_KERNEL dan sonra setlenmeli.Cunku bu widgetlarin default degeri 1 characterdir.
    //Ayrica eger bu widgetlara setlenmesi gereken degerler varsa bunu da uzunluklar setlendikten sonra yapmaliyiz.
    m_ui->line_edit_xyz->setMaxLength(25);
}

/**************************************************************************************
                   XYZ_BATCH::CHECK_VAR
***************************************************************************************/

int XYZ_BATCH::CHECK_VAR ( QObject * object )
{
    SQL_QUERY query (DB);

    if ( object EQ m_ui->dateedit_bts_tarihi ) {

        query.PREPARE_SELECT ("e9_sabit_degerler","muhasebe_yili_ilkgun,muhasebe_yili_songun");

        if (query.SELECT() NE 0 ) {
            query.NEXT();
            muhasebe_yili_ilkgun  = QDate::fromString (query.VALUE(0).toString(),"yyyy.MM.dd");
            muhasebe_yili_songun  = QDate::fromString (query.VALUE(1).toString(),"yyyy.MM.dd");
            if (rapor_bitis_tarihi < muhasebe_yili_ilkgun OR rapor_bitis_tarihi > muhasebe_yili_songun ) {
                MSG_WARNING(tr("Muhasebe Yılı ile Fiş Tarihinin Yılı birbirini tutmuyor,dönem dışı işlem yapamazsınız!.."), NULL);
                return ADAK_FAIL;
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_BATCH::CHECK_RUN
***************************************************************************************/

int XYZ_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_BATCH::RUN_BATCH
***************************************************************************************/

void XYZ_BATCH::RUN_BATCH ()
{
    // Burda mesela run batch butonuna tikladigimizda check var ve check empty den gectikten sonra run batch e duser
    // SHOW_PRINT_PREVIEW ekranini cagiririz.Bu ekran F_XYZ den fonksiyonundan donecek olan string i gosterir.

    SHOW_PRINT_PREVIEW ( OPEN_XYZ(), this );
}

/**************************************************************************************
                   XYZ_BATCH::CHECK_EXIT
***************************************************************************************/

int XYZ_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

// Search edit kullanilirsa bu fonksiyon tanimlanmali.Button a tikladigimizda buraya duser.Tek yapmamiz gereken degeri line edit e yazmak
// sonrasinda widgettan focus out olundugunda check var a duser.
//
/**************************************************************************************
                   XYZ_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void XYZ_BATCH::SEARCH_EDIT_CLICKED ( QWidget * , QLineEdit * line_edit)
{
    int xyz_id = OPEN_XYZ_ARAMA_EKRANI ( this );

    if ( xyz_id EQ -1 ) {
        return;
    }

    SQL_QUERY select_query (DB);

    select_query.PREPARE_SELECT("xyz_gruplari","xyz_kodu","xyz_id = :xyz_id");
    select_query.SET_VALUE(":xyz_id", xyz_id );

    if (select_query.SELECT() NE 0) {
        select_query.NEXT();
        line_edit->setText(select_query.VALUE(0).toString());
    }
}
