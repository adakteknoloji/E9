#include "adak_sql.h"
#include "ui_xyz_fisi.h"
#include "xyz_fisi_class.h"
#include "xyz_fisi_open.h"

extern ADAK_SQL *           DB;


// Not : Fis ve form kernellarinda kullanilan kernel button widgetlarinin horizontal sizepolicy expanding vertical size policy fixed olmali.

// Her bir satir icin tek tek columnlar define edilebilir boylelikle daha rahat columnlara erisebiliriz.
// Asagida basit bir ornegini gorebilirsiniz.Burada genelikle ROW_ID_COLUMN,FIS_ID_COLUMN ve ORDER_NUMBER_COLUMN ihtiyac duyariz.
#define ROW_ID_COLUMN                  0  // Satirin idsini tutacak column
#define FIS_ID_COLUMN                  1  // Satirin bagli oldugu fisin idsi
#define ORDER_NUMBER_COLUMN            2  // Satirlari siralamak amacli kullanilacak column
#define XYZ_ADI_COLUMN                 3
#define XYZ_SOYADI_COLUMN              4
#define XYZ_...                        5
  .
  .
  .

// Bu fisi cagirmak icin kullanacagimiz fonksiyon.Burda modal lik icin parent i aldik.
// Burdaki diger paremetre ise id dir buda direk kerneli bir kayitla baslatmak istedigimiz zaman kullanacagimiz deger.
//


/**************************************************************************************
                   OPEN_XYZ_FISI
***************************************************************************************/

void OPEN_XYZ_FISI ( int xyz_id, QWidget * parent )
{
    XYZ_FISI * F = new XYZ_FISI ( xyz_id, parent );
    F->setWindowState ( Qt::WindowMaximized );
    F->EXEC();
}

/**************************************************************************************
                   XYZ_FISI::XYZ_FISI
***************************************************************************************/

