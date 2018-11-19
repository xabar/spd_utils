#include "spdcreator.h"
#include "ui_spdcreator.h"
#include "spd.h"



QString infoMessage =   {
    "SPD_CREATOR - simple editor for SPD files in HEX format (CoreBoot-like)\n"
    "support UDIMM bytes and CRC calc\n\n"
    "For full and detailed bytes description use JEDEC DDR4 SDRAM Annex L\n\n\n"
    "written by Roman Perepelitsin (perepelitsin.roman@gmail.com)\n\nenjoy!"
};


QString bcsFieldNames[]	=	{
        "Number of Bytes Used",
        "SPD Revision",
        "DRAM device type",
        "Module type",
        "SDRAM Density and Banks ",
        "SDRAM Addressing",
        "Primary SDRAM Package Type ",
        "SDRAM Optional Features",
        "SDRAM Thermal and Refresh Options",
        "Other SDRAM Optional Features ",
        "Secondary SDRAM Package Type - like primary, except bit 3-2 ",
        "Module Nominal Voltage, VDD",
        "Module Organization ",
        "Module Memory Bus Width ",
        "Module Thermal Sensor",
        "Extended Module Type",
        "reserved",
        "Timebases",
        "SDRAM Minimum Cycle Time (tCKAVGmin) in MTB",
        "SDRAM Maximum Cycle Time (tCKAVGmax) in MTB",
        "CAS LAT, 1 byte ",
        "CAS LAT, 2 byte ",
        "CAS LAT, 3 byte",
        "CAS LAT, 4 byte",
        "Minimum CAS Latency Time (tAAmin) in MTB",
        "Minimum RAS to CAS Delay Time (tRCDmin) in MTB",
        "Minimum Row Precharge Delay Time (tRPmin) in MTB",
        "Upper Nibbles for tRASmin and tRCmin ",
        "Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte ",
        "Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte",
        "Minimum Refresh Recovery Delay Time (tRFC1min), LSB",
        "Minimum Refresh Recovery Delay Time (tRFC1min), MSB ",
        "Minimum Refresh Recovery Delay Time (tRFC2min), LSB ",
        "Minimum Refresh Recovery Delay Time (tRFC2min), MSB",
        "Minimum Refresh Recovery Delay Time (tRFC4min), LSB ",
        "Minimum Refresh Recovery Delay Time (tRFC4min), MSB ",
        "Upper Nibble for tFAW ",
        "Minimum Four Activate Window Delay Time (tFAWmin), Least Significant Byte ",
        "Minimum Activate to Activate Delay Time (tRRD_Smin), different bank group ",
        "Minimum Activate to Activate Delay Time (tRRD_Lmin), same bank group ",
        "Minimum CAS to CAS Delay Time (tCCD_Lmin), same bank group ",
        "Upper Nibble for tWRmin",
        "Minimum Write Recovery Time (t WRmin) ",
        "Upper Nibbles for tWTRmin",
        "Minimum Write to Read Time (tWTR_Smin), different bank group ",
        "Minimum Write to Read Time (tWTR_Lmin), same bank group",
        "reserved","reserved","reserved",
        "reserved","reserved","reserved",
        "reserved","reserved","reserved",
        "reserved","reserved","reserved",
        "reserved","reserved",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "Connector to SDRAM Bit Mapping",
        "reserved","reserved","reserved","reserved","reserved",
        "reserved","reserved","reserved","reserved","reserved",
        "reserved","reserved","reserved","reserved","reserved",
        "reserved","reserved","reserved","reserved","reserved",
        "reserved","reserved","reserved","reserved","reserved",
        "reserved","reserved","reserved","reserved","reserved",
        "reserved","reserved","reserved","reserved","reserved",
        "reserved","reserved","reserved","reserved",
        "Fine Offset for Minimum CAS to CAS Delay Time (tCCD_Lmin), same bank group",
        "Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Lmin), same bank group",
        "Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Smin), different bank group",
        "Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin) ",
        "Fine Offset for Minimum Row Precharge Delay Time (tRPmin) ",
        "Fine Offset for Minimum RAS to CAS Delay Time (tRCDmin) ",
        "Fine Offset for Minimum CAS Latency Time (tAAmin) ",
        "Fine Offset for SDRAM Maximum Cycle Time (tCKAVGmax)",
        "Fine Offset for SDRAM Minimum Cycle Time (tCKAVGmin)",
        "Cyclical Redundancy Code (CRC) for Base Configuration Section, LSB ",
        "Cyclical Redundancy Code (CRC) for Base Configuration Section, MSB",
};

