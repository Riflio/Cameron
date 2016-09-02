#ifndef H264_FUHEADER_H
#define H264_FUHEADER_H

namespace NS_H264 {

typedef char BYTE;

class H264_FUHeader {
public:


    //-- какая из частей фрагмента
    enum FU_FLAG {
        FU_NONE,
        FU_FIRST,
        FU_MIDDLE,
        FU_LAST
    };

    void set(BYTE fu){
        _fu = fu;
    }

    bool isFirst(){
        int f = (_fu & 0x80);
        return f != 0;

    }

    bool isEnd(){
        int e = (_fu & 0x40);
        return e != 0 ;
    }

    bool isMiddle() {
        int m = (_fu & 0xE0);
        return m == 0 ;
    }

    BYTE getType(){
        return (_fu & 0x1f);
    }

private:
     BYTE _fu;
};
}
#endif // H264_FUHEADER_H