XYZ_FISI::XYZ_FISI(int record_id, QWidget * parent) :FIS_KERNEL(parent) m_ui ( new Ui::XYZ_FISI )
{
    m_ui->setupUi    ( this );        
    START_FIS_KERNEL ( this, DB );
 }
 
 /**************************************************************************************
                   XYZ_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void XYZ_FISI::SETUP_FORM()
{
    // Single record mode setlenirse sadece yenile ve kaydet butonlari aktif olur.
    // record mode atamasi init kerneldan once yapilmalidir.Cunku gui init kernelda olusturulur.
    // single record mode da database de en az 1 kayit oldugu kabul edilir.bu yuzden yoksa bir kere yaratilmalidir.
    //
    // SET_SINGLE_RECORD_MODE (record_id)


    // Kernel butonlarinin bulunacagi widget lar setlenir.
    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    // Kernelin kullanacagi widget ve column sayisi setlenir.
    REGISTER_TABLE_WIDGET ( m_ui->table_widget_xyz, 6 );

    // Satir siralama yani order numaralarinin yazilacagi column setlenir.
    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_NUMBER_COLUMN );

    // Satirin bagli olacagi fisin numarasini tutacak column setlenir.
    SET_FORM_ID_COLUMN_POSITION ( FIS_ID_COLUMN );

    // Satirin idsini tutacak olan column setlenir.
    SET_ROW_ID_COLUMN_POSITION ( ROW_ID_COLUMN );

    // Kac columnin hidelanacagi setlenir.Burda dikkat edilmesi gereken verilen deger kadar ilk columnlar hidelanir.
    // Mesela 3 ise 0,1,2 numarali columnlar hidelanacaktir.
    SET_NUMBER_OF_HIDDEN_COLUMN ( 3 );

    // Bu fonksiyon ise kernelin otomatik olarak satir order numlarin reorganize edilmesi saglar.
    // Burda kernela database bilgilerini setleyerek bu isi yapabiliriz.
    SET_FIS_ORDER_DATABASE ("xyz_bilgileri", "order_number", "xyz_bilgisi_id" );

    // Sirasiyla her columna gelecek widgetlari setliyoruz.Parametre olarak column numarasini ve widget in turunu gonderiyoruz.
    SET_TABLE_ROW_WIDGETS ( XYZ_ADI_COLUMN     , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( XYZ_SOYADI_COLUMN  , WIDGET_LINE_EDIT );
    SET_TABLE_ROW_WIDGETS ( XYZ_...            , WIDGET_COMBO_BOX );
    SET_TABLE_ROW_WIDGETS ( ...                , WIDGET_DATE_EDIT );

    // Kernelin ilk acilis ve yeni kayit durumlarinda hangi widgeta focuslanilmasi isteniyorsa o setlenmelidir.
    SET_FIRST_FOCUS_WIDGET (widget_..._xyz);

    SET_PAGE_TITLE    (tr("BLA BLA EKRANI"));
    SET_SETTING_NAME  ("BLA_BLA_FISI");
    SET_HELP_PAGE     ("bla_bla.html");
       
    //Tum lineeditlerin ve limitedtexteditlerin uzunlugu INIT_KERNEL dan sonra setlenmeli.Cunku bu widgetlarin default degeri 1 characterdir.
    //Ayrica eger bu widgetlara setlenmesi gereken degerler varsa bunu da uzunluklar setlendikten sonra yapmaliyiz.
    m_ui->line_edit_xyz->setMaxLength(25);

    // Column headerlari setlenir.Dikkat burda hidelanacak columnlar(ornegimizde 3 tane row_id,fis_id ve order_column)
    // ve son da kernelin ekleyecegi 2(son 2 column "" olan) column icinde header ismi atanmali.
    m_ui->table_widget_xyz->setHorizontalHeaderLabels (QStringList() << "row_id" << "fis_id" << "order_number_column" << tr("XYZ Adı") << tr("XYZ Soyadı")
                                                                     << tr("XYZ ...") << tr("") << tr("") );

    m_ui->table_widget_xyz->setColumnWidth(XYZ_ADI_COLUMN     , 999);
    m_ui->table_widget_xyz->setColumnWidth(XYZ_SOYADI_COLUMN  , 999);
    m_ui->table_widget_xyz->setColumnWidth(...                , 999);


    // Normal de kernel otomatik olarak widgeta focuslanir ancak INIT_FIS_KERNEL dan sonra tablewidgettla oynadigimiz dan focus
    // table widget a gecer bu yuzden tekrar focusu first widget a veririz.
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_muhasebe_ayraci);
    FOCUS_FIRST_WIDGET ();;
}

/**************************************************************************************
                   XYZ_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void XYZ_FISI::CLEAR_FORM_MEMBERS()
{
    m_grup_idleri.clear();
}

/**************************************************************************************
                   XYZ_FISI::NEW_FIS_RECORD
***************************************************************************************/

void XYZ_FISI::NEW_FIS_RECORD()
{
    ////Bu fonksiyonla CLEAR_ALL_WIDGETS da clear lanmicak widgetlar setlenir.
    //void                   DISABLE_CLEAR_ON_WIDGET              (QObject * widget);
  
}

