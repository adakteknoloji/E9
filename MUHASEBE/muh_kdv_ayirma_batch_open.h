#ifndef MUH_KDV_AYIRMA_BATCH_OPEN_H
#define MUH_KDV_AYIRMA_BATCH_OPEN_H

#include <QString>
class QWidget;

bool OPEN_KDV_AYIRMA ( int p_fis_satiri_id , double  p_kdv_dahil_tutar , QString p_aciklama ,
                      void *  P_MUH_KDV_AYITMA_STRUCT,  QWidget * p_parent );

#endif // MUH_KDV_AYIRMA_BATCH_OPEN_H
