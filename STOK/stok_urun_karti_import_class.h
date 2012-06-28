#ifndef STOK_URUN_KARTI_IMPORT_H
#define STOK_URUN_KARTI_IMPORT_H

#include "import_kernel.h"
#include "stok_fis_utils.h"

class STOK_URUN_KARTI_IMPORT : public IMPORT_KERNEL
{

public:
       STOK_URUN_KARTI_IMPORT (QWidget * parent);

protected:

        int                  m_fis_id ;
        STK_IRS_FIS_STRUCT  *STOK_FIS;

        QStringList          SET_DEFAULT_ROW_VALUES();
        void                 IMPORT_ROW( QStringList row );

};

//void                OPEN_STOK_URUN_KARTI_IMPORT           (QWidget * parent);

#endif // STOK_URUN_KARTI_IMPORT_H
