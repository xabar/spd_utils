#ifndef SPDCREATOR_H
#define SPDCREATOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include "spdfile.h"
#include "spd.h"

namespace Ui {
class spdCreator;
}

class spdCreator : public QMainWindow
{
    Q_OBJECT

private slots:
    void 			openFileClick();
    void 			saveAsFileClick();
    void 			saveFileClick();
    void 			newFileClick();
    void            exitClick();
    void            cellActivated(int row, int column);
    void            cellChanged(int row, int column);
    void            itemActivated(QListWidgetItem *item);
    void            updateSpd();

public:
    explicit spdCreator(QWidget *parent = 0);
    void log(QString str);
    ~spdCreator();

private:
    void resetSpdEditor(void);
    bool loadBinToEditor(void);
    void decodeBCS(int from);
    void decodeMSD(int from);
    void decodeMSB(int from);
    void decodeUNK(int from);
    void decodeDevType(void);
    bool inReserve(int a);
    void flushEditor(void);

    QStringList *vlabels;
    QStringList *hlabels;
    Ui::spdCreator *ui;
    spdFile *sFile;
    const int colCount = 8;
    const int rowCount = 64;


    bool cellChangedBrk = false;
    bool cellChangedOk = false;
    bool spdLoaded = false;
    bool isUDIMM = false;
    bool isRDIMM = false;
    bool isHybrid = false;
    bool unSaved = false;
    int crc16(uchar *ptr, int count);
    ushort calc_spd_crc_bcs(uchar *spd, uchar *crc_lsb, uchar *crc_msb);
    ushort calc_spd_crc_mss(uchar *spd, uchar *crc_lsb, uchar *crc_msb);
    void update_crc(uchar *spd);

    inline int getOffs(int col, int row)    {
        return col + row*colCount;
    }

    const QString wName = "SPD Creator";
};

#endif // SPDCREATOR_H
