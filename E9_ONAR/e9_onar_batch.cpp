#include <QMessageBox>
#include "e9_onar_batch_class.h"
#include "ui_e9_onar_batch.h"
#include "e9_enum.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "e9_log.h"
#include "e9_onar_utils.h"
#include "sube_console_utils.h"
#include "adak_gruplar.h"
#include "e9_version_upgrade.h"
#include "belge_kernel_defines.h"


extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

/**************************************************/
/*              OPEN_E9_ONAR_BATCH                */
/**************************************************/

void OPEN_E9_ONAR_BATCH (QWidget * parent)
{
    E9_ONAR_BATCH * e9_onar = new E9_ONAR_BATCH(parent);
    e9_onar->EXEC( NOT_FULL_SCREEN );
}

/**************************************************/
/*  E9_ONAR_BATCH::E9_ONAR_BATCH                  */
/**************************************************/

E9_ONAR_BATCH::E9_ONAR_BATCH(QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::E9_ONAR_BATCH)
{
    m_ui->setupUi     (this);
    START_BATCH_KERNEL(this, DB );
}

/*****************************************************/
/*         E9_ONAR_BATCH::SETUP_FORM                  */
/*****************************************************/

void E9_ONAR_BATCH::SETUP_FORM()
{    
    SET_PAGE_TITLE                  ( tr ( "E9 ONAR" ) );
    SET_HELP_PAGE                   ( "yonetim-islemleri_e9-onar" );

    SET_AUTO_EXIT_BATCH             ( true );

    REGISTER_BUTTON_WIDGET                          ( m_ui->widget_batch_buttons );

    SET_ENTER_KEY_FOR_RUN_BATCH     ( true );
    SET_SETTING_NAME                ( "E9_ONAR" );
    SET_NAME_OF_RUN_BATCH_BUTTON    ( "Onar" );

    // default olarak tum sistemi onar gelecek
    m_ui->checkBox_tum_sistemi_onar->setChecked( true );
    m_ui->label_moduller->setHidden(true);
    m_ui->frame_moduller->setHidden(true);
    m_tum_sistemi_onar = true;
    //

    SET_FIRST_FOCUS_WIDGET          ( m_ui->checkBox_tum_sistemi_onar );
    m_ui->textEdit_aciklama->setReadOnly  ( true );

    BILGILENDIR ( "Bulunduğunuz firmadaki , seçmiş olduğunuz modüllerin fişlerini,"
                  "hesap bakiyelerini, depodaki ve ürün kartındaki stok miktarını onarır." );
}


/*****************************************************/
/*         E9_ONAR_BATCH::CHECK_VAR                  */
/*****************************************************/

int E9_ONAR_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->checkBox_tum_sistemi_onar ) {
        if ( m_ui->checkBox_tum_sistemi_onar->isChecked() EQ true ) {
            m_ui->label_moduller->setHidden(true);
            m_ui->frame_moduller->setHidden(true);
            m_tum_sistemi_onar = true;
        }
        else {
            m_ui->label_moduller->setHidden(false);
            m_ui->frame_moduller->setHidden(false);
            m_tum_sistemi_onar = false;
        }
    }
    return ADAK_OK;
}

/*****************************************************/
/*       E9_ONAR_BATCH::CHECK_RUN                  */
/*****************************************************/

