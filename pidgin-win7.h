
#define INITGUID
#include <windows.h>
#include <wtypes.h>
#include <shobjidl.h>
#include <shlguid.h>
//#include <propkeydef.h>
//#include <propsys.h>
#include <initguid.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdkwin32.h>

#ifndef PURPLE_PLUGINS
#	define PURPLE_PLUGINS
#endif

// Libpurple includes
#include "core.h"
#include "savedstatuses.h"
#include "signals.h"
#include "status.h"
#include "plugin.h"
#include "debug.h"
#include "buddyicon.h"

// Pidgin includes
#include "gtkft.h"
#include "gtkblist.h"
#include "pidginstock.h"
#include "gtkconv.h"
#include "gtkconvwin.h"
#include "gtkdialogs.h"
#include "gtkplugin.h"
#include "gtkaccount.h"
#include "gtkprefs.h"

#ifndef _
#define _(a) (a)
#endif

#ifndef PIDGIN_PLUGIN_TYPE
#define PIDGIN_UI "gtk-gaim"
#define PIDGIN_PLUGIN_TYPE PIDGIN_UI
#endif

#ifndef __RPC__in
#define __RPC__in
#define __RPC__in_opt
#define __RPC__in_string
#define __RPC__in_opt_string
#endif
#ifndef __RPC__out
#define __RPC__out
#define __RPC__deref_out
#define __RPC__deref_out_opt
#endif

/* Forward Declarations */

#ifndef __IObjectArray_FWD_DEFINED__
#define __IObjectArray_FWD_DEFINED__
typedef interface IObjectArray IObjectArray;
#endif 	/* __IObjectArray_FWD_DEFINED__ */

#ifndef __IObjectCollection_FWD_DEFINED__
#define __IObjectCollection_FWD_DEFINED__
typedef interface IObjectCollection IObjectCollection;
#endif 	/* __IObjectCollection_FWD_DEFINED__ */

#ifndef __ICustomDestinationList_FWD_DEFINED__
#define __ICustomDestinationList_FWD_DEFINED__
typedef interface ICustomDestinationList ICustomDestinationList;
#endif 	/* __ICustomDestinationList_FWD_DEFINED__ */

#ifndef __IPropertyStore_FWD_DEFINED__
#define __IPropertyStore_FWD_DEFINED__
typedef interface IPropertyStore IPropertyStore;
#endif 	/* __IPropertyStore_FWD_DEFINED__ */

typedef interface ITaskbarList3 ITaskbarList3;

#ifndef __IObjectArray_INTERFACE_DEFINED__
#define __IObjectArray_INTERFACE_DEFINED__

typedef struct IObjectArrayVtbl
{
	BEGIN_INTERFACE
	
	HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
		__RPC__in IObjectArray * This,
		/* [in] */ __RPC__in REFIID riid,
		/* [annotation][iid_is][out] */ 
		__RPC__deref_out  void **ppvObject);
	
	ULONG ( STDMETHODCALLTYPE *AddRef )( 
		__RPC__in IObjectArray * This);
	
	ULONG ( STDMETHODCALLTYPE *Release )( 
		__RPC__in IObjectArray * This);
	
	HRESULT ( STDMETHODCALLTYPE *GetCount )( 
		__RPC__in IObjectArray * This,
		/* [out] */ __RPC__out UINT *pcObjects);
	
	HRESULT ( STDMETHODCALLTYPE *GetAt )( 
		__RPC__in IObjectArray * This,
		/* [in] */ UINT uiIndex,
		/* [in] */ __RPC__in REFIID riid,
		/* [iid_is][out] */ __RPC__deref_out_opt void **ppv);
	
	END_INTERFACE
} IObjectArrayVtbl;

interface IObjectArray
{
	CONST_VTBL struct IObjectArrayVtbl *lpVtbl;
};

EXTERN_C const IID IID_IObjectArray = {0x92CA9DCD,0x5622,0x4bba,{0xA8,0x05,0x5E,0x9F,0x54,0x1B,0xD8,0xC9}}; // 92CA9DCD-5622-4bba-A805-5E9F541BD8C9

