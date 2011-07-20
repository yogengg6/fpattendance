/**
 * Project Name : FP_Attendance
 * description  : a fingerprint based attendance(work with moodle attendanc
 *                e module)
 * Copyleft     : This program is published under GPL
 * Author0      : Yusuke(Qiuchengxuan@gmail.com)
 * Author1      : Zembojue(zembojue@gmail.com)
 * Date	        : 2011-5-29 20:29
 */
#pragma once

#include "stdafx.h"

#include <vector>
#include <WinCrypt.h>

#include <dpDefs.h>
#include <dpRcodes.h>
#include <DPDevClt.h>

#include "afxwin.h"
#include "mdldb/Mdldb.h"

using namespace std;
using namespace mdldb;

// CAttendanceDlg �Ի���

class CAttendanceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttendanceDlg)

public:
	CAttendanceDlg(Mdldb& conn, CWnd* pParent = NULL);
	virtual ~CAttendanceDlg();

	void AddStatus(LPCTSTR s);

	// �Ի�������
	enum { IDD = IDD_ATTENDANT };
	enum { FP_NOTIFY = WM_USER + 100 };
	enum { FP_NOT_FOUND = -1, DEVICE_ERROR = -1, LACK_MEMORY = -3};

private:
	typedef struct {
		int index;
		uint status;
		CString description;
	} AttendInfo;

	enum {ALREADY_PRE_ATTEND = -1};
	
	//����ؼ���Ա����
	CListBox*		m_notify;

	//ָ��ʶ����س�Ա����
	FT_HANDLE       m_fxContext;
	FT_HANDLE       m_mcContext;
	HDPOPERATION    m_hOperationVerify;
	DATA_BLOB       m_RegTemplate;

	//���ݽṹ��Ա����
	Mdldb	m_mdl;
	vector<Student>	m_student_info;

	Session			m_session;

	vector<AttendInfo>		m_studentsToAttendant;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	int  locateStudentsToAttendant(int index);
	int preAttendant(int index);

	void initFpDevice();
	void TerminateFpDevice();

	/** 
	 * �����ݿ⣨�򱾵أ��е�ָ������һһ�ȶԣ��ҵ�ƥ������ݺ󷵻�
	 * ��ѧ����Ϣ��m_student_index�����е��±�
	 */
	int MatchFeatures(DATA_BLOB* const fpTemplate);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedBack();
    afx_msg LRESULT OnFpNotify (WPARAM wParam, LPARAM lParam);
};