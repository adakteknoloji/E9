#include "ui_xyz_tree.h"
#include "xyz_tree_open.h"
#include "xyz_tree_class.h"
#include "adak_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_XYZ_TREE
***************************************************************************************/

void OPEN_XYZ_TREE ( QWidget * parent )
{
    XYZ_TREE * F = new XYZ_TREE ( parent );
    F->setWindowState ( Qt::WindowMaximized );
    F->EXEC();
}

/**************************************************************************************
                   XYZ_TREE::XYZ_TREE
***************************************************************************************/

XYZ_TREE::XYZ_TREE ( QWidget * parent ) : m_ui ( new Ui::XYZ_TREE )
{
    m_ui->setupUi(this);

    SET_HELP_PAGE       ( "bla_bla.html" );
    SET_PAGE_TITLE      ( tr("BLA BLA"))

    // Burada columnlarin isimleri setlenir.Burda gonderilen header sayisi kadar otomatik olarak column yaratilir. id column da herhangi bir yere
    // konulabilir. Ancak id column u init kerneldan sonra hidelamayi unutmamak lazim.kernel id column i otomatik olarak hidelamaz.
    SET_COLUMN_HEADERS ( QStringList() << tr("Grup Kodu") << tr("Grup İsmi") << tr("...") << tr("...") << tr("...") << tr("id") );

    // Burada ise ekrana koymus oludugmuzu tree_view i setleriz.Dikkat tree_view kullanilmali widget olarak.
    REGISTER_TREE_WIDGET   ( m_ui->tree_view_xyz_tree_view );

    // Burda ise ekrandaki ekstra buttonlari ekleriz. Dikkat burda add button icin 2 buton tanimlanmis.Bunun anlami butonlar ayni slot a baglilar.
    // Bu yuzden butonlara tiklandiginda slot a button objeside gonderilir.Boylelikle obje ve butonlari karsilastirarak gerekli islemleri yapabiliriz.
    REGISTER_ADD_BUTTON    ( m_ui->button_alt_grup_ekle );
    REGISTER_ADD_BUTTON    ( m_ui->button_ana_grup_ekle );
    REGISTER_UPDATE_BUTTON ( m_ui->button_grubu_guncellestir );
    REGISTER_DELETE_BUTTON ( m_ui->button_grubu_sil );

    // Asagida ki fonksiyonla right align yapmak istedigimiz columnlari right align yapabiliriz.
    // Tek yapmamiz gereken gerekli columnlarin numarasini SET_RIGHT_ALIGN_COLUMNS fonksiyonunan gondermek
    QList<int> right_aligned_columns;
    right_aligned_columns << 2 << 3;
    SET_RIGHT_ALIGN_COLUMNS ( right_aligned_columns );

    INIT_KERNEL ( this, DB );

    // buradan sonraki fonksiyonlar tree view ve model yaratildiktan sonra yani init kernel dan sonra kullanilmali aksi takdirde crash olabilir.

    // istedigimiz columni hidelariz.Mesela burda id columni hidelayabiliriz.
    HIDE_COLUMN ( 5 );

    SET_SETTING_NAME            ( "XYZ_TREE" );

    // columnlarimizi gerektigi sekilde resize edebiliriz.
    m_ui->tree_view_xyz_tree_view->setColumnWidth ( 4, 150 );

    // Burda da normal de kernel tarafindan otomatik olarak eklenen action menunun kullanici tarafindan gorulecek olan
    // textlerini setleriz.
    SET_ACTION_MENU_TEXT ( "Alt Grup Ekle", "Grubu Güncelle", "Grubu Sil" );
}

/**************************************************************************************
                   XYZ_TREE::~XYZ_TREE                  
***************************************************************************************/

XYZ_TREE::~XYZ_TREE()
{

}

// Buraya herhangi bir add butonuna ve ya action menude add satirina tiklandiginda duser.Yeni bir row eklenir.
// Bu fonksiyona gonderilen obje tiklanin butondur.Item ise row dur.item daki datalar erismek icin GET_SELECTED_VALUE fonksiyonu kullanilir.

/**************************************************************************************
                   XYZ_TREE::ADD_ITEM
***************************************************************************************/

