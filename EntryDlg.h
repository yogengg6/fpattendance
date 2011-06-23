/**
 * Project Name : FP_Attendance
 * description  : a fingerprint based attendance(work with moodle attendanc
 *                e module)
 * Copyleft     : This program is published under GPL
 * Author       : Yusuke(Qiuchengxuan@gmail.com)
 * Date	        : 2011-5-11 20:29
 */
#pragma once

using namespace std;

#include <vector>

#include "afxwin.h"
#include "mdldb/Mdldb.h"

// CEntryDlg �Ի���

class CEntryDlg : public CDialog
{
	DECLARE_DYNAMIC(CEntryDlg)

public:
	CEntryDlg(mdldb::Mdldb &mdl, CWnd* pParent = NULL);
	virtual ~CEntryDlg();

	enum { IDD = IDD_ENTRY };

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);

	vector<mdldb::CourseInfo>	m_courseInfo;
	vector<string>				m_session;

	CButton   *					m_nextButton;
	CStatic	  *					m_static_course;
	CStatic   *					m_static_session;
	CComboBox *					m_courseComboBox;
	CComboBox *					m_sessionComboBox;

private:
	mdldb::Mdldb m_mdl;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelendokCourse();
	afx_msg void OnCbnSelendokSession();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedAttendant();
	afx_msg void OnCbnSelchangeCourse();
	afx_msg void OnCbnSetfocusSession();
	afx_msg void OnBnClickedEntryFpmanage();
};