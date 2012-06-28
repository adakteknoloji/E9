#ifndef PRS_KARTI_IMPORT_H
#define PRS_KARTI_IMPORT_H

#include "import_kernel.h"
#include "cari_fis_utils.h"

class PRS_KARTI_IMPORT : public IMPORT_KERNEL
{

public:
       PRS_KARTI_IMPORT ( QWidget * parent );
      ~PRS_KARTI_IMPORT() { }

private:
   void IMPORT_ROW ( QStringList row );
   CARI_FIS_STRUCT  *PRS_FIS;
   int               m_fis_id;

};


#endif // PRS_KARTI_IMPORT_H
