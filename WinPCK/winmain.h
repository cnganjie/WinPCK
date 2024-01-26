
#include "tLogDlg.h"

class TInstDlg : public TDlg
{
protected:
	TLogDlg			m_logdlg;

public:
	TInstDlg(LPTSTR cmdLine);
	virtual ~TInstDlg();

	virtual BOOL	EvCreate(LPARAM lParam) override;
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl) override;
	virtual BOOL	EvClose() override;
	virtual BOOL	EvTimer(WPARAM timerID, TIMERPROC proc) override;
	virtual BOOL	EvNotify(UINT ctlID, NMHDR *pNmHdr) override;
#ifdef _USE_CUSTOMDRAW_
	virtual BOOL	EvMeasureItem(UINT ctlID, MEASUREITEMSTRUCT *lpMis) override;
	virtual BOOL	EvDrawItem(UINT ctlID, DRAWITEMSTRUCT *lpDis) override;
#endif

	virtual BOOL	EvDropFiles(HDROP hDrop) override;

	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos) override;
	virtual BOOL	EvMouseMove(UINT fwKeys, POINTS pos) override;

	virtual BOOL	EvSize(UINT fwSizeType, WORD nWidth, WORD nHeight) override;

//�û�����
private:

	//��ǰ������
	std::wstring	m_MyFileName;
	fs::path	m_Filename;
	//����ǰ����·��
	std::wstring	m_CurrentPath;

	BOOL	m_isListviewRenaming;

	int		m_iListHotItem;	//��ǰ�����listview�ϵ�ѡ��������

	const PCK_UNIFIED_FILE_ENTRY*	m_currentNodeOnShow;

	BOOL	bGoingToExit;
	wchar_t	m_szStrToSearch[256];

	//addmode
	std::vector<std::wstring>	m_lpszFilePath;

	//�����Ҵ��ڵı���
	BOOL	m_isSearchWindow;

	HCURSOR	m_hCursorOld, m_hCursorAllow, m_hCursorNo;

	//��ǰ�����pck�ļ��е�Ŀ¼·��
	std::wstring	m_FolderBrowsed;

	HIMAGELIST	m_imageList;

	//����������
	ITaskbarList3* m_pTaskBarlist = nullptr;

	//�û�����
private:

	//winmain.cpp

	BOOL IsValidWndAndGetPath(wchar_t * szPath, BOOL isGetPath = FALSE);
	void RefreshProgress();
	std::wstring BuildSaveDlgFilterString();

	//threadproc.cpp
	void EnbaleButtons(BOOL bEnbale);
	static VOID UpdatePckFile(VOID *pParam);
	static VOID RenamePckFile(VOID *pParam);
	static VOID RebuildPckFile(VOID	*pParam);
	static VOID StripPckFile(VOID *pParam);
	static VOID CreateNewPckFile(VOID *pParam);
	static VOID ToExtractAllFiles(VOID *pParam);
	static VOID ToExtractSelectedFiles(VOID	*pParam);
	static VOID DeleteFileFromPckFile(VOID *pParam);


	//mainfunc.cpp
	BOOL NewPckFile();
	BOOL OpenPckFile(std::wstring sFileToOpen, BOOL isReOpen = FALSE);
	BOOL OpenPckFile(BOOL isReOpen = FALSE);
	VOID SearchPckFiles();
	VOID ShowPckFiles(const PCK_UNIFIED_FILE_ENTRY* lpNodeToShow);

#pragma region guilated.cpp
private:
	//guilated.cpp
	void		initCommctrls();
	void		initParams();
	void		initToolbar();
	void		initArgument();
#pragma endregion

#pragma region helpfunc.cpp
private:
	//helpfunc.cpp
	VOID ViewFileAttribute();
	VOID ViewFile(const PCK_UNIFIED_FILE_ENTRY* lpFileEntry);
	BOOL AddFiles();

	void SetupRegAsso(bool isInstall) const;
	void AddSetupReg();
	void DeleteSetupReg();

	void InitLogWindow();

	const PCK_UNIFIED_FILE_ENTRY* GetFileEntryByItem(int itemIndex);

	void DbClickListView(const int itemIndex);	//�����б��е�itemIndex�����Ŀ¼��Ԥ���ļ���
	void PopupRightMenu(const int itemIndex);		//listview���һ����˵�

	void UnpackFiles(int id, auto func);
	void UnpackAllFiles();					//��ѹ�����ļ�
	void UnpackSelectedFiles();				//��ѹѡ�е��ļ�

#pragma endregion
private:
	//�򿪡��رա���ԭ���¼�ע��
	static int MyFeedbackCallback(void* pTag, int32_t eventId, size_t wParam, ssize_t lParam);


	//listViewFunc.cpp

	BOOL EvNotifyListView(const NMHDR *pNmHdr);
	BOOL EvDrawItemListView(const DRAWITEMSTRUCT *lpDis);

public:
	void InsertList(CONST HWND hWndList, CONST INT iIndex, CONST UINT uiMask, CONST INT iImage, CONST LPVOID lParam, CONST INT nColCount, ...);
private:
	BOOL InitListView(CONST HWND hWndListView, const LPTSTR *lpszText, const int *icx, const int *ifmt);

	BOOL ListView_BeginLabelEdit(const HWND hWndList, LPARAM lParam);
	BOOL ListView_EndLabelEdit(const NMLVDISPINFOW* pNmHdr);

	void ListView_Init();
	void ListView_Uninit();

	void ProcessColumnClick(CONST HWND hWndList, CONST NMLISTVIEW * pnmlistview, DWORD& dwSortStatus);


#pragma region MenuButtonFuncs.cpp
	//menu
private:
	void MenuStrip();
	void MenuClose();
	void MenuInfo();
	void MenuSearch();
	void MenuNew(WORD wID);
	void MenuAdd(WORD wID);
	void MenuRebuild(WORD wID);
	void MenuCompressOpt();
	void MenuRename();
	void MenuDelete();
	void MenuSelectAll();
	void MenuSelectReverse();
	void MenuAttribute();
	void MenuView();
	void MenuAbout();
	void ListViewEnter();
	void MenuCancelPckOper();
#pragma endregion

#pragma region mainControlStatus.cpp

private:
	void SetStatusBarText(int iPart, LPCWSTR lpszText);
	void log_callback(const spdlog::details::log_msg& msg);

public:
	void SetStatusBarTitle(LPCWSTR lpszText);
	void SetStatusBarFileSize(uint64_t size);
	void SetStatusBarFileCount(uint32_t size);
	void ClearStatusBarProgress();
	void SetStatusBarProgress(LPCWSTR lpszText);
	void SetStatusBarInfo(LPCWSTR lpszText);

#pragma endregion


};

class TInstApp : public TApp
{
public:
	TInstApp(HINSTANCE _hI, LPTSTR _cmdLine, int _nCmdShow);
	virtual ~TInstApp();

	virtual void InitWindow(void) override;
};