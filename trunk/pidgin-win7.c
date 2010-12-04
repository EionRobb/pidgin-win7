/*

*/
#include "pidgin-win7.h"

/*void
pidgin_on_status_change()
{
	HICON icon = LoadIcon(winpidgin_exe_hinstance(), image number);
	SetOverlayIcon(winpidgin_exe_hinstance(), icon, L"Online");
}

void
on_file_transfer_progress()
{
	pcdl->SetProgressState(winpidgin_exe_hinstance(), TBPF_NOPROGRESS);
	//TBPF_INDETERMINATE
	//TBPF_NORMAL
	//TBPF_PAUSED
	//TBPF_ERROR
	pcdl->SetProgressValue(winpidgin_exe_hinstance(), 0, 100);
}*/

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
	const char *path, const char *args, const char *description)
{
	purple_debug_info("win7", "create_shell_link\n");
	IShellLink *psl;
	
	HRESULT hr = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (void**)&psl);
	if (SUCCEEDED(hr))
	{
		if (icon != NULL)
		{
			psl->lpVtbl->SetIconLocation(psl, icon, 0);
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

void pidgin_win7_add_tasks(ICustomDestinationList *pcdl, IObjectCollection *shellLinks)
{
	//IObjectCollection *shellLinks;
	IShellLink *shellLink;
	
	purple_debug_info("win7", "add_tasks\n");
	HRESULT hr = CoCreateInstance(&CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC, &IID_IObjectCollection, (void**)&shellLinks);
	if (SUCCEEDED(hr))
	{
		gchar *pidgin_path = g_find_program_in_path("pidgin");
		purple_debug_info("win7", "pidgin_path %s\n", pidgin_path);
		gchar *pidgin_dir = g_path_get_dirname(pidgin_path);
		purple_debug_info("win7", "pidgin_dir %s\n", pidgin_dir);
		gchar *iconpath;
		
		iconpath = g_strconcat(pidgin_dir, "\\pixmaps\\status\\16\\available.png");
		shellLink = pidgin_win7_create_shell_link(_("Available"), iconpath, pidgin_path, "--protocolhandler=win7:update?status=available", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		g_free(iconpath);
		
		iconpath = g_strconcat(pidgin_dir, "\\pixmaps\\status\\16\\away.png");
		shellLink = pidgin_win7_create_shell_link(_("Away"), iconpath, pidgin_path, "--protocolhandler=win7:update?status=away", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		g_free(iconpath);
		
		iconpath = g_strconcat(pidgin_dir, "\\pixmaps\\status\\16\\busy.png");
		shellLink = pidgin_win7_create_shell_link(_("Do not disturb"), iconpath, pidgin_path, "--protocolhandler=win7:update?status=unavailable", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		g_free(iconpath);
		
		iconpath = g_strconcat(pidgin_dir, "\\pixmaps\\status\\16\\invisible.png");
		shellLink = pidgin_win7_create_shell_link(_("Invisible"), iconpath, pidgin_path, "--protocolhandler=win7:update?status=invisible", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		g_free(iconpath);
		
		iconpath = g_strconcat(pidgin_dir, "\\pixmaps\\status\\16\\offline.png");
		shellLink = pidgin_win7_create_shell_link(_("Offline"), iconpath, pidgin_path, "--protocolhandler=win7:update?status=offline", NULL);
		shellLinks->lpVtbl->AddObject(shellLinks, shellLink);
		shellLink->lpVtbl->Release(shellLink);
		g_free(iconpath);
		
		//GList *list = purple_savedstatuses_get_popular(6);
		//title = purple_savedstatus_get_title

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
		g_free(pidgin_dir);
	}
}
