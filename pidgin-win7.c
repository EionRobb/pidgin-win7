/*

*/
#include "pidgin-win7.h"


static void
win7_blist_set_visible(PurpleBuddyList *list, gboolean show)
{
	PidginBuddyList *gtkblist = pidgin_blist_get_default_gtk_blist();
	
	if (!(gtkblist && gtkblist->window))
		return;
	
	if (show) {
		pidgin_old_set_visible(list, show);
	} else {
		if (!GTK_WIDGET_VISIBLE(gtkblist->window))
			gtk_widget_show(gtkblist->window);
		gtk_window_iconify(GTK_WINDOW(gtkblist->window));
	}
}

static gboolean pixbuf_to_hbitmaps_normal(GdkPixbuf *pixbuf, HBITMAP *color, HBITMAP *mask);
static gboolean pixbuf_to_hbitmaps_alpha_winxp(GdkPixbuf *pixbuf, HBITMAP *color, HBITMAP *mask);
static HICON pixbuf_to_hicon (GdkPixbuf *pixbuf);

// Conversation -> HWND  map
static GHashTable *win7_conv_hwnd = NULL;
// HWND -> Conversation  map
static GHashTable *win7_hwnd_conv = NULL;

/* From pidginsnarl */
static void
showConversation(PurpleConversation *conv){
	if(conv){
		PidginConversation *gtkconv = PIDGIN_CONVERSATION(conv);
		pidgin_conv_switch_active_conversation(conv);
		pidgin_conv_present_conversation(conv);
		if(gtkconv != NULL && gtkconv->win != NULL){
			pidgin_conv_window_switch_gtkconv(gtkconv->win, gtkconv);
			gtk_window_present(GTK_WINDOW(gtkconv->win->window));
			pidgin_conv_window_raise(gtkconv->win);
		}
	}
}

