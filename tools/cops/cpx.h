/*
 * COPS (c) 1995 - 2003 Sven & Wilfried Behne
 *                 2004 F.Naumann & O.Skancke
 *
 * A XCONTROL compatible CPX manager.
 *
 * This file is part of COPS.
 *
 * COPS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * COPS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XaAES; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _cpx_h
#define _cpx_h

#include <setjmp.h>
#include <gemx.h>
#include "global.h"


#define	PH_MAGIC 0x601a		/* Magic des Programmheaders */

struct program_header
{
	short ph_branch;
	long  ph_tlen;
	long  ph_dlen;
	long  ph_blen;
	long  ph_slen;
	long  ph_res1;
	long  ph_prgflags;
	short ph_absflag;
};

struct foobar
{
	short	dummy;
	short	*image;
};

struct cl_segm
{
	void	*text_seg;
	long	len_text;
	void	*data_seg;
	long	len_data;
	void	*bss_seg;
	long	len_bss;
};

struct cpxhead
{
	unsigned short magic;

	unsigned short flags;
#define CPX_SETONLY		0x0001		/* Set Only CPX Flag 		*/
#define CPX_BOOTINIT	0x0002		/* Boot Initialization Flag	*/
#define CPX_RESIDENT	0x0004		/* RAM Resident Flag		*/

	long cpx_id;
	unsigned short cpx_version;
	char i_text[14];
	unsigned short icon[48];

	struct
	{
		unsigned i_color:	4;
		unsigned reserved:	4;
		unsigned i_char:	8;
	} i_info;

	char title_txt[18];

	struct
	{
		unsigned c_board:	4;
		unsigned c_text:	4;
		unsigned pattern:	4;
		unsigned c_back:	4;
	} t_info;

	char buffer[64];
	char reserved[306];
};

/* Mausparameter */
typedef struct
{
	short x;
	short y;
	short buttons;
	short kstate;
} MRETS;

/* Struktur zur Verwaltung von CPX-Modulen */
struct cpxlist
{
	char f_name[14];
	short head_ok;
	short segm_ok;
	struct cl_segm *segm;
	struct cpxlist *next;
	struct cpxhead header;
};

/*
 * exported functions to cpx (xcontrol parameter block)
 *
 * 32bit clean
 */

/* helper structs for 16bit argument alignment */
struct rsh_fix_args
{
	short num_objs; short num_frstr; short num_frimg; short num_tree;
	OBJECT *rs_object; TEDINFO *rs_tedinfo; char **rs_strings; ICONBLK *rs_iconblk;
	BITBLK *rs_bitblk; long *rs_frstr; long *rs_frimg; long *rs_trindex; struct foobar *rs_imdope;
};
struct rsh_obfix_args
{
	OBJECT *tree; short ob;
};
struct Popup_args
{
	char **items; short no_items; short slct; short font; GRECT *up; GRECT *world;
};
struct Sl_size_args
{
	OBJECT *tree; short base; short slider;
	short num_items; short visible; short direction; short min_size;
};
struct Sl_xy_args
{
	OBJECT *tree; _WORD base; _WORD slider; _WORD value;
	_WORD min; _WORD max; void _cdecl (*userdef)(void);
};
struct Sl_arrow_args
{
	OBJECT *tree; _WORD base; _WORD slider; _WORD obj; _WORD inc;
	_WORD min; _WORD max; _WORD *value; _WORD direction; void _cdecl (*userdef)(void);
};
struct Sl_dragxy_args
{
	OBJECT *tree; _WORD base; short slider;
	_WORD min; _WORD max; _WORD *value; void _cdecl (*userdef)(void);
};
struct Xform_do_args
{
	OBJECT *tree; _WORD edit_obj; _WORD *msg;
};
struct Set_Evnt_Mask_args
{
	_WORD mask; MOBLK *m1; MOBLK *m2; long evtime;
};
struct XGen_Alert_args
{
	_WORD alert_id;
};
struct MFsave_args
{
	_WORD flag; MFORM *mf;
};

struct xcpb
{
	short handle;
	short booting;
	short reserved;
	short SkipRshFix;

	struct cpxlist *_cdecl (*get_cpx_list)(void);

	short   _cdecl (*save_header)(struct cpxlist *header);
	void    _cdecl (*rsh_fix)(struct rsh_fix_args);
	void    _cdecl (*rsh_obfix)(struct rsh_obfix_args);
	short   _cdecl (*Popup)(struct Popup_args);
	void    _cdecl (*Sl_size)(struct Sl_size_args);
	void    _cdecl (*Sl_x)(struct Sl_xy_args);
	void    _cdecl (*Sl_y)(struct Sl_xy_args);
	void    _cdecl (*Sl_arrow)(struct Sl_arrow_args);
	void    _cdecl (*Sl_dragx)(struct Sl_dragxy_args);
	void    _cdecl (*Sl_dragy)(struct Sl_dragxy_args);
	short   _cdecl (*Xform_do)(struct Xform_do_args);
	GRECT * _cdecl (*GetFirstRect)(GRECT *prect);
	GRECT * _cdecl (*GetNextRect)(void);
	void    _cdecl (*Set_Evnt_Mask)(struct Set_Evnt_Mask_args);
	short   _cdecl (*XGen_Alert)(struct XGen_Alert_args);
	short   _cdecl (*CPX_Save)(void *ptr, long bytes);
	void *  _cdecl (*Get_Buffer)(void);
	short   _cdecl (*getcookie)(long cookie, long *p_value);

