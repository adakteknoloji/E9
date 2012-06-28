#ifndef ADRES_IMPORT_H
#define ADRES_IMPORT_H

#include "import_kernel.h"

class ADRES_IMPORT : public IMPORT_KERNEL
{

public:
      ADRES_IMPORT ( QWidget * parent );
      ~ADRES_IMPORT() { }

private:
   void IMPORT_ROW (QStringList row );
   void TELEFON_BILGINI_PARSE_ET( QString p_tum_telefon, QString *p_ulke_kodu, QString *p_alan_kodu, QString *p_telefon );

};


#endif // ADRES_IMPORT_H
