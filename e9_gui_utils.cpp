#include <QComboBox>
#include <QtGui>
#include <QAdakDate.h>
#include "adak_sql.h"
#include "adak_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "sube_console_utils.h"

extern ADAK_SQL * DB;

static int                      g_kdv_orani_sayisi                      = 0;
static int                      g_otv_orani_sayisi                      = 0;
static double                   G_KDV_ORANLARI_ARRAY [KDV_OTV_ORANLARI_SAYISI];
static double                   G_OTV_ORANLARI_ARRAY [KDV_OTV_ORANLARI_SAYISI];



//*******************************************************************************
//             E9_FILL_COMBOBOX_AYLAR
//*******************************************************************************

void E9_FILL_COMBOBOX_AYLAR ( QComboBox * p_combo_box )
{
    QStringList month_list;
    month_list  << "OCAK"   << "ŞUBAT"      << "MART"
                << "NİSAN"  << "MAYIS"      << "HAZİRAN"
                << "TEMMUZ" << "AĞUSTOS"    << "EYLÜL"
                << "EKİM"   << "KASIM"      << "ARALIK";

    p_combo_box->addItems( month_list );

}

/********************************************************************************************************
                                       E9_FILL_KDV_ORANLARI_COMBOBOX
*********************************************************************************************************/

void E9_FILL_KDV_ORANLARI_COMBOBOX ( QComboBox * p_comboBox )
{
    if ( E9_GET_KDV_ORANLARI_OKUNDUMU_FLAG() EQ false ) {
        SQL_QUERY sql_query ( DB );
        sql_query.PREPARE_SELECT("e9_sabit_degerler","kdv_orani_sayisi,kdv_oranlari_array ");
        if ( sql_query.SELECT() EQ 0 ) {
            return;
        }
        sql_query.NEXT();
        g_kdv_orani_sayisi = sql_query.VALUE(0).toInt();
        UNPACK_DOUBLE_ARRAY(sql_query.VALUE(1).toString() , G_KDV_ORANLARI_ARRAY , KDV_OTV_ORANLARI_SAYISI);

        E9_SET_KDV_ORANLARI_OKUNDUMU_FLAG(true);
    }

    for ( int i = 0; i < g_kdv_orani_sayisi; i++ ) {
        p_comboBox->addItem ( QVariant ( G_KDV_ORANLARI_ARRAY[i] ).toString() );
    }
    p_comboBox->setEditable(false);
}

/********************************************************************************************************
                                       E9_FILL_OTV_ORANLARI_COMBOBOX
*********************************************************************************************************/

void E9_FILL_OTV_ORANLARI_COMBOBOX ( QComboBox * p_comboBox )
{
    if ( E9_GET_OTV_ORANLARI_OKUNDUMU_FLAG() EQ false ) {
        SQL_QUERY sql_query ( DB );
        sql_query.PREPARE_SELECT("e9_sabit_degerler","otv_orani_sayisi,otv_oranlari_array");
        if ( sql_query.SELECT() EQ 0 ) {
            return;
        }
        sql_query.NEXT();
        g_otv_orani_sayisi = sql_query.VALUE(0).toInt();
        UNPACK_DOUBLE_ARRAY(sql_query.VALUE(1).toString() , G_OTV_ORANLARI_ARRAY , KDV_OTV_ORANLARI_SAYISI);

        E9_SET_OTV_ORANLARI_OKUNDUMU_FLAG(true);
    }

    for ( int i = 0; i < g_otv_orani_sayisi; i++ ) {
        p_comboBox->addItem ( QVariant ( G_OTV_ORANLARI_ARRAY[i] ).toString() );
    }
    p_comboBox->setEditable(false);
}

/**************************************************************************************
                   E9_FILL_COMBOBOX_FIS_FILTRESI()
***************************************************************************************/

void E9_FILL_COMBOBOX_FIS_FILTRESI(QComboBox *combobox_entegre_filtresi)
{

    combobox_entegre_filtresi->addItems(QStringList() << QObject::tr("Hepsi")
                                        << QObject::tr("1.Şirkete Ait Olanlar") << QObject::tr("2.Şirkete Ait Olanlar"));
}

/********************************************************************************************************
                                    E9_FILL_COMBOBOX_ADAK_MODULES
*********************************************************************************************************/

void E9_FILL_COMBOBOX_ADAK_MODULES ( QComboBox * p_comboBox )
{
    p_comboBox->addItems ( QStringList () << QObject::tr ("Muhasebe") << QObject::tr ("İşletme")
                                          << QObject::tr ("Döviz")    << QObject::tr ("Adres")
                                          << QObject::tr ("Cari")     << QObject::tr ("ÇekSenet")
                                          << QObject::tr ("Banka")    << QObject::tr ("Şube / Ünite / Depo")
                                          << QObject::tr ("Fatura")   << QObject::tr ("İrsaliye")
                                          << QObject::tr ("Stok")     << QObject::tr ("Demirbaş")
                                          << QObject::tr ("Personel") << QObject::tr ("Faiz")
                                          << QObject::tr ("E9 Yönetim"));

}

/**************************************************************************************
                   E9_SET_WINDOW_ICON
***************************************************************************************/

void E9_SET_WINDOW_ICON ( QWidget * p_window )
{
    p_window->setWindowIcon( QIcon( ":/e9_icons/E9_ICONS/e9_logo.png" ) );
}

/**************************************************************************************
                   E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI()
***************************************************************************************/

bool E9_SUBEYE_UNITE_KAYDI_YAPILDI_MI ()
{
    if ( SUBE_GET_UNITE_ID() EQ -1 ) {
        ADAK_INFO( QObject::tr("Bu bilgisayarın kayıtlı olduğu bir ünite bulunamadı."
                           "\nLütfen Şubeler / Depolar menüsünden bir şubeye üniteyi kaydediniz."), NULL,NULL);
        return false;
    }
    return true;
}

/**************************************************************************************
                   E9_MALI_YIL_TARIH_ARALIGI_KONTROLU
***************************************************************************************/

int E9_MALI_YIL_TARIH_ARALIGI_KONTROLU(QAdakDate *p_baslangic_tarihi, QAdakDate *p_bitis_tarihi)
{
    if ( p_baslangic_tarihi->QDATE() > p_bitis_tarihi->QDATE() ) {
        ADAK_WARNING( QObject::tr("Başlangıç tarihi,bitiş tarihinden büyük olamaz."), p_baslangic_tarihi, NULL );
        return ADAK_FAIL;
    }
    if ( p_bitis_tarihi->QDATE() < p_baslangic_tarihi->QDATE() ) {
        ADAK_WARNING( QObject::tr("Bitiş tarihi,başlangıç tarihinden büyük olamaz."), p_bitis_tarihi, NULL );
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   E9_FILL_KULLANICININ_CALISTIGI_SIRKETLER
***************************************************************************************/

void E9_FILL_KULLANICININ_CALISTIGI_SIRKETLER(QComboBox *p_combo_box)
{
    QStringList kullanici_calistigi_sirket;
    kullanici_calistigi_sirket << "Şirket 1" << "Şirket 2";
    p_combo_box->addItems( kullanici_calistigi_sirket );
    p_combo_box->setCurrentIndex( 0 );
}