int E9_ONAR_BATCH::CHECK_RUN()
{
    if ( m_tum_sistemi_onar EQ false ) {
        if (m_ui->checkBox_banka_modulu->isChecked() EQ false AND m_ui->checkBox_cari_modulu->isChecked() EQ false AND
            m_ui->checkBox_muhasebe_modulu->isChecked() EQ false AND m_ui->checkBox_stok_modulu->isChecked() EQ false
            AND m_ui->checkBox_isletme_modulu->isChecked() EQ false){
            MSG_WARNING( tr("Lütfen onarılacak modül veya modülleri seçiniz!.."), NULL );
            return ADAK_FAIL;
        }
    }
    int msg_secim   = MSG_YES_NO( tr( "Onarim işlemi başlıyacaktır. Devam etmek istermisiniz ?" ), NULL );

    if ( msg_secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/*****************************************************/
/*          E9_ONAR_BATCH::RUN_BATCH                 */
/*****************************************************/

void E9_ONAR_BATCH::RUN_BATCH()
{

    // BENGE TASARIMCISI ICIN EKLENDI SILINECEK


   //! xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//    SQL_QUERY query( G_YONETIM_DB );
//    SQL_QUERY s_query( G_YONETIM_DB );

//    query.PREPARE_SELECT( "ynt_belge_degiskenleri, ynt_belge_tasarimlari",
//                          "belge_degisken_id, degisken_id, grup, yazi_boyutu, belge_id,text",
//                          "ynt_belge_tasarimlari.tasarim_id = ynt_belge_degiskenleri.tasarim_id " );

//    if ( query.SELECT() NE 0 ) {
//        while( query.NEXT() EQ true ) {

//            int belge_degisken_id = query.VALUE(0).toInt();
//            int degisken_id       = query.VALUE(1).toInt();
//            QString grup          = query.VALUE(2).toString();
//            int   text_size       = query.VALUE(3).toInt();
//            int   belge_id        = query.VALUE(4).toInt();

//            QString text          = query.VALUE(5).toString();

//            int grup_enum = grup.toInt();

//            if ( grup EQ "USER_TEXT_VARIABLE" ) {
//                grup_enum = 40;
//            }
//            if ( grup EQ "USER_PIXMAP_VARIABLE" ) {
//                grup_enum = 50;
//            }

//            for (int  j = 0;  j < ADAK_BELGE_COUNT; ++ j) {
//                if ( e9_belge_struct[j].belge_id EQ belge_id ) {

//                    if ( grup_enum EQ HEADER ) {
//                        for (int i = 0; i < e9_belge_struct[j].header_struct_size; ++i) {
//                            if ( e9_belge_struct[j].header_struct[i].degisken_id EQ degisken_id ) {
//                                text_size = e9_belge_struct[j].header_struct[i].karakter_sayisi;
//                                text      = e9_belge_struct[j].header_struct[i].degisken_adi;
//                            }
//                        }
//                    }
//                    else if ( grup_enum EQ LINE ) {
//                        for (int i = 0; i < e9_belge_struct[j].line_struct_size; ++i) {
//                            if ( e9_belge_struct[j].line_struct[i].degisken_id EQ degisken_id ) {
//                                text_size = e9_belge_struct[j].line_struct[i].karakter_sayisi;
//                                text      = e9_belge_struct[j].line_struct[i].degisken_adi;
//                            }

//                        }
//                    }
//                    else { // FOOTER
//                        for (int i = 0; i < e9_belge_struct[j].footer_struct_size; ++i) {
//                            if ( e9_belge_struct[j].footer_struct[i].degisken_id EQ degisken_id ) {
//                                text_size = e9_belge_struct[j].footer_struct[i].karakter_sayisi;
//                                text      = e9_belge_struct[j].footer_struct[i].degisken_adi;
//                            }
//                        }
//                    }
//                }
//            }

//            G_YONETIM_DB->START_TRANSACTION();
//            s_query.PREPARE_UPDATE( "ynt_belge_degiskenleri", "belge_degisken_id",
//                                    "grup_enum, text_size, text", "belge_degisken_id = :belge_degisken_id");
//            s_query.SET_VALUE     ( ":grup_enum", grup_enum );
//            s_query.SET_VALUE     ( ":text"     , text );
//            s_query.SET_VALUE     ( ":text_size", text_size );
//            s_query.SET_VALUE     ( ":belge_degisken_id", belge_degisken_id);
//            s_query.UPDATE();
//            G_YONETIM_DB->COMMIT_TRANSACTION();

//        }
//    }

    //! xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

    ADAK_CURSOR_BUSY();

    // Ceksenet hatali fis_uretiyordu. Disaridan gelen
    // tum fis satilarini tekrar kendilerine gonderiyordu.
    // Bundan dolayi eklendi

    DB->START_TRANSACTION();
    CEK_SENETIN_YANLIS_URETTIGI_FISLERI_SIL();
    FATURA_VE_IRSALIYE_FISLERINI_ONAR();

    QString log_detaylari = QObject::tr("E9 ONAR");

    if ( m_tum_sistemi_onar EQ false ) {
        QString message;

        bool muhasebe_modulu_onarilacak = false;
        bool cari_modulu_onarilacak     = false;
        bool banka_modulu_onarilacak    = false;
        bool stok_modulu_onarilacak     = false;
        bool isletme_modulu_onarilacak  = false;

        QMessageBox e9_onar_message (this);
        e9_onar_message.setWindowTitle(tr("Dikkat"));


        message =  QString("Bulunduğunuz firmanın ");

        int modul_sayisi = 0;

        if(m_ui->checkBox_muhasebe_modulu->isChecked() EQ true) {
            muhasebe_modulu_onarilacak = true;
            message.append(" MUHASEBE ");
            modul_sayisi++;
        }
        if(m_ui->checkBox_cari_modulu->isChecked() EQ true ) {
            cari_modulu_onarilacak = true;
            if(modul_sayisi NE 0 ) {
                message.append(",");
            }
            message.append(" CARİ ");
            modul_sayisi++;
        }
        if(m_ui->checkBox_banka_modulu->isChecked() EQ true ) {
            banka_modulu_onarilacak = true;
            if(modul_sayisi NE 0 ) {
                message.append(",");
            }
            message.append(" BANKA ");
            modul_sayisi++;
        }
        if(m_ui->checkBox_stok_modulu->isChecked() EQ true ) {
            stok_modulu_onarilacak = true;
            if(modul_sayisi NE 0 ) {
                message.append(",");
            }
            message.append(" STOK ");
            modul_sayisi++;
        }
        if ( m_ui->checkBox_isletme_modulu->isChecked() EQ true ) {
            isletme_modulu_onarilacak = true;

            if(modul_sayisi NE 0 ) {
                message.append(",");
            }
            message.append(" İŞLETME ");
            modul_sayisi++;
        }

        if(modul_sayisi EQ 1) {
            if(stok_modulu_onarilacak EQ true ) {
               message.append(" Modülündeki fişler , ürün kartındaki,depodaki ve partilerdeki stok miktarları onarılacak.\n");
            }
            else {
                message.append(" Modülündeki fişler , hesaplar ve hesap bakiyeleri onarılacak.\n");
            }
        }
        else {
           message.append(" Modülleri Onarılacak.\n");
        }
        e9_onar_message.setText(message);

        e9_onar_message.setInformativeText(tr("Emin misiniz?"));
        e9_onar_message.addButton(QMessageBox::Ok);
        e9_onar_message.addButton(QMessageBox::Cancel);
        e9_onar_message.setButtonText(QMessageBox::Ok, tr("Devam"));
        e9_onar_message.setButtonText(QMessageBox::Cancel, tr("Vazgeç"));
        e9_onar_message.exec();

        if ( e9_onar_message.clickedButton() EQ e9_onar_message.button(QMessageBox::Cancel) ) {

            return;
        }

        m_ui->textEdit_aciklama->clear();
        QCoreApplication::processEvents();

        if(muhasebe_modulu_onarilacak EQ true) {
            BILGILENDIR ("<font color = \"blue\">***** MUHASEBE MODÜLÜ *****</font><br>");
            MUHASEBE_BAKIYELERINI_SIFIRLA ();
            MUHASEBE_MODULUNU_ONAR();
            log_detaylari.append(" , MUHASEBE MODÜLÜ");

        }
        if(cari_modulu_onarilacak EQ true) {
            BILGILENDIR ("<font color = \"blue\">***** CARİ MODÜLÜ *****</font><br>");
            CARI_BAKIYELERINI_SIFIRLA ();
            CARI_MODULUNU_ONAR();
            log_detaylari.append(" , CARİ MODÜLÜ");
        }
        if(banka_modulu_onarilacak EQ true ) {
            BILGILENDIR ("<font color = \"blue\">***** BANKA MODÜLÜ *****</font><br>");
            BANKA_BAKIYELERINI_SIFIRLA ();
            BANKA_MODULUNU_ONAR();
            log_detaylari.append(" , BANKA MODÜLÜ");
        }
        if(stok_modulu_onarilacak EQ true ) {
            BILGILENDIR ("<font color = \"blue\">***** STOK MODÜLÜ *****</font><br>");
            STOK_BAKIYELERINI_SIFIRLA ();
            STOK_MODULUNU_ONAR();
            log_detaylari.append(" , STOK MODÜLÜ");
        }

        if(isletme_modulu_onarilacak EQ true ) {
            BILGILENDIR ("<font color = \"blue\">***** İŞLETME MODÜLÜ *****</font><br>");
            ISLETME_MODULUNU_ONAR();
            log_detaylari.append(" , İŞLETME MODÜLÜ");
        }

        log_detaylari.append(" ONARILDI");
    }
    else {
        BILGILENDIR (QString("<font color = \"red\"> %1</font> - E9 Öntanımlı Entegrasyon Hesapları Kontrol Ediliyor...<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        E9_UPDATE_TANIMLANMAMIS_ENT_HESAPLARI();

        SQL_QUERY query( DB );
        query.PREPARE_SELECT( "sub_subeler", "sube_id");
        if( query.SELECT() NE 0 ) {
            while( query.NEXT() EQ true ) {
                int sube_id = query.VALUE( 0 ).toInt();
                E9_SUBE_ENT_HESAPLARINI_GUNCELLE( sube_id );
            }
        }


        BILGILENDIR (QString("<font color = \"red\"> %1</font> - E9 Entegrasyon Hesapları Kontrol Edildi , Eksik Entegrasyon Hesapları Düzeltildi.<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

        TUM_SISTEMI_ONAR();
    }

    HATALI_FISLERIN_ENT_FISLERINI_YOK_ET();

    E9_LOG_KAYDI_EKLE ( E9_YONETIM , LOG_E9_ONAR , -1 , log_detaylari);

    DB->COMMIT_TRANSACTION();

    ADAK_CURSOR_NORMAL();

    MSG_WARNING( QObject::tr("Onarma işlemi tamamlandı."), NULL );

}

/*****************************************************/
/*          E9_ONAR_BATCH::CHECK_EXIT               */
/*****************************************************/

int E9_ONAR_BATCH::CHECK_EXIT()
{
    return ADAK_OK;
}

/*****************************************************/
/*          E9_ONAR_BATCH::BILGILENDIR               */
/*****************************************************/

void E9_ONAR_BATCH::BILGILENDIR (QString mesaj)
{
    m_ui->textEdit_aciklama->append(mesaj);
    QCoreApplication::processEvents();
}

/**************************************************************************************
                   TUM_SISTEMI_ONAR
***************************************************************************************/


void E9_ONAR_BATCH::TUM_SISTEMI_ONAR()
{

    int program_id = E9_PROGRAMI;

    MUH_ENT_FISLERINI_SIL    (program_id , MUHASEBE_MODULU , DB);

    CARI_ENT_FISLERINI_SIL   (program_id , CARI_MODULU     , DB);

    BNK_ENT_FISLERINI_SIL    (program_id , BANKA_MODULU    , DB);

    // BU FONKSIYON ENT FISLERI SILINMEDEN ONCE CAGRILMALIDIR.
    // ICIN CEK DURUMLARI SILBASTAN OLUSTURULMAKTADIR.
    TUM_CEK_DURUMLARINI_ONAR();

    CEK_ENT_FISLERINI_SIL    (program_id , CEKSENET_MODULU , DB);

    FAT_ENT_FISLERINI_SIL    (program_id , FATURA_MODULU   , DB);

    PRK_ENT_FISLERINI_SIL    (program_id , PERAKENDE_MODULU, DB);

    // Personel Modulu Disaridan herhangi bir Modul tarafindan kullanilmadigi icin silinecek bir sey yok,

    QString info = GET_MUH_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">MUHASEBE MODÜLÜ</font>" + info + "<br>"));
    }

    info = GET_CARI_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">CARİ MODÜLÜ</font>" + info + "<br>"));
    }

    info = GET_BNK_HAREKET_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">BANKA MODÜLÜ</font>" + info + "<br>"));
    }
    info = GET_BNK_DEFTER_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">BANKA MODÜLÜ</font>" + info + "<br>"));
    }

    info = GET_BNK_MAKRO_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">BANKA MODÜLÜ</font>" + info + "<br>"));
    }

    info = GET_CEK_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">ÇEKSENET MODÜLÜ</font>" + info + "<br>"));
    }

    info = GET_FAT_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">FATURA MODÜLÜ</font>" + info + "<br>"));
    }

    info = GET_PRK_ENT_KALAN_FIS_VE_SATIR_BILGISI(program_id);

    if ( info.isEmpty() EQ false ) {
        BILGILENDIR (QString(" <font color = \"red\">PERAKENDE MODÜLÜ</font>" + info + "<br>"));
    }

    MUHASEBE_BAKIYELERINI_SIFIRLA ();
    MUHASEBE_MODULUNU_ONAR();

    CARI_BAKIYELERINI_SIFIRLA ();
    CARI_MODULUNU_ONAR();

    BANKA_BAKIYELERINI_SIFIRLA ();
    BANKA_MODULUNU_ONAR();

    STOK_BAKIYELERINI_SIFIRLA ();
    STOK_MODULUNU_ONAR();

    ISLETME_MODULUNU_ONAR();

    CARI_ENT_FISLERINI_OLUSTUR(program_id);

    BILGILENDIR (QString(" <font color = \"red\">%1</font> CARİ MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    BNK_ENT_FISLERINI_OLUSTUR(program_id);

    BILGILENDIR (QString(" <font color = \"red\">%1</font> BANKA MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    CEK_ENT_FISLERINI_OLUSTUR(program_id);

    BILGILENDIR (QString(" <font color = \"red\">%1</font> ÇEKSENET MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    FAT_ENT_FISLERINI_OLUSTUR(program_id);

    BILGILENDIR (QString(" <font color = \"red\">%1</font> FATURA MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    PRK_ENT_FISLERINI_OLUSTUR(program_id);

    BILGILENDIR (QString(" <font color = \"red\">%1</font> PERAKENDE MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    STK_ENT_FISLERINI_OLUSTUR( program_id );

    BILGILENDIR (QString(" <font color = \"red\">%1</font> STOK MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    PRS_ENT_FISLERINI_OLUSTUR(program_id);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> PERSONEL MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    DMR_ENT_FISLERINI_OLUSTUR(program_id);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> SABİT KIYMET MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    SM_MAKBUZ_ENT_FISLERINI_OLUSTUR(program_id);
    BILGILENDIR (QString(" <font color = \"red\">%1</font> SM MAKBUZ MODÜLÜ ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));

    //Gruplardaki gruba kayitli sayisinda hata varsa kullanilabilir.

    SQL_QUERY query (DB);
    SQL_QUERY sql_query (DB);

    query.PREPARE_SELECT("adak_gruplar","grup_id,program_id,modul_id");
    query.SELECT();

    while( query.NEXT() EQ true ) {
        QList<int> grup_idleri;
        grup_idleri << query.VALUE(0).toInt();
        sql_query.PREPARE_UPDATE("adak_gruplar","grup_id","gruptaki_kayit_sayisi","grup_id=:grup_id");
        sql_query.SET_VALUE( ":grup_id",query.VALUE(0).toInt());
        sql_query.SET_VALUE( ":gruptaki_kayit_sayisi",GRP_GRUBA_EKLI_KAYITLARI_BUL(query.VALUE(1).toInt(),query.VALUE(2).toInt(),grup_idleri).size());
        sql_query.UPDATE();
    }

    BILGILENDIR (QString(" <font color = \"red\">%1</font> GRUPLAR ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));


    BILGILENDIR (QString(" <font color = \"red\">%1</font> TÜM SİSTEM ONARILDI" + info + "<br>").arg(QTime::currentTime().toString("hh:mm:ss")));
}




