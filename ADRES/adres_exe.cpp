#include <QMessageBox>
#include "adak_sql.h"
#include "adres_exe.h"
#include "yonetim.h"
#include "e9_ana_menu_open.h"
#include "adak_utils.h"


//Adres exe olarak calistiginda ihtiyac duyulan fonksiyonlar burada olacak

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_ADRES_ANA_MENU_ON_EXE_MODE
***************************************************************************************/

void OPEN_ADRES_ANA_MENU_ON_EXE_MODE ( QWidget * parent )
{
    SQL_QUERY query ( DB );

    if ( KULLANICI_ID() NE 1 ) {
        query.PREPARE_SELECT ( "e9_kullanici_yetkileri","kullanici_id","kullanici_id = :kullanici_id" );
        query.SET_VALUE      ( ":kullanici_id", KULLANICI_ID() );

        if ( query.SELECT() EQ 0 ) {
            ADAK_WARNING( QObject::tr ( "Bu kullanıcı için yetkilendirme yapılmamış."), NULL, parent );
            exit(0);

        }
    }

    OPEN_E9_ANA_MENU ( parent );

}
