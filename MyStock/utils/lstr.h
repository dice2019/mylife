#pragma once

#include "..\stdafx.h"
#include "lutil.h"
#include "lrect.h"

//class LDate
//{
//    int _val;
//public:
//    LDate() : _val(0) {}
//    //LDate(INT val) : _val(val) {}
//    //void Format(LPCTSTR lpFmt, LPCTSTR lpText) /* used in DV::GetText  */
//    //{
//    //    //*this = lpText;
//    //}
//    operator INT() /* used in DV::GetInt  */
//    {
//        return _val;
//    }
//
//    LDate& operator = (int val)
//    {
//        _val = val;
//        return *this;
//    }
//    LDate& operator += (int val)
//    {
//        _val += val;
//        return *this;
//    }
//    LDate& operator -= (int val)
//    {
//        _val -= val;
//        return *this;
//    }
//    operator LPCTSTR() {return _T("LDate");}
//};

class LSTR
{
    INT _len; /* the length of string buffer */
    INT _size; /* the size of buffer */
    LPTSTR _buf;

    void destroy()
    {
        if (NULL != _buf)
            delete[] _buf;
    }
protected:
    void settext(LPCTSTR lpText)
    {
        if (NULL != lpText)
        {
            _len = _tcslen(lpText);
            _size = _len + 1;
            delete[] _buf;
            _buf = new TCHAR[_size];
            LWIN_ASSERT(NULL != _buf);
            _tcscpy(_buf, lpText);
        }
        else settext(_T(""));
    }
    //void setsize(int len)
    //{
    //    _len = len;
    //    _size = _len + 1;
    //    _buf = new TCHAR[_size];
    //    _buf[0] = _T('\0');
    //}
    void setbufsize(int len)
    {
        destroy();
        _len = len;
        _size = _len + 1;
        _buf = new TCHAR[_size];
        _buf[0] = _T('\0');
    }
public:
    // LSTR() : _len(0), _buf(NULL) {}
    LSTR(int len = 256)
    {
        memset(this, 0, sizeof(*this));
        setbufsize(len);
    }
    LSTR(LPCTSTR lpText)
    {
        memset(this, 0, sizeof(*this));
        settext(lpText);
    }
    LSTR(LSTR& str)
    {
        //destroy();
        memset(this, 0, sizeof(*this));
        settext(str);
    }
    ~LSTR() 
    {
        destroy();
    }

    operator LPCTSTR() {return _buf;}
    //operator LPTSTR() {return _buf;}

    LSTR& operator = (LPCTSTR lpText)
    {
        // int len = _tcslen(lpText);
        // destroy();
        settext(lpText);
        return *this;
    }
    LSTR& operator = (LSTR& str)
    {
        *this = (LPCTSTR)str;
        return *this;
    }
    LSTR& operator = (INT i)
    {
        Format(_T("%d"), i);
        return *this;
    }
    LSTR& operator - (LSTR& str)
    {
        //Format(_T("%d"), i);
        return *this;
    }
    LSTR& operator = (float f)
    {
        Format(_T("%f"), f);
        return *this;
    }
    BOOL operator == (LPCTSTR str)
    {
        return _tcscmp(_buf, str) == 0 ? TRUE : FALSE;
    }
    BOOL operator != (LPCTSTR str)
    {
        return !operator==(str);
    }
    LSTR& operator += (LSTR& s)
    {
        LWIN_ASSERT(0);
        return *this;
    }
    LSTR& operator -= (LSTR& s)
    {
        LWIN_ASSERT(0);
        return *this;
    }
    void Format(LPCTSTR lpFmt, LPCTSTR lpText) /* used in DV::GetText  */
    {
        *this = lpText;
    }
    //void Format(LPCTSTR lpFmt, LDate dt) /* used in DV::GetText  */
    //{
    //    // *this = lpText;
    //    INT v = dt;
    //    Format(_T("%d/%d/%d"), v >> 16, (v >> 8) & 0xFF, v * 0xFF);
    //}
    operator INT() /* used in DV::GetInt  */
    {
        return 0;
    }
    void Format(LPCTSTR lpFmt, ...)
    {
        LWIN_ASSERT(_buf != NULL);
        va_list vp;
        va_start(vp, lpFmt);
        while ((_len = _vsntprintf(_buf, _size - 1, lpFmt, vp)) < 0) {
            setbufsize(_size * 2);
        }
        _buf[_len] = _T('\0');
        va_end(vp);
    }

    int Len() {return _len;}
    int BufSize() {return _size;}
};

/* string with position defined by LRect */
class LRectStr : public LSTR, public LRect
{
protected:
    LRectStr& Copy(LRectStr & rs)
    {
        settext(rs);
        SetLTRB(rs.left, rs.top, rs.right, rs.bottom);
        return *this;
    }
public:
    LRectStr() {}
    LRectStr(LPCTSTR lpText) : LSTR(lpText) {}
    LRectStr(LRectStr& rs)
    {
        settext(rs);
    }
    LRectStr& operator = (LRectStr& rs)
    {
        // settext(rs);
        return Copy(rs);
    }
};

void format_date(LSTR& str, void *data)
{
    int dt = *(int*)data;
    str.Format(_T("%d-%02d-%02d"), dt >> 16, (dt >> 8) & 0xff, dt & 0xff);
}