LRESULT CALLBACK 
win7_conv_handler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PurpleConversation *conv;
	PidginConversation *pconv;
	HBITMAP hbitmap, mask;
	GdkPixbuf *pixbuf = NULL;
	GdkColormap *cmap = NULL;
	
	conv = g_hash_table_lookup(win7_hwnd_conv, hwnd);
	if (!conv)
		return DefWindowProc(hwnd, msg, wparam, lparam);
	pconv = PIDGIN_CONVERSATION(conv);
	
	switch(msg)
	{
		case WM_CREATE:
		{
			
			// Create taskbar buttons?
			//TODO We dont have thumbnail buttons yet
			purple_debug_info("win7", "wm_create\n");
		}	break;
		case WM_CLOSE:
		{
			// Handle the thumbnail close button
			purple_debug_info("win7", "wm_close\n");
			purple_conversation_destroy(conv);
		}	break;
		case WM_ACTIVATE:
		{
			// Handle the thumbnail being clicked on
			purple_debug_info("win7", "wm_activate\n");
			showConversation(conv);
			return TRUE;
		}	break;
		case WM_COMMAND:
		{
			// Handle one of the buttons on the thumbnail being pressed
			//TODO We dont have thumbnail buttons yet
			purple_debug_info("win7", "wm_command\n");
		}	break;
		case WM_DWMSENDICONICTHUMBNAIL:
		{
			// Thumbnail needs to be drawn
			purple_debug_info("win7", "wm_dwmsendiconicthumbnail\n");
			/*cmap = gtk_widget_get_colormap(pconv->win->window);
			pixbuf = gdk_pixbuf_get_from_drawable(NULL, pconv->win->window, cmap, 0, 0, 0, 0, 200, 200);
			
			pixbuf_to_hbitmaps_normal(pixbuf, &hbitmap, &mask);
			DeleteObject (mask);
			
			DwmSetIconicThumbnail(hwnd, hbitmap, 0);*/
			gconstpointer data = NULL;
			PurpleStoredImage *custom_img = NULL;
			size_t len;
			/*PurpleAccount *account = purple_conversation_get_account(conv);
			PurpleBuddy *buddy = purple_find_buddy(account, purple_conversation_get_name(conv));
			if (buddy)
			{
				PurpleContact *contact = purple_buddy_get_contact(buddy);
				if (contact)
				{
					custom_img = purple_buddy_icons_node_find_custom_icon((PurpleBlistNode*)contact);
					if (custom_img)
					{
						data = purple_imgstore_get_data(custom_img);
						len = purple_imgstore_get_size(custom_img);
					}
				}
			}*/
			if (data == NULL)
			{
				PurpleBuddyIcon *icon = purple_conv_im_get_icon(PURPLE_CONV_IM(conv));
				if (icon)
				{
					data = purple_buddy_icon_get_data(icon, &len);
				}
			}
			if (data == NULL)
			{
				pixbuf = pidgin_conv_get_tab_icon(conv, FALSE);
			}
			
			if (data && len && !pixbuf)
			{
				GdkPixbufLoader *loader = gdk_pixbuf_loader_new();
				gdk_pixbuf_loader_write(loader, data, len, NULL);
				gdk_pixbuf_loader_close(loader, NULL);
				
				purple_imgstore_unref(custom_img);
				pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
			}
			if (pixbuf)
			{
				gint original_width = gdk_pixbuf_get_width (pixbuf);
				gint original_height = gdk_pixbuf_get_height (pixbuf);
				gint maxwidth = HIWORD(lparam);
				gint maxheight = LOWORD(lparam);
				gint size = MIN( maxwidth, maxheight );
				purple_debug_info("win7", "max size %d x %d\n", maxwidth, maxheight);
				purple_debug_info("win7", "scale to %d x %d\n", size, size);
				pixbuf = gdk_pixbuf_scale_simple(pixbuf, size, size, GDK_INTERP_BILINEAR);
				purple_debug_info("win7", "scaled %d\n", pixbuf);
				
				pixbuf_to_hbitmaps_alpha_winxp(pixbuf, &hbitmap, &mask);
				
				purple_debug_info("win7", "seticonicthumbnail (%d, %d)\n", hwnd, hbitmap);
				DwmSetIconicThumbnail(hwnd, hbitmap, 1);
				DeleteObject(mask);
				DeleteObject(hbitmap);
			}
			
		}	break;
		case WM_DWMSENDICONICLIVEPREVIEWBITMAP:
		{
			// Preview needs to be drawn
			purple_debug_info("win7", "wm_dwmsendiconiclivepreviewbitmap\n");
			//DwmSetIconicLivePreviewBitmap();
		}	break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

static void
win7_update_icon(PurpleConversation *conv, PurpleConvUpdateType type, gpointer user_data)
{
	HWND hTab;
	GdkPixbuf *icon;
	
	hTab = g_hash_table_lookup(win7_conv_hwnd, conv);
	if (hTab)
	{
		// Set the icon for the 'window'
		icon = pidgin_conv_get_tab_icon(conv, FALSE);
		if (icon)
		{
			SendMessage(hTab, WM_SETICON, ICON_SMALL, pixbuf_to_hicon(icon));
			g_object_unref(icon);
		}
		
		// Set the title for the 'window'
		SendMessage(hTab, WM_SETTEXT, 0, purple_conversation_get_title(conv));
	}
}

/* Create hidden window to process convtab messages */
static HWND 
win7_create_hiddenwin(PurpleConversation *conv)
{
	HWND hTab;
	DWORD lasterror;
	GdkPixbuf *icon;
	BOOL fForceIconic = TRUE;
	BOOL fHasIconicBitmap = TRUE;
	LPCTSTR wname = TEXT("WinpidginConvThumbCls");
	
	purple_debug_info("win7", "Wname %s, DesktopWin %d, hInstance %d, modulehandle %d\n", wname, GetDesktopWindow(), winpidgin_exe_hinstance(), GetModuleHandle(NULL));
	
	/* Create the window */
	hTab = CreateWindow(wname, purple_conversation_get_title(conv), 0, 0, 0, 0, 0, GetDesktopWindow(), NULL, winpidgin_exe_hinstance(), 0);
	lasterror = GetLastError();
	if (lasterror)
		purple_debug_error("win7", "CreateWindow error %d\n", lasterror);
	else
		purple_debug_info("win7", "CreateWindow %d\n", hTab);

	win7_update_icon(conv, PURPLE_CONV_UPDATE_ICON, NULL);
	
	if (purple_prefs_get_bool(PREF_TAB_THUMBS))
	{
		DwmSetWindowAttribute(hTab, DWMWA_FORCE_ICONIC_REPRESENTATION, 
			&fForceIconic, sizeof(fForceIconic));
		
		DwmSetWindowAttribute(hTab, DWMWA_HAS_ICONIC_BITMAP, &fHasIconicBitmap,
			sizeof(fHasIconicBitmap));
	}
	
	g_hash_table_insert(win7_conv_hwnd, conv, hTab);
	g_hash_table_insert(win7_hwnd_conv, hTab, conv);
	
	return hTab;
}
static void
win7_destroy_hiddenwin(PurpleConversation *conv)
{
	HWND hTab;
	
	hTab = g_hash_table_lookup(win7_conv_hwnd, conv);
	
	if(hTab)
	{
		DestroyWindow(hTab);
		g_hash_table_remove(win7_hwnd_conv, hTab);
	}
	
	g_hash_table_remove(win7_conv_hwnd, conv);
}

static void
win7_init_conv_windows(ITaskbarList3 *itl)
{
	WNDCLASSEX wcex;
	GList *convs;
	PurpleConversation *conv;
	LPCTSTR wname = TEXT("WinpidginConvThumbCls");
	DWORD lasterror;
	ATOM thumbcls;

	if (win7_hwnd_conv == NULL)
		win7_hwnd_conv = g_hash_table_new(NULL, NULL);
	if (win7_conv_hwnd == NULL)
		win7_conv_hwnd = g_hash_table_new(NULL, NULL);
	
	wcex.cbSize 		= sizeof(wcex);
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC) win7_conv_handler;  //DefWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= winpidgin_exe_hinstance();
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL,
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= wname;
	wcex.hIconSm		= NULL;
	thumbcls = RegisterClassEx(&wcex);
	lasterror = GetLastError();
	if (lasterror)
		purple_debug_error("win7", "RegisterClassEx error %d\n", lasterror);
	else
		purple_debug_info("win7", "RegisterClassEx %d\n", thumbcls);
	
	//loop over all conversations
	convs = purple_get_conversations();
	for(; convs; convs = g_list_next(convs))
	{
		//apply the on_conv_create to them
		conv = (PurpleConversation *) convs->data;
		on_conv_create(conv, itl);
	}
}

