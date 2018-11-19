#ifndef SPDFILE_H
#define SPDFILE_H


#include <QtCore>



class spdFile
{
public:
    spdFile();
    ~spdFile();
    bool openFile(QString path);
    bool loadHexSpdFile(void);
    bool loadBinSpdFile(void);
    void closeSpdFile(void);
    bool saveToHex(QString path);
    bool saveToBin(QString path);
    bool resaveToHex();
    void resetBin();
    uchar *spdBin;
    QByteArray *spdArray;
    const int spdBinSize = 512;
    bool fileLoaded = false;
private:
    QFile file;
    QFile outFile;
    const int maxFileSize = 0x10000;

};

#endif // SPDFILE_H
