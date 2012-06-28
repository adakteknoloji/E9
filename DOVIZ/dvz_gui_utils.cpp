#include <QComboBox>
#include "adak_sql.h"
#include "dvz_gui_utils.h"

extern ADAK_SQL * DB;



/**************************************************************************************
                   DVZ_FILL_COMBOBOX
***************************************************************************************/

void DVZ_FILL_COMBOBOX ( QComboBox * combo_box, bool doviz_adi_ekle  )
{
    //DOVIZLER_STRUCT_DOLDUR();

    // FIXME : Bu yapi kod duplication yerine struct ile cozulebilir gibi duruyor ?

    SQL_QUERY query ( DB );

//    query.PREPARE_LIMIT_SELECT ( "SELECT kur_id FROM dvz_kurlar",0,1,"kur_id" );

//    if ( query.SELECT() EQ 0 ) {
//        DOVIZLERI_GUNCELLE();
//    }

    query.PREPARE_SELECT("dvz_dovizler" , "doviz_id,doviz_adi,doviz_kodu" , "dovizi_kullan = :dovizi_kullan");
    query.SET_VALUE(":dovizi_kullan" , 1);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    int added_item = 0;
    QStringList dovizler_list;
    while ( query.NEXT() EQ true ) {
        QString doviz_str = query.VALUE( 2 ).toString();
        if ( doviz_adi_ekle EQ true ) {
              doviz_str.append ( QString ( " %1" ).arg ( query.VALUE( 1 ).toString() ) );
        }
        dovizler_list << doviz_str;
        added_item++;
    }

    combo_box->addItems ( dovizler_list );
    combo_box->setEditable ( false );

    if ( added_item EQ 1 ) {
        combo_box->setCurrentIndex(0);
    }
}