static void
win7_destroy_conv_windows(ITaskbarList3 *itl)
{
	GList *convs;
	PurpleConversation *conv;
	LPCTSTR wname = TEXT("WinpidginConvThumbCls");
	
	//loop over all conversations
	convs = purple_get_conversations();
	for(; convs; convs = g_list_next(convs))
	{
		//apply the on_conv_delete to them
		conv = (PurpleConversation *) convs->data;
		on_conv_delete(conv, itl);
	}
	
	UnregisterClass(wname, NULL);
}

static void
on_conv_switch(PurpleConversation *conv, gpointer user_data)
{
	ITaskbarList3 *itl = (ITaskbarList3 *)user_data;
	PidginConversation *pconv = PIDGIN_CONVERSATION(conv);
	HWND hWin = GDK_WINDOW_HWND(gtk_widget_get_window(pconv->win->window));
	HWND hTab = g_hash_table_lookup(win7_conv_hwnd, conv);
	
	purple_debug_info("win7", "on_conv_switch (%i, %i)\n", hWin, hTab);

	if (!hTab)
		hTab = win7_create_hiddenwin(conv);
	
	itl->lpVtbl->SetTabActive(itl, hTab, hWin, 0);
}
static void
on_conv_delete(PurpleConversation *conv, gpointer user_data)
{
	ITaskbarList3 *itl = (ITaskbarList3 *)user_data;
	//PidginConversation *pconv = PIDGIN_CONVERSATION(conv);
	HWND hTab = g_hash_table_lookup(win7_conv_hwnd, conv);

	purple_debug_info("win7", "on_conv_delete (%i)\n", hTab);
	
	if (hTab)
		itl->lpVtbl->UnregisterTab(itl, hTab);
	
	win7_destroy_hiddenwin(conv);
}
static void
on_conv_create(PurpleConversation *conv, gpointer user_data)
{
	const gchar *label;
	wchar_t *wlabel;
	ITaskbarList3 *itl = (ITaskbarList3 *)user_data;
	PidginConversation *pconv = PIDGIN_CONVERSATION(conv);
	HWND hWin = GDK_WINDOW_HWND(gtk_widget_get_window(pconv->win->window));
	HWND hTab = g_hash_table_lookup(win7_conv_hwnd, conv);

	purple_debug_info("win7", "on_conv_create (%i, %i)\n", hWin, hTab);
	
	if (!hTab)
		hTab = win7_create_hiddenwin(conv);
	
	itl->lpVtbl->RegisterTab(itl, hTab, hWin);
	itl->lpVtbl->SetTabOrder(itl, hTab, NULL);
	
	label = gtk_label_get_text(GTK_LABEL(pconv->tab_label));
	wlabel = g_utf8_to_utf16(label, -1, NULL, NULL, NULL);
	itl->lpVtbl->SetThumbnailTooltip(itl, hTab, wlabel);
	g_free(wlabel);
}

