#ifndef ISL_CON_UTILS_H
#define ISL_CON_UTILS_H

class QString;
class ADAK_SQL;

int             ISL_SIRADAKI_FIS_NO_AL                      ( QString p_fis_tarihi ,ADAK_SQL * P_GELEN_DB = NULL  );

QString         ISL_GET_FIS_TURU_STRING                     ( int fis_turu                  );
int             ISL_GET_FIS_TURU_ENUM                       ( QString fis_turu );


#endif // ISL_CON_UTILS_H