/**************************************************************************************
                   XYZ_FISI::GET_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY   query(DB);

    // Fisimizi okuyoruz.
    query.PREPARE_SELECT ( "xyz_bilgileri","xyz_kodu, xyz_..., ... ","xyz_id  = :xyz_id " );
    query.SET_VALUE ( ":xyz_id", record_id );

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    // Fis bilgilerini setliyoruz.
    m_ui->line_edit_xyz_kodu->setText ( query.VALUE(0).toString() );
    m_ui->line_edit_xyz...->setText ( query.VALUE(1).toString() );

    // Fisin satirlarini okuyoruz.Siralayarak okuyoruz.Burda dikkat edilmesi gereken siralamanin order num a gore yapilmasi gerekir.
    // Boylelikle satirlar uygun yerlestirilmis olur.
    query.PREPARE_SELECT ( "xyz_fis_satirlari","xyz_adi, xyz_soyadi, xyz_...,row_id, order_number","parent_id = :parent_id" );
    query.SET_VALUE      ( ":parent_id", record_id );

    if ( query.SELECT("order_number") EQ 0 ) {
        return ADAK_OK;
    }

    // Fis satirlarimizi ekliyoruz.

    int current_row  ;
    while ( query.NEXT() EQ true ) {
        current_row = ADD_NEW_LINE ();

        QLineEdit  *   line_edit   = ( QLineEdit * ) m_ui->table_widget_xyz->cellWidget( current_row, XYZ_ADI_COLUMN );
        line_edit->setText ( query.VALUE("xyz_adi").toString());
        line_edit   = ( QLineEdit * ) m_ui->table_widget_xyz->cellWidget( current_row, XYZ_SOYADI_COLUMN);
        line_edit->setText(query.VALUE("xyz_soyadi").toString());
        QComboBox * combo_box   = ( QComboBox * ) m_ui->table_widget_xyz->cellWidget( current_row, XYZ_...);
        combo_box->setEditText(query.VALUE("xyz_...").toString());

    QTableWidgetItem * new_item;

    // new_item = new QTableWidgetItem(tr("%1").arg(...));
    // table_widget_xyz->setItem(current_row, ..._COLUMN, new_item);

    // hide edilmis columnlarimizi setliyoruz.
        QString row_id       = query.VALUE ("row_id").toString();
        QString order_number = query.VALUE ("order_number").toString();

        new_item = new QTableWidgetItem ( tr("%1").arg(row_id) );
        m_ui->table_widget_xyz->setItem (current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( tr("%1").arg(record_id) );
        m_ui->table_widget_xyz->setItem ( current_row, FIS_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( tr("%1").arg(order_number) );
        m_ui->table_widget_xyz->setItem ( current_row, ORDER_COLUMN, new_item );

    }

    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void XYZ_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    // Her satir eklendikten sonra direk buraya duser o satirlarla ilgili eklendikten sonra
    // ayarlamak istedigimiz degisklik varsa onlari burda yapioruz.
}

// TableWidget larda item changed yada double clicked eventlari check var da kontrol edilebilir.
// Gelen object daima table widget olacaktir.Bu yuzden objectname kontrol etmeliyiz mesela check var dusen bir table widget icin;
//     if (object EQ m_ui->tableWidget ) {
//         if (object.objectName() EQ "ITEM_CHANGED") {
//             item changed gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//         }
//         if (object.objectName() EQ "ITEM_DOUBLE_CLICKED") {
//             item double clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//         }
//         if (object.objectName() EQ "ITEM_CLICKED") {
//             item clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//         }
//     }

/**************************************************************************************
                   XYZ_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int XYZ_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{

    // Widgetlarda focus out olundugunda buraya duser. Burda o widgeta yazilan degerin dogrulugunu kontrol ediyoruz.
    if (object EQ m_ui->line_edit_xyz) {
        if (m_ui->line_edit_xyz->text().isEmpty() EQ true ) {
      MSG_ERROR(tr("Hata ....") , NULL);
    }
    }
    else if (object EQ m_ui->comma_edit_xyz) {
        if (m_ui->comma_edit_xyz->text().isEmpty() EQ true ) {
      MSG_ERROR(tr("Hata ....") , NULL);
    }
    }
    .
    .
    .

    // TableWidget larda item changed yada double clicked eventlari check var da kontrol edilebilir.
    // Gelen object daima table widget olacaktir.Bu yuzden objectname kontrol etmeliyiz mesela check var dusen bir table widget icin;
    //     if (object EQ m_ui->tableWidget ) {
    //         if (object.objectName() EQ "ITEM_CHANGED") {
    //             item changed gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
    //         }
    //         if (object.objectName() EQ "ITEM_DOUBLE_CLICKED") {
    //             item double clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
    //         }
    //         if (object.objectName() EQ "ITEM_CLICKED") {
    //             item clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
    //         }
    //     }

    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int XYZ_FISI::CHECK_FIS_FORM_EMPTY()
{
    // Kaydetmeden veya guncellemeden once formda bos birakilamicak alanlari kontrol ediyoruz
    if ( m_ui->line_edit_xyz_ad->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("Ad boş bırakılamaz"), m_ui->line_edit_xyz_ad);
        return ADAK_FAIL;
    }
    else if ( m_ui->line_edit_xyz_soyad->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("Soyad boş bırakılamaz"), m_ui->line_edit_xyz_soyad);
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int XYZ_FISI::CHECK_FIS_RECORD_ADD ()
{
  // Burda fisimizi add etmemize engel durum olup olmadigina bakacaz.
  // Eger sorun cikarsa ADAK_FAIL; Aksi takdirde ADAK_OK dondurulmeli.
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::ADD_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::ADD_FIS_RECORD ()
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_INSERT ( "xyz_bilgileri","xyz_id", "xyz_kodu, xyz_..., ...");

    sql_query.SET_VALUE ( ":xyz_kodu" , m_ui->line_edit_xyz_kodu->text() );
    sql_query.SET_VALUE ( ":xyz_..."  , m_ui->line_edit_xyz_...->text() );
    sql_query.SET_VALUE ( ":..."      , m_ui->line_edit_...->text() );

    return sql_query.INSERT();
}

/**************************************************************************************
                   XYZ_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int XYZ_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
  // Burda fisimizi update etmemize engel durum olup olmadigina bakacaz.
  // Eger sorun cikarsa ADAK_FAIL; Aksi takdirde ADAK_OK dondurulmeli.
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void XYZ_FISI::UPDATE_FIS_RECORD ( int record_id )
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_INSERT ( "xyz_bilgileri","id", "xyz_kodu, xyz_..., ... " );

    sql_query.SET_VALUE ( ":xyz_kodu" , m_ui->line_edit_xyz_kodu->text() );
    sql_query.SET_VALUE ( ":xyz_..."  , m_ui->line_edit_xyz_...->text() );
    sql_query.SET_VALUE ( ":..."      , m_ui->line_edit_...->text() );

    sql_query.UPDATE();
}

/**************************************************************************************
                   XYZ_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int XYZ_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    // Kaydi silip silemeyecegimizi bakariz.
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void XYZ_FISI::DELETE_FIS_RECORD ( int record_id )
{
    // kaydi sileriz.Fisten sonra onun satirlarininda silinmesi gereklidir.

    SQL_QUERY query (DB);

    query.PREPARE_DELETE ( "xyz_bilgileri" ,"fis_id = :fis_id" );
    query.SET_VALUE ( ":fis_id", fis_id );
    query.DELETE();

    query.PREPARE_DELETE ( "xyz_fis_satirlari","fis_id = :fis_id" );
    query.SET_VALUE ( ":fis_id"   , fis_id );
    query.DELETE();
}

/**************************************************************************************
                   XYZ_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::SELECT_FIS_RECORD()
{
    // Bul butotuna basilinca buraya duser bizde bir ekran falan acarak kayit sectirip kullanici ordan gelen idyide kernel a return ederiz.
    int id = OPEN_XYZ_ARAMA ( this )
    return id;
}

/**************************************************************************************
                   XYZ_FISI::FIND_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY   query(DB);

    query.PREPARE_SELECT ( "adr_sahis_bilgileri", "sahis_id","sahis_adi = :sahis_adi AND sahis_soyadi = :sahis_soyadi)" );
    query.SET_VALUE      ( ":sahis_adi"   , m_ui->line_edit_ad->text() );
    query.SET_VALUE      ( ":sahis_soyadi", m_ui->line_edit_soyad->text() );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   XYZ_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::FIND_FIRST_FIS_RECORD()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore deil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

    // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini degil

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT       ( "xyz_fisler", "fis_id","fis_turu = :fis_turu ");

    query.SET_VALUE            ( ":fis_turu"   ,  xyz_fis_turu    );

    if ( query.SELECT("fis_tarihi, fis_numarasi",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   XYZ_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::FIND_LAST_FIS_RECORD()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore degil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

   // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini deil

    // Sadece bir id ye ihtiyacimiz var bu yuzden limit select yaptik.Daha hizli query icin gereklidir.

    SQL_QUERY query ( DB );

    query.PREPARE_LIMIT_SELECT ( "xyz_fisler","fis_id","fis_turu = :fis_turu");

    query.SET_VALUE            (  ":fis_turu"   ,  m_fis_turu    );

    if ( query.SELECT("fis_tarihi DESC, fis_numarasi DESC",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();

/**************************************************************************************
             XYZ_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::FIND_PREV_FIS_RECORD()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore deil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

    // Onceki ve sonraki kayitlarda formdan siralama kriterini alarak ona gore siralama yapmaliyiz.Mesela burda ogrenci no.
    // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini değil


    // Sadece bir id ye ihtiyacimiz var bu yuzden limit select yaptik.Daha hizli query icin gereklidir.

    // Eger fisler tarih ve numara bilgilerini iceriyorsa, onceki veya sonraki kayitlar bulunulan tarih icinde
    // numaraya gore siralanmalidir. O gune ait son fise gelindiginde, sonraki ve onceki gunun
    // en kucuk numarali fisinden bakilarak fisler getirilmelidir. Asagida ikinci sorgunun yapilma sebebi budur
    // ve bu sorgu, sonraki veya onceki kayit butonuna basildiginda, icinde bulunulan tarihe ait fisin olmamasi
    // durumunda gerceklesir.


    SQL_QUERY  query ( DB );

    query.PREPARE_LIMIT_SELECT ( "xyz_fisler","fis_id","fis_turu   = :fis_turu "
                                 "AND fis_tarihi   = :fis_tarihi "
                                 "AND fis_numarasi < :fis_numarasi",0,1,  );

    query.SET_VALUE            ( ":fis_turu"       , xyz_fis_turu );
    query.SET_VALUE            ( ":fis_tarihi"     , m_ui->dateedit_fis_tarihi->date().toString ( "yyyy.MM.dd" ) );
    query.SET_VALUE            ( ":fis_numarasi"   , m_ui->line_edit_fis_no->text().toInt()                      );

    if ( query.SELECT("fis_tarihi DESC, fis_numarasi DESC",0,1) > 0 ) {
        query.NEXT();
        return query.VALUE(0).toInt();
    }

    query.PREPARE_LIMIT_SELECT ( "xyz_fisler","fis_id","fis_turu   = :fis_turu "
                                 "AND fis_tarihi   < :fis_tarihi " );

    query.SET_VALUE     (  ":fis_turu"      , xyz_fis_turu );
    query.SET_VALUE     (  ":fis_tarihi"    , m_ui->dateedit_fis_tarihi->date().toString ( "yyyy.MM.dd" ) );

    if ( query.SELECT("fis_tarihi DESC, fis_numarasi DESC",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   XYZ_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::FIND_NEXT_FIS_RECORD()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore deil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

    // Onceki ve sonraki kayitlarda formdan siralama kriterini alarak ona gore siralama yapmaliyiz.Mesela burda ogrenci no.
    // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini deil

    // Sadece bir id ye ihtiyacimiz var bu yuzden limit select yaptik.Daha hizli query icin gereklidir.

    // Eger fisler tarih ve numara bilgilerini iceriyorsa, onceki veya sonraki kayitlar bulunulan tarih icinde
    // numaraya gore siralanmalidir. O gune ait son fise gelindiginde, sonraki ve onceki gunun
    // en kucuk numarali fisinden bakilarak fisler getirilmelidir. Asagida ikinci sorgunun yapilma sebebi budur
    // ve bu sorgu, sonraki veya onceki kayit butonuna basildiginda, icinde bulunulan tarihe ait fisin olmamasi
    // durumunda gerceklesir.



    SQL_QUERY query ( DB );

    query.PREPARE_LIMIT_SELECT ( "xyz_fisler","fis_id","fis_turu   = :fis_turu "
                                 "AND fis_tarihi   = :fis_tarihi "
                                 "AND fis_numarasi > :fis_numarasi");

    query.SET_VALUE     (  ":fis_turu"      , xyz_fis_turu );
    query.SET_VALUE     (  ":fis_tarihi"    , m_ui->dateedit_fis_tarihi->date().toString ( "yyyy.MM.dd" ) );
    query.SET_VALUE     (  ":fis_numarasi"  , m_ui->line_edit_fis_no->text().toInt()                      );

    if ( query.SELECT("fis_tarihi, fis_numarasi ",0,1) > 0 ) {
        query.NEXT();
        return query.VALUE(0).toInt();
    }

    query.PREPARE_LIMIT_SELECT ( "xyz_fisler","fis_id","fis_turu   = :fis_turu "
                                 "AND fis_tarihi   > :fis_tarihi ");

    query.SET_VALUE     (  ":fis_turu"      , m_fis_turu );
    query.SET_VALUE     (  ":fis_tarihi"    , m_ui->dateedit_fis_tarihi->date().toString ( "yyyy.MM.dd" ) );

    if ( query.SELECT("fis_tarihi, fis_numarasi ",0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();}



/**************************************************************************************
                   XYZ_FISI::CHECK_LINE_VAR
***************************************************************************************/

