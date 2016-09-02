#ifndef MOTIONDETECT_H
#define MOTIONDETECT_H

#include <QObject>
#include <QSharedPointer>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


using namespace std;
using namespace cv;

class MotionDetect : public QObject
{
    Q_OBJECT
public:
    explicit MotionDetect(QObject *parent = 0);
    void setSensitivity(int s);

signals:    
    void detected(int motionCount);

public slots:
    void detect(QSharedPointer<uint8_t> data, int width, int height);

private:   
    Mat _prevFrame;
    Mat _curFrame;
    Mat _nextFrame;
    int _sensitivity;
};

#endif // MOTIONDETECT_H
