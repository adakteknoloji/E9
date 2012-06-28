#include "e9_version_upgrade.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include <stdlib.h>
#include "prs_enum.h"
#include "prs_con_utils.h"
#include "e9_enum.h"
#include "sube_console_utils.h"
#include "e9_console_utils.h"
#include "muh_console_utils.h"
#include "cari_enum.h"
#include "stok_enum.h"
#include "fat_console_utils.h"
#include "fat_fis_utils.h"
#include "irs_console_utils.h"
#include "irs_enum.h"
#include "stok_fis_utils.h"
#include "fat_enum.h"
#include "stok_console_utils.h"
#include "e9_onar_utils.h"
#include "dvz_console_utils.h"
#include "cek_console_utils.h"
#include "cek_enum.h"
#include "belge_kernel_defines.h"
#include "e9_belge.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "isl_enum.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;


/**************************************************************************************
                         E9_V2_81_TO_V2_82_UPGRADE
***************************************************************************************/

void E9_V2_81_TO_V2_82_UPGRADE()
{
    ISL_ENT_FISLERINI_SIL(E9_PROGRAMI, ISLETME_MODULU);

    SQL_QUERY f_query(DB);
    SQL_QUERY s_query(DB);
    SQL_QUERY t_query(DB);

    f_query.PREPARE_SELECT("isl_fisi", "fis_id, fis_turu");

    if ( f_query.SELECT() NE 0 ) {

        while( f_query.NEXT() EQ true ) {
            int fis_id   = f_query.VALUE(0).toInt();
            int fis_turu = f_query.VALUE(1).toInt();

            if ( fis_turu EQ ENUM_GIDER_FISI ) {
                continue;
            }

            s_query.PREPARE_SELECT("isl_fis_satirlari", "fis_satiri_id, gider_turu ", "fis_id = :fis_id");
            s_query.SET_VALUE     ( ":fis_id", fis_id );

            if( s_query.SELECT() NE 0 ) {
                while( s_query.NEXT() EQ true ) {

                    int fis_satiri_id = s_query.VALUE("fis_satiri_id").toInt();
                    int islem_turu    = s_query.VALUE("gider_turu").toInt();

                    if ( islem_turu EQ -1 ) {
                        t_query.PREPARE_UPDATE("isl_fis_satirlari", "fis_satiri_id", "gider_turu", "fis_satiri_id = :fis_satiri_id");
                        t_query.SET_VALUE     (":gider_turu", ENUM_MAL_SATISI );
                        t_query.SET_VALUE     (":fis_satiri_id", fis_satiri_id );
                        t_query.UPDATE();
                    }
                }
            }
        }
    }
}
