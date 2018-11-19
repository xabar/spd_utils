#include "spdfile.h"

spdFile::spdFile()
{
      spdBin = new uchar[spdBinSize];
      qMemSet(spdBin, 0x0, spdBinSize);
}

spdFile::~spdFile()
{

}


bool spdFile::openFile(QString path)
{
    if(file.isOpen())   {
        qWarning("File alredy open - close");
        file.close();
    }

    file.setFileName(path);
    if(file.open(QIODevice::ReadOnly))
        if(file.size() > maxFileSize)   {
            qWarning("File too large!");
            file.close();
            return false;
        }
    fileLoaded = true;
    return true;
}



bool spdFile::loadHexSpdFile()
{
    int proc = 0, out_pos = 0;
    uchar comment = 0, elem, half = 0;
    bool ret = true;


    if(!file.isOpen())   {
        qWarning("loadHexSpdFile - no open file!");
        return false;
    }

    uchar *mem = file.map(0, file.size());

    if(mem == NULL) {
        qWarning("Error at map file to memory");
        return false;
    }

    do{

        if(comment){
            if(mem[proc] == 0x0a)	{
                comment = 0;
            }
            proc++;
            continue;
        }

        if( mem[proc] == '#'){
            comment = 1;
            proc++;
            continue;
        }

        if(mem[proc] >= 0x30)	{
            if(isxdigit(elem = toupper(mem[proc])))	{
                elem = elem - 0x30;
                if(elem > 9)
                    elem = elem - 7;
            }
            else	{
                qWarning("Error in SPD HEX file format! elem %d\n", elem);
                ret = false;
                break;
            }

            if(half == 0)	{
                spdBin[out_pos] = ( elem << 4 ) & 0xf0;
                half = 1;
            } else	{
                spdBin[out_pos] = spdBin[out_pos] | elem;
                half = 0;
                out_pos++;
            }
            proc++;
            continue;
        }

        proc++;

    }while(proc < file.size() );

    file.unmap(mem);

    return ret;
}


bool spdFile::loadBinSpdFile()
{

    return false;
}


void spdFile::closeSpdFile()
{
    if(file.isOpen())   {
        file.close();
    }

}

void spdFile::resetBin()
{
    spdBin = new uchar[spdBinSize];
    qMemSet(spdBin, 0x0, spdBinSize);
    fileLoaded = false;
}

bool spdFile::resaveToHex()
{
    outFile.setFileName(file.fileName());
    return saveToHex(outFile.fileName());
}


bool spdFile::saveToHex(QString path)
{
    if(outFile.isOpen())   {
        qWarning("File alredy open - close");
        outFile.close();
    }

    outFile.setFileName(path);

    if(outFile.open(QIODevice::ReadWrite))  {
        if(outFile.size() > 0)   {
            outFile.remove();
            outFile.close();
            if(!outFile.open(QIODevice::ReadWrite)) {
                return false;
            }
        }

        QString str;
        str = "# This SPD was created in spdCreator \n";

        outFile.write(str.toLocal8Bit());

        for(int i = 0; i < this->spdBinSize; i++)  {
            if((i & 0x0F) == 0x0F)  {
                str.sprintf("%02X \n", this->spdBin[i]);
            } else
                str.sprintf("%02X ", this->spdBin[i]);

            outFile.write(str.toLocal8Bit());
        }

        file.setFileName(outFile.fileName());
        fileLoaded = true;
        outFile.close();
        return true;
    }

    return false;

}



bool spdFile::saveToBin(QString path)
{

    return false;
}
