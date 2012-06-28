#include "xyz_tree_secimi_class.h"
#include "xyz_tree_secimi_open.h"
#include "adak_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_XYZ_TREE_SECIMI
***************************************************************************************/

int OPEN_XYZ_TREE_SECIMI ( QDialog * parent )
{
    int secilen_id = -1;

    XYZ_TREE_SECIMI * F = new XYZ_TREE_SECIMI ( parent );

    F->m_secilen_id = &secilen_id;
    F->exec();

    return (secilen_id);
}


/**************************************************************************************
                   XYZ_TREE_SECIMI::XYZ_TREE_SECIMI
***************************************************************************************/

XYZ_TREE_SECIMI::XYZ_TREE_SECIMI ( QWidget * parent )
{
    SET_WINDOW_SIZE ( 300, 800 );

    SET_PAGE_TITLE ( tr("XYZ SECİMİ") );

    SET_HELP_PAGE ( tr("xyz_tree_secimi.html") );


    SET_COLUMN_HEADERS ( QStringList() << tr("Grup Kodu") << tr("Grup İsmi") << tr("Grup Sayısı") << tr("Gruptaki Kayıt Sayısı") << tr("...") << tr("id"));

    // Asagida ki fonksiyonla right align yapmak istedigimiz columnlari right align yapabiliriz.
    // Tek yapmamiz gereken gerekli columnlarin numarasini SET_RIGHT_ALIGN_COLUMNS fonksiyonunan gondermek
    QList<int> right_aligned_columns;
    right_aligned_columns << 2 << 3;
    SET_RIGHT_ALIGN_COLUMNS ( right_aligned_columns );

    INIT_KERNEL ( this, DB );
    // Dikkat expand 0. columnda yer alir bu yuzden kesinlikle 0.column hidelanmamali
    // init kerneldan sonra kullanilmali columni hidelar
    HIDE_COLUMN ( 5 );

    /*
     * SET_COLUMN_WIDTH fonksiyonu init kernel dan sonra kullanilmali.Cunku atanan degerlere gore tree
     * init kernel da yaratiliyor.Eger init kerneldan once bu fonksiyon cagrilirsa table henuz
     * yaratilmadigindan crash olur.
     */
    SET_COLUMN_WIDTH ( 2, 200 );

    SET_SETTING_NAME    ( "XYZ_TREE_SECIMI" );

}

/**************************************************************************************
                   XYZ_TREE_SECIMI::~XYZ_TREE_SECIMI
***************************************************************************************/

XYZ_TREE_SECIMI::~XYZ_TREE_SECIMI ()
{

}

/**************************************************************************************
                   XYZ_TREE_SECIMI::FILL_TREE_TABLE
***************************************************************************************/

void XYZ_TREE_SECIMI::FILL_TREE_TABLE ()
{
    // Buraya dustugunde amacimiz ADD_PARENT_ITEM la parentleri treeye eklememiz lazim.

    SQL_QUERY sql_query      ( DB );

    // parent itemlarimizi query ile buluyoruz.
    sql_query.PREPARE_SELECT ( "zzz_parent_gruplari","parent_grup_adi, parent_grup_tam_ismi,parent_grup_id,...." );

    if ( sql_query.SELECT("grup_adi") NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {

      // mesela burada parentin kac child i oldugunu bulup onuda row bilgisi olarak ekleyebiliriz.
           int parent_id = sql_query.VALUE(3).toInt();
           SQL_QUERY select_child_query( DB );
           select_child_query.PREPARE_SELECT ( "zzz_child_gruplari","child_grup_id","parent_id=:parent_id" );
           select_child_query.SET_VALUE      ( ":parent_id", parent_id );

       // her bir \t diger column a gecis yapar.
           QString tree_row = sql_query.VALUE(99).toString() + "\t" + sql_query.VALUE(99).toString() +
                                 "\t" + QVariant(select_child_query.SELECT()).toString() + "\t" + sql_query.VALUE(99).toString() + "\t" +
                                    sql_query.VALUE(99).toInt()) + "\t" + sql_query.VALUE(99).toString() ;

       // ve bu row u tree ye ekleriz.
           ADD_PARENT_ITEM ( tree_row , select_child_query.SELECT() );
       }
    }
}

/**************************************************************************************
                   XYZ_TREE_SECIMI::ADD_CHILD_ITEMS
***************************************************************************************/

void XYZ_TREE_SECIMI::ADD_CHILD_ITEMS ( TREE_ITEM * item )
{
    // Buradada parentin childlari eklenir.Yani kullanici bir row u expand ederse buraya duser
    // Burada dikkat edilmesi gereken kernel tarafindan gonderilecek deger TREE_ITEM dir.Bu TREE_ITEM kullanicinin expand ettigi rowu tutar
    // GET_SELECTED_VALUE(TREE_ITEM *,column_number)  fonksiyonu ile kullanarak istenilen columndan deger QVariant olarak dondurulebilir.

    // 5. column dan parent id mizi aldik.
    int item_id = GET_SELECTED_VALUE ( item, 5 );

    SQL_QUERY select_child_query  ( DB );
    SQL_QUERY sql_query           ( DB );

    // parentin childlarini bulduk
    sql_query.PREPARE_SELECT ( "zzz_child_gruplari","child_grup_id,child_grup_adi,child_grup_tam_ismi","parent_id=:parent_id" );
    sql_query.SET_VALUE ( ":parent_id", item_id );

    if ( sql_query.SELECT() > 0 ) {
        while ( sql_query.NEXT() EQ true ) {
    
      // simdi burada ise childin childlarinin sayisina bakip onuda row a bilgi olarak ekleyebiliriz.
           select_child_query.PREPARE_SELECT ( "zzz_child_gruplari","child_grup_id,child_grup_adi,child_grup_tam_ismi","parent_id=:parent_id" );
           select_child_query.SET_VALUE      ( ":parent_id", sql_query.VALUE(0).toInt() );

            QString tree_row = sql_query.VALUE(99).toString() + "\t" + sql_query.VALUE(99).toString() + "\t" +
                               QVariant(select_child_query.SELECT()).toString() + "\t" + sql_query.VALUE(99).toString()
                              + "\t" + " " + "\t" +
                              sql_query.VALUE(99).toString();

           ADD_CHILD_ITEM ( tree_row , select_child_query.SELECT() );
       }
    }
}

/**************************************************************************************
                   XYZ_TREE_SECIMI::DOUBLE_CLICKED
***************************************************************************************/

int XYZ_TREE_SECIMI::DOUBLE_CLICKED ( TREE_ITEM * item )
{
    *m_secilen_id = GET_SELECTED_VALUE ( item, 5 ).toInt();

    return ADAK_EXIT;
}
