#ifndef CARI_KART_IMPORT_CLASS_H
#define CARI_KART_IMPORT_CLASS_H

#include <QStringList>
#include "import_kernel.h"
#include "cari_fis_utils.h"

class CARI_KART_IMPORT : public IMPORT_KERNEL
{
public:
    CARI_KART_IMPORT( QWidget *parent );
    ~CARI_KART_IMPORT(){}

private:
   void              IMPORT_ROW ( QStringList row );
   int               m_fis_id ;
   CARI_FIS_STRUCT  *CARI_FIS;
   void              TELEFON_BILGISINI_PARSE_ET( QString &ulke_kodu , QString &alan_kodu, QString &telefon, QString &tum_telefon );
   void              ADRES_BILGISINI_PARSE_ET( QString &adres_satiri_1, QString &adres_satiri_2, QString &adres_satiri_3, QString &tum_adres );

};

#endif // CARI_KART_IMPORT_CLASS_H
