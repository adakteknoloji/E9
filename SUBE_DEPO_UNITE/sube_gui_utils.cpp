#include <QComboBox>


/*****************************************************************/
/*            SUB_FILL_DEPO_ISLEM_YETKISI_COMBOBOX               */
/*****************************************************************/

void SUBE_FILL_DEPO_ISLEM_YETKISI_COMBOBOX ( QComboBox * comboBox )
{
    comboBox->addItems(QStringList() << QObject::tr("SADECE ÜNİTE DEPOSUNDA İŞLEM YAPABİLİR")
                                     << QObject::tr("TÜM DEPOLARDA İŞLEM YAPABİLİR"));
}
/*****************************************************************/
/*            SUB_FILL_SUBE_ISLEM_YETKISI_COMBOBOX               */
/*****************************************************************/

void SUBE_FILL_SUBE_ISLEM_YETKISI_COMBOBOX ( QComboBox * comboBox )
{
    comboBox->addItems(QStringList() << QObject::tr("SADECE ÜNİTENİN BULUNDUĞU ŞUBEDE İŞLEM YAPABİLİR")
                                     << QObject::tr("TÜM ŞUBELERDE İŞLEM YAPABİLİR"));
}
