#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QComboBox>
 
#include "adak_utils.h"
#include "yonetim.h"
#include "adak_sql.h"
#include <QSize>
#include "dvz_enum.h"
#include "dvz_console_utils.h"

extern ADAK_SQL * DB;

/*****************************************************************************************************
                              BNK_FILL_COMBO_BOX_HESABA_ETKISI
*****************************************************************************************************/

void BNK_FILL_COMBO_BOX_HESABA_ETKISI (QComboBox * combo_box)
{
    QStringList hesaba_etkisi;

    hesaba_etkisi << "BORCU ARTTIR" << "ALACAGI ARTTIR";

    combo_box->addItems(hesaba_etkisi);
    combo_box->setEditable(false);
}

/*****************************************************************************************************
                                  BNK_FILL_COMBOBOX_HESAP_TURU
*****************************************************************************************************/

void BNK_FILL_COMBOBOX_HESAP_TURU (QComboBox * combo_box)
{
    QStringList hesap_turleri;

    hesap_turleri << "BANKA" << "CARİ" << "MUHASEBE"<<"POS"<<"KREDİ KARTI"<<"KASA" << "POS KOMİSYON HESABI";

    combo_box->addItems(hesap_turleri);
    combo_box->setEditable(false);
}


/*****************************************************************************************************
                      BNK_FILL_COMBO_BOX_KRS_HESAP_TURU
*****************************************************************************************************/

void BNK_FILL_COMBO_BOX_KRS_HESAP_TURU (QComboBox * combo_box)
{
    QStringList hesap_turleri;

    hesap_turleri << "BANKA" << "CARİ" << "MUHASEBE"<<"POS"<<"KREDİ KARTI"<<"KASA" << "POS KOMİSYON HESABI";

    combo_box->addItems(hesap_turleri);
    combo_box->setEditable(false);
}

/*****************************************************************************************************
                                 BNK_FILL_COMBO_BOX_ETK_HESAP_TURU
*****************************************************************************************************/

void BNK_FILL_COMBO_BOX_ETK_HESAP_TURU (QComboBox * combo_box)
{
    QStringList hesap_turleri;

    hesap_turleri << "BANKA" <<"POS"<<"KREDİ KARTI";

    combo_box->addItems(hesap_turleri);
    combo_box->setEditable(false);
}


/*****************************************************************************************************
                               BNK_FILL_COMBO_BOX_BANKA_HESAPLARI
*****************************************************************************************************/

