#ifndef FAT_FATURA_DETAYLARI_BATCH_OPEN_H
#define FAT_FATURA_DETAYLARI_BATCH_OPEN_H

class QWidget;
struct FATURA_DETAYLARI_STRUCT;

bool OPEN_FAT_FATURA_DETAYLARI_BATCH ( FATURA_DETAYLARI_STRUCT * P_FATURA_DETAYLARI, int p_fis_id, int p_fatura_alis_satis_iade_turu, QWidget * p_parent, int p_iade_faturasi_mi = 0 );

#endif // FAT_FATURA_DETAYLARI_BATCH_OPEN_H
