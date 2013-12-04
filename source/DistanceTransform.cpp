
#include <QDebug>
#include <QImage>

#include "MathMacros.h"

#include "DistanceTransform.h"

DistanceTransform::DistanceTransform(
    QImage source
,   unsigned short dstWidth
,   unsigned short dstHeight
,   float distScale)
:   m_source(source)
,   m_dstSize(dstWidth, dstHeight)
,   m_distScale(distScale)
,   m_sedt(nullptr)
{
}

DistanceTransform::~DistanceTransform()
{
    delete m_sedt;
}

float * DistanceTransform::data()
{
    if (!m_sedt)
    {
        int size = m_source.height() * m_source.width();
        m_sedt = new float[size];

        // start distance transform

        sedt(255);

        // convert back to image 

        QImage dst(m_source);
        const int step = dst.bytesPerLine() / dst.width();

        for (int i = 0; i < size; ++i)
            dst.bits()[i * step] = clamp(0, 255, static_cast<unsigned char>(m_sedt[i] * 255.0));
            // ignore other 3 channels

        // rescale image

        dst = dst.scaled(m_dstSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // blit data back to m_sedt

        size = dst.height() * dst.width();

        delete m_sedt;       
        m_sedt = new float[size];

        for (int i = 0; i < size; ++i)
            m_sedt[i] = static_cast<float>(dst.bits()[i * step]) / 255.f;
    }
    return m_sedt;
}

void DistanceTransform::sedt(const unsigned char threshold)
{
    const unsigned char * source = m_source.constBits();

    const int w = m_source.width();
    const int h = m_source.height();

    const int offset = 0;
    const int step = m_source.bytesPerLine() / m_source.width();


    // Task_3_2 - ToDo Begin

    QVector<QPoint> p(w*h,QPoint(-1,-1));
    QVector<float> d(w*h,w+h);
    float d1 = 0.f;
    float d2 = 0.f;

    for(int y=0; y<h; ++y)
        for(int x=0; x<w; ++x){
            uchar color = *(source+step*(y*w+x));
            if( (color != uchar(*(source+step*(y*w+std::max(0,x-1)))))||
                (color != uchar(*(source+step*(y*w+std::min(w-1,x+1)))))||
                (color != uchar(*(source+step*(std::max(0,y-1)*w+x))))||
                (color != uchar(*(source+step*(std::min(h-1,y+1)*w+x))))){
                    d[y*w+x] = 0.f;
                    p[y*w+x] = QPoint(x,y);
                    m_sedt[y*w+x] = 1.0;
            }
        }

    for(int y=1; y<h; ++y)
        for(int x=1; x<w-1; ++x){
            if(d[(y-1)*w+(x-1)] + d2 < d[y*w+x]){
                p[y*w+x] = p[(y-1)*w+(x-1)];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
            if(d[(y-1)*w+x] + d1 < d[y*w+x]){
                p[y*w+x] = p[(y-1)*w+x];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
            if(d[(y-1)*w+(x+1)] + d2 < d[y*w+x]){
                p[y*w+x] = p[(y-1)*w+(x+1)];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
            if(d[y*w+(x-1)] + d1 < d[y*w+x]){
                p[y*w+x] = p[y*w+(x-1)];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
        }


    for(int y=h-2; y>=0; --y)
        for(int x=w-2; x>0; --x){
            if(d[y*w+(x+1)] + d1 < d[y*w+x]){
                p[y*w+x] = p[y*w+(x+1)];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
            if(d[(y+1)*w+(x-1)] + d2 < d[y*w+x]){
                p[y*w+x] = p[(y+1)*w+(x-1)];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
            if(d[(y+1)*w+x] + d1 < d[y*w+x]){
                p[y*w+x] = p[(y+1)*w+x];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
            if(d[(y+1)*w+(x+1)] + d2 < d[y*w+x]){
                p[y*w+x] = p[(y+1)*w+(x+1)];
                d[y*w+x] = sqrt( float(pow(x-p[y*w+x].x(),2) + pow(y-p[y*w+x].y(),2)));
            }
        }

    for(int y=0; y<h; ++y)
        for(int x=0; x<w; ++x){
            if(d[y*w+x] > threshold) 
                d[y*w+x] = threshold;
            //d[y*w+x] = threshold- d[y*w+x];
            if(uchar(*(source+step*(y*w+x))) == 0)
                d[y*w+x] = -d[y*w+x];
        }

    for(int y=0; y<h; ++y)
        for(int x=0; x<w; ++x)
            m_sedt[y*w+x] = 0.5f * (d[y*w+x]/float(threshold)) + 0.5f;

/*    float newDist = 0.0;
    float actDist = 0.0;
    char actColor = 0;

    for(int iy=0;iy<h;iy++)
        for(int ix=0;ix<w;ix++){
            actColor = *(source+step*(iy*w+ix));
            actDist = sqrt(float(pow(threshold/2,2)+pow(threshold/2,2)));
            for(int jy=std::max(0,iy-threshold/2); jy<=std::min(h-1,iy+threshold/2); jy++)
                for(int jx=std::max(0,ix-threshold/2); jx<=std::min(w-1,ix+threshold/2); jx++)
                    if( uchar(*(source+step*(jy*w+jx))) != actColor){
                        newDist = sqrt(float(pow(ix-jx,2)+pow(iy-jy,2)));
                        if(newDist < actDist)
                            actDist = newDist;
                    }
            switch(uchar(*(source+step*(iy*w+ix))) ){
                case 0: 
                    m_sedt[iy*w+ix] = 0.5f * (float(actDist)/float(threshold/2));
                    //printf("%f\n", actDist);
                    break;
                case 255: 
                    m_sedt[iy*w+ix] = 0.5f + 0.5f * (float(actDist)/float(threshold/2));
                    break;
                default: 
                    m_sedt[iy*w+ix] = 0.5f;
            }
        }*/



    // ...

    // m_sedt << should contain all scaled distances ;)


    // Task_3_2 - ToDo End
}
 
