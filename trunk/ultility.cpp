#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <blowfish.h>

#include "ultility.h"

using namespace std;
using namespace CryptoPP;

CString stringToCString(string& str)
{
	//str.size();
	static wchar_t conv_buf[200];
	ZeroMemory(conv_buf, 200);
	MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), -1, conv_buf, str.length());
	return CString(conv_buf);
}

string CStringToString(CString& cstr)
{
	static size_t max_size = 0;
	size_t cstr_size = (cstr.GetLength()+1)*2;
	static auto_ptr<char> conv_buf;
	if (cstr_size > max_size) {
		conv_buf.release();
		conv_buf = auto_ptr<char>(new char[max_size = cstr.GetLength()*2]);
	}
	ZeroMemory(conv_buf.get(), max_size);
	wcstombs_s(NULL, conv_buf.get(), (cstr.GetLength() + 1) * 2, cstr, _TRUNCATE);
	return string(conv_buf.get());
}

CString Config::m_path = L"";
CString Config::m_buffer = L"";
string  Config::m_dbhost = "";
string  Config::m_dbport = "";
string  Config::m_dbuser = "";
string  Config::m_dbpasswd = "";
string  Config::m_passwordsalt = "";

Config::Config()
{
	if (m_path == L"") {
		::GetCurrentDirectory(BUFFER_SIZE, m_path.GetBufferSetLength(BUFFER_SIZE));
		m_path.ReleaseBuffer();
		m_path += "\\config.ini";
	}
}

void Config::encode_buffer()
{
	const int BLOCKSIZE = BlowfishEncryption::BLOCKSIZE;
	BlowfishEncryption be;
	const char *LicenseKey = "test";
	be.SetKey( (const byte*)LicenseKey, BlowfishEncryption::DEFAULT_KEYLENGTH);
	
	size_t strlength = m_buffer.GetLength()*2;
	size_t block_count;
	if (strlength % BLOCKSIZE == 0)
		block_count = strlength/BLOCKSIZE;
	else
		block_count = strlength/BLOCKSIZE +1;
	size_t size = block_count * BLOCKSIZE;

	byte* enc_block = NULL;
	enc_block = new byte[size];
	ZeroMemory(enc_block, size);

	wchar_t* pBuf = m_buffer.GetBuffer();

	memcpy(enc_block, pBuf, strlength);

	for (size_t i = 0; i < block_count; ++i)
		be.ProcessBlock(enc_block + i*BLOCKSIZE);

	m_buffer = "";

	for (size_t i = 0; i < size; ++i)
		m_buffer.AppendFormat(L"%02x", enc_block[i]);

	delete enc_block;
}

void Config::decode_buffer()
{
	const int BLOCKSIZE = BlowfishDecryption::BLOCKSIZE;
	BlowfishDecryption bd;
	const char *LicenseKey = "test";
	bd.SetKey((const byte*)LicenseKey, BlowfishDecryption::DEFAULT_KEYLENGTH);  

	size_t size = m_buffer.GetLength()/2;
	size_t block_count;
	block_count = size/BLOCKSIZE;

	byte* dec_block = NULL;
	dec_block = new byte[size];
	ZeroMemory(dec_block, size);

	//ʮ�������ַ���ת��Ϊ����
	for (size_t i = 0; i < size; ++i) {
		dec_block[i] = hexchar_to_byte(m_buffer[i*2]) << 4;
		dec_block[i] += hexchar_to_byte(m_buffer[i*2+1]);
	}

	for (size_t i = 0; i < block_count; ++i)
		bd.ProcessBlock(dec_block + i*BLOCKSIZE);

	m_buffer = CString((wchar_t*)dec_block);
}

bool Config::load()
{
	if (read_profile_string(L"database", L"dbhost"))
		m_dbhost = CStringToString(m_buffer);
	else 
		return false;

	if (read_profile_string(L"database", L"dbport"))
		m_dbport = CStringToString(m_buffer);
	else
		return false;

	if (read_profile_string(L"database", L"dbuser"))
		m_dbuser = CStringToString(m_buffer);
	else
		return false;

	if (read_profile_string(L"database", L"dbpasswd")) {
		decode_buffer();
		m_dbpasswd = CStringToString(m_buffer);
	}

	if (read_profile_string(L"moodle", L"passwordsalt"))
		m_passwordsalt = CStringToString(m_buffer);
	else
		return false;

	return true;
}

void Config::create()
{
	m_dbhost = "172.16.81.156";
	m_dbport = "3306";
	m_dbuser = "attendance";
	m_buffer = "attendance";
	encode_buffer();
	m_dbpasswd = CStringToString(m_buffer);
	m_passwordsalt = "r+a~,qm_5(.M D]9[4-9T]u=JT/fu&z";
}

void Config::save()
{
	write_profile_string(L"database", L"dbhost", stringToCString(m_dbhost));
	write_profile_string(L"database", L"dbport", stringToCString(m_dbport));
	write_profile_string(L"database", L"dbuser", stringToCString(m_dbuser));
	write_profile_string(L"database", L"dbpasswd", stringToCString(m_dbpasswd));
	write_profile_string(L"moodle", L"passwordsalt", stringToCString(m_passwordsalt));
}