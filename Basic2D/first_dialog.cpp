// first_dialog.cpp: file di implementazione
//

#include "stdafx.h"
#include "first_dialog.h"
#include "afxdialogex.h"
#include "resource.h"

// Finestra di dialogo first_dialog

IMPLEMENT_DYNAMIC(first_dialog, CDialog)

first_dialog::first_dialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

first_dialog::~first_dialog()
{
}

void first_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(first_dialog, CDialog)
END_MESSAGE_MAP()


// Gestori di messaggi di first_dialog