int XYZ_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
  // Satirlar icin check var her satirdaki widgettan focus out olundugunda buraya duser.
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int XYZ_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QLineEdit * line_edit_xyz        = ( QLineEdit * ) m_ui->table_widget_xyz->cellWidget ( row_number, XYZ_ADI_COLUMN );

    if ( line_edit_xyz->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("XYZ adı boş bırakılamaz"), line_edit_xyz);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::CHECK_ADD_LINE
***************************************************************************************/

int XYZ_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::ADD_LINE
***************************************************************************************/

void XYZ_FISI::ADD_LINE ( int record_id, int row_number )
{
    SQL_QUERY query(DB);

    QString         order_number;
    QString         xyz_ad;
    QString         xyz_soyad;

    QLineEdit * lineEdit = ( QLineEdit * ) m_ui->table_widget_xyz->cellWidget ( row_number, XYZ_ADI_COLUMN );
    xyz_ad               = lineEdit->text();

    lineEdit             = ( QLineEdit * ) m_ui->table_widget_xyz->cellWidget ( row_number, SAHIS_SOYADI_COLUMN);
    xyz_soyad            = lineEdit->text();

    order_number            = m_ui->table_widget_xyz->item( row_number , ORDER_NUMBER_COLUMN )->text();

    query.PREPARE_INSERT ( "xyz_fis_satirlari","xyz_id","order_number, ad, soyad");

    query.SET_VALUE ( ":order_number"  ,order_number );
    query.SET_VALUE ( ":xyz_ad"     ,xyz_ad );
    query.SET_VALUE ( ":xyz_soyad"  ,xyz_soyad );

    int row_id = query.INSERT();

    // insert bittikten sonra row_id table widget daki row_id_column i yaziyoruz.
    table_widget_xyz->setItem (row_number, ROW_ID_COLUMN, new QTableWidgetItem ( (QVariant(row_id)).toString() ) );
}

