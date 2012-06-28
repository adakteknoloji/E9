#ifndef PRK_CONSOLE_UTILS_H
#define PRK_CONSOLE_UTILS_H

#include <QList>

class QString;

int                 PRK_SIRADAKI_FIS_NO_AL              ( QString fis_tarihi);

QList <double>      PRK_GET_FIS_KDV_HARIC_TOPLAM_TUTAR ( QString bas_tarihi, QString bts_tarihi, double max_tutar, int fat_alis_satis_turu );


#endif // PRK_CONSOLE_UTILS_H