#define MSB_UDIMM_RESERVED_FROM	0x84
#define MSB_UDIMM_RESERVED_TILL 0xFD

QString msbUdimmField[]	=	{
        "Raw Card Extension, Module Nominal Height",
        "Module Maximum Thickness",
        "Reference Raw Card Used",
        "Address Mapping from Edge Connector to DRAM",
        "reserved",
        "Cyclical Redundancy Code (CRC) for Module Specific Section, LSB",
        "Cyclical Redundancy Code (CRC) for Module Specific Section, MSB",
};

QString msdField[]	=	{
        "Module Manufacturer ID Code, LSB",
        "Module Manufacturer ID Code, MSB",
        "Module Manufacturing Location",
        "Module Manufacturing Date0",
        "Module Manufacturing Date1",
        "Module Serial Number0",
        "Module Serial Number1",
        "Module Serial Number2",
        "Module Serial Number3",
        "Module Part Number",
        "",	"","","","","","","","",
        "",	"","","","","","","","","",
        "Module Revision Code",
        "DRAM Manufacturer ID Code, LSB",
        "DRAM Manufacturer ID Code, MSB",
        "DRAM Stepping",
        "Manufacturer Specific Data",
};

const QString moduleTypes[]	=	{
     "EXT.DIMM",
     "RDIMM",
     "UDIMM",
     "SO-DIMM",
     "LRDIMM",
     "Mini-RDIMM",
     "Mini-UDIMM",
     "BROKEN!",
     "72b-SO-RDIMM",
     "72b-SO-UDIMM",
     "BROKEN!",
     "BROKEN!",
     "16b-SO-DIMM",
     "32b-SO-DIMM",
     "BROKEN!",
     "BROKEN!",
 };

 const QString dramTypes[] = {
     "BROKEN!",
     "Fast Page Mode",
     "EDO",
     "Pipeline Nibble",
     "SDRAM",
     "ROM",
     "DDR SGRAM",
     "DDR SDRAM",
     "DDR2 SDRAM",
     "DDR2 SDRAM FB-DIMM",
     "DDR2 SDRAM FB-DIMM PROBE",
     "DDR3 SDRAM",
     "DDR4 SDRAM",
     "BROKEN!",
     "DDR4E SDRAM",
     "LPDDR3 SDRAM",
     "LPDDR4 SDRAM",
     "BROKEN!",
 };

 const int reserved_bcs[][2]  =    {
     {0x10, 0x10},
     {0x2E, 0x3B},
     {0x4E, 0x74},
     {0x0, 0x0},
 };


 const int reserved_udimm[][2]   =   {
    {0x84, 0xBF},
    {0x17E, 0x17F},
    {0x0, 0x0},
 };


const int only_hybrid[][2]  =   {
    {0xC0, 0xFD},
    {0x100, 0x13F},
    {0x0, 0x0},
};


 bool spdCreator::inReserve(int a)
 {
     int i;
     for(i = 0; reserved_bcs[i][0] != 0;i++)    {
        if(a >= reserved_bcs[i][0] && a <= reserved_bcs[i][1])
            return true;
     }


     if(isUDIMM)    {
         for(i = 0; reserved_udimm[i][0] != 0 ;i++)   {
              if(a >= reserved_udimm[i][0] && a <= reserved_udimm[i][1])
                  return true;
         }
     }

     if(!isHybrid)  {
         for(i = 0; only_hybrid[i][0] != 0; i++)    {
             if(a >= only_hybrid[i][0] && a <= only_hybrid[i][1])
                 return true;
         }
     }

     return false;
 }

