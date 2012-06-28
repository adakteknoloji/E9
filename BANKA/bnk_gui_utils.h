#ifndef BNK_GUI_UTILS_H
#define BNK_GUI_UTILS_H

class QString;
class QComboBox;
class QVariant;

void        BNK_GET_FIRMANIN_BANKA_SUBELERI                  ( QString banka_ismi,QComboBox * combobox);

int         BNK_HESAPLARI_MODEL_INDEX_BUL                    ( int p_banka_hesap_id, QComboBox * p_comboBox );

void        BNK_FILL_COMBOBOX_HESAP_TURU                     ( QComboBox * combo_box);

void        BNK_FILL_COMBO_BOX_ETK_HESAP_TURU                ( QComboBox * combo_box);

void        BNK_FILL_COMBO_BOX_KRS_HESAP_TURU                ( QComboBox * combo_box);

void        BNK_FILL_COMBO_BOX_BANKA_HESAPLARI               ( QComboBox * combo_box , QString sube_kodu = "");

void        BNK_FILL_COMBO_BOX_HESABA_ETKISI                 ( QComboBox * combo_box);

QVariant    BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL            ( QComboBox * combo_box, int row , int column );

void        BNK_FILL_COMBO_BOX_ENT_FIS_TURLERI               ( QComboBox * p_combo_box );



#endif // BNK_GUI_UTILS_H
