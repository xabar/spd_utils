/*
 * spd.h
 * rel.25
 *
 *  Created on: 07.11.2018
 *      Author: Perepelitsin Roman, NKBVS
 */
#ifndef SDP_H_
#define SDP_H_

#include "stdint.h"

/****************************
 * Base configuration section
 ****************************/

/* 0x0  Number of Bytes Used / */
#define SPD_BYTES	0x0
#define SPD_BYTES_TOTAL_256	(1 << 4)
#define SPD_BYTES_TOTAL_512	(1 << 5)
#define SPD_BYTES_TOTAL_MASK 0x70

#define SPD_BYTES_USED_INIT_VAL	128
#define SPD_BYTES_USED_128	(1 << 0)
#define SPD_BYTES_USED_256	(1 << 1)
#define SPD_BYTES_USED_384	(1 << 2)
#define SPD_BYTES_USED_512	(1 << 3)
#define SPD_BYTES_USED_MASK	0xF


/* 0x1 SPD Revision*/
#define SPD_REVISION	0x1


/* 0x2 DRAM device type */
#define SPD_DEVTYPE		0x2
#define SPD_DEVTYPE_MAX		0x10
#define SPD_DEVTYPE_UDIMM_KEY	0xC

/* 0x3 Module type */
#define SPD_MODULE_TYPE		0x3
#define SPD_MODULE_TYPE_HYBRID	(1 << 7)
#define SPD_MODULE_TYPE_HYBRID_NVDIMM	(1 << 4)

#define SPD_MODULE_TYPE_MASK 0x0F
#define SPD_MODULE_TYPE_EDIMM 	0x0
#define SPD_MODULE_TYPE_RDIMM	0x1
#define SPD_MODULE_TYPE_UDIMM	0x2
#define SPD_MODULE_TYPE_SODIMM	0x3
#define SPD_MODULE_TYPE_LRDIMM	0x4
#define SPD_MODULE_TYPE_MINI_RDIMM		0x5
#define SPD_MODULE_TYPE_MINI_UDIMM		0x6
#define SPD_MODULE_TYPE_72SO_RDIMM		0x8
#define SPD_MODULE_TYPE_72SO_UDIMM		0x9
#define SPD_MODULE_TYPE_16B_SODIMM		0xC
#define SPD_MODULE_TYPE_32B_SODIMM		0xD
#define SPD_MODULE_TYPE_UDIMM_KEY_BYTE (1<<1)

/* 0x4  SDRAM Density and Banks */
#define SPD_DENS_BANKS	0x4
#define SPD_DENS_BANKS_ADDR_BITS_MASK	0x30
#define SPD_DENS_BANKS_ADDR_BITS_4BANKS		0x0
#define SPD_DENS_BANKS_ADDR_BITS_8BANKS		(1 << 4)

#define SPD_DENS_BANKS_GROUP_MASK		0xC0
#define SPD_DENS_BANKS_GROUP_NO_GPOUP	0x0
#define SPD_DENS_BANKS_GROUP_2_GROUP	(1 << 6)
#define SPD_DENS_BANKS_GROUP_4_GROUP	(1 << 7)

#define SPD_DENS_BANKS_CAP_PER_DIE_MASK	0x0F
#define SPD_DENS_BANKS_CAP_PER_DIE_INIT_VAL	256


/* 0x5 SDRAM Addressing */
#define SPD_SDRAM_ADDR			0x5
#define SPD_SDRAM_ADDR_ROW_OFFS		0x3
#define SPD_SDRAM_ADDR_ROW_INIT_VAL	12
#define SPD_SDRAM_ADDR_ROW_MASK		0x38

#define SPD_SDRAM_ADDR_COL_OFFS			0x0
#define SPD_SDRAM_ADDR_COL_INIT_VAL		9
#define SPD_SDRAM_ADDR_COL_MASK			0x7


/* 0x6 Primary SDRAM Package Type */
#define SPD_PACK_TYPE		0x6

#define SPD_PACK_TYPE_SIGNAL_MASK	0x3
#define SPD_PACK_TYPE_MULTI_STACK	(1 << 0)
#define SPD_PACK_TYPE_SINGLE_STACK	(1 << 1)
#define SPD_PACK_TYPE_NON_MONOLITH	(1 << 7)

#define SPD_PACK_TYPE_DIE_CNT_OFFS	0x4
#define SPD_PACK_TYPE_DIE_CNT_MASK	0x70
#define SPD_PACK_TYPE_DIE_CNT_INIT_VAL	1