spdCreator::spdCreator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::spdCreator)
{

    ui->setupUi(this);
    ui->updateSPD->setEnabled(false);
    ui->fieldInfo->append(infoMessage);
    resetSpdEditor();



    connect(ui->actionOpen,SIGNAL(triggered()), this, SLOT(openFileClick()));
    connect(ui->actionExit,SIGNAL(triggered()), this, SLOT(exitClick()));
    connect(ui->actionSave_as,SIGNAL(triggered()), this, SLOT(saveAsFileClick()));
    connect(ui->actionSave,SIGNAL(triggered()), this, SLOT(saveFileClick()));
    connect(ui->actionNew,SIGNAL(triggered()), this, SLOT(newFileClick()));

    sFile = new(spdFile);
}

void spdCreator::resetSpdEditor()
{
    char str[7];
    QStringList horzHeaders;

    ui->spdViewer->setRowCount(rowCount);
    ui->spdViewer->setColumnCount(colCount);
    for(int i = 0; i < colCount; i++) {
        ui->spdViewer->setColumnWidth(i,45);

    }

    for(int i = 0; i < rowCount; i++) {
        ui->spdViewer->setRowHeight(i,18);
        sprintf(str,"0x%03X",i*colCount);
        horzHeaders << str;
    }

    ui->spdViewer->setVerticalHeaderLabels(horzHeaders);

    ui->spdViewer->horizontalHeader()->setEnabled(false);
    ui->spdViewer->verticalHeader()->setEnabled(false);
    ui->spdViewer->setEnabled(false);

    this->setWindowTitle(wName);

}

bool spdCreator::loadBinToEditor()
{
    int i = 0, spdUsedBytes = SPD_CRC_MSS_MSB;
    int row = 0, col = 0;
    QString str;


    if((sFile->spdBin == NULL) || (sFile->spdBinSize > ui->spdViewer->rowCount() * ui->spdViewer->columnCount()))    {
        log("Error at loading SPD");
        return false;
    }

    spdUsedBytes = SPD_BYTES_USED_INIT_VAL * (sFile->spdBin[SPD_BYTES] & SPD_BYTES_USED_MASK) - 1;
    if(spdUsedBytes <= 0)
        spdUsedBytes = sFile->spdBinSize;
    log("Spd bytes used " + QString::number(spdUsedBytes));
    decodeDevType();

    ui->listWidget->clear();

    do{
        ui->spdViewer->setItem(row, col, new(QTableWidgetItem));
        ui->spdViewer->item(row,col)->setText(str.sprintf("0x%02X",sFile->spdBin[i]));

        if((i == SPD_CRC_BCS_LSB) || (i == SPD_CRC_BCS_MSB) || (i == SPD_CRC_MSS_LSB) || (i == SPD_CRC_MSS_MSB))    {
            ui->spdViewer->item(row,col)->setBackground(Qt::black);
            ui->spdViewer->item(row,col)->setTextColor(Qt::white);
            ui->spdViewer->item(row,col)->setFlags(Qt::ItemIsEnabled);
        } else  if(i > spdUsedBytes || inReserve(i))  {
            ui->spdViewer->item(row,col)->setBackground(Qt::lightGray);
            ui->spdViewer->item(row,col)->setFlags(Qt::ItemIsEnabled);
        } else  {
            if(i < (SPD_CRC_BCS_MSB)) {
                if(bcsFieldNames[i] != "reserved")    {
                    ui->listWidget->addItem(bcsFieldNames[i]);
                    ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, i);
                }
            } else if((i >= SPD_MSB_START_OFFS) && (i < SPD_CRC_MSS_LSB))   {
                if(i < MSB_UDIMM_RESERVED_FROM) {
                     ui->listWidget->addItem(msbUdimmField[i - SPD_MSB_START_OFFS]);
                     ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, i);
                }
            } else if((i >= SPD_MSD_START_OFFS) && (i < SDP_MSD_END))  {
                if(i <= SPD_MM_SPECIFIC_START + SPD_MM_SPECIFIC_LEN) {
                    if((i >= SPD_M_PNUM_START) && (i < SPD_M_PNUM_START + SPD_M_PNUM_LEN))  {
                        ui->listWidget->addItem(msdField[SPD_M_PNUM_START - SPD_MSD_START_OFFS] + QString::number(i - SPD_M_PNUM_START));
                        ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, i);
                    } else if(i < SPD_MM_SPECIFIC_START) {
                        ui->listWidget->addItem(msdField[i - SPD_MSD_START_OFFS]);
                        ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, i);
                    } else if((i >= SPD_MM_SPECIFIC_START) && (i <= SPD_MM_SPECIFIC_START + SPD_MM_SPECIFIC_LEN))   {
                        ui->listWidget->addItem(msdField[SPD_MM_SPECIFIC_START - SPD_MSD_START_OFFS] + QString::number(i - SPD_MM_SPECIFIC_START));
                        ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, i);
                    }

                }
            }
        }

        ui->spdViewer->item(row,col)->setToolTip(str.sprintf("%d", getOffs(col, row)));

        col++;
        if(col >= ui->spdViewer->columnCount()) {
            col = 0;
            row++;
        }
        i++;
    }while(i < sFile->spdBinSize);



    connect(ui->spdViewer, SIGNAL(cellActivated(int, int)), this, SLOT(cellActivated(int, int)));
    connect(ui->spdViewer, SIGNAL(cellChanged(int,int)), this, SLOT(cellChanged(int, int)));
    connect(ui->listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemActivated(QListWidgetItem*)));
    connect(ui->updateSPD, SIGNAL(clicked(bool)), this, SLOT(updateSpd()));


    ui->spdViewer->blockSignals(false);
    ui->updateSPD->setEnabled(true);

    spdLoaded = true;

    return true;
}

