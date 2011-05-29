/**
 * Project Name : FP_Attendance
 * description  : a fingerprint based attendance(work with moodle attendanc
 *                e module)
 * Copyleft     : This program is published under GPL
 * Author       : Yusuke(Qiuchengxuan@gmail.com)
 * Author       : Bojue(zembojue@gmail.com)
 * Date	        : 2011-5-11 20:29
 */
// AttendanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FP_Attendance.h"
#include "AttendanceDlg.h"
#include "MDLDB_Connector/MDLDB_Exception.h"

// CAttendanceDlg 对话框

IMPLEMENT_DYNAMIC(CAttendanceDlg, CDialog)

CAttendanceDlg::CAttendanceDlg(mdldb::Connector& conn, CWnd* pParent /*=NULL*/)
	: CDialog(CAttendanceDlg::IDD, pParent), 
	  m_conn(conn),
	  m_Context(0),
	  m_notifyListBox(NULL)
{

}

CAttendanceDlg::~CAttendanceDlg()
{
}

void CAttendanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAttendanceDlg::OnInitDialog()                     //对话框初始化
{
	if (! m_conn.connected())
		return FALSE;
	
	m_notifyListBox	  = (CListBox *) GetDlgItem(IDC_LIST_NOTIFY);

	m_student_info	  = m_conn.get_course_student_info();

    
    tm=CTime::GetCurrentTime(); //得到系统时间
	
	
	return TRUE;
}

BEGIN_MESSAGE_MAP(CAttendanceDlg, CDialog)
	ON_BN_CLICKED(ID_EXIT, &CAttendanceDlg::OnBnClickedExit)
	ON_BN_CLICKED(ID_BACK, &CAttendanceDlg::OnBnClickedBack)
END_MESSAGE_MAP()


// CAttendanceDlg 消息处理程序

void CAttendanceDlg::OnBnClickedExit()
{
	this->EndDialog(ID_EXIT);
}

void CAttendanceDlg::OnBnClickedBack()
{
	this->EndDialog(ID_BACK);
}


//这个函数是抽象出来的动作，向IDC_LIST_NOTIFY中添加信息

void CAttendanceDlg::AddStatus(LPCTSTR s) {
	int lIdx = SendDlgItemMessage(IDC_LIST_NOTIFY, LB_ADDSTRING, 0, (LPARAM)s);
	SendDlgItemMessage(IDC_LIST_NOTIFY, LB_SETTOPINDEX, lIdx, 0);
}



//比较的函数,已完成
 bool CAttendanceDlg::VerifyFeatures(vector<mdldb::StudentInfo>	m_student_info, DATA_BLOB* pImageBlob){
 
     
 	for(vector<mdldb::StudentInfo>::iterator it = m_student_info.begin(); it != m_student_info.end(); ++it)
 	{
        FT_BOOL* pComparisonDecision;//返回比对结果的指针

 		MC_verifyFeaturesEx(
 		    m_Context,               //这个参数可能有问题？？？？？？？ ？？？？？？                 
 			it->fpsize,     
 			it->fpdata,     
 			pImageBlob->cbData,               
 			pImageBlob->pbData,               
 			0,                                
 			NULL ,                           
 			NULL ,                            
 			NULL ,                            
 			NULL ,                            									
 		    pComparisonDecision);            									
 											 
 		if(FT_TRUE == pComparisonDecision)
		{
			return true;
		}
		
 	}
	
	return false;
 
 }


//得到采集的指纹数据,数据接口pImageBlob
 LRESULT CAttendanceDlg::OnFpNotify(WPARAM wParam, LPARAM lParam) {
 	if (m_idnumber == "")//这个变量对应的是什么？？？？？？？？？？？？？
 		return S_OK;
 
 	switch(wParam) {
 	case WN_COMPLETED: {
 		SetDlgItemText(IDC_EDIT_PROMPT, L"指纹图像已获取");
 		DATA_BLOB* pImageBlob = reinterpret_cast<DATA_BLOB*>(lParam);
		
         if(true == VerifyFeatures( m_StudentInfo_list, pImageBlob))
 			{  
				//比对成功
 				//此处再次得到系统时间，与初始化时的时间tm进行比较,以确定是否迟到？？？？？
				 AddStatus(_T("指纹成功识别，祝您愉快。"));
	 
 			}
 		else
 			{   //比对失败
				 AddStatus(_T("指纹识别失败！"));
 			}



		 //这里需要做识别后的相应操作，调用学长的函数。
		
	     //释放内存？？？？？？？？？？？？？？？？？？

 
 		break;
 					   }
 	case WN_ERROR: {
 		TCHAR buffer[101] = {0};
 		_sntprintf_s(buffer, 100, _TRUNCATE, _T("发生错误。错误代码: 0x%X"), lParam);
 		AddStatus(buffer);
 		break;
 				   }
 	case WN_DISCONNECT:
 		SetDlgItemText(IDC_EDIT_PROMPT, L"指纹识别仪没有连接或失去连接");
 		break;
 	case WN_RECONNECT:
 		SetDlgItemText(IDC_EDIT_PROMPT, L"指纹识别仪已连接");
 		break;
 	case WN_FINGER_TOUCHED:
 		SetDlgItemText(IDC_EDIT_PROMPT, L"手指已接触识别区");
 		break;
 	case WN_FINGER_GONE:
 		SetDlgItemText(IDC_EDIT_PROMPT, L"手指离开识别区");
		break;
 	case WN_IMAGE_READY:
 		SetDlgItemText(IDC_EDIT_PROMPT, L"指纹图像已准备好");
 		break;
 	case WN_OPERATION_STOPPED:
		SetDlgItemText(IDC_EDIT_PROMPT, L"指纹注册操作停止");
		break;
 		
 	}
 	return S_OK;
 }

