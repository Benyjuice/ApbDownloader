#ifndef URL_H
#define URL_H

#include "QString"

#define METAID      "metaid"
#define OBJID          "objid"

//#define LOW_QUALITY    0
//#define MED_QUALITY    1
//#define STD_QUALITY     2
//#define HIG_QUALITY     3
//#define UHI_QUALITY     4

enum Quality {
    LOW_QUALITY,
    MED_QUALITY,
     STD_QUALITY,
     HIG_QUALITY,
     UHI_QUALITY,
};

struct pageSize {
    int width;
    int height;
};


namespace Ui {
class Url;
}

class Url {

public:
    QString getUrl() {
        return url;
    }

    QString getMetaId() {
        return metaId;
    }

    int getPPI() {
        return ppi;
    }

    bool setHeigth(Quality qu);

    bool load(QString);
private:
    QString url;
    QString metaId;
    QString objID;
    int width;
    int height;
    int ppi;
};

#endif // URL_H