void XYZ_TREE::ADD_ITEM ( QObject * button, TREE_ITEM * item)
{
    // Yukarda REGISTER_ADD_BUTTON fonksiyonu ile ve ya action menuden Alt Grup Ekle ye tiklanirsa buraya duser.
    // Burada gelen obje ile butonlara karsilastirarak tiklanilan buton a gore islemler yapilir.Dikkat action menu den tiklanirsa da buraya duser
    // Bu yuzden obje butonlardan biri degilse kesinlikle action menuden geliyordur.

    SQL_QUERY select_query(DB);

    // Mesela burda item_id tanimladik bu degisken row muzdaki idyi atamak icin kullanilacagiz.
    int item_id = -1;

    // Eger gonderilen item NULL sa bunun anlami secilen herhangi bir row soz konusu degil yada ekranda row mevcut degildir.Ancak her turlu
    // NULL geldiginde ADD_PARENT_ITEM fonksiyonu ile item eklenmeli yani kesinlikle parent item olmalidir.Ancak null degilse satir secildigi anlamina gelir.
    // Boyle durumda GET_SELECTED_VALUE(item,column_number) item ve degerini almak istedigimiz column number gondererek istedigimiz degeri alabiliriz.Dikkat donen
    // deger QVariant tir cast edilmelidir.
    if (item NE NULL) {
        item_id = GET_SELECTED_VALUE ( item, 5 ).toInt();
    }
    else {
        item_id = -1;
    }


    // Burda ise tiklanan buton turunu bulduk boylelikle child mi yoksa parent mi ekliyecegimizi bulabiliriz.
    // Asagida normalde if condition da son else gereksiz cunku ALT GRUP EKLE buton ve action ayni seyi temsil edior.Ancak asagidaki sekilde 3
    // olasilikli olarakda yapilabilir.3 olasik eger ki butonlardan biri ile action menude ki add ayni gorevlere sahip degilse yapilmalidir.
    int tiklanan_buton_turu;

    // ANA_GRUP_EKLE,ALT_GRUP_EKLE bunlar tamamen enum dir.Kernelda tanimli degildir.Bu yuzden ihtiyaca gore degisebilirler.Burda ornek olarak kullanilmistir.
    // Programlarda tanimlanmasi lazim.

    if ( button EQ m_ui->button_ana_grup_ekle ) {
        tiklanan_buton_turu = ANA_GRUP_EKLE;
    }
    else if ( button EQ m_ui->button_alt_grup_ekle ) {
        tiklanan_buton_turu = ALT_GRUP_EKLE
    }
    else {
    // Action menudeki add tiklandigida buraya duser.
    tiklanan_buton_turu = ALT_GRUP_EKLE;
    }



    if ( tiklanan_buton_turu EQ ALT_GRUP_EKLE ) {


        // Simdi alt grup ekle ye tiklandigini kabul edersek burda alt grup eklemek icin kullanilacak mevcut gui ye item_id yani parent id yi gonderirirz.
    // Orda ise gerekli bilgiler kullanicidan alinip sql e gerekli bilgiler insert edilir.Sonra sinda ise columnlara yazmak istedigimiz bilgileri arasina
    // her column icin "\t" koyarak return ederiz ve sonrasinda ise bu stringi ADD_CHILD_ITEM ( string, 0 ) ile gondeririz.Buda treeye ekler.Dikkat burdaki 0
    // Eklenen kaydin varsa child itemlarinin sayisidir.Bizim child item olmadigindan(cunku ilk kez ekleniyor) 0 gondeririz.Ve o anki childi ekledigimiz
    // parenti de guncelleriz. cunku tree tarafindan child eklendiginde parenti expand edilebilir hale gelmeli ayrica columda child sayisi ni tutan columnlar da 1
    // artirilmali.

    
    QString child_row = F_ALT_GRUP_EKLEME_EKRANI ( item_id );
    // ornek bir child_row = "column_0_degeri" +"\t" +"column_1_degeri"+"\t" + "column_2_degeri" +"\t" + "column_3_degeri" +"\t" + "column_4_degeri";
        ADD_CHILD_ITEM ( child_row, 0 );
    
    // Burdada childin eklendigi parent row u guncelleriz mesela sadece child sayisini tutan column degerini 1 artiralim.Child sayisi 2.columnda tutulsun mesela
    
    int child_sayisi = GET_SELECTED_VALUE ( item, 2 ).toInt();
    child_sayisi += 1;
    
    QString guncellenen_row = GET_SELECTED_VALUE ( item, 0 ).toString() +"\t" +GET_SELECTED_VALUE ( item, 1 ).toString()"+"\t" +
                   QVariant(child_sayisi).toString()     +"\t" +GET_SELECTED_VALUE ( item, 3 ).toString()"+"\t" +
                  GET_SELECTED_VALUE ( item, 4 ).toString() +"\t" +GET_SELECTED_VALUE ( item, 5 ).toString();
                
        UPDATE_SELECTED_ITEM ( guncellenen_row );
    
    // ve ekrandaki parenti guncelliyoruz ki degisiklikler yansisin.
    REFRESH_TREE_VIEW ( item );
    }
    else if ( tiklanan_buton_turu EQ ANA_GRUP_EKLE ) {
        QString parent_row = OPEN_ALT_GRUP_EKLEME_EKRANI ( item_id );
        ADD_PARENT_ITEM ( parent_row, 0 );
    }
}

// parentler eklenir.

/**************************************************************************************
                   XYZ_TREE::ADD_PARENT_ITEMS
***************************************************************************************/

