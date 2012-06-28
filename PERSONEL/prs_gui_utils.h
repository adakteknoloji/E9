#ifndef PRS_GUI_UTILS_H
#define PRS_GUI_UTILS_H
#include "QCommaEdit.h"

class QComboBox;
class QWidget;
class QSearchEdit;
class QLineEdit;


void            PRS_FILL_COMBO_BOX_EKSIK_GUN_NEDENLERI          ( QComboBox * combo_box_eksik_gun_nedenleri );

void            PRS_FILL_COMBO_BOX_ISTEN_CIKMA_NEDENLERI        ( QComboBox * combo_box_isten_cikma_nedenleri );
void            PRS_FILL_COMBO_BOX_OZEL_DURUMLAR                ( QComboBox * combo_box_ozel_durumlar );

void            PRS_HIGHLIGHT_NON_DEFAULT_COMMA_EDIT            ( QCommaEdit * comma_edit, double default_value, int sabit_deger_turu );

void            PRS_FILL_COMBO_BOX_MEDENI_HAL                   ( QComboBox*    p_combo_box_medeni_hal);
void            PRS_FILL_COMBO_BOX_SUBE_SECIMI                  ( QComboBox*    combo_box_subeler);
void            PRS_FILL_COMBO_BOX_YABANCI_DILLER               ( QComboBox*    p_combo_box_yabanci_dil_1 ,QComboBox*p_combo_box_yabanci_dil_2);
void            PRS_FILL_COMBO_BOX_SENDIKA                      ( QComboBox*    p_combo_box_sendika_uyesi_mi);
void            PRS_FILL_COMBO_BOX_CALISMA_SEKLI                ( QComboBox*    p_combo_box_ucret_tipi) ;
void            PRS_FILL_COMBO_BOX_UCRET_TIPI                   ( QComboBox*    p_combo_box_ucret_sekli);

void            PRS_FILL_COMBO_BOX_BORDRO_YILLARI               ( QComboBox * );
void            PRS_FILL_COMBO_BOX_ODENEK_KESINTI_YARDIMLARI    ( QComboBox * p_combobox_odenek_kesinti_yardimi);

void            PRS_FILL_COMBO_BOX_CALISMA_TURLERI              ( QComboBox * p_combo_box );
void            PRS_FILL_COMBO_BOX_INDIRIM_KANUNLARI            ( QComboBox * p_combo_box );

int             PRS_GET_EKSIK_GUN_NEDENLERI_ENUM            ( QString p_eksik_gun_nedenleri );


#endif // PRS_GUI_UTILS_H