#endif  // __IObjectArray_INTERFACE_DEFINED__

#ifndef __IObjectCollection_INTERFACE_DEFINED__
#define __IObjectCollection_INTERFACE_DEFINED__

typedef struct IObjectCollectionVtbl
{
	BEGIN_INTERFACE
	
	HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
		__RPC__in IObjectCollection * This,
		/* [in] */ __RPC__in REFIID riid,
		/* [annotation][iid_is][out] */ 
		__RPC__deref_out  void **ppvObject);
	
	ULONG ( STDMETHODCALLTYPE *AddRef )( 
		__RPC__in IObjectCollection * This);
	
	ULONG ( STDMETHODCALLTYPE *Release )( 
		__RPC__in IObjectCollection * This);
	
	HRESULT ( STDMETHODCALLTYPE *GetCount )( 
		__RPC__in IObjectCollection * This,
		/* [out] */ __RPC__out UINT *pcObjects);
	
	HRESULT ( STDMETHODCALLTYPE *GetAt )( 
		__RPC__in IObjectCollection * This,
		/* [in] */ UINT uiIndex,
		/* [in] */ __RPC__in REFIID riid,
		/* [iid_is][out] */ __RPC__deref_out_opt void **ppv);
	
	HRESULT ( STDMETHODCALLTYPE *AddObject )( 
		__RPC__in IObjectCollection * This,
		/* [in] */ __RPC__in_opt IUnknown *punk);
	
	HRESULT ( STDMETHODCALLTYPE *AddFromArray )( 
		__RPC__in IObjectCollection * This,
		/* [in] */ __RPC__in_opt IObjectArray *poaSource);
	
	HRESULT ( STDMETHODCALLTYPE *RemoveObjectAt )( 
		__RPC__in IObjectCollection * This,
		/* [in] */ UINT uiIndex);
	
	HRESULT ( STDMETHODCALLTYPE *Clear )( 
		__RPC__in IObjectCollection * This);
	
	END_INTERFACE
} IObjectCollectionVtbl;

interface IObjectCollection
{
	CONST_VTBL struct IObjectCollectionVtbl *lpVtbl;
};

EXTERN_C const IID IID_IObjectCollection = {0x5632b1a4,0xe38a,0x400a,{0x92,0x8a,0xd4,0xcd,0x63,0x23,0x02,0x95}}; // 5632b1a4-e38a-400a-928a-d4cd63230295

#endif  // __IObjectCollection_INTERFACE_DEFINED__



#ifndef __ICustomDestinationList_INTERFACE_DEFINED__
#define __ICustomDestinationList_INTERFACE_DEFINED__


typedef /* [v1_enum] */ enum tagKNOWNDESTCATEGORY {
  KDC_FREQUENT = 1,
  KDC_RECENT = (KDC_FREQUENT + 1)
} KNOWNDESTCATEGORY;