static gboolean
blist_delete_event_cb(GtkWidget *w, GdkEvent *e, gpointer user_data)
{
	purple_debug_info("win7", "blist_delete_event_cb\n");
	// Prevent the window from being closed,
	// minimise instead
	gtk_window_iconify(GTK_WINDOW(w));
	
	// return true to prevent the event from propegating
	return TRUE;
}


/* This is all pinched from gtkdocklet-win32.c */
#define WIN32_GDI_FAILED(api) printf("GDI FAILED %s\n", api)
static HICON cached_icons[PURPLE_STATUS_NUM_PRIMITIVES + 2];
static GtkWidget *image = NULL;

static HBITMAP
create_alpha_bitmap (gint     size,
		     guchar **outdata)
{
  BITMAPV5HEADER bi;
  HDC hdc;
  HBITMAP hBitmap;

  ZeroMemory (&bi, sizeof (BITMAPV5HEADER));
  bi.bV5Size = sizeof (BITMAPV5HEADER);
  bi.bV5Height = bi.bV5Width = size;
  bi.bV5Planes = 1;
  bi.bV5BitCount = 32;
  bi.bV5Compression = BI_BITFIELDS;
  /* The following mask specification specifies a supported 32 BPP
   * alpha format for Windows XP (BGRA format).
   */
  bi.bV5RedMask   = 0x00FF0000;
  bi.bV5GreenMask = 0x0000FF00;
  bi.bV5BlueMask  = 0x000000FF;
  bi.bV5AlphaMask = 0xFF000000;

  /* Create the DIB section with an alpha channel. */
  hdc = GetDC (NULL);
  if (!hdc)
    {
      WIN32_GDI_FAILED ("GetDC");
      return NULL;
    }
  hBitmap = CreateDIBSection (hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS,
			      (PVOID *) outdata, NULL, (DWORD)0);
  if (hBitmap == NULL)
    WIN32_GDI_FAILED ("CreateDIBSection");
  ReleaseDC (NULL, hdc);

  return hBitmap;
}

static HBITMAP
create_color_bitmap (gint     size,
		     guchar **outdata,
		     gint     bits)
{
  struct {
    BITMAPV4HEADER bmiHeader;
    RGBQUAD bmiColors[2];
  } bmi;
  HDC hdc;
  HBITMAP hBitmap;

  ZeroMemory (&bmi, sizeof (bmi));
  bmi.bmiHeader.bV4Size = sizeof (BITMAPV4HEADER);
  bmi.bmiHeader.bV4Height = bmi.bmiHeader.bV4Width = size;
  bmi.bmiHeader.bV4Planes = 1;
  bmi.bmiHeader.bV4BitCount = bits;
  bmi.bmiHeader.bV4V4Compression = BI_RGB;

  /* when bits is 1, these will be used.
   * bmiColors[0] already zeroed from ZeroMemory()
   */
  bmi.bmiColors[1].rgbBlue = 0xFF;
  bmi.bmiColors[1].rgbGreen = 0xFF;
  bmi.bmiColors[1].rgbRed = 0xFF;

  hdc = GetDC (NULL);
  if (!hdc)
    {
      WIN32_GDI_FAILED ("GetDC");
      return NULL;
    }
  hBitmap = CreateDIBSection (hdc, (BITMAPINFO *)&bmi, DIB_RGB_COLORS,
			      (PVOID *) outdata, NULL, (DWORD)0);
  if (hBitmap == NULL)
    WIN32_GDI_FAILED ("CreateDIBSection");
  ReleaseDC (NULL, hdc);

  return hBitmap;
}

