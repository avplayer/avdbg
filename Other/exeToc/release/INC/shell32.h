int __stdcall AssocCreateForClasses(const ASSOCIATIONELEMENT* rgClasses,ULONG cClasses,REFIID riid,void** ppv);
int __stdcall AssocGetDetailsOfPropKey(IShellFolder* psf,PCUITEMID_CHILD pidl,PROPERTYKEY* pkey,VARIANT* pv,BOOL* pfFoundPropKey);
int __stdcall CDefFolderMenu_Create2(PCIDLIST_ABSOLUTE pidlFolder,HWND hwnd,UINT cidl,PCUITEMID_CHILD_ARRAY* apidl,IShellFolder* psf,LPFNDFMCALLBACK lpfn,UINT nKeys,const HKEY* ahkeys,IContextMenu** ppcm);
int __stdcall CIDLData_CreateFromIDArray(PCIDLIST_ABSOLUTE pidlFolder,UINT cidl,PCUIDLIST_RELATIVE_ARRAY apidl,IDataObject** ppdtobj);
int __stdcall CallCPLEntry16(HINSTANCE hInst,FARPROC16 lpfnEntry,HWND hwndCPL,UINT msg,LPARAM lParam1,LPARAM lParam2);
int __stdcall CommandLineToArgvW(LPCWSTR lpCmdLine,int* pNumArgs);
int __stdcall DAD_AutoScroll(HWND hwnd,AUTO_SCROLL_DATA* pad,const POINT* pptNow);
int __stdcall DAD_DragEnterEx(HWND hwndTarget,const POINT ptStart);
int __stdcall DAD_DragLeave();
int __stdcall DAD_DragMove(POINT pt);
int __stdcall DAD_SetDragImage(HIMAGELIST him,POINT* pptOffset);
int __stdcall DAD_ShowDragImage(BOOL fShow);
int __stdcall DllGetVersion(DLLVERSIONINFO* pDllVersionInfo);
int __stdcall DoEnvironmentSubst(LPTSTR pszString,UINT cchString);
int __stdcall DragAcceptFiles(HWND hWnd,BOOL fAccept);
int __stdcall DragFinish(HDROP hDrop);
int __stdcall DragQueryFile(HDROP hDrop,UINT iFile,LPTSTR lpBuffer,UINT nBuffer);
int __stdcall DragQueryFileA(HDROP hDrop,UINT iFile,LPTSTR lpBuffer,UINT nBuffer);
int __stdcall DragQueryFileW(HDROP hDrop,UINT iFile,LPTSTR lpBuffer,UINT nBuffer);
int __stdcall DragQueryPoint(HDROP hDrop,LPPOINT lppt);
int __stdcall DriveType(int iDrive);
int __stdcall DuplicateIcon(HINSTANCE hInst,HICON hIcon);
int __stdcall ExtractAssociatedIcon(HINSTANCE hInst,LPTSTR lpIconPath,LPWORD lpiIcon);
int __stdcall ExtractAssociatedIconA(HINSTANCE hInst,LPTSTR szIconPath,LPWORD lpiIcon);
int __stdcall ExtractAssociatedIconEx(HINSTANCE hInst,LPTSTR lpIconPath,LPWORD lpiIconIndex,LPWORD lpiIconId);
int __stdcall ExtractAssociatedIconW(HINSTANCE hInst,LPTSTR szIconPath,LPWORD lpiIcon);
int __stdcall ExtractIcon(DWORD hInst,DWORD lpszExeFileName,DWORD nIconIndex);
int __stdcall ExtractIcon(HINSTANCE hInst,LPCTSTR lpszExeFileName,UINT nIconIndex);
int __stdcall ExtractIconA(HINSTANCE hInst,LPCTSTR lpszExeFileName,UINT nIconIndex);
int __stdcall ExtractIconEx(LPCTSTR lpszFile,int nIconIndex,HICON* phiconLarge,HICON* phiconSmall,UINT nIcons);
int __stdcall ExtractIconExW(LPCTSTR lpszFile,int nIconIndex,HICON* phiconLarge,HICON* phiconSmall,UINT nIcons);
int __stdcall ExtractIconW(HINSTANCE hInst,LPCTSTR lpszExeFileName,UINT nIconIndex);
int __stdcall FileIconInit(BOOL fRestoreCache);
int __stdcall FindExecutable(LPCTSTR lpFile,LPCTSTR lpDirectory,LPTSTR lpResult);
int __stdcall GUIDFromString(LPCTSTR psz,LPGUID pguid);
int __stdcall GetCurrentProcessExplicitAppUserModelID(PWSTR* AppID);
int __stdcall GetFileNameFromBrowse(HWND hwnd,LPWSTR pszFilePath,UINT cchFilePath,LPCWSTR pszWorkingDir,LPCWSTR pszDefExt,LPCWSTR pszFilters,LPCWSTR szTitle);
int __stdcall ILAppendID(PIDLIST_RELATIVE pidl,LPSHITEMID pmkid,BOOL fAppend);
int __stdcall ILClone(PCUIDLIST_RELATIVE pidl);
int __stdcall ILCloneFirst(PCUIDLIST_RELATIVE pidl);
int __stdcall ILCombine(PCIDLIST_ABSOLUTE pidl1,PCUIDLIST_RELATIVE pidl2);
int __stdcall ILCreateFromPath(LPCSTR pszPath);
int __stdcall ILFindChild(PCIDLIST_ABSOLUTE pidlParent,PCIDLIST_ABSOLUTE pidlChild);
int __stdcall ILFindLastID(PCUIDLIST_RELATIVE pidl);
int __stdcall ILFree(PIDLIST_RELATIVE pidl);
int __stdcall ILGetNext(PCUIDLIST_RELATIVE pidl);
int __stdcall ILGetSize(PCUIDLIST_RELATIVE pidl);
int __stdcall ILIsEqual(PCIDLIST_ABSOLUTE pidl1,PCIDLIST_ABSOLUTE pidl2);
int __stdcall ILIsParent(PCIDLIST_ABSOLUTE pidl1,PCIDLIST_ABSOLUTE pidl2,BOOL fImmediate);
int __stdcall ILLoadFromStream(IStream* pstm,PIDLIST_RELATIVE* pidl);
int __stdcall ILRemoveLastID(PUIDLIST_RELATIVE pidl);
int __stdcall ILSaveToStream(IStream* pstm,PCUIDLIST_RELATIVE pidl);
int __stdcall InitNetworkAddressControl();
int __stdcall IsNetDrive(int iDrive);
int __stdcall IsUserAnAdmin();
int __stdcall LinkWindow_RegisterClass();
int __stdcall LinkWindow_UnregisterClass();
int __stdcall NTSHChangeNotifyDeregister(ULONG ulID);
int __stdcall NTSHChangeNotifyRegister(HWND hwnd,int fSources,LONG fEvents,UINT wMsg,int cEntries,SHChangeNotifyEntry* pfsne);
int __stdcall OpenRegStream(HKEY hkey,LPCWSTR pszSubkey,LPCWSTR pszValue,DWORD grfMode);
int __stdcall ParseField(LPCTSTR* szData,int n,LPTSTR* szBuf,int iBufLen);
int __stdcall PathCleanupSpec(LPCWSTR pszDir,LPWSTR pszSpec);
int __stdcall PathGetShortPath(LPWSTR pszLongPath);
int __stdcall PathIsExe(LPCWSTR szfile);
int __stdcall PathIsSlow(LPCTSTR pszFile,DWORD dwFileAttr);
int __stdcall PathMakeUniqueName(LPWSTR pszUniqueName,UINT cchMax,LPCWSTR pszTemplate,LPCWSTR pszLongPlate,LPCWSTR pszDir);
int __stdcall PathProcessCommand(LPCWSTR lpSrc,LPWSTR lpDest,int iDestMax,DWORD dwFlags);
int __stdcall PathResolve(LPWSTR pszPath,LPCWSTR* dirs,UINT fFlags);
int __stdcall PathYetAnotherMakeUniqueName(LPWSTR pszUniqueName,LPCWSTR pszPath,LPCWSTR pszShort,LPCWSTR pszFileSpec);
int __stdcall PickIconDlg(HWND hwnd,LPWSTR pszIconPath,UINT cchIconPath,int* piIconIndex);
int __stdcall PifMgr_CloseProperties(HANDLE hProps,UINT flOpt);
int __stdcall PifMgr_GetProperties(HANDLE hProps,LPCSTR pszGroup,VOID* lpProps,int cbProps,UINT flOpt);
int __stdcall PifMgr_OpenProperties(LPCWSTR pszApp,LPCWSTR lpszPIF,UINT hInf,UINT flOpt);
int __stdcall PifMgr_SetProperties(HANDLE hProps,LPCSTR pszGroup,VOID* lpProps,int cbProps,UINT flOpt);
int __stdcall ReadCabinetState(CABINETSTATE* pcs,int cLength);
int __stdcall RealDriveType(int iDrive,BOOL fOKToHitNet);
int __stdcall RestartDialog(HWND hParent,LPCWSTR pszPrompt,DWORD dwFlags);
int __stdcall RestartDialogEx(HWND hParent,LPCWSTR pszPrompt,DWORD dwFlags,HWND dwReasonCode);
int __stdcall SHAddDefaultPropertiesByExt(PCWSTR pszExt,IPropertyStore* pPropStore);
int __stdcall SHAddFromPropSheetExtArray(HPSXA hpsxa,LPFNADDPROPSHEETPAGE lpfnAddPage,LPARAM lParam);
int __stdcall SHAddToRecentDocs(UINT uFlags,LPCVOID pv);
int __stdcall SHAlloc(SIZE_T cb);
int __stdcall SHAppBarMessage(DWORD dwMessage,PAPPBARDATA pData);
int __stdcall SHAssocEnumHandlers(LPCWSTR pszExtra,ASSOC_FILTER afFilter,IEnumAssocHandlers** ppEnumHandler);
int __stdcall SHAssocEnumHandlersForProtocolByApplication(PCWSTR protocol,REFIID riid,void** enumHandlers);
int __stdcall SHBindToFolderIDListParent(IShellFolder* psfRoot,PCUIDLIST_RELATIVE pidl,REFIID riid,void** ppv,PCUITEMID_CHILD* ppidlLast);
int __stdcall SHBindToObject(IShellFolder* psf,PCUIDLIST_RELATIVE pidl,IBindCtx* pbc,REFIID riid,void** ppv);
int __stdcall SHBindToParent(PCIDLIST_ABSOLUTE pidl,REFIID riid,VOID** ppv,PCUITEMID_CHILD* ppidlLast);
int __stdcall SHBrowseForFolder(LPBROWSEINFO lpbi);
int __stdcall SHCLSIDFromString(LPCWSTR psz,CLSID* pclsid);
int __stdcall SHChangeNotification_Lock(HANDLE hChange,DWORD dwProcId,PIDLIST_ABSOLUTE** pppidl,LONG* plEvent);
int __stdcall SHChangeNotification_Unlock(HANDLE hLock);
int __stdcall SHChangeNotify(DWORD wEventId,DWORD uFlags,LPCVOID dwItem1,LPCVOID dwItem2);
int __stdcall SHChangeNotifyDeregister(ULONG ulID);
int __stdcall SHChangeNotifyRegister(HWND hwnd,int fSources,LONG fEvents,UINT wMsg,int cEntries,const SHChangeNotifyEntry* pshcne);
int __stdcall SHChangeNotifyRegisterThread(SCNRT_STATUS status);
int __stdcall SHCloneSpecialIDList(HWND hwndOwner,DWORD csidl,BOOL fCreate);
int __stdcall SHCoCreateInstance(LPCWSTR pszCLSID,const CLSID* pclsid,IUnknown* pUnkOuter,REFIID riid,void** ppv);
int __stdcall SHCreateAssociationRegistration(REFIID riid,void** ppv);
int __stdcall SHCreateDataObject(PCIDLIST_ABSOLUTE pidlFolder,UINT cidl,PCUITEMID_CHILD_ARRAY apidl,IDataObject* pdtInner,REFIID riid,void** ppv);
int __stdcall SHCreateDefaultContextMenu(const DEFCONTEXTMENU* pdcm,REFIID riid,void** ppv);
int __stdcall SHCreateDefaultExtractIcon(REFIID riid,void** ppv);
int __stdcall SHCreateDefaultPropertiesOp(IShellItem* psi,IFileOperation** ppFileOp);
int __stdcall SHCreateDirectory(HWND hwnd,LPCWSTR pszPath);
int __stdcall SHCreateDirectoryEx(HWND hwnd,LPCTSTR pszPath,const SECURITY_ATTRIBUTES* psa);
int __stdcall SHCreateFileExtractIconW(LPCWSTR pszFile,DWORD dwFileAttributes,REFIID riid,void** ppv);
int __stdcall SHCreateItemFromIDList(PCIDLIST_ABSOLUTE pidl,REFIID riid,void** ppv);
int __stdcall SHCreateItemFromParsingName(PCWSTR pszPath,IBindCtx* pbc,REFIID riid,void** ppv);
int __stdcall SHCreateItemFromRelativeName(IShellItem* psiParent,PCWSTR pszName,IBindCtx* pbc,REFIID riid,void** ppv);
int __stdcall SHCreateItemInKnownFolder(REFKNOWNFOLDERID kfid,KNOWN_FOLDER_FLAG dwKFFlags,PCWSTR pszItem,REFIID riid,void** ppv);
int __stdcall SHCreateItemWithParent(PCIDLIST_ABSOLUTE pidlParent,IShellFolder* psfParent,PCUITEMID_CHILD pidl,REFIID riid,void** ppvItem);
int __stdcall SHCreateProcessAsUserW(PSHCREATEPROCESSINFOW pscpi);
int __stdcall SHCreatePropSheetExtArray(HKEY hkey,LPCWSTR pszSubkey,UINT max_iface);
int __stdcall SHCreateQueryCancelAutoPlayMoniker(IMoniker** ppmoniker);
int __stdcall SHCreateShellFolderView(const SFV_CREATE* pcsfv,IShellView** ppsv);
int __stdcall SHCreateShellFolderViewEx(LPCSFV pcsfv,IShellView** ppsv);
int __stdcall SHCreateShellItem(PCIDLIST_ABSOLUTE pidlParent,IShellFolder* psfParent,PCUITEMID_CHILD pidl,IShellItem** ppsi);
int __stdcall SHCreateShellItemArray(PCIDLIST_ABSOLUTE pidlParent,IShellFolder* psf,UINT cidl,PCUITEMID_CHILD_ARRAY ppidl,IShellItemArray** ppsiItemArray);
int __stdcall SHCreateShellItemArrayFromDataObject(IDataObject* pdo,REFIID riid,void** ppv);
int __stdcall SHCreateShellItemArrayFromIDLists(UINT cidl,PCIDLIST_ABSOLUTE_ARRAY rgpidl,IShellItemArray** ppsiItemArray);
int __stdcall SHCreateShellItemArrayFromShellItem(IShellItem* psi,REFIID riid,void** ppv);
int __stdcall SHCreateStdEnumFmtEtc(UINT cfmt,IEnumFORMATETC** ppenumFormatEtc);
int __stdcall SHDefExtractIcon(LPCTSTR pszIconFile,int iIndex,UINT uFlags,HICON* phiconLarge,HICON* phiconSmall,UINT nIconSize);
int __stdcall SHDestroyPropSheetExtArray(HPSXA hpsxa);
int __stdcall SHDoDragDrop(HWND hwnd,IDataObject* pdtobj,IDropSource* pdsrc,DWORD dwEffect,DWORD* pdwEffect);
int __stdcall SHEmptyRecycleBin(HWND hwnd,LPCTSTR pszRootPath,DWORD dwFlags);
int __stdcall SHEnumerateUnreadMailAccountsW(HKEY hKeyUser,DWORD dwIndex,LPWSTR pszMailAddress,int cchMailAddress);
int __stdcall SHEvaluateSystemCommandTemplate(PCWSTR pszCmdTemplate,PWSTR* ppszApplication,PWSTR* ppszCommandLine,PWSTR* ppszParameters);
int __stdcall SHExtractIconsW(LPCWSTR pszFileName,int nIconIndex,int cxIcon,int cyIcon,HICON* phIcon,UINT* pIconId,UINT nIcons,UINT flags);
int __stdcall SHFileOperation(LPSHFILEOPSTRUCT lpFileOp);
int __stdcall SHFindFiles(PCIDLIST_ABSOLUTE pidlFolder,PCIDLIST_ABSOLUTE pidlSaveFile);
int __stdcall SHFind_InitMenuPopup(HMENU hmenu,HWND hwnd,UINT idCmdFirst,UINT idCmdLast);
int __stdcall SHFlushClipboard();
int __stdcall SHFlushSFCache();
int __stdcall SHFormatDrive(HWND hwnd,UINT drive,UINT fmtID,UINT options);
int __stdcall SHFree(void* pv);
int __stdcall SHFreeNameMappings(HANDLE hNameMappings);
int __stdcall SHGetAttributesFromDataObject(IDataObject* pdo,DWORD dwAttributeMask,DWORD* pdwAttributes,UINT* pcItems);
int __stdcall SHGetDataFromIDList(IShellFolder* psf,PCUITEMID_CHILD pidl,DWORD nFormat,PVOID pv,int cb);
int __stdcall SHGetDesktopFolder(IShellFolder** ppshf);
int __stdcall SHGetDiskFreeSpace(LPCTSTR pszVolume,ULARGE_INTEGER* pqwFreeCaller,ULARGE_INTEGER* pqwTot,ULARGE_INTEGER* pqwFree);
int __stdcall SHGetDiskFreeSpaceEx(LPCTSTR pszVolume,ULARGE_INTEGER* pqwFreeCaller,ULARGE_INTEGER* pqwTot,ULARGE_INTEGER* pqwFree);
int __stdcall SHGetDiskFreeSpaceExW(LPCTSTR pszVolume,ULARGE_INTEGER* pqwFreeCaller,ULARGE_INTEGER* pqwTot,ULARGE_INTEGER* pqwFree);
int __stdcall SHGetDriveMedia(LPCWSTR pszDrive,DWORD* pdwMediaContent);
int __stdcall SHGetFileInfo(LPCTSTR szPath,DWORD dwFileAttributes,SHFILEINFO* pSFileInfo,UINT cbFileInfo,UINT uFlags);
int __stdcall SHGetFileInfoW(LPCTSTR szPath,DWORD dwFileAttributes,SHFILEINFO* pSFileInfo,UINT cbFileInfo,UINT uFlags);
int __stdcall SHGetFolderLocation(HWND hwndOwner,DWORD nFolder,HANDLE hToken,DWORD dwReserved,PIDLIST_ABSOLUTE* ppidl);
int __stdcall SHGetFolderPath(HWND hwndOwner,int nFolder,HANDLE hToken,DWORD dwFlags,LPTSTR pszPath);
int __stdcall SHGetFolderPathA(HWND hwndOwner,int nFolder,HANDLE hToken,DWORD dwReserved,LPTSTR lpBuffer);
int __stdcall SHGetFolderPathAndSubDir(HWND hwnd,DWORD csidl,HANDLE hToken,DWORD dwFlags,LPCTSTR pszSubDir,LPTSTR pszPath);
int __stdcall SHGetFolderPathW(HWND hwndOwner,int nFolder,HANDLE hToken,DWORD dwReserved,LPTSTR lpBuffer);
int __stdcall SHGetIDListFromObject(IUnknown* punk,PIDLIST_ABSOLUTE* ppidl);
int __stdcall SHGetIconOverlayIndex(LPCTSTR pszIconPath,int iIconIndex);
int __stdcall SHGetImageList(int iImageList,REFIID riid,void** ppv);
int __stdcall SHGetInstanceExplorer(IUnknown** ppunk);
int __stdcall SHGetItemFromDataObject(IDataObject* pdtobj,DATAOBJ_GET_ITEM_FLAGS dwFlags,REFIID riid,void** ppv);
int __stdcall SHGetItemFromObject(IUnknown* punk,REFIID riid,void** ppv);
int __stdcall SHGetKnownFolderIDList(REFKNOWNFOLDERID rfid,KNOWN_FOLDER_FLAG dwFlags,HANDLE hToken,PIDLIST_ABSOLUTE* ppidl);
int __stdcall SHGetKnownFolderItem(REFKNOWNFOLDERID rfid,KNOWN_FOLDER_FLAG dwFlags,HANDLE hToken,REFIID riid,void** ppv);
int __stdcall SHGetKnownFolderPath(REFKNOWNFOLDERID rfid,DWORD dwFlags,HANDLE hToken,PWSTR* ppszPath);
int __stdcall SHGetLocalizedName(LPCWSTR pszPath,LPWSTR pszResModule,UINT cch,int* pidsRes);
int __stdcall SHGetMalloc(LPMALLOC* ppMalloc);
int __stdcall SHGetNameFromIDList(PCIDLIST_ABSOLUTE pidl,SIGDN sigdnName,PWSTR* ppszName);
int __stdcall SHGetNameFromPropertyKey(REFPROPERTYKEY propkey,PWSTR* ppszCanonicalName);
int __stdcall SHGetNewLinkInfo(LPCTSTR pszLinkTo,LPCTSTR pszDir,LPTSTR pszName,BOOL* pfMustCopy,UINT uFlags);
int __stdcall SHGetPathFromIDList(PCIDLIST_ABSOLUTE pidl,LPTSTR pszPath);
int __stdcall SHGetPathFromIDListEx(PCIDLIST_ABSOLUTE pidl,PWSTR pszPath,DWORD cchPath,GPFIDL_FLAGS uOpts);
int __stdcall SHGetPathFromIDListW(PCIDLIST_ABSOLUTE pidlTarget,LPTSTR pszPath);
int __stdcall SHGetPropertyStoreForWindow(HWND hwnd,REFIID riid,void** ppv);
int __stdcall SHGetPropertyStoreFromIDList(PCIDLIST_ABSOLUTE pidl,GETPROPERTYSTOREFLAGS flags,REFIID riid,void** ppv);
int __stdcall SHGetPropertyStoreFromParsingName(PCWSTR pszPath,IBindCtx* pbc,GETPROPERTYSTOREFLAGS flags,REFIID riid,void** ppv);
int __stdcall SHGetRealIDL(IShellFolder* psf,PCUITEMID_CHILD pidlSimple,PITEMID_CHILD* ppidlReal);
int __stdcall SHGetSetFolderCustomSettings(LPSHFOLDERCUSTOMSETTINGS pfcs,LPCTSTR pszPath,DWORD dwReadWrite);
int __stdcall SHGetSetSettings(LPSHELLSTATE lpss,DWORD dwMask,BOOL bSet);
int __stdcall SHGetSettings(LPSHELLFLAGSTATE lpsfs,DWORD dwMask);
int __stdcall SHGetShellStyleHInstance();
int __stdcall SHGetSpecialFolderLocation(HWND hwndOwner,DWORD nFolder,PIDLIST_ABSOLUTE* pidl);
int __stdcall SHGetSpecialFolderPath(HWND hwndOwner,LPTSTR lpszPath,DWORD csidl,BOOL fCreate);
int __stdcall SHGetSpecialFolderPathA(HWND hwnd,LPTSTR pszPath,DWORD csidl,BOOL fCreate);
int __stdcall SHGetSpecialFolderPathW(HWND hwnd,LPTSTR pszPath,DWORD csidl,BOOL fCreate);
int __stdcall SHGetStockIconInfo(SHSTOCKICONID siid,UINT uFlags,SHSTOCKICONINFO* psii);
int __stdcall SHGetTemporaryPropertyForItem(IShellItem* psi,REFPROPERTYKEY pk,PROPVARIANT* ppropvarInk);
int __stdcall SHGetUnreadMailCountW(HKEY hKeyUser,LPCWSTR pszMailAddress,DWORD* pdwCount,FILETIME* pFileTime,LPCWSTR pszShellExecuteCommand,int cchShellExecuteCommand);
int __stdcall SHHandleUpdateImage(PCIDLIST_ABSOLUTE pidlExtra);
int __stdcall SHILCreateFromPath(LPCWSTR pszPath,PIDLIST_ABSOLUTE* ppidl,DWORD* rgflnOut);
int __stdcall SHInvokePrinterCommand(HWND hwnd,UINT uAction,LPCTSTR lpBuf1,LPCTSTR lpBuf2,BOOL fModal);
int __stdcall SHIsFileAvailableOffline(LPCWSTR pszPath,LPDWORD pdwStatus);
int __stdcall SHLimitInputEdit(HWND hwndEdit,IShellFolder* psf);
int __stdcall SHLoadInProc(REFCLSID rclsid);
int __stdcall SHLoadNonloadedIconOverlayIdentifiers();
int __stdcall SHLoadOLE(LPARAM lParam);
int __stdcall SHLocalStrDup(LPCWSTR psz,LPWSTR* ppsz);
int __stdcall SHMapIDListToImageListIndexAsync(IShellTaskScheduler* pts,IShellFolder* psf,LPCITEMIDLIST pidl,UINT flags,PFNASYNCICONTASKBALLBACK pfn,void* pvData,void* pvHint,int* piIndex,int* piIndexSel);
int __stdcall SHMapPIDLToSystemImageListIndex(IShellFolder* psf,PCUITEMID_CHILD pidl,int* piIndex);
int __stdcall SHMultiFileProperties(IDataObject* pdtobj,DWORD dwFlags);
int __stdcall SHObjectProperties(HWND hwnd,DWORD shopObjectType,PCWSTR pszObjectName,PCWSTR pszPropertyPage);
int __stdcall SHOpenFolderAndSelectItems(PCIDLIST_ABSOLUTE pidlFolder,UINT cidl,PCUITEMID_CHILD_ARRAY* apidl,DWORD dwFlags);
int __stdcall SHOpenPropSheetW(LPCWSTR pszCaption,UINT ckeys,const CLSID* pclsidDef,IDataObject* pdtobj,IShellBrowser* psb,LPCWSTR pStartPage);
int __stdcall SHOpenWithDialog(HWND hwndParent,const OPENASINFO* poainfo);
int __stdcall SHParseDisplayName(LPCWSTR pszName,IBindCtx* pbc,PIDLIST_ABSOLUTE* ppidl,SFGAOF sfgaoIn,SFGAOF* psfgaoOut);
int __stdcall SHPathPrepareForWrite(HWND hwnd,IUnknown* punkEnableModless,LPCTSTR pszPath,DWORD dwFlags);
int __stdcall SHPropStgCreate(IPropertySetStorage* psstg,REFFMTID fmtid,const CLSID* pclsid,DWORD grfFlags,DWORD grfMode,DWORD dwDisposition,IPropertyStorage** ppstg,UINT* puCodePage);
int __stdcall SHPropStgReadMultiple(IPropertyStorage* pps,UINT uCodePage,ULONG cpspec);
int __stdcall SHPropStgWriteMultiple(IPropertyStorage* pps,UINT* uCodePage,ULONG cpspec,PROPID propidNameFirst);
int __stdcall SHQueryRecycleBin(LPCTSTR pszRootPath,LPSHQUERYRBINFO pSHQueryRBInfo);
int __stdcall SHQueryUserNotificationState(QUERY_USER_NOTIFICATION_STATE* pquns);
int __stdcall SHRemoveLocalizedName(LPCWSTR pszPath);
int __stdcall SHReplaceFromPropSheetExtArray(HPSXA hpsxa,UINT uPageID,LPFNADDPROPSHEETPAGE lpfnReplaceWith,LPARAM lParam);
int __stdcall SHResolveLibrary(IShellItem* psiLibrary);
int __stdcall SHRestricted(RESTRICTIONS rest);
int __stdcall SHRunControlPanel(LPCWSTR lpcszCmdLine,HWND hwndMsgParent);
int __stdcall SHSetDefaultProperties(HWND hwnd,IShellItem* psi,DWORD dwFileOpFlags,IFileOperationProgressSink* pfops);
int __stdcall SHSetFolderPath(DWORD csidl,HANDLE hToken,DWORD dwFlags,LPCTSTR pszPath);
int __stdcall SHSetInstanceExplorer(IUnknown* punk);
int __stdcall SHSetKnownFolderPath(REFKNOWNFOLDERID rfid,DWORD dwFlags,HANDLE hToken,PCWSTR pszPath);
int __stdcall SHSetLocalizedName(LPCWSTR pszPath,LPCWSTR pszResModule,int idsRes);
int __stdcall SHSetTemporaryPropertyForItem(IShellItem* psi,REFPROPERTYKEY propkey,REFPROPVARIANT propvar);
int __stdcall SHSetUnreadMailCountW(LPCWSTR pszMailAddress,DWORD dwCount,LPCWSTR pszShellExecuteCommand);
int __stdcall SHShellFolderView_Message(HWND hwnd,UINT uMsg,LPARAM lparam);
int __stdcall SHShowManageLibraryUI(IShellItem* psiLibrary,HWND hwndOwner,LPCWSTR pszTitle,LPCWSTR pszInstruction,LIBRARYMANAGEDIALOGOPTIONS lmdOptions);
int __stdcall SHSimpleIDListFromPath(LPCWSTR pszPath);
int __stdcall SHStartNetConnectionDialogW(HWND hwnd,LPCWSTR pszRemoteName,DWORD dwType);
int __stdcall SHTestTokenMembership(HANDLE hToken,ULONG ulRID);
int __stdcall SHUnlockShared(void* pvData);
int __stdcall SHUpdateImage(LPCTSTR pszHashItem,int iIndex,UINT uFlags,int iImageIndex);
int __stdcall SHValidateUNC(HWND hwndOwner,LPWSTR pszFile,UINT fConnect);
int __stdcall SetCurrentProcessExplicitAppUserModelID(PCWSTR AppID);
int __stdcall ShellAbout(HWND hWnd,LPCTSTR szApp,LPCTSTR szOtherStuff,HICON hIcon);
int __stdcall ShellAboutA(HWND hWnd,LPCTSTR szApp,LPCTSTR szOtherStuff,HICON hIcon);
int __stdcall ShellAboutW(HWND hWnd,LPCTSTR szApp,LPCTSTR szOtherStuff,HICON hIcon);
int __stdcall ShellExecute(HWND hwnd,LPCTSTR lpOperation,LPCTSTR lpFile,LPCTSTR lpParameters,LPCTSTR lpDirectory,INT nShowCmd);
int __stdcall ShellExecuteA(HWND hWnd,LPCTSTR lpOperation,LPCTSTR lpFile,LPCTSTR lpParameters,LPCTSTR lpDirectory,INT nShowCmd);
int __stdcall ShellExecuteEx(LPSHELLEXECUTEINFO lpExecInfo);
int __stdcall ShellExecuteExW(LPSHELLEXECUTEINFO lpExecInfo);
int __stdcall ShellExecuteW(HWND hWnd,LPCTSTR lpOperation,LPCTSTR lpFile,LPCTSTR lpParameters,LPCTSTR lpDirectory,INT nShowCmd);
int __stdcall Shell_GetCachedImageIndex(LPCWSTR pwszIconPath,int iIconIndex,UINT uIconFlags);
int __stdcall Shell_GetImageLists(HIMAGELIST* phiml,HIMAGELIST* phimlSmall);
int __stdcall Shell_MergeMenus(HMENU hmDst,HMENU hmSrc,UINT uInsert,UINT uIDAdjust,UINT uIDAdjustMax,ULONG uFlags);
int __stdcall Shell_NotifyIcon(DWORD dwMessage,PNOTIFYICONDATA pnid);
int __stdcall Shell_NotifyIconGetRect(const NOTIFYICONIDENTIFIER* identifier,RECT* iconLocation);
int __stdcall Shell_NotifyIconW(DWORD dwMessage,PNOTIFYICONDATA pnid);
int __stdcall SignalFileOpen(PCIDLIST_ABSOLUTE pidl);
int __stdcall StgMakeUniqueName(IStorage* pstgParent,PCWSTR pszFileSpec,DWORD grfMode,REFIID riid,void** ppv);
int __stdcall WOWShellExecute(HWND hwnd,LPCTSTR lpOperation,LPCTSTR lpFile,LPCTSTR lpParameters,LPCTSTR lpDirectory,INT nShowCmd,void* lpfnCBWinExec);
int __stdcall Win32DeleteFile(LPCTSTR pszFileName);
int __stdcall WriteCabinetState(CABINETSTATE* pcs);