typedef struct ICustomDestinationListVtbl
{
	BEGIN_INTERFACE
	
	HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
		__RPC__in ICustomDestinationList * This,
		/* [in] */ __RPC__in REFIID riid,
		/* [annotation][iid_is][out] */ 
		__RPC__deref_out  void **ppvObject);
	
	ULONG ( STDMETHODCALLTYPE *AddRef )( 
		__RPC__in ICustomDestinationList * This);
	
	ULONG ( STDMETHODCALLTYPE *Release )( 
		__RPC__in ICustomDestinationList * This);
	
	HRESULT ( STDMETHODCALLTYPE *SetAppID )( 
		__RPC__in ICustomDestinationList * This,
		/* [string][in] */ __RPC__in_string LPCWSTR pszAppID);
	
	HRESULT ( STDMETHODCALLTYPE *BeginList )( 
		__RPC__in ICustomDestinationList * This,
		/* [out] */ __RPC__out UINT *pcMinSlots,
		/* [in] */ __RPC__in REFIID riid,
		/* [iid_is][out] */ __RPC__deref_out_opt void **ppv);
	
	HRESULT ( STDMETHODCALLTYPE *AppendCategory )( 
		__RPC__in ICustomDestinationList * This,
		/* [string][in] */ __RPC__in_string LPCWSTR pszCategory,
		/* [in] */ __RPC__in_opt IObjectArray *poa);
	
	HRESULT ( STDMETHODCALLTYPE *AppendKnownCategory )( 
		__RPC__in ICustomDestinationList * This,
		/* [in] */ KNOWNDESTCATEGORY category);
	
	HRESULT ( STDMETHODCALLTYPE *AddUserTasks )( 
		__RPC__in ICustomDestinationList * This,
		/* [in] */ __RPC__in_opt IObjectArray *poa);
	
	HRESULT ( STDMETHODCALLTYPE *CommitList )( 
		__RPC__in ICustomDestinationList * This);
	
	HRESULT ( STDMETHODCALLTYPE *GetRemovedDestinations )( 
		__RPC__in ICustomDestinationList * This,
		/* [in] */ __RPC__in REFIID riid,
		/* [iid_is][out] */ __RPC__deref_out_opt void **ppv);
	
	HRESULT ( STDMETHODCALLTYPE *DeleteList )( 
		__RPC__in ICustomDestinationList * This,
		/* [string][unique][in] */ __RPC__in_opt_string LPCWSTR pszAppID);
	
	HRESULT ( STDMETHODCALLTYPE *AbortList )( 
		__RPC__in ICustomDestinationList * This);
	
	END_INTERFACE
} ICustomDestinationListVtbl;

interface ICustomDestinationList
{
	CONST_VTBL struct ICustomDestinationListVtbl *lpVtbl;
};

EXTERN_C const IID IID_ICustomDestinationList = {0x6332debf,0x87b5,0x4670,{0x90,0xc0,0x5e,0x57,0xb4,0x08,0xa4,0x9e}}; // 6332debf-87b5-4670-90c0-5e57b408a49e;

#endif  // __ICustomDestinationList_INTERFACE_DEFINED__

const CLSID CLSID_DestinationList = {
  0x77f10cf0, 0x3db5, 0x4966, {0xb5, 0x20, 0xb7, 0xc5, 0x4f, 0xd3, 0x5e, 0xd6}
};

const CLSID CLSID_EnumerableObjectCollection = {
  0x2d3468c1, 0x36a7, 0x43b6, {0xac, 0x24, 0xd3, 0xf0, 0x2f, 0xd9, 0x60, 0x7a}
};

#ifndef DEFINE_PROPERTYKEY
typedef struct {
  GUID  fmtid;
  DWORD pid;
} PROPERTYKEY;
#define REFPROPERTYKEY const PROPERTYKEY * __MIDL_CONST
#define REFPROPVARIANT const PROPVARIANT * __MIDL_CONST
#ifdef INITGUID
#define DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) EXTERN_C const PROPERTYKEY /*DECLSPEC_SELECTANY*/ name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }
#else
#define DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) EXTERN_C const PROPERTYKEY name
#endif // INITGUID 
#endif // DEFINE_PROPERTYKEY


DEFINE_PROPERTYKEY(PKEY_Title, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9, 2);
DEFINE_PROPERTYKEY(PKEY_AppUserModel_IsDestListSeparator, 0x9F4C2855, 0x9F79, 0x4B39, 0xA8, 0xD0, 0xE1, 0xD4, 0x2D, 0xE1, 0xD5, 0xF3, 6);

#ifndef __IPropertyStore_INTERFACE_DEFINED__
#define __IPropertyStore_INTERFACE_DEFINED__

EXTERN_C const IID IID_IPropertyStore = { 0x886d8eeb, 0x8cf2, 0x4446, { 0x8d, 0x02, 0xcd, 0xba, 0x1d, 0xbd, 0xcf, 0x99 } };