static gboolean
pixbuf_to_hbitmaps_alpha_winxp (GdkPixbuf *pixbuf,
				HBITMAP   *color,
				HBITMAP   *mask)
{
  /* Based on code from
   * http://www.dotnet247.com/247reference/msgs/13/66301.aspx
   */
  HBITMAP hColorBitmap, hMaskBitmap;
  guchar *indata, *inrow;
  guchar *colordata, *colorrow, *maskdata, *maskbyte;
  gint width, height, size, i, i_offset, j, j_offset, rowstride;
  guint maskstride, mask_bit;
  
  purple_debug_misc("win7", "pixbuf_to_hbitmaps_alpha_winxp\n");

  width = gdk_pixbuf_get_width (pixbuf); /* width of icon */
  height = gdk_pixbuf_get_height (pixbuf); /* height of icon */

  /* The bitmaps are created square */
  size = MAX (width, height);

  hColorBitmap = create_alpha_bitmap (size, &colordata);
  if (!hColorBitmap)
    return FALSE;
  hMaskBitmap = create_color_bitmap (size, &maskdata, 1);
  if (!hMaskBitmap)
    {
      DeleteObject (hColorBitmap);
      return FALSE;
    }

  /* MSDN says mask rows are aligned to "LONG" boundaries */
  maskstride = (((size + 31) & ~31) >> 3);

  indata = gdk_pixbuf_get_pixels (pixbuf);
  rowstride = gdk_pixbuf_get_rowstride (pixbuf);

  if (width > height)
    {
      i_offset = 0;
      j_offset = (width - height) / 2;
    }
  else
    {
      i_offset = (height - width) / 2;
      j_offset = 0;
    }

  for (j = 0; j < height; j++)
    {
      colorrow = colordata + 4*(j+j_offset)*size + 4*i_offset;
      maskbyte = maskdata + (j+j_offset)*maskstride + i_offset/8;
      mask_bit = (0x80 >> (i_offset % 8));
      inrow = indata + (height-j-1)*rowstride;
      for (i = 0; i < width; i++)
	{
	  colorrow[4*i+0] = inrow[4*i+2];
	  colorrow[4*i+1] = inrow[4*i+1];
	  colorrow[4*i+2] = inrow[4*i+0];
	  colorrow[4*i+3] = inrow[4*i+3];
	  if (inrow[4*i+3] == 0)
	    maskbyte[0] |= mask_bit;	/* turn ON bit */
	  else
	    maskbyte[0] &= ~mask_bit;	/* turn OFF bit */
	  mask_bit >>= 1;
	  if (mask_bit == 0)
	    {
	      mask_bit = 0x80;
	      maskbyte++;
	    }
	}
    }

  *color = hColorBitmap;
  *mask = hMaskBitmap;

  return TRUE;
}

static gboolean
pixbuf_to_hbitmaps_normal (GdkPixbuf *pixbuf,
			   HBITMAP   *color,
			   HBITMAP   *mask)
{
  /* Based on code from
   * http://www.dotnet247.com/247reference/msgs/13/66301.aspx
   */
  HBITMAP hColorBitmap, hMaskBitmap;
  guchar *indata, *inrow;
  guchar *colordata, *colorrow, *maskdata, *maskbyte;
  gint width, height, size, i, i_offset, j, j_offset, rowstride, nc, bmstride;
  gboolean has_alpha;
  guint maskstride, mask_bit;
  
  purple_debug_misc("win7", "pixbuf_to_hbitmaps_normal\n");

  width = gdk_pixbuf_get_width (pixbuf); /* width of icon */
  height = gdk_pixbuf_get_height (pixbuf); /* height of icon */

  /* The bitmaps are created square */
  size = MAX (width, height);

  hColorBitmap = create_color_bitmap (size, &colordata, 24);
  if (!hColorBitmap)
    return FALSE;
  hMaskBitmap = create_color_bitmap (size, &maskdata, 1);
  if (!hMaskBitmap)
    {
      DeleteObject (hColorBitmap);
      return FALSE;
    }

  /* rows are always aligned on 4-byte boundarys */
  bmstride = size * 3;
  if (bmstride % 4 != 0)
    bmstride += 4 - (bmstride % 4);

  /* MSDN says mask rows are aligned to "LONG" boundaries */
  maskstride = (((size + 31) & ~31) >> 3);

  indata = gdk_pixbuf_get_pixels (pixbuf);
  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  nc = gdk_pixbuf_get_n_channels (pixbuf);
  has_alpha = gdk_pixbuf_get_has_alpha (pixbuf);

  if (width > height)
    {
      i_offset = 0;
      j_offset = (width - height) / 2;
    }
  else
    {
      i_offset = (height - width) / 2;
      j_offset = 0;
    }

  for (j = 0; j < height; j++)
    {
      colorrow = colordata + (j+j_offset)*bmstride + 3*i_offset;
      maskbyte = maskdata + (j+j_offset)*maskstride + i_offset/8;
      mask_bit = (0x80 >> (i_offset % 8));
      inrow = indata + (height-j-1)*rowstride;
      for (i = 0; i < width; i++)
	{
	  if (has_alpha && inrow[nc*i+3] < 128)
	    {
	      colorrow[3*i+0] = colorrow[3*i+1] = colorrow[3*i+2] = 0;
	      maskbyte[0] |= mask_bit;	/* turn ON bit */
	    }
	  else
	    {
	      colorrow[3*i+0] = inrow[nc*i+2];
	      colorrow[3*i+1] = inrow[nc*i+1];
	      colorrow[3*i+2] = inrow[nc*i+0];
	      maskbyte[0] &= ~mask_bit;	/* turn OFF bit */
	    }
	  mask_bit >>= 1;
	  if (mask_bit == 0)
	    {
	      mask_bit = 0x80;
	      maskbyte++;
	    }
	}
    }

  *color = hColorBitmap;
  *mask = hMaskBitmap;

  return TRUE;
}