/* 0x7  SDRAM Optional Features */
#define SPD_OPT_FEATURES 	0x7
#define SPD_OPT_FEATURES_TMAW_OFFSET	0x4
#define SPD_OPT_FEATURES_TMAW_MASK	(3 << 4)
#define SPD_OPT_FEATURES_TMAW_INIT_VAL	8192
#define SPD_OPT_FEATURES_TMAW_8192	0
#define SPD_OPT_FEATURES_TMAW_4096	1
#define SPD_OPT_FEATURES_TMAW_2048	2

#define SPD_OPT_FEATURES_MAC_MASK 0x0F



/* 0x8 SDRAM Thermal and Refresh Options */
#define SPD_THERM_OPT	0x8


/* 0x9 Other SDRAM Optional Features */
#define SPD_OTHER_SDRAM_OPT 	0x9
#define SPD_OTHER_SDRAM_OPT_PPR_SUPP	(1 << 6)
#define SPD_OTHER_SDRAM_OPT_SOFT_PPR_SUP	(1 << 5)


/* 0xA  Secondary SDRAM Package Type - like primary, except bit 3-2 */
#define SPD_PACK_TYPE_SEC	0xA
#define SPD_PACK_TYPE_SEC_RATIO_MASK 0x0C
#define SPD_PACK_TYPE_SEC_RATIO_OFFSET	0x2
#define SPD_PACK_TYPE_SEC_RATIO_1_3_ONE	0
#define SPD_PACK_TYPE_SEC_RATIO_1_3_TWO	1



/* 0xB : Module Nominal Voltage, VDD */
#define SPD_VDD 	0xB
#define SPD_VDD_1_2V_OPERABLE	(1 << 0)
#define SPD_VDD_1_2V_ENDURANT	(1 << 1)


/* 0xC  Module Organization  */
#define SPD_MODULE_ORG		0xC
#define SPD_MODULE_ORG_DEV_WIDTH_MASK	0x7
#define SPD_MODULE_ORG_DEV_WIDTH_4BIT	0
#define SPD_MODULE_ORG_DEV_WIDTH_8BIT	1
#define SPD_MODULE_ORG_DEV_WIDTH_16BIT	2
#define SPD_MODULE_ORG_DEV_WIDTH_32BIT	3

#define SPD_MODULE_ORG_PACK_RANK_PER_DIMM_OFFS 		0x3
#define SPD_MODULE_ORG_PACK_RANK_PER_DIMM_INIT_VAL	1

#define SPD_MODULE_ORG_RANK_MIX_ASSYM	(1 << 6)


/* 0xD Module Memory Bus Width */
#define SPD_MODULE_MEM_BUS_WIDTH	0xD

#define SPD_MODULE_MEM_BUS_WIDTH_PRIM_MASK 0x7
#define SPD_MODULE_MEM_BUS_WIDTH_PRIM_INIT_VAL 8
#define SPD_MODULE_MEM_BUS_WIDTH_PRIM_8BIT		0
#define SPD_MODULE_MEM_BUS_WIDTH_PRIM_16BIT		1
#define SPD_MODULE_MEM_BUS_WIDTH_PRIM_32BIT		2
#define SPD_MODULE_MEM_BUS_WIDTH_PRIM_64BIT		3

#define SPD_MODULE_MEM_BUS_WIDTH_EXT_8BIT	(1 << 3)



/* 0xE  Module Thermal Sensor */
#define SPD_MODULE_THERM_SENS 0xE
#define SPD_MODULE_THERM_SENS_INCORP (1 << 7)

/* 0x11 Timebases */
#define SPD_TIMEBASES		0x11
#define SPD_TIMEBASES_VAL	0x0


/* 0x12  SDRAM Minimum Cycle Time (tCKAVGmin) in MTB */
#define SPD_TCKAVG_MIN	0x12

/* 0x13 SDRAM Maximum Cycle Time (tCKAVGmax) in MTB */
#define SPD_TCKAVG_MAX	0x13


/* 0x14 CAS LAT, 1 byte */
/* 0x15 CAS LAT, 2 byte */
/* 0x16 CAS LAT, 3 byte */
/* 0x17 CAS LAT, 4 byte */
#define SPD_CASLAT_1	0x14
#define SPD_CASLAT_2	0x15
#define SPD_CASLAT_3	0x16
#define SPD_CASLAT_4	0x17


/* 0x18  Minimum CAS Latency Time (tAAmin) in MTB */
#define SPD_TAA_MIN		0x18

/* 0x19  Minimum RAS to CAS Delay Time (tRCDmin) in MTB */
#define SPD_TRCD_MIN	0x19