	short Country_Code;

	void _cdecl (*MFsave)(struct MFsave_args);
};

/*
 * CPX interface entry points
 *
 * 32bit clean
 */

/* helper structs for 16bit argument alignment */
struct cpx_key_args { short kstate; short key; short *quit; };
struct cpx_button_args { MRETS *mrets; short nclicks; short *quit; };
struct cpx_hook_args { _WORD event; _WORD *msg; MRETS *mrets; _WORD *key; _WORD *nclicks; };
struct cpx_close_args { short flag; };

typedef struct
{
	short _cdecl (*cpx_call)(GRECT *rect, DIALOG * /* COPS extension */);
	void  _cdecl (*cpx_draw)(GRECT *clip);
	void  _cdecl (*cpx_wmove)(GRECT *work);
	void  _cdecl (*cpx_timer)(short *quit);
	void  _cdecl (*cpx_key)(struct cpx_key_args);
	void  _cdecl (*cpx_button)(struct cpx_button_args);
	void  _cdecl (*cpx_m1)(MRETS *mrets, short *quit);
	void  _cdecl (*cpx_m2)(MRETS *mrets, short *quit);
	short _cdecl (*cpx_hook)(struct cpx_hook_args);
	void  _cdecl (*cpx_close)(struct cpx_close_args);
} CPXINFO;


/* Documented alert message numbers for XGen_Alert  */
#define XAL_SAVE_DEFAULTS	0
#define XAL_MEM_ERR		1
#define XAL_FILE_ERR		2
#define XAL_FILE_NOT_FOUND	3
/* Undocumented alert message numbers for XGen_Alert  */
#define XAL_NO_NODES_ERR	4
#define XAL_RELOAD_CPXS		5
#define XAL_UNLOAD_CPX		6
#define XAL_NO_RELOAD		7
#define XAL_SAVE_HEADER		8
#define XAL_FILE_NOT_CPX	9
#define XAL_NO_SOUND_DMA	10	/* used by sound.cpx/fsound.cpx */
#define XAL_SHUTDOWN		11

#define MFSAVE      1
#define MFRESTORE   0

#ifndef	CT_KEY
#define CT_KEY  53
#endif

#define	CPXD_INVALID	0x8000	/* signalisiert ungueltigen CPX_DESC-Eintrag */
#define	CPXD_AUTOSTART	0x0002
#define	CPXD_INACTIVE	0x0001

typedef	struct cpx_desc
{
	struct cpx_desc *next;	/* Zeiger auf die naechste CPX-Beschreibung oder 0L */

	void	*start_of_cpx;	/* Startadresse des CPX im Speicher */
	void	*end_of_cpx;	/* Endadresse des CPX im Speicher */

	jmp_buf	jb;		/* longjmp buffer for interrupted Xform_do */
	char	*stack;		/* temporary malloced() stack */

	CPXINFO	*info;		/* Zeiger auf CPXINFO-Struktur die bei cpx_call() zurueckgeliefert wird */

	DIALOG	*dialog;	/* Zeiger auf die Dialogbeschreibung oder 0L, wenn das CPX nicht offen ist */
	OBJECT	*tree;		/* ist bei Form-CPX 0L, bis cpx_form_do() aufgerufen wird */
	_WORD	whdl;		/* Handle des Dialogfensters */
	_WORD	window_x;	/* Position des Dialogfensters */
	_WORD	window_y;

	short	is_evnt_cpx;	/* 0: Form-CPX 1: Event-CPX */
	_WORD	button;		/* wird von handle_form_cpx() zurueckgeliefert */
	_WORD	*msg;		/* Zeiger auf den bei Xform_do() uebergebenen Messagebuffer */

	short	obfix_cnt;
	_WORD	box_width;
	_WORD	box_height;
	GRECT	size;		/* Dialogausmasse und -position */
	OBJECT	empty_tree[2];	/* IBOX fuer leeren Objektbaum am Anfang,
				   wegen WDIALOG-Fehler aus 2 Objekten bestehend */

	struct cl_segm segm;

	_WORD	icon_x;		/* Iconposition innerhalb des Hauptfensters */
	_WORD	icon_y;

	int	selected;
	short	flags;

	/* cpx read/write accessible data */

	GRECT	redraw_area;	/* Bereich den ein CPX nach WM_REDRAW neuzeichnen soll */
	GRECT	dirty_area;	/* Bereich eines neuzuzeichnenden Rechtecks */
	short	mask;		/* zusaetzliche Ereignismaske fuer evnt_multi() */
	MOBLK	m1;		/* Mausrechteck fuer evnt_multi() */
	MOBLK	m2;		/* Mausrechteck fuer evnt_multi() */
	long	time;		/* Timerintervall fuer evnt_multi() */

	struct cpxlist old;	/* Beschreibung fuer get_cpx_list */
	struct xcpb xctrl_pb;	/* Parameterblock fuer das CPX */

	/* cpx name */
	char	file_name[0];

} CPX_DESC;

#endif /* _cpx_h */