static HICON
pixbuf_to_hicon (GdkPixbuf *pixbuf)
{
  gint x = 0, y = 0;
  gboolean is_icon = TRUE;
  ICONINFO ii;
  HICON icon;
  gboolean success;

  if (pixbuf == NULL)
    return NULL;

  if (gdk_pixbuf_get_has_alpha (pixbuf))
    success = pixbuf_to_hbitmaps_alpha_winxp (pixbuf, &ii.hbmColor, &ii.hbmMask);
  else
    success = pixbuf_to_hbitmaps_normal (pixbuf, &ii.hbmColor, &ii.hbmMask);

  if (!success)
    return NULL;

  ii.fIcon = is_icon;
  ii.xHotspot = x;
  ii.yHotspot = y;
  icon = CreateIconIndirect (&ii);
  DeleteObject (ii.hbmColor);
  DeleteObject (ii.hbmMask);
  return icon;
}

static HICON load_hicon_from_stock(const char *stock) {
	if(image == NULL)
	{
		image = gtk_image_new();
#if GLIB_CHECK_VERSION(2,10,0)
		g_object_ref_sink(image);
#else
		g_object_ref(image);
		gtk_object_sink(GTK_OBJECT(image));
#endif
	}

	HICON hicon = NULL;
	GdkPixbuf *pixbuf = gtk_widget_render_icon(image, stock,
		gtk_icon_size_from_name(PIDGIN_ICON_SIZE_TANGO_EXTRA_SMALL), NULL);

	if (pixbuf) {
		hicon = pixbuf_to_hicon(pixbuf);
		g_object_unref(pixbuf);
	} else
		purple_debug_error("win7", "Unable to load pixbuf for %s.\n", stock);

	return hicon;
}

static void
pidgin_on_status_change(PurpleSavedStatus *new, PurpleSavedStatus *old)
{
	purple_debug_info("win7", "pidgin_on_status_change\n");
	
	if(!this_plugin || !this_plugin->extra)
		return;
	
	ITaskbarList3 *itl = ((PidginWin7Store *)this_plugin->extra)->itl;
	PidginBuddyList *blist = pidgin_blist_get_default_gtk_blist();
	HWND blist_window = GDK_WINDOW_HWND(gtk_widget_get_window(blist->window));
	
	if (!itl || !blist)
		return;
	
	if (new == NULL)
	{
		//Unset the tray icon
		itl->lpVtbl->SetOverlayIcon(itl, blist_window, NULL, NULL);
	}
	
	PurpleStatusPrimitive status = 	purple_savedstatus_get_type(new);
	const gchar *status_name = purple_savedstatus_get_title(new);
	int icon_index = status;
	wchar_t *wstatus_name;
	
	/* Look up and cache the HICON if we don't already have it */
	if (cached_icons[icon_index] == NULL) {
		const gchar *icon_name = NULL;
		switch (status) {
			case PURPLE_STATUS_OFFLINE:
				icon_name = PIDGIN_STOCK_STATUS_OFFLINE;
				break;
			case PURPLE_STATUS_AWAY:
				icon_name = PIDGIN_STOCK_STATUS_AWAY;
				break;
			case PURPLE_STATUS_UNAVAILABLE:
				icon_name = PIDGIN_STOCK_STATUS_BUSY;
				break;
			case PURPLE_STATUS_EXTENDED_AWAY:
				icon_name = PIDGIN_STOCK_STATUS_XA;
				break;
			case PURPLE_STATUS_INVISIBLE:
				icon_name = PIDGIN_STOCK_STATUS_INVISIBLE;
				break;
			default:
				icon_name = PIDGIN_STOCK_STATUS_AVAILABLE;
				break;
		}
	
		g_return_if_fail(icon_name != NULL);

		cached_icons[icon_index] = load_hicon_from_stock(icon_name);
	}
	
	wstatus_name = g_utf8_to_utf16(status_name, -1, NULL, NULL, NULL);
	
	itl->lpVtbl->SetOverlayIcon(itl, blist_window, cached_icons[icon_index], wstatus_name);
	
	g_free(wstatus_name);
}