/**************************************************************************************
                   XYZ_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int XYZ_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::UPDATE_LINE
***************************************************************************************/

void XYZ_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    SQL_QUERY query(DB);

    QString             order_number;
    QString             xyz_ad;
    QString             xyz_soyad;

    QLineEdit * lineEdit = ( QLineEdit * ) m_ui->table_widget_xyz->cellWidget ( row_number, XYZ_ADI_COLUMN );
    xyz_ad               = lineEdit->text();

    lineEdit             = ( QLineEdit * ) m_ui->table_widget_xyz->cellWidget ( row_number, SAHIS_SOYADI_COLUMN);
    xyz_soyad            = lineEdit->text();

    int xyz_id    = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    order_number         = m_ui->table_widget_xyz->item( row_number , ORDER_NUMBER_COLUMN )->text();

    query.PREPARE_INSERT ( "xyz_fis_satirlari" ,"xyz_id","order_number,ad,soyad" , "record_id = :record_id" );

    query.SET_VALUE ( ":order_number"   ,order_number );
    query.SET_VALUE ( ":xyz_ad"         ,xyz_ad );
    query.SET_VALUE ( ":xyz_soyad"      ,xyz_soyad );
    query.SET_VALUE ( ":record_id"      ,xyz_id );

    query.UPDATE();
}

/**************************************************************************************
                   XYZ_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int XYZ_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FISI::DELETE_LINE
***************************************************************************************/

void XYZ_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );

    SQL_QUERY query(DB);

    int xyz_id = m_ui->table_widget_xyz->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    query.PREPARE_DELETE ( "xyz_fis_satirlari","xyz_id = :xyz_id" );
    query.SET_VALUE      ( ":xyz_id", xyz_id );

    query.DELETE();
}

/**************************************************************************************
                   XYZ_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int XYZ_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "xyz_id", "xyz_bilgileri", QString ( "xyz_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   XYZ_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void XYZ_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "xyz_id", "xyz_bilgileri", QString ( "xyz_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   XYZ_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void XYZ_FISI::SAVER_BUTTON_CLICKED (QAbstractButton * button, int record_id )
{
   // REGISTER_SAVE_BUTTON ile register edilen buttonlara tiklandiginda bu fonksiyona duser.
}
