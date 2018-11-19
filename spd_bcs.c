/*
 * spd_bcs.c
 *
 *  Created on: 09.11.2018
 *      Author: Perepelitsin Roman
 */

#include "spd.h"
#include "spd_bcs.h"
#include "spd_verbose.h"

char *dram_types[]	=	{
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


char *module_types[]	=	{
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

char *signal_loading[]	=	{
		"Not specified",
		"Multi load stack",
		"Single load stack (3DS)",
		"BROKEN!"
};

char *dram_density_ratio[]	=	{
		"Rank 1 and 3 device densities are the same as rank 0 and 2 densities",
		"Rank 1 and 3 are one standard device density smaller than rank 0 and 2",
		"Rank 1 and 3 are two standard device densities smaller than rank 0 and 2",
		"BROKEN!"
};


static char *bcs_field_names[]	=	{
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


void decode_connector_bit_mapping(uint8_t *spd, spd_bcs *bcs)
{
	int i, j = 0;
	SPDT_PRINTF("Connector to SDRAM Bit Mapping \n");

	for(i = SPD_DQ0_3; i <= SPD_DQ60_63; i++)	{
		bcs->conn_bit_mapping[i - SPD_DQ0_3] = spd[i];

		if(i == SPD_CB0_3) {
			continue;
		} else if (i == SPD_CB4_7)	{
			continue;
		}
		SPDT_PRINTF("DQ%02d-%02d - 0x%02x    ",j, j + 3, bcs->conn_bit_mapping[i - SPD_DQ0_3]);
		j += 4;
		if(j != 0 && ((j) % 16 == 0))	{
			SPDT_PRINTF(" \n");
		}

	}
	SPDT_PRINTF("CB00_03 - 0x%02x    ",bcs->conn_bit_mapping[SPD_CB0_3 - SPD_DQ0_3]);
	SPDT_PRINTF("CB04_07 - 0x%02x\n",bcs->conn_bit_mapping[SPD_CB4_7 - SPD_DQ0_3]);
}



int decode_bcs(uint8_t *spd, spd_bcs *bcs)
{
	uint8_t tval;
	uint32_t ival;

	/* 0x0 */
	if((spd[SPD_BYTES] & SPD_BYTES_TOTAL_MASK) == SPD_BYTES_TOTAL_256)
		SPDT_PRINTF("SPD size\t 256\n");
	else if((spd[SPD_BYTES] & SPD_BYTES_TOTAL_MASK) == SPD_BYTES_TOTAL_512)
	{
		SPDT_PRINTF("SPD size\t 512\n");
	} else
		SPDT_PRINTF("SPD size\t BROKEN!\n");


	ival = (spd[SPD_BYTES] & SPD_BYTES_USED_MASK) * SPD_BYTES_USED_INIT_VAL;
	if(ival == 0){
		SPDT_PRINTF("SPD bytes Used\t BROKEN!\n");
	}
	else	{
		SPDT_PRINTF("SPD bytes Used\t %d\n", ival);
	}

	bcs->bytes = spd[SPD_BYTES];


	/* 0x01 */
	bcs->revision = spd[SPD_REVISION];
	SPDT_PRINTF("SPD revision\t %02x\n", bcs->revision);

	/* 0x02 */
	bcs->dram_type = spd[SPD_DEVTYPE];
	tval = bcs->dram_type;
	if(tval > SPD_DEVTYPE_MAX)
		tval = 0;
	SPDT_PRINTF("SPD DRAM Device type\t %s\n", dram_types[tval]);

	/* 0x3 */
	bcs->module_type = spd[SPD_MODULE_TYPE];
	SPDT_PRINTF("SPD Module Type\t %s\n", module_types[bcs->module_type & SPD_MODULE_TYPE_MASK]);
	if(bcs->module_type & SPD_MODULE_TYPE_HYBRID_NVDIMM)	{
		SPDT_PRINTF(" SPD Module Type hybrid NVDIMM\n");
	}

	/* 0x4 */
	bcs->dens_bansks = spd[SPD_DENS_BANKS];

	ival =   SPD_DENS_BANKS_CAP_PER_DIE_INIT_VAL << (bcs->dens_bansks & SPD_DENS_BANKS_CAP_PER_DIE_MASK);

	SPDT_PRINTF("Total SDRAM capacitey per die\t %d%s \n",
			(ival > 1000)?(ival/1000):ival,
			(ival > 1000)?"Gbits":"Mbits");
	SPDT_PRINTF("Banks\t %d banks \n", (bcs->dens_bansks & SPD_DENS_BANKS_ADDR_BITS_MASK)?8:4);

	SPDT_PRINTF("Groups\t ");
	if(!(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_MASK))
		SPDT_PRINTF("no bank group\n");
	if(!(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_MASK))
			SPDT_PRINTF("no bank group\n");
	else if(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_2_GROUP)
			SPDT_PRINTF("2 bank groups\n");
	else if(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_4_GROUP)
			SPDT_PRINTF("4 bank groups\n");
	else
		SPDT_PRINTF("BROKEN! bank groups\n");


	/* 0x5 */
	bcs->sdram_addressing = spd[SPD_SDRAM_ADDR];
	SPDT_PRINTF("Column address bits\t %d\n", SPD_SDRAM_ADDR_COL_INIT_VAL + (bcs->sdram_addressing & SPD_SDRAM_ADDR_COL_MASK));
	SPDT_PRINTF("Row address bits\t %d\n",
			SPD_SDRAM_ADDR_ROW_INIT_VAL + ((bcs->sdram_addressing & SPD_SDRAM_ADDR_ROW_MASK) >> SPD_SDRAM_ADDR_ROW_OFFS));


	/* 0x6 */
	bcs->prim_sdram_pack_type = spd[SPD_PACK_TYPE];
	SPDT_PRINTF("Signal loading\t %s\n", signal_loading[(bcs->prim_sdram_pack_type & SPD_PACK_TYPE_SIGNAL_MASK)]);
	SPDT_PRINTF("Die count\t %d die\n",  ((bcs->prim_sdram_pack_type & SPD_PACK_TYPE_DIE_CNT_MASK) >> SPD_PACK_TYPE_DIE_CNT_OFFS)
			+ SPD_PACK_TYPE_DIE_CNT_INIT_VAL);

	SPDT_PRINTF("%s\n", (bcs->prim_sdram_pack_type & SPD_PACK_TYPE_NON_MONOLITH)?"Non-monolithic device":
																		   "Monolithic DRAM device");


	/* 0x7 */
	bcs->sdram_opt_feat = spd[SPD_OPT_FEATURES];

	SPDT_PRINTF("Maximum activate count\t ");
	tval = bcs->sdram_opt_feat & SPD_OPT_FEATURES_MAC_MASK;
	if(tval == 0)
		SPDT_PRINTF("Untested MAC\n");
	else if(tval == 0x8 )	{
		SPDT_PRINTF("Unlimited MAC\n");
	}
	else	{
		SPDT_PRINTF(" %d K\n", 800 - (tval * 100));
	}

	SPDT_PRINTF("Maximum activate window (tMAW)\t %d * tREFI\n", SPD_OPT_FEATURES_TMAW_INIT_VAL /
			(((bcs->sdram_opt_feat & SPD_OPT_FEATURES_TMAW_MASK) >> SPD_OPT_FEATURES_TMAW_OFFSET) + 1));

	/* 0x9 */
	bcs->sdram_other_feat = spd[SPD_OTHER_SDRAM_OPT];
	SPDT_PRINTF("Post package repair (PPR)\t %s \n", (bcs->sdram_other_feat & SPD_OTHER_SDRAM_OPT_PPR_SUPP)?
			"supported, one row per bank group":
			"not supported");
	SPDT_PRINTF("Soft PPR\t %s\n",(bcs->sdram_other_feat & SPD_OTHER_SDRAM_OPT_SOFT_PPR_SUP)?"supported":"not supported");

	/* 0xA */
	bcs->secnd_sdram_pack_type = spd[SPD_PACK_TYPE_SEC];

	if(bcs->secnd_sdram_pack_type == 0x0){
		SPDT_PRINTF("Module with symmetrical assembly\n");
	}	else	{
		SPDT_PRINTF("Module have  asymmetrical assembly\n");
		SPDT_PRINTF("Secondary signal loading\t %s\n", signal_loading[(bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_SIGNAL_MASK)]);
		SPDT_PRINTF("Seconary die count\t %d die\n",  ((bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_DIE_CNT_MASK) >> SPD_PACK_TYPE_DIE_CNT_OFFS)
				+ SPD_PACK_TYPE_DIE_CNT_INIT_VAL);
		SPDT_PRINTF("%s\n", (bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_NON_MONOLITH)?"Non-monolithic device":
																			   "Monolithic DRAM device");
		SPDT_PRINTF("Secondary DRAM density ration\t %s\n",
				dram_density_ratio[(bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_SEC_RATIO_MASK) >> SPD_PACK_TYPE_SEC_RATIO_OFFSET]);


	}


	/* 0xB */
	bcs->vdd = spd[SPD_VDD];

	SPDT_PRINTF("DRAM VDD 1.2v\t %s operable and %s endurant \n", (bcs->vdd & SPD_VDD_1_2V_OPERABLE)?"":"not",
			(bcs->vdd & SPD_VDD_1_2V_ENDURANT)?"":"not");

	/* 0xC */
	bcs->module_org = spd[SPD_MODULE_ORG];
	SPDT_PRINTF("SDRAM device width\t %d bits\n", 4 << (bcs->module_org & SPD_MODULE_ORG_DEV_WIDTH_MASK));
	SPDT_PRINTF("Number of package ranks per DIMM\t %d\n", (bcs->module_org >> SPD_MODULE_ORG_PACK_RANK_PER_DIMM_OFFS) +
			SPD_MODULE_ORG_PACK_RANK_PER_DIMM_INIT_VAL);
	SPDT_PRINTF("Rank mix\t %s\n", (bcs->module_org & SPD_MODULE_ORG_RANK_MIX_ASSYM)?"asymmetrical":"symmetrical");

	/* 0xD */
	bcs->module_mem_buswidth = spd[SPD_MODULE_MEM_BUS_WIDTH];
	SPDT_PRINTF("Primary bus width\t %d\n", SPD_MODULE_MEM_BUS_WIDTH_PRIM_INIT_VAL <<
			(bcs->module_mem_buswidth & SPD_MODULE_MEM_BUS_WIDTH_PRIM_MASK));
	SPDT_PRINTF("Bus width extensions\t %s \n", (bcs->module_mem_buswidth & SPD_MODULE_MEM_BUS_WIDTH_EXT_8BIT)?
			"8 bits (ECC present)":"0 bits (no ECC)");


	/* 0xE */
	bcs->module_thermal_sens = spd[SPD_MODULE_THERM_SENS];
	SPDT_PRINTF("Thermal sensor %sincorporated onto this assembly\n",
			(bcs->module_thermal_sens & SPD_MODULE_THERM_SENS_INCORP)?"":"not ");

	/* 0xF */

	/* 0x10 */

	/* 0x11 */
	bcs->timebases = spd[SPD_TIMEBASES];

	if(bcs->timebases != SPD_TIMEBASES_VAL)	{
		SPDT_PRINTF("Timebase BROKEN!\n");
	} else	{
		SPDT_PRINTF("MTB\t 125ps\nFTB\t 1ps\n");
	}

	/* 0x12 */
	bcs->tckavg_min = spd[SPD_TCKAVG_MIN];
	SPDT_PRINTF("tCKAVG min\t 0x%02x (%d) MTB\n", bcs->tckavg_min, (char)bcs->tckavg_min);

	/* 0x13 */
	bcs->tckavg_max = spd[SPD_TCKAVG_MAX];
	SPDT_PRINTF("tCKAVG max\t 0x%02x (%d) MTB\n", bcs->tckavg_max, (char)bcs->tckavg_max);

	/* 0x14 */
	/* 0x15 */
	/* 0x16 */
	/* 0x17 */
	bcs->cas_lat_1 = spd[SPD_CASLAT_1];
	bcs->cas_lat_2 = spd[SPD_CASLAT_2];
	bcs->cas_lat_3 = spd[SPD_CASLAT_3];
	bcs->cas_lat_4 = spd[SPD_CASLAT_4];
	SPDT_PRINTF("CAS Latencies\t %02x %02x %02x %02x\n", bcs->cas_lat_1, bcs->cas_lat_2, bcs->cas_lat_3, bcs->cas_lat_4);

	/* 0x18 */
	bcs->taa_min = spd[SPD_TAA_MIN];
	SPDT_PRINTF("tAA min\t\t 0x%02x (%d) MTB\n", bcs->taa_min, (char)bcs->taa_min);

	/* 0x19 */
	bcs->trcd_min = spd[SPD_TRCD_MIN];
	SPDT_PRINTF("tRCD min\t 0x%02x (%d) MTB\n", bcs->trcd_min, (char)bcs->trcd_min);

	/* 0x1A */
	bcs->trp_min = spd[SPD_TRP_MIN];
	SPDT_PRINTF("tRP min \t 0x%02x (%d) MTB\n", bcs->trp_min, (char)bcs->trp_min);

	/* 0x1B */
	bcs->tras_trc_up = spd[SPD_UP_NIBBLES];
	SPDT_PRINTF("tRC min most significant nibble\t 0x%02x\n", bcs->tras_trc_up >> SPD_UP_NIBBLES_TRC_MIN_OFFS);
	SPDT_PRINTF("tRAS min most significant nibble\t 0x%02x\n", bcs->tras_trc_up & SPD_UP_NIBBLES_TRAS_MIN_MASK);

	/* 0x1C */
	bcs->tras_min = spd[SPD_TRAS_MIN];
	SPDT_PRINTF("tRAS min Least Significant Byte\t 0x%02x\n", bcs->tras_min);

	/* 0x1D */
	bcs->trc_min = spd[SPD_TRC_MIN];
	SPDT_PRINTF("tRC min Least Significant Byte\t 0x%02x\n", bcs->trc_min);

	/* 0x1E */
	bcs->trfc1_min_lsb = spd[SPD_TRFC1_MIN_LSB];
	SPDT_PRINTF("tRFC1 min lsb\t 0x%02x\n", bcs->trfc1_min_lsb);

	/* 0x1F */
	bcs->trfc1_min_msb = spd[SPD_TRFC1_MIN_MSB];
	SPDT_PRINTF("tRFC1 min msb\t 0x%02x\n", bcs->trfc1_min_msb);

	/* 0x20 */
	bcs->trfc2_min_lsb = spd[SPD_TRFC2_MIN_LSB];
	SPDT_PRINTF("tRFC2 min lsb\t 0x%02x\n", bcs->trfc2_min_lsb);

	/* 0x21 */
	bcs->trfc2_min_msb = spd[SPD_TRFC2_MIN_MSB];
	SPDT_PRINTF("tRFC2 min msb\t 0x%02x\n", bcs->trfc2_min_msb);

	/* 0x22 */
	bcs->trfc4_min_lsb = spd[SPD_TRFC4_MIN_LSB];
	SPDT_PRINTF("tRFC4 min lsb\t 0x%02x\n", bcs->trfc4_min_lsb);

	/* 0x23 */
	bcs->trfc4_min_msb = spd[SPD_TRFC4_MIN_MSB];
	SPDT_PRINTF("tRFC4 min msb\t 0x%02x\n", bcs->trfc4_min_msb);

	/* 0x24 */
	bcs->tfaw_min_upper = spd[SPD_TFAW_NIBBLE];
	SPDT_PRINTF("tFAW min upper nibble\t 0x%02x\n", bcs->tfaw_min_upper & SPD_TFAW_NIBBLE_MASK);

	/* 0x25 */
	bcs->tfaw_min = spd[SPD_TFAW_MIN];
	SPDT_PRINTF("tFAW min Least Significant Byte\t 0x%02x\n", bcs->tfaw_min);

	/* 0x26 */
	bcs->trrd_s_min= spd[SPD_TRRD_S_MIN];
	SPDT_PRINTF("tRRD_S min \t 0x%02x (%d) MTB\n", bcs->trrd_s_min, (char)bcs->trrd_s_min);

	/* 0x27 */
	bcs->trrd_l_min= spd[SPD_TRRD_L_MIN];
	SPDT_PRINTF("tRRD_L min \t 0x%02x (%d) MTB\n", bcs->trrd_l_min, (char)bcs->trrd_l_min);

	/* 0x28 */
	bcs->tccd_l_min = spd[SPD_TCCD_L_MIN];
	SPDT_PRINTF("tCCD_L min \t 0x%02x (%d) MTB\n", bcs->tccd_l_min, (char)bcs->tccd_l_min);

	/* 0x29 */
	bcs->twr_min_upper = spd[SPD_UP_NIBBL_TWR_MIN];
	SPDT_PRINTF("tWR min upped nibble\t 0x%02x\n",bcs->twr_min_upper & SPD_UP_NIBBL_TWR_MIN_MASK);

	/* 0x2A */
	bcs->twr_min = spd[SPD_TWR_MIN];
	SPDT_PRINTF("tWR min Least Significant Byte\t 0x%02x\n",bcs->twr_min);

	/* 0x2B */
	bcs->twtr_s_min_upper= spd[SPD_UP_NIBBL_TWTR_MIN];
	SPDT_PRINTF("tWTR_S min upped nibble\t 0x%02x\n",bcs->twtr_s_min_upper & SPD_UP_NIBBL_TWTR_MIN_MASK);

	/* 0x2C */
	bcs->twtr_s_min = spd[SPD_TWTR_SMIN];
	SPDT_PRINTF("tWTR_S min Least Significant Byte\t 0x%02x\n",bcs->twtr_s_min);

	/* 0x2D */
	bcs->twtr_l_min= spd[SPD_TWTR_L_MIN];
	SPDT_PRINTF("tWTR_L min \t 0x%02x (%d) MTB\n", bcs->twtr_l_min, (char)bcs->twtr_l_min);
	if(bcs->twtr_l_min != SPD_TWTR_L_MIN_VAL)	{
		SPDT_PRINTF("tWTR_L min\t BROKEN! Must be\t 0x%02x\n", SPD_TWTR_L_MIN_VAL);
	}

	/* 0x2E-0x3B - reserver */
	/* 0x3C-0x4D */
	decode_connector_bit_mapping(spd, bcs);


	/* 0x75 */
	bcs->tccd_l_min_fine = spd[SPD_TCCD_MIN];
	SPDT_PRINTF("tCCD_L min fine \t 0x%02x (%d) FTB\n", bcs->tccd_l_min_fine, (char)bcs->tccd_l_min_fine);

	/* 0x76 */
	bcs->trrd_l_min_fine = spd[SPD_TRRD_MIN];
	SPDT_PRINTF("tRRD_L min fine \t 0x%02x (%d) FTB\n", bcs->trrd_l_min_fine, (char)bcs->trrd_l_min_fine);

	/* 0x77 */
	bcs->trrd_s_min_fine = spd[SPD_TRRD_SMIN];
	SPDT_PRINTF("tRRD_S min fine \t 0x%02x (%d) FTB\n", bcs->trrd_s_min_fine, (char)bcs->trrd_s_min_fine);

	/* 0x78 */
	bcs->trc_min_fine = spd[SPD_TRC_MIN_FINE];
	SPDT_PRINTF("tRC min fine \t\t 0x%02x (%d) FTB\n", bcs->trc_min_fine, (char)bcs->trc_min_fine);

	/* 0x79 */
	bcs->trp_min_fine = spd[SPD_TRP_MIN_FINE];
	SPDT_PRINTF("tRP min fine \t\t 0x%02x (%d) FTB\n", bcs->trp_min_fine, (char)bcs->trp_min_fine);

	/* 0x7A */
	bcs->trcd_min_fine = spd[SPD_TRCD_MIN_FINE];
	SPDT_PRINTF("tRCD min fine \t\t 0x%02x (%d) FTB\n", bcs->trcd_min_fine, (char)bcs->trcd_min_fine);

	/* 0x7B */
	bcs->taa_min_fine = spd[SPD_TAA_MIN_FINE];
	SPDT_PRINTF("tAA min fine \t\t 0x%02x (%d) FTB\n", bcs->taa_min_fine, (char)bcs->taa_min_fine);

	/* 0x7C */
	bcs->tckavg_max_fine = spd[SPD_TCKAVG_MAX_FINE];
	SPDT_PRINTF("tCKAVG max fine \t 0x%02x (%d) FTB\n", bcs->tckavg_max_fine, (char)bcs->tckavg_max_fine);

	/* 0x7D */
	bcs->tckavg_min_fine = spd[SPD_TCKAVG_MIN_FINE];
	SPDT_PRINTF("tCKAVG min fine \t 0x%02x (%d) FTB\n", bcs->tckavg_min_fine, (char)bcs->tckavg_min_fine);

	/* 0x7E */
	bcs->crc_lsb = spd[SPD_CRC_BCS_LSB];
	SPDT_PRINTF("CRC for Base Configuration Section, LSB \t0x%02x\n", bcs->crc_lsb);

	/* 0x7F */
	bcs->crc_msb = spd[SPD_CRC_BCS_MSB];
	SPDT_PRINTF("CRC for Base Configuration Section, MSB \t0x%02x\n", bcs->crc_msb);

	return 0;
}



uint16_t decode_bcs_case(uint8_t *spd, spd_bcs *bcs, uint16_t from, int show_fild_names)
{
	uint8_t tval;
	uint32_t ival;

	if(show_fild_names){
		SPDT_PRINTF("\n[0x%02X:%d]\t%s\n", from, from, bcs_field_names[from]);
	}


	switch(from)	{
		/* 0x0 */
		case SPD_BYTES:
			if((spd[SPD_BYTES] & SPD_BYTES_TOTAL_MASK) == SPD_BYTES_TOTAL_256)
				SPDT_PRINTF("SPD size\t 256\n");
			else if((spd[SPD_BYTES] & SPD_BYTES_TOTAL_MASK) == SPD_BYTES_TOTAL_512)
			{
				SPDT_PRINTF("SPD size\t 512\n");
			} else
				SPDT_PRINTF("SPD size\t BROKEN!\n");


			ival = (spd[SPD_BYTES] & SPD_BYTES_USED_MASK) * SPD_BYTES_USED_INIT_VAL;
			if(ival == 0){
				SPDT_PRINTF("SPD bytes Used\t BROKEN!\n");
			}
			else	{
				SPDT_PRINTF("SPD bytes Used\t %d\n", ival);
			}

			bcs->bytes = spd[SPD_BYTES];

			return SPD_BYTES + 1;

		case SPD_REVISION:
			/* 0x01 */
			bcs->revision = spd[SPD_REVISION];
			SPDT_PRINTF("SPD revision\t %02x\n", bcs->revision);

			return SPD_REVISION + 1;

		case SPD_DEVTYPE:
			/* 0x02 */
			bcs->dram_type = spd[SPD_DEVTYPE];
			tval = bcs->dram_type;
			if(tval > SPD_DEVTYPE_MAX)
				tval = 0;
			SPDT_PRINTF("SPD DRAM Device type\t %s\n", dram_types[tval]);
			return SPD_DEVTYPE + 1;

		case SPD_MODULE_TYPE:
			/* 0x3 */
			bcs->module_type = spd[SPD_MODULE_TYPE];
			SPDT_PRINTF("SPD Module Type\t %s\n", module_types[bcs->module_type & SPD_MODULE_TYPE_MASK]);
			if(bcs->module_type & SPD_MODULE_TYPE_HYBRID_NVDIMM)	{
				SPDT_PRINTF(" SPD Module Type hybrid NVDIMM\n");
			}

			return SPD_MODULE_TYPE + 1;

		case SPD_DENS_BANKS:
			/* 0x4 */
			bcs->dens_bansks = spd[SPD_DENS_BANKS];

			ival =   SPD_DENS_BANKS_CAP_PER_DIE_INIT_VAL << (bcs->dens_bansks & SPD_DENS_BANKS_CAP_PER_DIE_MASK);

			SPDT_PRINTF("Total SDRAM capacitey per die\t %d%s \n",
					(ival > 1000)?(ival/1000):ival,
					(ival > 1000)?"Gbits":"Mbits");
			SPDT_PRINTF("Banks\t %d banks \n", (bcs->dens_bansks & SPD_DENS_BANKS_ADDR_BITS_MASK)?8:4);

			SPDT_PRINTF("Groups\t ");
			if(!(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_MASK))
				SPDT_PRINTF("no bank group\n");
			if(!(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_MASK))
					SPDT_PRINTF("no bank group\n");
			else if(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_2_GROUP)
					SPDT_PRINTF("2 bank groups\n");
			else if(bcs->dens_bansks & SPD_DENS_BANKS_GROUP_4_GROUP)
					SPDT_PRINTF("4 bank groups\n");
			else
				SPDT_PRINTF("BROKEN! bank groups\n");

			return SPD_DENS_BANKS + 1;

		case SPD_SDRAM_ADDR:
			/* 0x5 */
			bcs->sdram_addressing = spd[SPD_SDRAM_ADDR];
			SPDT_PRINTF("Column address bits\t %d\n", SPD_SDRAM_ADDR_COL_INIT_VAL + (bcs->sdram_addressing & SPD_SDRAM_ADDR_COL_MASK));
			SPDT_PRINTF("Row address bits\t %d\n",
					SPD_SDRAM_ADDR_ROW_INIT_VAL + ((bcs->sdram_addressing & SPD_SDRAM_ADDR_ROW_MASK) >> SPD_SDRAM_ADDR_ROW_OFFS));
			return SPD_SDRAM_ADDR + 1;

		case SPD_PACK_TYPE:
			/* 0x6 */
			bcs->prim_sdram_pack_type = spd[SPD_PACK_TYPE];
			SPDT_PRINTF("Signal loading\t %s\n", signal_loading[(bcs->prim_sdram_pack_type & SPD_PACK_TYPE_SIGNAL_MASK)]);
			SPDT_PRINTF("Die count\t %d die\n",  ((bcs->prim_sdram_pack_type & SPD_PACK_TYPE_DIE_CNT_MASK) >> SPD_PACK_TYPE_DIE_CNT_OFFS)
					+ SPD_PACK_TYPE_DIE_CNT_INIT_VAL);

			SPDT_PRINTF("%s\n", (bcs->prim_sdram_pack_type & SPD_PACK_TYPE_NON_MONOLITH)?"Non-monolithic device":
																				   "Monolithic DRAM device");
			return SPD_PACK_TYPE + 1;

		case SPD_OPT_FEATURES:
			/* 0x7 */
			bcs->sdram_opt_feat = spd[SPD_OPT_FEATURES];

			SPDT_PRINTF("Maximum activate count\t ");
			tval = bcs->sdram_opt_feat & SPD_OPT_FEATURES_MAC_MASK;
			if(tval == 0)
				SPDT_PRINTF("Untested MAC\n");
			else if(tval == 0x8 )	{
				SPDT_PRINTF("Unlimited MAC\n");
			}
			else	{
				SPDT_PRINTF(" %d K\n", 800 - (tval * 100));
			}

			SPDT_PRINTF("Maximum activate window (tMAW)\t %d * tREFI\n", SPD_OPT_FEATURES_TMAW_INIT_VAL /
					(((bcs->sdram_opt_feat & SPD_OPT_FEATURES_TMAW_MASK) >> SPD_OPT_FEATURES_TMAW_OFFSET) + 1));

			return SPD_OPT_FEATURES + 1;

		case SPD_OTHER_SDRAM_OPT:
			/* 0x9 */
			bcs->sdram_other_feat = spd[SPD_OTHER_SDRAM_OPT];
			SPDT_PRINTF("Post package repair (PPR)\t %s \n", (bcs->sdram_other_feat & SPD_OTHER_SDRAM_OPT_PPR_SUPP)?
					"supported, one row per bank group":
					"not supported");
			SPDT_PRINTF("Soft PPR\t %s\n",(bcs->sdram_other_feat & SPD_OTHER_SDRAM_OPT_SOFT_PPR_SUP)?"supported":"not supported");

			return SPD_OTHER_SDRAM_OPT + 1;

		case SPD_PACK_TYPE_SEC:
			/* 0xA */
			bcs->secnd_sdram_pack_type = spd[SPD_PACK_TYPE_SEC];

			if(bcs->secnd_sdram_pack_type == 0x0){
				SPDT_PRINTF("Module with symmetrical assembly\n");
			}	else	{
				SPDT_PRINTF("Module have  asymmetrical assembly\n");
				SPDT_PRINTF("Secondary signal loading\t %s\n", signal_loading[(bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_SIGNAL_MASK)]);
				SPDT_PRINTF("Seconary die count\t %d die\n",  ((bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_DIE_CNT_MASK) >> SPD_PACK_TYPE_DIE_CNT_OFFS)
						+ SPD_PACK_TYPE_DIE_CNT_INIT_VAL);
				SPDT_PRINTF("%s\n", (bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_NON_MONOLITH)?"Non-monolithic device":
																					   "Monolithic DRAM device");
				SPDT_PRINTF("Secondary DRAM density ration\t %s\n",
						dram_density_ratio[(bcs->secnd_sdram_pack_type & SPD_PACK_TYPE_SEC_RATIO_MASK) >> SPD_PACK_TYPE_SEC_RATIO_OFFSET]);


			}

			return SPD_PACK_TYPE_SEC + 1;

		case SPD_VDD:
			/* 0xB */
			bcs->vdd = spd[SPD_VDD];

			SPDT_PRINTF("DRAM VDD 1.2v\t %s operable and %s endurant \n", (bcs->vdd & SPD_VDD_1_2V_OPERABLE)?"":"not",
					(bcs->vdd & SPD_VDD_1_2V_ENDURANT)?"":"not");

			return SPD_VDD + 1;

		case SPD_MODULE_ORG:
			/* 0xC */
			bcs->module_org = spd[SPD_MODULE_ORG];
			SPDT_PRINTF("SDRAM device width\t %d bits\n", 4 << (bcs->module_org & SPD_MODULE_ORG_DEV_WIDTH_MASK));
			SPDT_PRINTF("Number of package ranks per DIMM\t %d\n", (bcs->module_org >> SPD_MODULE_ORG_PACK_RANK_PER_DIMM_OFFS) +
					SPD_MODULE_ORG_PACK_RANK_PER_DIMM_INIT_VAL);
			SPDT_PRINTF("Rank mix\t %s\n", (bcs->module_org & SPD_MODULE_ORG_RANK_MIX_ASSYM)?"asymmetrical":"symmetrical");

			return SPD_MODULE_ORG + 1;

		case SPD_MODULE_MEM_BUS_WIDTH:
			/* 0xD */
			bcs->module_mem_buswidth = spd[SPD_MODULE_MEM_BUS_WIDTH];
			SPDT_PRINTF("Primary bus width\t %d\n", SPD_MODULE_MEM_BUS_WIDTH_PRIM_INIT_VAL <<
					(bcs->module_mem_buswidth & SPD_MODULE_MEM_BUS_WIDTH_PRIM_MASK));
			SPDT_PRINTF("Bus width extensions\t %s \n", (bcs->module_mem_buswidth & SPD_MODULE_MEM_BUS_WIDTH_EXT_8BIT)?
					"8 bits (ECC present)":"0 bits (no ECC)");
			return SPD_MODULE_MEM_BUS_WIDTH + 1;

		case SPD_MODULE_THERM_SENS:
			/* 0xE */
			bcs->module_thermal_sens = spd[SPD_MODULE_THERM_SENS];
			SPDT_PRINTF("Thermal sensor %sincorporated onto this assembly\n",
					(bcs->module_thermal_sens & SPD_MODULE_THERM_SENS_INCORP)?"":"not ");
			return SPD_MODULE_THERM_SENS + 1;

		case SPD_TIMEBASES:
			/* 0x11 */
			bcs->timebases = spd[SPD_TIMEBASES];

			if(bcs->timebases != SPD_TIMEBASES_VAL)	{
				SPDT_PRINTF("Timebase BROKEN!\n");
			} else	{
				SPDT_PRINTF("MTB\t 125ps\nFTB\t 1ps\n");
			}
			return SPD_TIMEBASES + 1;

		case SPD_TCKAVG_MIN:
			/* 0x12 */
			bcs->tckavg_min = spd[SPD_TCKAVG_MIN];
			SPDT_PRINTF("tCKAVG min\t 0x%02x (%d) MTB\n", bcs->tckavg_min, (char)bcs->tckavg_min);
			return SPD_TCKAVG_MIN + 1;

		case SPD_TCKAVG_MAX:
			/* 0x13 */
			bcs->tckavg_max = spd[SPD_TCKAVG_MAX];
			SPDT_PRINTF("tCKAVG max\t 0x%02x (%d) MTB\n", bcs->tckavg_max, (char)bcs->tckavg_max);
			return SPD_TCKAVG_MAX + 1;

		case SPD_CASLAT_1:
			/* 0x14 */
			bcs->cas_lat_1 = spd[SPD_CASLAT_1];
			return SPD_CASLAT_1 + 1;

		case SPD_CASLAT_2:
			/* 0x15 */
			bcs->cas_lat_2 = spd[SPD_CASLAT_2];
			return SPD_CASLAT_2 + 1;

		case SPD_CASLAT_3:
			/* 0x16 */
			bcs->cas_lat_3 = spd[SPD_CASLAT_3];
			return SPD_CASLAT_3 + 1;

		case SPD_CASLAT_4:
			/* 0x17 */
			bcs->cas_lat_4 = spd[SPD_CASLAT_4];
			SPDT_PRINTF("CAS Latencies\t %02x %02x %02x %02x\n", bcs->cas_lat_1, bcs->cas_lat_2, bcs->cas_lat_3, bcs->cas_lat_4);
			return SPD_CASLAT_4 + 1;

		case SPD_TAA_MIN:
			/* 0x18 */
			bcs->taa_min = spd[SPD_TAA_MIN];
			SPDT_PRINTF("tAA min\t\t 0x%02x (%d) MTB\n", bcs->taa_min, (char)bcs->taa_min);
			return SPD_TAA_MIN + 1;


		case SPD_TRCD_MIN:
			/* 0x19 */
			bcs->trcd_min = spd[SPD_TRCD_MIN];
			SPDT_PRINTF("tRCD min\t 0x%02x (%d) MTB\n", bcs->trcd_min, (char)bcs->trcd_min);

			return SPD_TRCD_MIN + 1;

		case SPD_TRP_MIN:
			/* 0x1A */
			bcs->trp_min = spd[SPD_TRP_MIN];
			SPDT_PRINTF("tRP min \t 0x%02x (%d) MTB\n", bcs->trp_min, (char)bcs->trp_min);

			return SPD_TRP_MIN + 1;

		case SPD_UP_NIBBLES:
			/* 0x1B */
			bcs->tras_trc_up = spd[SPD_UP_NIBBLES];
			SPDT_PRINTF("tRC min most significant nibble\t 0x%02x\n", bcs->tras_trc_up >> SPD_UP_NIBBLES_TRC_MIN_OFFS);
			SPDT_PRINTF("tRAS min most significant nibble\t 0x%02x\n", bcs->tras_trc_up & SPD_UP_NIBBLES_TRAS_MIN_MASK);

			return SPD_UP_NIBBLES + 1;

		case SPD_TRAS_MIN:
			/* 0x1C */
			bcs->tras_min = spd[SPD_TRAS_MIN];
			SPDT_PRINTF("tRAS min Least Significant Byte\t 0x%02x\n", bcs->tras_min);
			return SPD_TRAS_MIN + 1;

		case SPD_TRC_MIN:
			/* 0x1D */
			bcs->trc_min = spd[SPD_TRC_MIN];
			SPDT_PRINTF("tRC min Least Significant Byte\t 0x%02x\n", bcs->trc_min);
			return SPD_TRC_MIN + 1;

		case SPD_TRFC1_MIN_LSB:
			/* 0x1E */
			bcs->trfc1_min_lsb = spd[SPD_TRFC1_MIN_LSB];
			SPDT_PRINTF("tRFC1 min lsb\t 0x%02x\n", bcs->trfc1_min_lsb);
			return SPD_TRFC1_MIN_LSB + 1;


		case SPD_TRFC1_MIN_MSB:
			/* 0x1F */
			bcs->trfc1_min_msb = spd[SPD_TRFC1_MIN_MSB];
			SPDT_PRINTF("tRFC1 min msb\t 0x%02x\n", bcs->trfc1_min_msb);
			return SPD_TRFC1_MIN_MSB + 1;

		case SPD_TRFC2_MIN_LSB:
			/* 0x20 */
			bcs->trfc2_min_lsb = spd[SPD_TRFC2_MIN_LSB];
			SPDT_PRINTF("tRFC2 min lsb\t 0x%02x\n", bcs->trfc2_min_lsb);
			return SPD_TRFC2_MIN_LSB + 1;

		case SPD_TRFC2_MIN_MSB:
			/* 0x21 */
			bcs->trfc2_min_msb = spd[SPD_TRFC2_MIN_MSB];
			SPDT_PRINTF("tRFC2 min msb\t 0x%02x\n", bcs->trfc2_min_msb);
			return SPD_TRFC2_MIN_MSB + 1;

		case SPD_TRFC4_MIN_LSB:
			/* 0x22 */
			bcs->trfc4_min_lsb = spd[SPD_TRFC4_MIN_LSB];
			SPDT_PRINTF("tRFC4 min lsb\t 0x%02x\n", bcs->trfc4_min_lsb);
			return SPD_TRFC4_MIN_LSB + 1;

		case SPD_TRFC4_MIN_MSB:
			/* 0x23 */
			bcs->trfc4_min_msb = spd[SPD_TRFC4_MIN_MSB];
			SPDT_PRINTF("tRFC4 min msb\t 0x%02x\n", bcs->trfc4_min_msb);
			return SPD_TRFC4_MIN_MSB + 1;

		case SPD_TFAW_NIBBLE:
			/* 0x24 */
			bcs->tfaw_min_upper = spd[SPD_TFAW_NIBBLE];
			SPDT_PRINTF("tFAW min upper nibble\t 0x%02x\n", bcs->tfaw_min_upper & SPD_TFAW_NIBBLE_MASK);
			return SPD_TFAW_NIBBLE + 1;

		case SPD_TFAW_MIN:
			/* 0x25 */
			bcs->tfaw_min = spd[SPD_TFAW_MIN];
			SPDT_PRINTF("tFAW min Least Significant Byte\t 0x%02x\n", bcs->tfaw_min);
			return SPD_TFAW_MIN + 1;

		case SPD_TRRD_S_MIN:
			/* 0x26 */
			bcs->trrd_s_min= spd[SPD_TRRD_S_MIN];
			SPDT_PRINTF("tRRD_S min \t 0x%02x (%d) MTB\n", bcs->trrd_s_min, (char)bcs->trrd_s_min);
			return SPD_TRRD_S_MIN + 1;

		case SPD_TRRD_L_MIN:
			/* 0x27 */
			bcs->trrd_l_min= spd[SPD_TRRD_L_MIN];
			SPDT_PRINTF("tRRD_L min \t 0x%02x (%d) MTB\n", bcs->trrd_l_min, (char)bcs->trrd_l_min);
			return SPD_TRRD_L_MIN + 1;

		case SPD_TCCD_L_MIN:
			/* 0x28 */
			bcs->tccd_l_min = spd[SPD_TCCD_L_MIN];
			SPDT_PRINTF("tCCD_L min \t 0x%02x (%d) MTB\n", bcs->tccd_l_min, (char)bcs->tccd_l_min);
			return SPD_TCCD_L_MIN + 1;

		case SPD_UP_NIBBL_TWR_MIN:
			/* 0x29 */
			bcs->twr_min_upper = spd[SPD_UP_NIBBL_TWR_MIN];
			SPDT_PRINTF("tWR min upped nibble\t 0x%02x\n",bcs->twr_min_upper & SPD_UP_NIBBL_TWR_MIN_MASK);
			return SPD_UP_NIBBL_TWR_MIN + 1;

		case SPD_TWR_MIN:
			/* 0x2A */
			bcs->twr_min = spd[SPD_TWR_MIN];
			SPDT_PRINTF("tWR min Least Significant Byte\t 0x%02x\n",bcs->twr_min);
			return SPD_TWR_MIN + 1;

		case SPD_UP_NIBBL_TWTR_MIN:
			/* 0x2B */
			bcs->twtr_s_min_upper= spd[SPD_UP_NIBBL_TWTR_MIN];
			SPDT_PRINTF("tWTR_S min upped nibble\t 0x%02x\n",bcs->twtr_s_min_upper & SPD_UP_NIBBL_TWTR_MIN_MASK);
			return SPD_UP_NIBBL_TWTR_MIN + 1;

		case SPD_TWTR_SMIN:
			/* 0x2C */
			bcs->twtr_s_min = spd[SPD_TWTR_SMIN];
			SPDT_PRINTF("tWTR_S min Least Significant Byte\t 0x%02x\n",bcs->twtr_s_min);
			return SPD_TWTR_SMIN + 1;

		case SPD_TWTR_L_MIN:
			/* 0x2D */
			bcs->twtr_l_min= spd[SPD_TWTR_L_MIN];
			SPDT_PRINTF("tWTR_L min \t 0x%02x (%d) MTB\n", bcs->twtr_l_min, (char)bcs->twtr_l_min);
			if(bcs->twtr_l_min != SPD_TWTR_L_MIN_VAL)	{
				SPDT_PRINTF("tWTR_L min\t BROKEN! Must be\t 0x%02x\n", SPD_TWTR_L_MIN_VAL);
			}
			return SPD_TWTR_L_MIN + 1;

		/* 0x2E-0x3B - reserver */
		/* 0x3C-0x4D */
		case SPD_DQ0_3:
		case SPD_DQ4_7:
		case SPD_DQ8_11:
		case SPD_DQ12_15: case SPD_DQ16_19:
		case SPD_DQ20_23: case SPD_DQ24_27:
		case SPD_DQ28_31: case SPD_CB0_3:
		case SPD_CB4_7:	case SPD_DQ32_35:
		case SPD_DQ36_39: case SPD_DQ40_43:
		case SPD_DQ44_47: case SPD_DQ48_51:
		case SPD_DQ52_55: case SPD_DQ56_59: case SPD_DQ60_63:
			decode_connector_bit_mapping(spd, bcs);
			return SPD_DQ60_63 + 1;

		case SPD_TCCD_MIN:
			/* 0x75 */
			bcs->tccd_l_min_fine = spd[SPD_TCCD_MIN];
			SPDT_PRINTF("tCCD_L min fine \t 0x%02x (%d) FTB\n", bcs->tccd_l_min_fine, (char)bcs->tccd_l_min_fine);
			return SPD_TCCD_MIN + 1;

		case SPD_TRRD_MIN:
			/* 0x76 */
			bcs->trrd_l_min_fine = spd[SPD_TRRD_MIN];
			SPDT_PRINTF("tRRD_L min fine \t 0x%02x (%d) FTB\n", bcs->trrd_l_min_fine, (char)bcs->trrd_l_min_fine);
			return SPD_TRRD_MIN + 1;

		case SPD_TRRD_SMIN:
			/* 0x77 */
			bcs->trrd_s_min_fine = spd[SPD_TRRD_SMIN];
			SPDT_PRINTF("tRRD_S min fine \t 0x%02x (%d) FTB\n", bcs->trrd_s_min_fine, (char)bcs->trrd_s_min_fine);
			return SPD_TRRD_SMIN + 1;

		case SPD_TRC_MIN_FINE:
			/* 0x78 */
			bcs->trc_min_fine = spd[SPD_TRC_MIN_FINE];
			SPDT_PRINTF("tRC min fine \t\t 0x%02x (%d) FTB\n", bcs->trc_min_fine, (char)bcs->trc_min_fine);
			return SPD_TRC_MIN_FINE + 1;

		case SPD_TRP_MIN_FINE:
			/* 0x79 */
			bcs->trp_min_fine = spd[SPD_TRP_MIN_FINE];
			SPDT_PRINTF("tRP min fine \t\t 0x%02x (%d) FTB\n", bcs->trp_min_fine, (char)bcs->trp_min_fine);
			return SPD_TRP_MIN_FINE + 1;

		case SPD_TRCD_MIN_FINE:
			/* 0x7A */
			bcs->trcd_min_fine = spd[SPD_TRCD_MIN_FINE];
			SPDT_PRINTF("tRCD min fine \t\t 0x%02x (%d) FTB\n", bcs->trcd_min_fine, (char)bcs->trcd_min_fine);
			return SPD_TRCD_MIN_FINE + 1;

		case SPD_TAA_MIN_FINE:
			/* 0x7B */
			bcs->taa_min_fine = spd[SPD_TAA_MIN_FINE];
			SPDT_PRINTF("tAA min fine \t\t 0x%02x (%d) FTB\n", bcs->taa_min_fine, (char)bcs->taa_min_fine);
			return SPD_TAA_MIN_FINE + 1;

		case SPD_TCKAVG_MAX_FINE:
			/* 0x7C */
			bcs->tckavg_max_fine = spd[SPD_TCKAVG_MAX_FINE];
			SPDT_PRINTF("tCKAVG max fine \t 0x%02x (%d) FTB\n", bcs->tckavg_max_fine, (char)bcs->tckavg_max_fine);
			return SPD_TCKAVG_MAX_FINE + 1;

		case SPD_TCKAVG_MIN_FINE:
			/* 0x7D */
			bcs->tckavg_min_fine = spd[SPD_TCKAVG_MIN_FINE];
			SPDT_PRINTF("tCKAVG min fine \t 0x%02x (%d) FTB\n", bcs->tckavg_min_fine, (char)bcs->tckavg_min_fine);
			return SPD_TCKAVG_MIN_FINE + 1;

		case SPD_CRC_BCS_LSB:
			/* 0x7E */
			bcs->crc_lsb = spd[SPD_CRC_BCS_LSB];
			SPDT_PRINTF("CRC for Base Configuration Section, LSB \t0x%02x\n", bcs->crc_lsb);
			return SPD_CRC_BCS_LSB + 1;

		case SPD_CRC_BCS_MSB:
			/* 0x7F */
			bcs->crc_msb = spd[SPD_CRC_BCS_MSB];
			SPDT_PRINTF("CRC for Base Configuration Section, MSB \t0x%02x\n", bcs->crc_msb);
			return 0;

		default:
			return from + 1;

	}
	return 0;
}
