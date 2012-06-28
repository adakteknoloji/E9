#ifndef ADAK_BELGE_STRUCTURE
#define ADAK_BELGE_STRUCTURE

class QString;

struct ADAK_BELGE_DEFAULTS_STRUCT {
        int          	degisken_id;
        const char * 	degisken_kodu;
        const char * 	degisken_adi;
        int          	font_size;
        int          	align;
        bool         	is_visible_variable;
        double         	x_position;
        double         	y_position;
        int             monospace_mi;
        int             karakter_sayisi;
};

struct ADAK_BELGELER_STRUCT {
	int         			belge_id;
        const char *  			belge_adi;
	int     			header_struct_size;
	int     			footer_struct_size;
	int     			line_struct_size;
	ADAK_BELGE_DEFAULTS_STRUCT *   header_struct;	
	ADAK_BELGE_DEFAULTS_STRUCT *   footer_struct;
	ADAK_BELGE_DEFAULTS_STRUCT *   line_struct;  
};

#endif // ADAK_BELGE_STRUCTURE