void BNK_FILL_COMBO_BOX_BANKA_HESAPLARI( QComboBox * combo_box , QString sube_kodu  )
{
    Q_UNUSED ( sube_kodu );
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("bnk_hesaplar",
                             "hesap_no_id,bnk_hesaplar.doviz_id,banka_hesap_kodu,banka_ismi,sube_kodu,"
                             "sube_ismi,hesap_numarasi,doviz_id, banka_hesap_unvani",
                             "",
                             "banka_hesap_kodu,banka_ismi,sube_kodu,sube_ismi,"
                             "hesap_numarasi,bnk_hesaplar.doviz_id,hesap_no_id, banka_hesap_unvani");

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    QStandardItemModel * item_model = new QStandardItemModel;

    item_model->setColumnCount(8);
    item_model->setHorizontalHeaderLabels(QStringList() << "" << "" << "Banka Hesap\nKodu" << "Banka Hesap\nÜnvanı" << "Banka İsmi" << "Şube Kodu" << "Şube İsmi" << "Hesap Numarası" << "Para\nBirimi");

    int row_index = 0;

    while ( sql_query.NEXT() EQ true ) {
        item_model->insertRow(row_index);

        item_model->setItem(row_index,0,new QStandardItem(sql_query.VALUE(0).toString()));
        item_model->setItem(row_index,1,new QStandardItem(sql_query.VALUE(1).toString()));
        item_model->setItem(row_index,2,new QStandardItem(sql_query.VALUE(2).toString()));
        item_model->setItem(row_index,3,new QStandardItem(sql_query.VALUE("banka_hesap_unvani").toString()));
        item_model->setItem(row_index,4,new QStandardItem(sql_query.VALUE(3).toString()));
        item_model->setItem(row_index,5,new QStandardItem(sql_query.VALUE(4).toString()));
        item_model->setItem(row_index,6,new QStandardItem(sql_query.VALUE(5).toString()));
        item_model->setItem(row_index,7,new QStandardItem(sql_query.VALUE(6).toString()));
        item_model->setItem(row_index,8,new QStandardItem( DVZ_GET_DOVIZ_KODU( sql_query.VALUE(7).toInt() )));
    }

    QTableView * table_view = new QTableView;

    combo_box->setView(table_view);

    table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    combo_box->setModel(item_model);
    combo_box->setModelColumn(2);

    QFont font = table_view->font();
    font.setPointSize(8);

    table_view->setFont(font);

    table_view->horizontalHeader()->setStretchLastSection(true);

    table_view->hideColumn(0);
    table_view->hideColumn(1);

    //10 satirlik alan ayirir.
    int table_widget_height = (table_view->verticalHeader()->length() / item_model->columnCount()) * 10;

    table_view->setColumnWidth(2,125);
    table_view->setColumnWidth(3,125);
    table_view->setColumnWidth(4,125);
    table_view->setColumnWidth(5, 75);
    table_view->setColumnWidth(6,250);
    table_view->setColumnWidth(7,150);
    table_view->setColumnWidth(8,50);

    table_view->setMinimumHeight(table_widget_height + table_view->horizontalHeader()->height());
    table_view->setMaximumHeight(table_widget_height + table_view->horizontalHeader()->height());

    table_view->setMinimumWidth (table_view->horizontalHeader()->length() + 20);
    table_view->setMaximumWidth (table_view->horizontalHeader()->length() + 20);

    combo_box->setCurrentIndex(-1);
}

/*************************************************************************************
                      BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL
**************************************************************************************/

QVariant BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(QComboBox * combo_box, int row, int column)
{
    QStandardItemModel * model = static_cast<QStandardItemModel *> (combo_box->model());

    return QVariant(model->item(row,column)->text());
}

/**************************************************************************************
                    BNK_HESAPLARI_MODEL_INDEX_BUL
***************************************************************************************/

int BNK_HESAPLARI_MODEL_INDEX_BUL ( int p_banka_hesap_id, QComboBox * p_comboBox )
{
    QStandardItemModel * model = static_cast<QStandardItemModel *> ( p_comboBox->model() );

    for ( int i = 0; i < model->rowCount(); i++ ) {
        if ( model->item ( i, 0 )->text().toInt() EQ p_banka_hesap_id ) {
            return i;
        }
    }
    return -1;
}


/********************************************************************************
                      BNK_GET_FIRMANIN_BANKA_SUBELERI
 ********************************************************************************/

void BNK_GET_FIRMANIN_BANKA_SUBELERI ( QString banka_ismi,QComboBox * combobox)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","sube_ismi","banka_ismi = :banka_ismi","sube_ismi");
    select_query.SET_VALUE(":banka_ismi",banka_ismi);
    if (select_query.SELECT() NE 0 ) {

        while (select_query.NEXT()) {
            combobox->addItem(select_query.VALUE(0).toString());
        }
    }

}

/********************************************************************************
                      BNK_GET_FIRMANIN_BANKA_SUBELERI
 ********************************************************************************/

void BNK_FILL_COMBO_BOX_ENT_FIS_TURLERI ( QComboBox * p_combo_box )
{

   QStringList ent_fis_turleri;

    ent_fis_turleri << QObject::tr ( "Tüm Entegrasyon Fişleri" )
                    << QObject::tr ( "Cari Entegrasyon Fişi" )
                    << QObject::tr ( "Çek-Senet Entegrasyon Fişi" )
                    << QObject::tr ( "Perakende Entegrasyon Fişi" )
                    << QObject::tr ( "Personel Entegrasyon Fişi" )
                    << QObject::tr ( "Sm Makbuz Entegrasyon Fişi" )
                    << QObject::tr ( "İşletme Entegrasyon Fişleri" )
                    << QObject::tr ( "Diğer Entegrasyon Fişleri" );

    p_combo_box->addItems(ent_fis_turleri);
}