typedef struct IPropertyStoreVtbl
{
	BEGIN_INTERFACE
	
	HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
		IPropertyStore * This,
		/* [in] */ __RPC__in REFIID riid,
		/* [iid_is][out] */ 
		__RPC__deref_out  void **ppvObject);
	
	ULONG ( STDMETHODCALLTYPE *AddRef )( 
		IPropertyStore * This);
	
	ULONG ( STDMETHODCALLTYPE *Release )( 
		IPropertyStore * This);
	
	HRESULT ( STDMETHODCALLTYPE *GetCount )( 
		IPropertyStore * This,
		/* [out] */ __RPC__out DWORD *cProps);
	
	HRESULT ( STDMETHODCALLTYPE *GetAt )( 
		IPropertyStore * This,
		/* [in] */ DWORD iProp,
		/* [out] */ __RPC__out PROPERTYKEY *pkey);
	
	HRESULT ( STDMETHODCALLTYPE *GetValue )( 
		IPropertyStore * This,
		/* [in] */ __RPC__in REFPROPERTYKEY key,
		/* [out] */ __RPC__out PROPVARIANT *pv);
	
	HRESULT ( STDMETHODCALLTYPE *SetValue )( 
		IPropertyStore * This,
		/* [in] */ __RPC__in REFPROPERTYKEY key,
		/* [in] */ __RPC__in REFPROPVARIANT propvar);
	
	HRESULT ( STDMETHODCALLTYPE *Commit )( 
		IPropertyStore * This);
	
	END_INTERFACE
} IPropertyStoreVtbl;

interface IPropertyStore
{
	CONST_VTBL struct IPropertyStoreVtbl *lpVtbl;
};

    
#endif /* __IPropertyStore_INTERFACE_DEFINED__ */

/*
extern "C" const CLSID CLSID_TaskbarList              = {0x56fdf344,0xfd6d,0x11d0,{0x95,0x8a,0x00,0x60,0x97,0xc9,0xa0,0x90}}; // 56fdf344-fd6d-11d0-958a-006097c9a090;
extern "C" const IID IID_ITaskbarList                 = {0x56FDF342,0xFD6D,0x11d0,{0x95,0x8A,0x00,0x60,0x97,0xC9,0xA0,0x90}}; // 56FDF342-FD6D-11d0-958A-006097C9A090;
extern "C" const IID IID_ITaskbarList2                = {0x602D4995,0xB13A,0x429b,{0xA6,0x6E,0x19,0x35,0xE4,0x4F,0x43,0x17}}; // 602D4995-B13A-429b-A66E-1935E44F4317;
extern "C" const IID IID_ICustomDestinationList         = {0x6332debf,0x87b5,0x4670,{0x90,0xc0,0x5e,0x57,0xb4,0x08,0xa4,0x9e}}; // 6332debf-87b5-4670-90c0-5e57b408a49e
extern "C" const IID IID_IObjectArray                   = {0x92CA9DCD,0x5622,0x4bba,{0xA8,0x05,0x5E,0x9F,0x54,0x1B,0xD8,0xC9}}; // 92CA9DCD-5622-4bba-A805-5E9F541BD8C9
extern "C" const IID IID_IObjectCollection              = {0x5632b1a4,0xe38a,0x400a,{0x92,0x8a,0xd4,0xcd,0x63,0x23,0x02,0x95}}; // 5632b1a4-e38a-400a-928a-d4cd63230295
extern "C" const IID IID_ITaskbarList3                  = {0xea1afb91,0x9e28,0x4b86,{0x90,0xe9,0x9e,0x9f,0x8a,0x5e,0xef,0xaf}}; // ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf
extern "C" const CLSID CLSID_CustomDestinationList      = {0x77f10cf0,0x3db5,0x4966,{0xb5,0x20,0xb7,0xc5,0x4f,0xd3,0x5e,0xd6}};
extern "C" const CLSID CLSID_EnumerableObjectCollection = {0x2d3468c1,0x36a7,0x43b6,{0xac,0x24,0xd3,0xf0,0x2f,0xd9,0x60,0x7a}};
*/

EXTERN_C const IID IID_ITaskbarList3 = {0xea1afb91,0x9e28,0x4b86,{0x90,0xe9,0x9e,0x9f,0x8a,0x5e,0xef,0xaf}}; // ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf
EXTERN_C const CLSID CLSID_TaskbarList = {0x56fdf344,0xfd6d,0x11d0,{0x95,0x8a,0x00,0x60,0x97,0xc9,0xa0,0x90}}; // 56fdf344-fd6d-11d0-958a-006097c9a090;

