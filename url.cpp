#include "url.h"

#define META_MAX 100

bool Url::load(QString input)
{
    metaId.clear();
    objID.clear();
   int  curt_pos = input.indexOf("Default.aspx?");
    url=input.replace(curt_pos,13,"command/imagepage.ashx?");
    url=url+QString("&width=%1&height=%2").arg(width).arg(height);
    return true;
}

//bool Url::getKey(QString input)
//{
////    int curt_pos,end_pos;
////    curt_pos = input.indexOf(METAID)+QString(METAID).length();
////    QString temp;
////    end_pos=curt_pos;
////    for (;end_pos<=curt_pos+META_MAX;end_pos++) {
////        temp=input.mid(end_pos,1);
////        if(temp == "&")
////            break;
////        if (end_pos == curt_pos+META_MAX) {
////            return false;
////        }
////    }
////    metaId=input.mid(curt_pos+1,end_pos-curt_pos-2);
////    objID=metaId;
//    return true;
//}

bool Url::setHeigth(Quality qu)
{
    switch (qu) {
    case LOW_QUALITY:
        width=595;
        height=742;
        ppi=72;
        break;
    case MED_QUALITY:
        width=1240;
        height=1754;
        ppi=120;
        break;
    case STD_QUALITY:
        width=1487;
        height=2105;
        ppi=150;
        break;
    case HIG_QUALITY:
        width=2479;
        height=3508;
        ppi=300;
        break;
    case UHI_QUALITY:
        width=3719;
        height=5262;
        ppi=450;
        break;
    default:
        width=1487;
        height=2105;
        ppi=150;
        break;
    }
    return true;
}