struct _PidginXferDialog
{
      gboolean keep_open;
      gboolean auto_clear;

      gint num_transfers;

      PurpleXfer *selected_xfer;

      GtkWidget *window;
}; //Don't need all the struct, just enough to get the window

static HWND
pidgin_get_ft_hwnd()
{
	HWND ft_window = NULL;
	struct _PidginXferDialog *xfer_dialog;
	
	xfer_dialog = pidgin_get_xfer_dialog();
	
	ft_window = GDK_WINDOW_HWND(gtk_widget_get_window(xfer_dialog->window));
	
	return ft_window;
}

gboolean
ft_update_transferred(gpointer data)
{
	PurpleXfer *xfer = (PurpleXfer *)data;
	ITaskbarList3 *itl = ((PidginWin7Store *)this_plugin->extra)->itl;
	HWND ft_window;
	purple_debug_info("win7", "ft_update_transferred\n");
	
	ft_window = pidgin_get_ft_hwnd();
	if (!ft_window)
		return FALSE;
	
	itl->lpVtbl->SetProgressValue(itl, ft_window, purple_xfer_get_bytes_sent(xfer), purple_xfer_get_size(xfer));
	
	switch(xfer->status)
	{
		case PURPLE_XFER_STATUS_CANCEL_LOCAL:
		case PURPLE_XFER_STATUS_CANCEL_REMOTE:
		case PURPLE_XFER_STATUS_DONE:
			return FALSE;
		default:
			break;
	}
	return TRUE;
}

static void
ft_update(PurpleXfer *xfer, gpointer data)
{
	ITaskbarList3 *itl = (ITaskbarList3 *)data;
	HWND ft_window;
	purple_debug_info("win7", "ft_update\n");
	
	ft_window = pidgin_get_ft_hwnd();
	if (!ft_window)
		return;
	
	switch(xfer->status)
	{
		case PURPLE_XFER_STATUS_UNKNOWN:
		case PURPLE_XFER_STATUS_NOT_STARTED:
			itl->lpVtbl->SetProgressState(itl, ft_window, TBPF_INDETERMINATE);
			break;
		case PURPLE_XFER_STATUS_STARTED:
			purple_timeout_add_seconds(1, ft_update_transferred, xfer);
		case PURPLE_XFER_STATUS_ACCEPTED:
			itl->lpVtbl->SetProgressState(itl, ft_window, TBPF_NORMAL);
			break;
		case PURPLE_XFER_STATUS_DONE:
			itl->lpVtbl->SetProgressState(itl, ft_window, TBPF_NOPROGRESS);
			break;
		case PURPLE_XFER_STATUS_CANCEL_LOCAL:
		case PURPLE_XFER_STATUS_CANCEL_REMOTE:
			itl->lpVtbl->SetProgressState(itl, ft_window, TBPF_ERROR);
			break;
	}
}