void spdCreator::flushEditor()
{
    ui->spdViewer->blockSignals(true);
}

void spdCreator::updateSpd()
{
    flushEditor();
    update_crc(sFile->spdBin);
    loadBinToEditor();
    log("Update SPD done.");
}



void spdCreator::itemActivated(QListWidgetItem *item)
{
    int offs = item->data(Qt::UserRole).toInt();
    int row, col;

    row = offs / colCount;
    col = offs % colCount;

    ui->spdViewer->setCurrentItem(ui->spdViewer->item(row,col));
    cellActivated(row,col);

}

void spdCreator::cellActivated(int row, int column)
{
    int offs = getOffs(column, row);

    if( offs <= SPD_CRC_BCS_MSB)    {
        decodeBCS(offs);

        return;
    }

    if((offs >= SPD_MSB_START_OFFS) && (offs <= SPD_CRC_MSS_MSB))   {
        decodeMSB(offs);

        return;
    }

    if((offs >= SPD_MSD_START_OFFS) && (offs <= SDP_MSD_END))   {
        decodeMSD(offs);

        return;
    }

    decodeUNK(offs);

}


void spdCreator::cellChanged(int row, int column)
{
    bool _ok;
    int offs = getOffs(column, row);
    QString str;

    if(cellChangedBrk)  {
        cellChangedBrk = false;
        return;
    }

    int newVal = ui->spdViewer->item(row,column)->text().toInt(&_ok, 16);

    if(_ok & (newVal <= 0xFF) & !cellChangedBrk) {
        ui->spdViewer->item(row,column)->setTextColor(Qt::red);
        sFile->spdBin[offs] = (uchar)(newVal & 0xff);
        ui->spdViewer->item(row,column)->setText(str.sprintf("0x%02X",sFile->spdBin[offs]));

        if(!unSaved)    {
            unSaved = true;
            this->setWindowTitle(this->windowTitle() + "*");
        }
    }
    else
    {
        log("Invalid value for byte "+QString::number(offs));
        cellChangedBrk = true;
        ui->spdViewer->item(row,column)->setText(str.sprintf("0x%02X",sFile->spdBin[offs]));
    }
}


void spdCreator::log(QString str)
{
    ui->textBrowser->append(str);
}


void spdCreator::saveFileClick()
{
    if(!spdLoaded)  {
        log("Nothing to save...");
        return;
    }

    updateSpd();

    if(!sFile->fileLoaded)  {
        saveAsFileClick();
        return;
    } else  {
        unSaved = !sFile->resaveToHex();
    }

    if(!unSaved)    {
        this->setWindowTitle(this->windowTitle().left(this->windowTitle().lastIndexOf("*")));
    }
}