typedef enum TBPFLAG
{
    TBPF_NOPROGRESS = 0,
    TBPF_INDETERMINATE      = 0x1,
    TBPF_NORMAL     = 0x2,
    TBPF_ERROR      = 0x4,
    TBPF_PAUSED     = 0x8
} TBPFLAG;

typedef enum TBATFLAG
{
    TBATF_USEMDITHUMBNAIL   = 0x1,
    TBATF_USEMDILIVEPREVIEW = 0x2
} TBATFLAG;

typedef struct tagTHUMBBUTTON
{
    DWORD dwMask;
    UINT iId;
    UINT iBitmap;
    HICON hIcon;
    //    WCHAR pszTip[ 260 ];
    wchar_t pszTip[ 260 ];
    DWORD dwFlags;
} THUMBBUTTON;

typedef struct tagTHUMBBUTTON *LPTHUMBBUTTON;

typedef struct ITaskbarList3Vtbl
{

	BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		ITaskbarList3 * This, 
		REFIID riid, 
		void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )( 
		ITaskbarList3 * This);

    ULONG ( STDMETHODCALLTYPE *Release )( 
		ITaskbarList3 * This);

    HRESULT ( STDMETHODCALLTYPE *HrInit )( 
		ITaskbarList3 * This);

    HRESULT ( STDMETHODCALLTYPE *AddTab )( 
		ITaskbarList3 * This, 
		HWND hwnd);

    HRESULT ( STDMETHODCALLTYPE *DeleteTab )( 
		ITaskbarList3 * This, 
		HWND hwnd);

    HRESULT ( STDMETHODCALLTYPE *ActivateTab )( 
		ITaskbarList3 * This, 
		HWND hwnd);

    HRESULT ( STDMETHODCALLTYPE *SetActiveAlt )( 
		ITaskbarList3 * This, 
		HWND hwnd);

    HRESULT ( STDMETHODCALLTYPE *MarkFullscreenWindow )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        BOOL fFullscreen);

    HRESULT ( STDMETHODCALLTYPE *SetProgressValue )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        ULONGLONG ullCompleted, 
		ULONGLONG ullTotal);

    HRESULT ( STDMETHODCALLTYPE *SetProgressState )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        TBPFLAG tbpFlags);

    HRESULT ( STDMETHODCALLTYPE *RegisterTab )(  
		ITaskbarList3 * This, 
		HWND hwndTab, 
		HWND hwndMDI);

    HRESULT ( STDMETHODCALLTYPE *UnregisterTab )( 
		ITaskbarList3 * This, 
		HWND hwndTab);

    HRESULT ( STDMETHODCALLTYPE *SetTabOrder )( 
		ITaskbarList3 * This, 
		HWND hwndTab,
        HWND hwndInsertBefore);

    HRESULT ( STDMETHODCALLTYPE *SetTabActive )( 
		ITaskbarList3 * This, 
		HWND hwndTab,
        HWND hwndMDI, 
		TBATFLAG tbatFlags);

    HRESULT ( STDMETHODCALLTYPE *ThumbBarAddButtons )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        UINT cButtons, 
		LPTHUMBBUTTON pButton);

    HRESULT ( STDMETHODCALLTYPE *ThumbBarUpdateButtons )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        UINT cButtons, 
		LPTHUMBBUTTON pButton);

    HRESULT ( STDMETHODCALLTYPE *ThumbBarSetImageList )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        HIMAGELIST himl);

    HRESULT ( STDMETHODCALLTYPE *SetOverlayIcon )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        HICON hIcon, 
		LPCWSTR pszDescription);

    HRESULT ( STDMETHODCALLTYPE *SetThumbnailTooltip )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        LPCWSTR pszTip);

    HRESULT ( STDMETHODCALLTYPE *SetThumbnailClip )( 
		ITaskbarList3 * This, 
		HWND hwnd,
        RECT *prcClip);

	END_INTERFACE
} ITaskbarList3Vtbl;

interface ITaskbarList3
{
	CONST_VTBL struct ITaskbarList3Vtbl *lpVtbl;
};

