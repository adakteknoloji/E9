#include <QComboBox>
#include "adak_defines.h"
#include "cek_enum.h"


/**************************************************************************************
                   CEK_FILL_COMBO_BOX_BORDRO_ISLEM_TURU
***************************************************************************************/

void  CEK_FILL_COMBO_BOX_BORDRO_ISLEM_TURU ( int p_cek_senet_turu, QComboBox * p_comboBox, int p_fis_kaynagi )
{
    p_comboBox->clear();

    if ( p_fis_kaynagi EQ CARI_MODULU ) {

        p_comboBox->addItems ( QStringList ()<< QObject::tr ( "Satıcıya Verildi ( Firma Çekleri )"           )
                                             << QObject::tr ( "Satıcıya Verildi ( Firma Senetleri )"         )
                                             << QObject::tr ( "Satıcıya Verildi ( Müşteri Çekleri )"         )
                                             << QObject::tr ( "Satıcıya Verildi ( Müşteri Senetleri )"       )
                                             << QObject::tr ( "Müşteriden Çek Alındı ( Müşteri Çekleri )"     )
                                             << QObject::tr ( "Müşteriden Senet Alındı ( Müşteri Senetleri )" ) );
        return;
    }

    switch ( p_cek_senet_turu ) {

        case ENUM_FIRMA_CEK_BORDROSU   :
        case ENUM_FIRMA_SENET_BORDROSU :

            p_comboBox->addItems ( QStringList ()<< QObject::tr ( "Satıcıdan iade alındı" )
                                                 << QObject::tr ( "Satıcıya verildi / Ciro edildi" )
                                                 << QObject::tr ( "Ödendi - Nakit"         )
                                                 << QObject::tr ( "Ödendi - Hesaptan"      )
                                                 << QObject::tr ( "İptal"                  ));
            break;

        case ENUM_MUSTERI_CEK_BORDROSU   :
        case ENUM_MUSTERI_SENET_BORDROSU :

            p_comboBox->addItems ( QStringList () << QObject::tr ( "Bankadan iade alındı" )
                                                  << QObject::tr ( "Satıcıdan iade alındı" )
                                                  << QObject::tr ( "Bankaya teminat için verildi" )
                                                  << QObject::tr ( "Bankaya tahsilat için verildi" )
                                                  << QObject::tr ( "Satıcıya verildi / Ciro edildi" )
                                                  << QObject::tr ( "Müşteriye iade edildi" )
                                                  << QObject::tr ( "Tahsil edildi - Nakit" )
                                                  << QObject::tr ( "Tahsil edildi - Hesaba" ));

            if ( p_cek_senet_turu EQ ENUM_MUSTERI_CEK_BORDROSU ) {
                p_comboBox->addItem ( QObject::tr ( "Karşılıksız" ) );
            }
            else {
                p_comboBox->addItem ( QObject::tr ( "Protestolu" ) );
            }

            p_comboBox->addItems ( QStringList () << QObject::tr ( "Şüpheli Alacaklar" )
                                                  << QObject::tr ( "Tahsil Edilemeyen" ) );
            break;

            case  ENUM_CEK_SENET_ACILIS_BORDROSU :

                p_comboBox->addItems ( QStringList ()<< QObject::tr ( "Satıcıya Verildi ( Firma Çekleri )" )
                                                     << QObject::tr ( "Satıcıya Verildi ( Firma Senetleri )" )
                                                     << QObject::tr ( "Satıcıya Verildi ( Müşteri Çekleri )" )
                                                     << QObject::tr ( "Satıcıya Verildi ( Müşteri Senetleri )" )
                                                     << QObject::tr ( "Bankaya Teminat İçin Verildi ( Müşteri Çekleri )" )
                                                     << QObject::tr ( "Bankaya Teminat İçin Verildi ( Müşteri Senetleri )" )
                                                     << QObject::tr ( "Bankaya Tahsilat İçin Verildi ( Müşteri Çekleri )" )
                                                     << QObject::tr ( "Bankaya Tahsilat İçin Verildi ( Müşteri Senetleri )" )
                                                     << QObject::tr ( "Karşılıksız ( Müşteri Çekleri )" )
                                                     << QObject::tr ( "Protestolu  ( Müşteri Senetleri )" )
                                                     << QObject::tr ( "Şüpheli Alacaklar ( Müşteri Çekleri )" )
                                                     << QObject::tr ( "Şüpheli Alacaklar ( Müşteri Senetleri )" )
                                                     << QObject::tr ( "Portföyde ( Müşteri Çekleri )")
                                                     << QObject::tr ( "Portföyde ( Müşteri Senetleri )"));

                break;

    }
}

/**************************************************************************************
                   CEK_FILL_ENT_FIS_TURU_COMBO_BOX
***************************************************************************************/

void  CEK_FILL_ENT_FIS_TURU_COMBO_BOX ( QComboBox * p_combo_box )
{
    p_combo_box->addItems ( QStringList ()  << QObject::tr ( "Cari Ent. Tüm Fişler" )
                                            << QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Firma Çekleri )" )
                                            << QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Firma Senetleri )" )
                                            << QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Müşteri Çekleri )" )
                                            << QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Müşteri Senetleri )" )
                                            << QObject::tr ( "Cari Ent. Fişi-Müşteriden Çek Alındı" )
                                            << QObject::tr ( "Cari Ent. Fişi-Müşteriden Senet Alındı" ) );

}
