#pragma once


// Finestra di dialogo first_dialog

class first_dialog : public CDialog
{
	DECLARE_DYNAMIC(first_dialog)

public:
	first_dialog(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~first_dialog();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
};