#ifndef WM_DWMSENDICONICTHUMBNAIL
#	define WM_DWMSENDICONICTHUMBNAIL           0x0323
#endif
#ifndef WM_DWMSENDICONICLIVEPREVIEWBITMAP
#	define WM_DWMSENDICONICLIVEPREVIEWBITMAP   0x0326
#endif

enum {
	DWMWA_NCRENDERING_ENABLED = 1,
	DWMWA_NCRENDERING_POLICY,
	DWMWA_TRANSITIONS_FORCEDISABLED,
	DWMWA_ALLOW_NCPAINT,
	DWMWA_CAPTION_BUTTON_BOUNDS,
	DWMWA_NONCLIENT_RTL_LAYOUT,
	DWMWA_FORCE_ICONIC_REPRESENTATION,
	DWMWA_FLIP3D_POLICY,
	DWMWA_EXTENDED_FRAME_BOUNDS,
	DWMWA_HAS_ICONIC_BITMAP,
	DWMWA_DISALLOW_PEEK,
	DWMWA_EXCLUDED_FROM_PEEK,
	DWMWA_LAST
};

extern HRESULT DwmSetWindowAttribute(HWND hwnd, DWORD attr, LPCVOID attrValue, DWORD attrSize);
extern HRESULT DwmSetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags);

G_MODULE_EXPORT LRESULT CALLBACK win7_conv_handler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void pidgin_win7_create_jumplist(ICustomDestinationList *pcdl);
void pidgin_win7_delete_jumplist(ICustomDestinationList *pcdl);
IShellLink *pidgin_win7_create_shell_link(const char *title, const char *icon, 
	gint icon_num, const char *path, const char *args, const char *description);
static void pidgin_win7_add_tasks();

static void ft_update(PurpleXfer *xfer, gpointer data);
static gboolean uri_handler(const char *proto, const char *cmd, GHashTable *params);
static void win7_on_status_change(PurpleSavedStatus *new, PurpleSavedStatus *old, gpointer user_data);
static gboolean blist_delete_event_cb(GtkWidget *w, GdkEvent *e, gpointer user_data);
static void on_conv_switch(PurpleConversation *conv, gpointer user_data);
static void on_conv_delete(PurpleConversation *conv, gpointer user_data);
static void on_conv_create(PurpleConversation *conv, gpointer user_data);
static void win7_init_conv_windows(ITaskbarList3 *itl);
static void win7_destroy_conv_windows(ITaskbarList3 *itl);

static void (*pidgin_old_set_visible)(PurpleBuddyList *list, gboolean show);
static void win7_blist_set_visible(PurpleBuddyList *list, gboolean show);

static void win7_update_icon(PurpleConversation *conv, PurpleConvUpdateType type, gpointer user_data);

#define PREF_PREFIX 	"/plugins/gtk/eionrobb-pidgin-win7"
#define PREF_JUMPLIST		PREF_PREFIX "/jumplist-enable"
#define PREF_OVERLAY_ICON	PREF_PREFIX "/overlay-icon-enable"
#define PREF_CONV_WIN		PREF_PREFIX "/seperate-conv-windows"
#define	PREF_TAB_THUMBS		PREF_PREFIX "/show-tab-thumbnails"
#define PREF_FILE_PROGRESS	PREF_PREFIX "/show-transfer-progress"

typedef struct {
	ICustomDestinationList *pcdl;
	ITaskbarList3 *itl;
	gulong blist_destroy_handler_id;
	gulong pidgin_destroy_handler_id;
} PidginWin7Store;

void win7_jumplist_pref_cb(const gchar *name, PurplePrefType type, gconstpointer val, gpointer data);
void win7_overlay_pref_cb(const gchar *name, PurplePrefType type, gconstpointer val, gpointer data);
void win7_convwin_pref_cb(const gchar *name, PurplePrefType type, gconstpointer val, gpointer data);
void win7_tabthumb_pref_cb(const gchar *name, PurplePrefType type, gconstpointer val, gpointer data);
void win7_progress_pref_cb(const gchar *name, PurplePrefType type, gconstpointer val, gpointer data);