void XYZ_TREE::ADD_PARENT_ITEMS ()
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT ( "xyz_parent_gruplari","SELECT parent_id, parent_kodu,parent_adi ...");

    if ( sql_query.SELECT("parent_grup_kodu") NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {

       // Mesela burda parentin childlarinin sayisinida bulup onuda column a ekstra bilgi olarak yazabiliriz.
           int parent_id                  = sql_query.VALUE(0).toInt();    
           SQL_QUERY select_query ( DB );
           select_query.PREPARE_SELECT ( "adr_child_gruplari","child_id","parent_id = :parent_id" );
           select_query.SET_VALUE      ( ":parent_id", parent_id );

       // Parent row olusturuyor.Her bir \t de diger columna gecer ve sonrasinda eklenen degerleri oraya yazar
           QString parent_row  = sql_query.VALUE(3).toString() + "\t" + sql_query.VALUE(4).toString() +
                             "\t" + QVariant(select_query.SELECT()).toString() + "\t" + sql_query.VALUE(6).toString() + "\t" +
                                sql_query.VALUE(5).toInt() + "\t" + sql_query.VALUE(0).toString() ;

       // select_query.NUM_OF_ROWS() childlarin sayisidir.Gonderilmelidir boylece expand edilebilir yapar.
           ADD_PARENT_ITEM ( parent_row , select_query.NUM_OF_ROWS() );
       }
    }
}


/**************************************************************************************
                   XYZ_TREE::ADD_CHILD_ITEMS
***************************************************************************************/

void XYZ_TREE::ADD_CHILD_ITEMS ( TREE_ITEM * item )
{
    // Herhangi bir parent expand edildiginde buraya duser.
    // oncelikle parentin idsi bulunur.Ve bu idle childlar bulunup tek tek eklenir.
    int item_id;
    item_id = GET_SELECTED_VALUE ( item, 5 ).toInt();

    SQL_QUERY sql_query     ( DB );

    sql_query.PREPARE_SELECT ( "xyz_child_gruplari","child_id, child_kodu,child_adi ...","parent_id = :parent_id");
    sql_query.SET_VALUE ( ":parent_id" ,item_id );

    if ( sql_query.SELECT("child_grup_kodu") > 0 ) {
        while ( sql_query.NEXT() EQ true ) {

       // mesela burda childin childlarinin sayisinida bulup onuda column a ekstra bilgi olarak yazabiliriz.
           SQL_QUERY select_query( DB );
           select_query.PREPARE_SELECT ( "adr_child_gruplari","child_id","parent_id = :parent_id" );
           select_query.SET_VALUE      ( ":parent_id", sql_query.VALUE(0).toInt() );


       // child row olusturuyor.Her bir \t de diger columna gecer ve sonrasinda eklenen degerleri oraya yazar
           QString child_row  = sql_query.VALUE(3).toString() + "\t" + sql_query.VALUE(4).toString() +
                             "\t" + QVariant(select_query.SELECT()).toString() + "\t" + sql_query.VALUE(6).toString() + "\t" +
                                sql_query.VALUE(5).toInt() + "\t" + sql_query.VALUE(0).toString() ;

       // select_query.NUM_OF_ROWS() childlarin sayisidir.Gonderilmelidir boylece expand edilebilir yapar.
           ADD_CHILD_ITEM ( child_row, select_query.NUM_OF_ROWS() );
       }
    }

}

// Burada ise guncelle butonuna yada action menuden guncelle secenegi secilirse duser. Tek yapilmasi gereken guncelleme ekrani acip
// Bu ekrana girilen degerlere gore olusan yeni row stringini UPDATE_SELECTED_ITEM fonksyionu ile gondermek gerekir.

/**************************************************************************************
                   XYZ_TREE::UPDATE_ITEM
***************************************************************************************/

void XYZ_TREE::UPDATE_ITEM ( TREE_ITEM * item )
{
    int item_id = -1;

    if ( item NE NULL ) {
        item_id = GET_SELECTED_VALUE ( item, 5 ).toInt();
    }

    QString guncellenmis_row = OPEN_GUNCELLEME_EKRANI (item_id, this );

    UPDATE_SELECTED_ITEM ( guncellenmis_row );
}

/**************************************************************************************
                   XYZ_TREE::CHECK_DELETE_ITEM
***************************************************************************************/

int XYZ_TREE::CHECK_DELETE_ITEM ( TREE_ITEM * item )
{
    int item_id = -1;

    if ( item NE NULL ) {
        item_id = GET_SELECTED_VALUE ( item, 5 ).toInt();
    }

    // Burada gonderilen TREE_ITEM dan silinmek istenen rowun idsi bulunarak silinip silinemeyecegini bakilir.
    // Eger silinmesinde sorun yoksa ADAK_OK aksi takdirde silinmesine izin yoksa ADAK_FAIL dondurulur.

    return ADAK_OK;
}


// burada ise gonderilen item silinir.

/**************************************************************************************
                   XYZ_TREE::DELETE_ITEM
***************************************************************************************/

void XYZ_TREE::DELETE_ITEM ( TREE_ITEM * item)
{
    int item_id = -1;

    if ( item NE NULL ) {
        item_id = GET_SELECTED_VALUE ( item, 5 ).toInt();
    }

    DB->START_TRANSACTION();
    sql_query.PREPARE_DELETE ( "adres_xyz_gruplari","item_id = :item_id " );
    sql_query.SET_VALUE      ( ":item_id", item_id );
    sql_query.DELETE();
    DB->COMMIT_TRANSACTION();
}

