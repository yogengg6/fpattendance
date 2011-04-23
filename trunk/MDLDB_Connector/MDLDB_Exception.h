/**
 * Copyleft : This program is published under GPL
 * Author   : Yusuke
 * Email    : Qiuchengxuan@gmail.com
 * Date	    : 2011-4-22 21:18
 */

#include <iostream>
#include <exception>

using namespace std;

const int MDLDB_CONNECTED           =  0;
const int MDLDB_DISCONNECTED        = -1;
const int MDLDB_UNKNOWN_ERROR       = -2;
const int MDLDB_CONNECTION_FAIL     = -3;
const int MDLDB_CONNECTION_REFUSED  = -4;

const int MDLDB_NO_COURSE           = -6;
const int MDLDB_DUPLICATE_COURSE    = -7;

const int MDLDB_NO_SESSION          = -8;
const int MDLDB_DUPLICATE_SESSION   = -9;

class MDLDB_Exception: public std::runtime_error
{
public:
    MDLDB_Exception(const std::string& msg, int err_no = MDLDB_UNKNOWN_ERROR);
    MDLDB_Exception();
    virtual ~MDLDB_Exception() throw() {};
    int get_error_code() const;
private:
    int err_no;
};