gboolean win7_enable_jumplist(PidginWin7Store *store);
void win7_disable_jumplist(PidginWin7Store *store);

gboolean win7_enable_progress(PidginWin7Store *store);
void win7_disable_progress(PidginWin7Store *store);

gboolean win7_enable_overlay(PidginWin7Store *store);
void win7_disable_overlay(PidginWin7Store *store);

gboolean win7_enable_convwin(PidginWin7Store *store);
void win7_disable_convwin(PidginWin7Store *store);

static gboolean
plugin_load(PurplePlugin *plugin)
{
	PidginWin7Store *store;
	purple_debug_info("win7", "plugin_load\n");
	
	store = g_new0(PidginWin7Store, 1);
	plugin->extra = store;
	
	purple_prefs_connect_callback(plugin, PREF_JUMPLIST, win7_jumplist_pref_cb, store);
	purple_prefs_connect_callback(plugin, PREF_OVERLAY_ICON, win7_overlay_pref_cb, store);
	purple_prefs_connect_callback(plugin, PREF_CONV_WIN, win7_convwin_pref_cb, store);
	purple_prefs_connect_callback(plugin, PREF_TAB_THUMBS, win7_tabthumb_pref_cb, store);
	purple_prefs_connect_callback(plugin, PREF_FILE_PROGRESS, win7_progress_pref_cb, store);
	
	if (purple_prefs_get_bool(PREF_JUMPLIST))
	{
		win7_enable_jumplist(store);
	}
	
	if (purple_prefs_get_bool(PREF_FILE_PROGRESS))
	{
		win7_enable_progress(store);
	}
	
	if (purple_prefs_get_bool(PREF_OVERLAY_ICON))
	{
		win7_enable_overlay(store);
	}
	
	if (purple_prefs_get_bool(PREF_CONV_WIN))
	{
		win7_enable_convwin(store);
	}

	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	PidginWin7Store *store = (PidginWin7Store *)plugin->extra;
	purple_debug_info("win7", "plugin_unload\n");
	
	purple_prefs_disconnect_by_handle(plugin);
	
	win7_disable_jumplist(store);
	win7_disable_progress(store);
	win7_disable_overlay(store);
	win7_disable_convwin(store);
	
	g_free(store);
	
	return TRUE;
}

static PurplePlugin *this_plugin = NULL;

static void
plugin_init(PurplePlugin *plugin)
{
	this_plugin = plugin;
	
	purple_prefs_add_none(PREF_PREFIX);
	purple_prefs_add_bool(PREF_JUMPLIST, TRUE);
	purple_prefs_add_bool(PREF_OVERLAY_ICON, TRUE);
	purple_prefs_add_bool(PREF_CONV_WIN, FALSE);
	purple_prefs_add_bool(PREF_TAB_THUMBS, FALSE);
	purple_prefs_add_bool(PREF_FILE_PROGRESS, TRUE);
}

static PurplePluginPrefFrame *
plugin_config_frame(PurplePlugin *plugin)
{
	PurplePluginPrefFrame *frame;
	PurplePluginPref *pref;
	
	frame = purple_plugin_pref_frame_new();
	
	pref = purple_plugin_pref_new_with_name_and_label(PREF_JUMPLIST, _("Show right-click jumplist menu"));
	purple_plugin_pref_frame_add(frame, pref);

	pref = purple_plugin_pref_new_with_name_and_label(PREF_OVERLAY_ICON, _("Show status overlay icon"));
	purple_plugin_pref_frame_add(frame, pref);

	pref = purple_plugin_pref_new_with_name_and_label(PREF_FILE_PROGRESS, _("Show file transfer progress"));
	purple_plugin_pref_frame_add(frame, pref);

	pref = purple_plugin_pref_new_with_name_and_label(PREF_CONV_WIN, _("Seperate conversation tabs into seperate windows (Buggy)"));
	purple_plugin_pref_frame_add(frame, pref);
	
	pref = purple_plugin_pref_new_with_name_and_label(PREF_TAB_THUMBS, _("Show tab thumbnail previews/aero peek (Buggy)"));
	purple_plugin_pref_frame_add(frame, pref);
	
	return frame;
}