void pidgin_win7_create_jumplist(ICustomDestinationList *pcdl)
{
	guint minSlots;
	IObjectArray *poa;
	IObjectCollection *shellLinks;
	
	purple_debug_info("win7", "create_jumplist\n");
	
	if (SUCCEEDED(pcdl->lpVtbl->BeginList(pcdl, &minSlots, &IID_IObjectArray, (void**)&poa)))
		purple_debug_info("win7", "Beginlist begun\n");
	HRESULT hr = CoCreateInstance(&CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC_SERVER, &IID_IObjectCollection, (void**)&shellLinks);
	
	if (SUCCEEDED(hr))
	{
		pidgin_win7_add_tasks(pcdl, shellLinks);
		pcdl->lpVtbl->CommitList(pcdl);
	}
	poa->lpVtbl->Release(poa);
}
void pidgin_win7_delete_jumplist(ICustomDestinationList *pcdl)
{
	purple_debug_info("win7", "delete_jumplist\n");
	pcdl->lpVtbl->DeleteList(pcdl, NULL);
	pcdl->lpVtbl->Release(pcdl);
}

IShellLink *pidgin_win7_create_shell_link(const char *title, const char *icon, 
	gint icon_index, const char *path, const char *args, const char *description)
{
	purple_debug_info("win7", "create_shell_link\n");
	IShellLink *psl;
	
	HRESULT hr = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (void**)&psl);
	if (SUCCEEDED(hr))
	{
		if (icon != NULL)
		{
			psl->lpVtbl->SetIconLocation(psl, g_strdup(icon), icon_index);
		}
		if (path != NULL)
		{
			psl->lpVtbl->SetPath(psl, path);
		}
		if (args != NULL)
		{
			psl->lpVtbl->SetArguments(psl, args);
		}
		if (description != NULL)
		{
			psl->lpVtbl->SetDescription(psl, description);
		}
//#ifdef __IPropertyStore_INTERFACE_DEFINED__
		//Add title and description
		IPropertyStore *pps;
		PROPVARIANT propvar;

		psl->lpVtbl->QueryInterface(psl, &IID_IPropertyStore, (void **)&pps);
		PropVariantInit(&propvar);
		propvar.vt = VT_LPSTR;
		propvar.pszVal = g_strdup(title);
		pps->lpVtbl->SetValue(pps, &PKEY_Title, &propvar);
		g_free(propvar.pszVal);
		pps->lpVtbl->Commit(pps);
		pps->lpVtbl->Release(pps);
//#endif
	}
	return psl;
}

static void
pidgin_win7_add_tasks(ICustomDestinationList *pcdl, IObjectCollection *shellLinks)
{
	//IObjectCollection *shellLinks;
	IShellLink *shellLink;
	
	purple_debug_info("win7", "add_tasks\n");
	HRESULT hr = CoCreateInstance(&CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC, &IID_IObjectCollection, (void**)&shellLinks);
	if (SUCCEEDED(hr))
	{
		gchar *pidgin_path = g_find_program_in_path("pidgin");
		purple_debug_info("win7", "pidgin_path %s\n", pidgin_path);
		const gchar *iconpath = this_plugin->path;
		
		//TODO add tasks for sending a new message
		
		shellLink = pidgin_win7_create_shell_link(_("Available"), iconpath, 0, pidgin_path, "--protocolhandler=win7:update?status=available", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		
		shellLink = pidgin_win7_create_shell_link(_("Away"), iconpath, 1, pidgin_path, "--protocolhandler=win7:update?status=away", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		
		shellLink = pidgin_win7_create_shell_link(_("Do not disturb"), iconpath, 2, pidgin_path, "--protocolhandler=win7:update?status=unavailable", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		
		shellLink = pidgin_win7_create_shell_link(_("Invisible"), iconpath, 3, pidgin_path, "--protocolhandler=win7:update?status=invisible", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		
		shellLink = pidgin_win7_create_shell_link(_("Offline"), iconpath, 4, pidgin_path, "--protocolhandler=win7:update?status=offline", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		
		//GList *list = purple_savedstatuses_get_popular(6);
		//title = purple_savedstatus_get_title
		
		//TODO Add in the quit task with a seperator

		IObjectArray *jumplist_items_array;
		hr = shellLinks->lpVtbl->QueryInterface(shellLinks, &IID_IObjectArray, (void **)&jumplist_items_array);
		if (SUCCEEDED(hr))
		{
			hr = pcdl->lpVtbl->AddUserTasks(pcdl, jumplist_items_array);
			pcdl->lpVtbl->CommitList(pcdl);
			jumplist_items_array->lpVtbl->Release(jumplist_items_array);
		}		
		shellLinks->lpVtbl->Release(shellLinks);
		
		g_free(pidgin_path);
	}
}