void spdCreator::saveAsFileClick()
{
    if(!spdLoaded)  {
        log("Nothing to save...");
        return;
    }

    updateSpd();

    QString selfilter = tr("HEX format (*.hex)");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save SPD file"),
                                                     "./",
                                                    tr("HEX format (*.hex)"),
                                                    &selfilter);

    if(selfilter == tr("HEX format (*.hex)"))   {
        if(!fileName.contains(".hex", Qt::CaseInsensitive)) {
            fileName = fileName + ".hex";
        }

        unSaved = !sFile->saveToHex(fileName);
        if(fileName.length() > 40 ) {
            fileName = "~" + fileName.right(fileName.length() - fileName.lastIndexOf("/", -1));
        }
        this->setWindowTitle(wName + " - " + fileName);
    } else  {
        fileName = fileName + ".bin";
    }
}




void spdCreator::newFileClick()
{
    flushEditor();
    resetSpdEditor();
    sFile->resetBin();
    ui->spdViewer->setEnabled(true);
    if(!loadBinToEditor())  {
        log("invalid file");
        resetSpdEditor();
    }
    this->setWindowTitle(wName + " - unsaved");
}

void spdCreator::openFileClick()
{
    QString selfilter = tr("HEX format (*.hex)");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open SPD file"),
                                                     "./",
                                                    tr("HEX format (*.hex);;BIN format (*.bin)"),
                                                    &selfilter);
    if(sFile->openFile(fileName))   {
        log("Load file: " + fileName);
    }



    if(selfilter == tr("HEX format (*.hex)"))   {
        if(sFile->loadHexSpdFile())
            log("Load hex done");
        else    {
            log("Load hex fail");
            return;
        }
    }
    flushEditor();
    resetSpdEditor();
    ui->spdViewer->setEnabled(true);
    if(!loadBinToEditor())  {
        log("invalid file");
        resetSpdEditor();
    }

    if(fileName.length() > 40 ) {
        fileName = "~" + fileName.right(fileName.length() - fileName.lastIndexOf("/", -1));
    }

    this->setWindowTitle(wName + " - " + fileName);

}

void spdCreator::decodeDevType(void)
{
    uchar dramType = sFile->spdBin[SPD_DEVTYPE];
    if(dramType > SPD_DEVTYPE_MAX)  {
        dramType = 0;
    }

    log("DRAM Device type - " + dramTypes[dramType]);


    dramType = sFile->spdBin[SPD_MODULE_TYPE];
    log("Module type -  "+ moduleTypes[dramType & SPD_MODULE_TYPE_MASK]);
    if(dramType & SPD_MODULE_TYPE_HYBRID_NVDIMM)    {
        isHybrid = true;
    }

    dramType = dramType & SPD_MODULE_TYPE_MASK;
    if((dramType == SPD_MODULE_TYPE_UDIMM) ||
        (dramType == SPD_MODULE_TYPE_SODIMM) ||
        (dramType == SPD_MODULE_TYPE_MINI_UDIMM) ||
        (dramType == SPD_MODULE_TYPE_72SO_UDIMM) ||
        (dramType == SPD_MODULE_TYPE_16B_SODIMM) ||
        (dramType == SPD_MODULE_TYPE_32B_SODIMM)){
        isUDIMM = true;
    }

    if((dramType == SPD_MODULE_TYPE_RDIMM) ||
        (dramType == SPD_MODULE_TYPE_LRDIMM) ||
        (dramType == SPD_MODULE_TYPE_MINI_RDIMM) ||
        (dramType == SPD_MODULE_TYPE_72SO_RDIMM)){
        isRDIMM = true;
    }

    if(isRDIMM & isUDIMM)   {
        log("Dirty device type - SPD corrupted or usupported version (or bullshit)");
        isRDIMM = isUDIMM = false;
    }


}

void spdCreator::decodeBCS(int from)
{
    QString str;

    ui->fieldInfo->clear();

    str.sprintf("Byte 0x%02X (%d)\n", from, from);
    ui->fieldInfo->append(str);
    ui->fieldInfo->append(" - Base Configuration Section - \n");

    if(from >= sizeof(bcsFieldNames)/sizeof(QString))   {
        ui->fieldInfo->append("Unknown field...");
    } else  {
        ui->fieldInfo->append(bcsFieldNames[from]);
    }
}


