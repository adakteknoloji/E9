#include "adak_utils.h"
#include "XYZ_TREE_ARAMA_OPEN.h"
#include "XYZ_TREE_ARAMA_CLASS.h"
#include "ui_XYZ_TREE_ARAMA.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_XYZ_TREE_ARAMA
***************************************************************************************/

int OPEN_XYZ_TREE_ARAMA( QDialog * parent )
{
    int secilen_id = -1;
  
    XYZ_TREE_ARAMA * F  = new XYZ_TREE_ARAMA ( parent );
    F->m_secilen_id = &secilen_id
    F->SHOW();

    return (secilen_id);
}

/**************************************************************************************
                   XYZ_TREE_ARAMA::XYZ_TREE_ARAMA
***************************************************************************************/

XYZ_TREE_ARAMA::XYZ_TREE_ARAMA(QWidget *parent) : TREE_ARAMA_KERNEL(parent),ui(new Ui::XYZ_TREE_ARAMA)
{
    ui->setupUi(this);

    SET_PAGE_TITLE     ( tr("XYZ SEÇİMİ") );
    SET_HELP_PAGE      ( tr("XYZ_TREE_ARAMA.html") );
    SET_COLUMN_HEADERS ( QStringList() << tr("Grup Kodu") << tr("Grup İsmi") );

    // Asagida ki fonksiyonla right align yapmak istedigimiz columnlari right align yapabiliriz.
    // Tek yapmamiz gereken gerekli columnlarin numarasini SET_RIGHT_ALIGN_COLUMNS fonksiyonunan gondermek

    QList<int> right_aligned_columns;

    right_aligned_columns << 2 << 3;

    SET_RIGHT_ALIGN_COLUMNS ( right_aligned_columns );

    REGISTER_BUTTON_WIDGET(ui->search_buttons_widget);

    INIT_KERNEL (ui->tree_view, this, DB );

    SET_SETTING_NAME    ( "XYZ_TREE_ARAMA" );
    
    //Tum lineeditlerin ve limitedtexteditlerin uzunlugu INIT_KERNEL dan sonra setlenmeli.Cunku bu widgetlarin default degeri 1 characterdir.
    //Ayrica eger bu widgetlara setlenmesi gereken degerler varsa bunu da uzunluklar setlendikten sonra yapmaliyiz.
    m_ui->line_edit_xyz->setMaxLength(25);
}

/**************************************************************************************
                   XYZ_TREE_ARAMA::~XYZ_TREE_ARAMA
***************************************************************************************/

XYZ_TREE_ARAMA::~XYZ_TREE_ARAMA()
{
    delete ui;
}

/**************************************************************************************
                   XYZ_TREE_ARAMA::DOUBLE_CLICKED
***************************************************************************************/

int XYZ_TREE_ARAMA::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_TREE_ARAMA::DOUBLE_CLICKED
***************************************************************************************/

int XYZ_TREE_ARAMA::CHECK_VAR(QObject *object)
{
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_TREE_ARAMA::FILL_TREE_TABLE
***************************************************************************************/

void XYZ_TREE_ARAMA::SEARCH ()
{
    // Asagidaki sekilde verilerimizi ekleriz her \t de diger column a yazar.
    int num_of_child = 99;
    
    QString tree_row = "000\t000 Grubu";
    ADD_PARENT_ITEM ( tree_row , num_of_child );
}

/**************************************************************************************
                   XYZ_TREE_ARAMA::ADD_CHILD_ITEMS
***************************************************************************************/

void XYZ_TREE_ARAMA::ADD_CHILD_ITEMS ( TREE_ITEM * item )
{
    // Buradada parentin childlari eklenir.Yani kullanici bir row u expand ederse buraya duser
    // Burada dikkat edilmesi gereken kernel tarafindan gonderilecek deger TREE_ITEM dir.Bu TREE_ITEM kullanicinin expand ettigi rowu tutar
    // GET_SELECTED_VALUE(TREE_ITEM *,column_number)  fonksiyonu ile kullanarak istenilen columndan deger QVariant olarak dondurulebilir.
    // Boylelikle hangi satira ne ekliyecegini bilir.

    // 5. column dan parent id mizi aldik.
    //int item_id = GET_SELECTED_VALUE ( item, 5 );
  
    // Asagidaki sekilde verilerimizi ekleriz her \t de diger column a yazar.
    int num_of_child = 99;
    
    QString tree_row = "01\t000 1. Alt Grubu";

    ADD_CHILD_ITEM( tree_row , num_of_child );
}

/**************************************************************************************
                   XYZ_TREE_ARAMA::DOUBLE_CLICKED
***************************************************************************************/

int XYZ_TREE_ARAMA::DOUBLE_CLICKED ( TREE_ITEM * item )
{
    //Secim yapilir.
    
    *m_secilen_id = GET_SELECTED_VALUE ( item, id_column );
    
    return ADAK_EXIT;
}