/* 0x1A  Minimum Row Precharge Delay Time (tRPmin) in MTB */
#define SPD_TRP_MIN 0x1A

/* 0x1B  Upper Nibbles for tRASmin and tRCmin */
#define SPD_UP_NIBBLES	0x1B
#define SPD_UP_NIBBLES_TRC_MIN_OFFS		4
#define SPD_UP_NIBBLES_TRC_MIN_MASK		0xF0
#define SPD_UP_NIBBLES_TRAS_MIN_OFFS		0
#define SPD_UP_NIBBLES_TRAS_MIN_MASK		0x0F


/* 0x1C Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte */
#define SPD_TRAS_MIN	0x1C

/* 0x1D  Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte */
#define SPD_TRC_MIN		0x1D


/* 0x1E  Minimum Refresh Recovery Delay Time (tRFC1min), LSB */
#define SPD_TRFC1_MIN_LSB	0x1E
/* 0x1F  Minimum Refresh Recovery Delay Time (tRFC1min), MSB */
#define SPD_TRFC1_MIN_MSB	0x1F
/* 0x20  Minimum Refresh Recovery Delay Time (tRFC2min), LSB */
#define SPD_TRFC2_MIN_LSB	0x20
/* 0x21  Minimum Refresh Recovery Delay Time (tRFC2min), MSB */
#define SPD_TRFC2_MIN_MSB	0x21
/* 0x22  Minimum Refresh Recovery Delay Time (tRFC4min), LSB */
#define SPD_TRFC4_MIN_LSB	0x22
/* 0x23  Minimum Refresh Recovery Delay Time (tRFC4min), MSB */
#define SPD_TRFC4_MIN_MSB	0x23

/* 0x24  Upper Nibble for tFAW */
#define SPD_TFAW_NIBBLE		0x24
#define SPD_TFAW_NIBBLE_MASK	0x0F

/* 0x25  Minimum Four Activate Window Delay Time (tFAWmin), Least Significant Byte */
#define SPD_TFAW_MIN	0x25

/* 0x26  Minimum Activate to Activate Delay Time (tRRD_Smin), different bank group */
#define SPD_TRRD_S_MIN		0x26


/* 0x27  Minimum Activate to Activate Delay Time (tRRD_Lmin), same bank group */
#define SPD_TRRD_L_MIN		0x27

/* 0x28 Minimum CAS to CAS Delay Time (tCCD_Lmin), same bank group */
#define SPD_TCCD_L_MIN	0x28

/* 0x29  Upper Nibble for tWRmin */
#define SPD_UP_NIBBL_TWR_MIN	0x29
#define SPD_UP_NIBBL_TWR_MIN_MASK 0x0f

/* 0x2A  Minimum Write Recovery Time (t WRmin) */
#define SPD_TWR_MIN		0x2A

/* 0x2B  Upper Nibbles for tWTRmin */
#define SPD_UP_NIBBL_TWTR_MIN	0x2B
#define SPD_UP_NIBBL_TWTR_MIN_MASK	0x0F

/* 0x2C  Minimum Write to Read Time (tWTR_Smin), different bank group */
#define SPD_TWTR_SMIN	0x2C

/* 0x2D Minimum Write to Read Time (tWTR_Lmin), same bank group */
#define SPD_TWTR_L_MIN	0x2D
#define SPD_TWTR_L_MIN_VAL 0x3C

/* 0x3C-0x4D Connector to SDRAM Bit Mapping */
#define SPD_DQ0_3		0x3C
#define SPD_DQ4_7		0x3D
#define SPD_DQ8_11		0x3E
#define SPD_DQ12_15		0x3F
#define SPD_DQ16_19		0x40
#define SPD_DQ20_23		0x41
#define SPD_DQ24_27		0x42
#define SPD_DQ28_31		0x43
#define SPD_CB0_3		0x44
#define SPD_CB4_7		0x45
#define SPD_DQ32_35		0x46
#define SPD_DQ36_39		0x47
#define SPD_DQ40_43		0x48
#define SPD_DQ44_47		0x49
#define SPD_DQ48_51		0x4A
#define SPD_DQ52_55		0x4B
#define SPD_DQ56_59		0x4C
#define SPD_DQ60_63		0x4D


/* 0x75  Fine Offset for Minimum CAS to CAS Delay Time (tCCD_Lmin), same bank group */
#define SPD_TCCD_MIN	0x75

/* 0x76  Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Lmin), same bank group*/
#define SPD_TRRD_MIN 	0x76