static PurplePluginUiInfo prefs_info = {
	plugin_config_frame,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static PurplePluginInfo info = 
{
	PURPLE_PLUGIN_MAGIC,
	2,
	5,
	PURPLE_PLUGIN_STANDARD,
	PIDGIN_PLUGIN_TYPE,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,

	"eionrobb-pidgin-win7",
	"Windows 7",
	"0.4",
	"Win7 Extensions for Pidgin",
	"Windows 7 Extensions for Pidgin.  Lets you use new eye-candy such as jumplists, thumbnail previews and file transfer progress indicators.",
	"Eion Robb <eionrobb@gmail.com>",
	"http://pidgin-win7.googlecode.com/", //URL
	
	plugin_load,
	plugin_unload,
	NULL,
	
	NULL,
	NULL,
	&prefs_info,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(pidginwin7, plugin_init, info);

static void
dump_hash_table(gchar *key, gchar *value, gpointer data)
{
	printf("'%s' = '%s'\n", key, value);
}

static PurpleSavedStatus *
create_transient_status(PurpleStatusPrimitive primitive, PurpleStatusType *status_type)
{
	PurpleSavedStatus *saved_status = purple_savedstatus_new(NULL, primitive);

	if(status_type != NULL) {
		GList *tmp, *active_accts = purple_accounts_get_all_active();
		for (tmp = active_accts; tmp != NULL; tmp = tmp->next) {
			purple_savedstatus_set_substatus(saved_status,
				(PurpleAccount*) tmp->data, status_type, NULL);
		}
		g_list_free(active_accts);
	}

	return saved_status;
}

static gboolean
uri_handler(const char *proto, const char *cmd, GHashTable *params)
{
	gchar *temp;
	PurpleSavedStatus *saved_status;
	PurpleStatusPrimitive primitive = PURPLE_STATUS_UNSET;

	//only deal with win7: uri's
	if (!g_str_equal(proto, "win7"))
		return FALSE;

	purple_debug_info("win7", "uri_handler cmd: %s\n", cmd);
	//lets have a look at the hash table!
	purple_debug_info("win7", "dumping uri handler hashtable\n");
	g_hash_table_foreach(params, (GHFunc)dump_hash_table, NULL);
	
	if ((temp = (gchar *)g_hash_table_lookup(params, "status")))
	{
		purple_debug_info("win7", "set status to %s\n", temp);
		if (g_str_equal(temp, "available"))
		{
			primitive = PURPLE_STATUS_AVAILABLE;
		} else if (g_str_equal(temp, "away"))
		{
			primitive = PURPLE_STATUS_AWAY;
		} else if (g_str_equal(temp, "unavailable"))
		{
			primitive = PURPLE_STATUS_UNAVAILABLE;
		} else if (g_str_equal(temp, "invisible"))
		{
			primitive = PURPLE_STATUS_INVISIBLE;
		} else if (g_str_equal(temp, "offline"))
		{
			primitive = PURPLE_STATUS_OFFLINE;
		}
		saved_status = purple_savedstatus_find_transient_by_type_and_message(primitive, NULL);
		if (saved_status == NULL)
			saved_status = create_transient_status(primitive, NULL);
		purple_savedstatus_activate(saved_status);
		
		return TRUE;
	} else if ((temp = (gchar *)g_hash_table_lookup(params, "window")))
	{
		purple_debug_info("win7", "open window %s\n", temp);
		if (g_str_equal(temp, "im"))
		{
			pidgin_dialogs_im();
		} else if (g_str_equal(temp, "chat"))
		{
			pidgin_blist_joinchat_show();
		} else if (g_str_equal(temp, "plugins"))
		{
			pidgin_plugin_dialog_show();
		} else if (g_str_equal(temp, "accounts"))
		{
			pidgin_accounts_window_show();
		} else if (g_str_equal(temp, "prefs"))
		{
			pidgin_prefs_show();
		}
	} else if (g_str_equal(cmd, "quit"))
	{
		purple_core_quit();
	}
	
	//we don't know how to handle this
	return FALSE;
}
