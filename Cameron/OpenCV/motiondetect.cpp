#include "motiondetect.h"

#include <QDebug>

using namespace std;
using namespace cv;
MotionDetect::MotionDetect(QObject *parent) : QObject(parent)
{
    _sensitivity = 30;
}

void MotionDetect::setSensitivity(int s)
{
    _sensitivity = s;
}


/**
 * @brief Попытаемся засечь движение
 */
void MotionDetect::detect(QSharedPointer<uint8_t> data, int width, int height)
{
    _prevFrame = _curFrame;
    _curFrame = _nextFrame;

    _nextFrame = Mat( height, width, CV_8UC3, data.data() );
    cvtColor( _nextFrame, _nextFrame, CV_RGB2GRAY );

    if (_curFrame.rows!=_prevFrame.rows || _curFrame.rows!=_nextFrame.rows) return;

    Mat d1, d2, kernelEro, motion;

    kernelEro = getStructuringElement( MORPH_RECT, Size(2,2) );


    absdiff(_prevFrame, _nextFrame, d1 );
    absdiff(_nextFrame, _curFrame, d2 );

    bitwise_and( d1, d2, motion );
    threshold( motion, motion, 35,255, CV_THRESH_BINARY );
    erode( motion, motion, kernelEro );

    int motionCount = countNonZero(motion);

    if (motionCount>_sensitivity) {        
        emit detected(motionCount);
    }

}