/* 0x77 Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Smin), different bank group*/
#define SPD_TRRD_SMIN	0x77

/* 0x78  Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin) */
#define SPD_TRC_MIN_FINE		0x78

/* 0x79  Fine Offset for Minimum Row Precharge Delay Time (tRPmin) */
#define SPD_TRP_MIN_FINE		0x79

/* 0x7A   Fine Offset for Minimum RAS to CAS Delay Time (tRCDmin) */
#define SPD_TRCD_MIN_FINE	0x7A

/* 0x7B Fine Offset for Minimum CAS Latency Time (tAAmin) */
#define SPD_TAA_MIN_FINE	0x7B


/* 0x7C Fine Offset for SDRAM Maximum Cycle Time (tCKAVGmax) */
#define SPD_TCKAVG_MAX_FINE	0x7C

/* 0x7D  Fine Offset for SDRAM Minimum Cycle Time (tCKAVGmin) */
#define SPD_TCKAVG_MIN_FINE	0x7D


/* 0x7E  Cyclical Redundancy Code (CRC) for Base Configuration Section, LSB */
#define SPD_CRC_BCS_LSB	0x7E

/* 0x7F  Cyclical Redundancy Code (CRC) for Base Configuration Section, MSB */
#define SPD_CRC_BCS_MSB	0x7F



/****************************
 * Module-Specific section (UDIMM)
 ****************************/

#define SPD_MSB_START_OFFS	0x80
#define SPD_MSB_END_OFFS	0xBF

/* 0x80 Raw Card Extension, Module Nominal Height */
#define SPD_RCE_MNH	0x80
#define SPD_RCE_MNH_HEIGHT_OFFS 0
#define SPD_RCE_MNH_HEIGHT_MASK 0x1F
#define SPD_RCE_MNH_HEIGHT_INIT_VAL		15


#define SPD_RCE_MNH_RCE_OFFS	5
#define SPD_RCE_MNH_RCE_OFFS_MASK	0xE0
#define SPD_RCE_MNH_RCE_OFFS_INIT_VAL	3


/* 0x81  Module Maximum Thickness */
#define SPD_MMT		0x81
#define SPD_MMT_BACK_OFFS		4
#define SPD_MMT_BACK_MASK		0xf0
#define SPD_MMT_FRONT_OFFS		0
#define SPD_MMT_FRONT_MASK		0x0f

/* 0x82  Reference Raw Card Used */
#define SPD_RRCU	0x82

/* 0x83  Address Mapping from Edge Connector to DRAM */
#define SPD_ADDR_MAPPING 0x83
#define SPD_ADDR_MAPPING_MIRRORED	(1 << 0)



/* 0xFE  Cyclical Redundancy Code (CRC) for Module Specific Section, LSB */
#define SPD_CRC_MSS_LSB	0xFE

/* 0xFF  Cyclical Redundancy Code (CRC) for Module Specific Section, MSB */
#define SPD_CRC_MSS_MSB	0xFF



/****************************
 * Module Supplier Data
 ****************************/
#define SPD_MSD_START_OFFS 0x140

/* 0x140 Module Manufacturer ID Code, LSB */
#define SPD_MM_ID_LSB	0x140

/* 0x141 Module Manufacturer ID Code, MSB */
#define SPD_MM_ID_MSB	0x141

/* 0x142 Module Manufacturing Location */
#define SPD_MM_LOC		0x142

/* 0x143-0x144 Module Manufacturing Date */
#define SPD_MM_DATE0	0x143
#define SPD_MM_DATE1	0x144

/* 0x145-0x148 Module Serial Number  */
#define SPD_M_SN0 0x145
#define SPD_M_SN1 0x146
#define SPD_M_SN2 0x147
#define SPD_M_SN3 0x148

/* 0x149-0x15C Module Part Number */
#define SPD_M_PNUM_START	0x149
#define SPD_M_PNUM_LEN		20

/* 0x15D Module Revision Code */
#define SPD_M_REVCODE		0x15D


/* 0x15E DRAM Manufacturer ID Code, LSB */
#define SPD_DRAMM_ID_LSB	0x15E

/* 0x15F DRAM Manufacturer ID Code, MSB */
#define SPD_DRAMM_ID_MSB	0x15F


/* 0x160 DRAM Stepping */
#define SPD_DRAM_STEPPING	0x160

/* 0x161-0x17D -  Manufacturer’s Specific Data */
#define SPD_MM_SPECIFIC_START	0x161
#define SPD_MM_SPECIFIC_LEN		28

#define SDP_MSD_END		0x17F


#endif /* SDP_H_ */
