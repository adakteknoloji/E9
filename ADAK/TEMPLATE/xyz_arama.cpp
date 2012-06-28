#include <adak_utils.h>
#include <adak_sql.h>
#include "xyz_arama.h"
#include "ui_xyz_arama.h"

extern ADAK_SQL * DB;


// Not - 1: Tum fonksiyonlar ve kullanim detaylari ayni zamanda arama_kernel.h dosyasindada mevcuttur.

// Not - 2: Eger pencerelerin tum isletim sistemlerinde duzgun tam ekran olmasini istiyorsak xxx.show() komutundan sonra
// setWindowState(Qt::WindowMaximized); komutunu cagirmaliyiz.Mdi kullaniyorsak ise SET_MDI_WINDOW dan sonra setWindowState(Qt::WindowMaximized);
// cagirmaliyiz.Tam ekran olmayanlarda ise max min sizelari designer dan ve ya koddan sabitleyerek resize olmasini engellemeliyiz.

// Not - 3: Arama da default olarak dialog da kullanilmasi gereken grid kesinlikle QGridLayout aksi takdirde crash olur.

// Not - 4: Arama ekranlarinda kriter alanlarinin enable/disable yapan checkboxlarin kontrolu check_var da yapilmali.

// Not - 4: Arama ve Batch ekranlarinda kullanilan kernel button widgetlarin sizepolicyleri vertical ve horizontal icin fixed olmali.

/**************************************************************************************
                   OPEN_XYZ_ARAMA
***************************************************************************************/

int OPEN_XYZ_ARAMA ( int xxxx, QWidget * parent )
{
    int    secilen_id    = -1;

    XYZ_ARAMA *  F  = new XYZ_ARAMA( xxxx, parent);

    F->m_secilen_id   = &secilen_id;
    F->EXEC();

    return secilen_id;
}

/**************************************************************************************
                   XYZ_ARAMA::XYZ_ARAMA
***************************************************************************************/

XYZ_ARAMA::XYZ_ARAMA ( int xxxx, QWidget * parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::XYZ_ARAMA )
{
    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );

}

/**************************************************************************************
                   XYZ_ARAMA::SETUP_FORM ()
***************************************************************************************/

int XYZ_ARAMA::SETUP_FORM ()
{
    // burada arama kernelin ihtiyac duydugu widgetlar register e edilir.

    // Sonuclarin gosterilecegi table widget burda register edilmelidir.
    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );

    // Burda arama butonu register edilmelidir.
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    // table widgettaki headerlar burda setlenir
    SET_HEADERS ( QStringList() << tr("Header 0") << tr("Header 1") << tr("Header 2") << tr("Header 3") << tr("Header 4")<<tr("Header 5"));

    SET_SORTING ( false );

    // Programa ilk girildiginde focuslanacak widget setlenmelidir.
    SET_FIRST_FOCUS_WIDGET ( m_ui->xyz_widget );

    SET_PAGE_TITLE    (tr("BLA BLA EKRANI"));

    SET_HELP_PAGE     ("bla_bla.html");

    m_ui->tableWidget->setColumnWidth( 3   , 10);

    SET_SETTING_NAME  ("BLA_BLA_ARAMA");

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME ( "xxyyzz Bul" );
   
    //Tum lineeditlerin ve limitedtexteditlerin uzunlugu INIT_KERNEL dan sonra setlenmeli.Cunku bu widgetlarin default degeri 1 characterdir.
    //Ayrica eger bu widgetlara setlenmesi gereken degerler varsa bunu da uzunluklar setlendikten sonra yapmaliyiz.
    m_ui->line_edit_xyz->setMaxLength(25);    
}

/**************************************************************************************
                   XYZ_ARAMA::CHECK_VAR
***************************************************************************************/

int XYZ_ARAMA::CHECK_VAR ( QObject * object )
{

    // Arama ekranlarinda kriter alanlarinin enable/disable yapan checkboxlarin kontrolu check_var da yapilmali.
    if ( object EQ m_ui->date_edit_second_date ) {
        if ( m_ui->date_edit_first_date->date() > m_ui->date_edit_second_date->date() ) {
            MSG_ERROR(tr("Son tarih ilk tarihten küçük olamaz!..") , NULL);
            return ADAK_FAIL;
        }
        if ( rapor_bitis_tarihi < muhasebe_yili_ilkgun OR rapor_bitis_tarihi > muhasebe_yili_songun ) {
            MSG_ERROR(tr("Muhasebe Yılı ile Fiş Tarihinin Yılı birbirini tutmuyor,dönem dışı işlem yapamazsınız!.."), NULL);
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_ARAMA::CHECK_RUN
***************************************************************************************/

int XYZ_ARAMA::CHECK_RUN ()
{
    if ( m_ui->date_check_box->isChecked() EQ false AND m_ui->fis_aciklama_checkbox->isChecked() EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_ARAMA::SEARCH
***************************************************************************************/

void XYZ_ARAMA::SEARCH ()
{
    // Filter lere gore sorgumuzu olusturup gelen sonuclari table widget a yaziyoruz.
    QString select_query = ".........................';

    SQL_QUERY sql_query      ( DB );
    sql_query.PREPARE_SELECT ( select_query );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    int current_row;

    while ( sql_query.NEXT() EQ true ) {

        // ADD_NEW_ROW table widgeta yeni bir satir ekler ve son ekledigi satirin row numarasini bize dondurur.
        // bizde bu satirlari doldururuz.
        current_row = ADD_NEW_ROW();

        // Tanimlamis oldugumuz header qstringlist inin size i kadar column yaratilir.
        m_ui->tableWidget->item(current_row,0)->setText(sql_query.VALUE(0).toString());
        m_ui->tableWidget->item(current_row,1)->setText(..........);
        m_ui->tableWidget->item(current_row,2)->setText(..........);
        m_ui->tableWidget->item(current_row,3)->setText(..........);
        m_ui->tableWidget->item(current_row,4)->setText(..........);
        m_ui->tableWidget->item(current_row,5)->setText(..........);
        m_ui->tableWidget->item(current_row,6)->setText(..........);
    }
}

/**************************************************************************************
                        XYZ_ARAMA::SELECTED_LINE
***************************************************************************************/

void XYZ_ARAMA::SELECTED_LINE ( int selected_row_number )
{
    // Satırlardan herhangi biri tıklandığında bu fonksiyona tıklanan satır numarası ile düşer.
    // Tıklanan satıra ait detaylı bilgi gösterimi gibi işlemler burada yapılabilir.

    m_ui->textEdit_yapilan_islem->setText ( satir_detaylari );
}

/**************************************************************************************
                   XYZ_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int XYZ_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    // Buraya kullanici bir sonucu double click yaptiginda duser ve donus degerine gore surec devam eder.
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number, 0 )->text().toInt();
    return ADAK_EXIT;
}
