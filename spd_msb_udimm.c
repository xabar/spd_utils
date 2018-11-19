/*
 * spd_msb_udimm.c
 *
 *  Created on: 09.11.2018
 *      Author: Perepelitsin Roman
 */

#include "spd_msb_udimm.h"
#include "spd.h"
#include "spd_verbose.h"

#define MSB_UDIMM_RESERVED_FROM	0x84
#define MSB_UDIMM_RESERVED_TILL 0xFD

static char *msb_udimm_field_names[]	=	{
		"Raw Card Extension, Module Nominal Height",
		"Module Maximum Thickness",
		"Reference Raw Card Used",
		"Address Mapping from Edge Connector to DRAM",
		"reserved",
		"Cyclical Redundancy Code (CRC) for Module Specific Section, LSB",
		"Cyclical Redundancy Code (CRC) for Module Specific Section, MSB",
};

int is_udimm_memory(uint8_t *spd)
{
	if((spd[SPD_MODULE_TYPE] & SPD_MODULE_TYPE_UDIMM_KEY_BYTE) && (spd[SPD_DEVTYPE] == SPD_DEVTYPE_UDIMM_KEY))
		return 1;
	return 0;
}


uint16_t decode_msb_udimm_case(uint8_t *spd, msb_udimm *msb, uint16_t from, int show_fild_names)
{
	uint8_t tval;

	if(show_fild_names){
		if(from < MSB_UDIMM_RESERVED_FROM)
			SPDT_PRINTF("\n[0x%02X:%d]\t%s\n", from, from, msb_udimm_field_names[from - SPD_MSB_START_OFFS]);
		if(from > MSB_UDIMM_RESERVED_FROM && from < MSB_UDIMM_RESERVED_TILL)
			SPDT_PRINTF("\n[0x%02X:%d]\t%s\n", from, from, msb_udimm_field_names[4]);
		if(from > MSB_UDIMM_RESERVED_TILL)
			SPDT_PRINTF("\n[0x%02X:%d]\t%s\n", from, from, msb_udimm_field_names[from - SPD_MSB_START_OFFS -
																				 (MSB_UDIMM_RESERVED_TILL - MSB_UDIMM_RESERVED_FROM)]);

	}

	switch(from){
		case SPD_RCE_MNH:
			/* 0x80 */
			msb->rce_mnh = spd[SPD_RCE_MNH];
			SPDT_PRINTF("Raw Card Extension \t %d\n", (msb->rce_mnh >> SPD_RCE_MNH_RCE_OFFS) + 3 );
			tval = (msb->rce_mnh & SPD_RCE_MNH_HEIGHT_MASK) + SPD_RCE_MNH_HEIGHT_INIT_VAL;
			SPDT_PRINTF("Module nominal height \t\t %d mm < height <= %d mm\n", tval, tval+1);
			return SPD_RCE_MNH + 1;

		case SPD_MMT:
			msb->mmt = spd[SPD_MMT];
			tval = (msb->mmt >> SPD_MMT_BACK_OFFS);
			SPDT_PRINTF("Module Maximum Thickness max, Back \t %d mm < thickness <= %d\n", tval, tval + 1);
			tval = (msb->mmt & SPD_MMT_FRONT_MASK);
			SPDT_PRINTF("Module Maximum Thickness max, Front \t %d mm < thickness <= %d\n", tval, tval + 1);
			return SPD_MMT + 1;

		case SPD_RRCU:
			msb->rrcu = spd[SPD_RRCU];
			SPDT_PRINTF("Reference Raw Card Used \t 0x%02X\n", msb->rrcu);
			return SPD_RRCU + 1;

		case SPD_ADDR_MAPPING:
			msb->addr_map_ec_dram = spd[SPD_ADDR_MAPPING];
			SPDT_PRINTF("Address Mapping from Edge Connector to DRAM - %s\n", (msb->addr_map_ec_dram & SPD_ADDR_MAPPING_MIRRORED)?
					"mirrored":"standart");
			return SPD_ADDR_MAPPING + 1;

		case SPD_CRC_MSS_LSB:
			msb->crc_lsb = spd[SPD_CRC_MSS_LSB];
			SPDT_PRINTF("CRC for SPD block 1, LSB \t 0x%02X\n", msb->crc_lsb);
			return SPD_CRC_MSS_LSB + 1;

		case SPD_CRC_MSS_MSB:
			msb->crc_msb = spd[SPD_CRC_MSS_MSB];
			SPDT_PRINTF("CRC for SPD block 1, MSB \t 0x%02X\n", msb->crc_msb);
			return 0;

		default:
			return from + 1;
	}

	return 0;
}