void spdCreator::decodeMSD(int from)
{
    QString str;

    ui->fieldInfo->clear();

    str.sprintf("Byte 0x%02X (%d)\n", from, from);
    ui->fieldInfo->append(str);
    ui->fieldInfo->append(" - Module Supplier Data- \n");


    if((from >= SPD_M_PNUM_START) && (from < SPD_M_PNUM_START + SPD_M_PNUM_LEN))  {
        ui->fieldInfo->append(msdField[SPD_M_PNUM_START - SPD_MSD_START_OFFS] + QString::number(from - SPD_M_PNUM_START) );
    } else if(from < SPD_MM_SPECIFIC_START) {
        ui->fieldInfo->append(msdField[from - SPD_MSD_START_OFFS]);
    }
    else if((from >= SPD_MM_SPECIFIC_START) && (from <= SPD_MM_SPECIFIC_START + SPD_MM_SPECIFIC_LEN))   {
       ui->fieldInfo->append(msdField[SPD_MM_SPECIFIC_START - SPD_MSD_START_OFFS] + QString::number(from - SPD_MM_SPECIFIC_START));

   } else {
        ui->fieldInfo->append("reserved");
   }
}


void spdCreator::decodeMSB(int from)
{
    QString str;

    ui->fieldInfo->clear();

    str.sprintf("Byte 0x%02X (%d)\n", from, from);
    ui->fieldInfo->append(str);
    if(isUDIMM) {
        ui->fieldInfo->append(" - Module specific bytes for unbuffered memory - \n");

        if(from < MSB_UDIMM_RESERVED_FROM)  {
            ui->fieldInfo->append(msbUdimmField[from - SPD_MSB_START_OFFS]);
        }else  if((from >= MSB_UDIMM_RESERVED_FROM) && (from <= MSB_UDIMM_RESERVED_TILL))   {
            ui->fieldInfo->append("reserved");
        } else if(from > MSB_UDIMM_RESERVED_TILL)   {
            ui->fieldInfo->append(msbUdimmField[from - SPD_MSB_START_OFFS - (MSB_UDIMM_RESERVED_TILL - MSB_UDIMM_RESERVED_FROM)]);
        }
    }
}

void spdCreator::decodeUNK(int from)
{
    QString str;

    ui->fieldInfo->clear();

    str.sprintf("Byte 0x%02X (%d)\n", from, from);
    ui->fieldInfo->append(str);
    ui->fieldInfo->append(" - reseved or unsupported - \n");
}


void spdCreator::exitClick()
{
    exit(0);
}

spdCreator::~spdCreator()
{
    delete ui;
}


int spdCreator::crc16(uchar *ptr, int count)
{
    int crc, i;

    crc = 0;
    while (--count >= 0) {
        crc = crc ^ (int)*ptr++ << 8;
        for (i = 0; i < 8; ++i)
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
    }

    return (crc & 0xFFFF);
}


ushort spdCreator::calc_spd_crc_bcs(uchar *spd, uchar *crc_lsb, uchar *crc_msb)
{
    ushort ret = (ushort)crc16(spd, 126);

    *crc_lsb = ret & 0xff;
    *crc_msb = ret >> 8;

    return ret;
}

ushort spdCreator::calc_spd_crc_mss(uchar *spd, uchar *crc_lsb, uchar *crc_msb)
{
    ushort ret = (ushort)crc16(&spd[128], 126);

    *crc_lsb = ret & 0xff;
    *crc_msb = ret >> 8;

    return ret;
}

void spdCreator::update_crc(uchar *spd)
{
    uchar crc_msb, crc_lsb;

    calc_spd_crc_bcs(spd, &crc_lsb, &crc_msb);
    if((spd[SPD_CRC_BCS_LSB] == crc_lsb) && (spd[SPD_CRC_BCS_MSB] == crc_msb))	{

    } else	{
        spd[SPD_CRC_BCS_LSB] = crc_lsb;
        spd[SPD_CRC_BCS_MSB] = crc_msb;
    }

    calc_spd_crc_mss(spd, &crc_lsb, &crc_msb);

    if((spd[SPD_CRC_MSS_LSB] == crc_lsb) && (spd[SPD_CRC_MSS_MSB] == crc_msb))	{

    } else	{
        spd[SPD_CRC_MSS_LSB] = crc_lsb;
        spd[SPD_CRC_MSS_MSB] = crc_msb;
    }
}
