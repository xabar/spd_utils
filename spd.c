/*
 * spd.c
 *
 *  Created on: 07.11.2018
 *      Author: Perepelitsin Roman
 */

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "spd.h"
#include "spd_bcs.h"
#include "spd_msb_udimm.h"
#include "spd_msd.h"
#include "spd_verbose.h"

int crc16(uint8_t *ptr, int count)
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


uint16_t calc_spd_crc_bcs(uint8_t *spd, uint8_t *crc_lsb, uint8_t *crc_msb)
{
	uint16_t ret = (uint16_t)crc16(spd, 126);

	*crc_lsb = ret & 0xff;
	*crc_msb = ret >> 8;

	return ret;
}

uint16_t calc_spd_crc_mss(uint8_t *spd, uint8_t *crc_lsb, uint8_t *crc_msb)
{
	uint16_t ret = (uint16_t)crc16(&spd[128], 126);

	*crc_lsb = ret & 0xff;
	*crc_msb = ret >> 8;

	return ret;
}


void update_crc(uint8_t *spd)
{
	uint8_t crc_msb, crc_lsb;

	calc_spd_crc_bcs(spd, &crc_lsb, &crc_msb);
	SPDT_PRINTF("RE-CALC CRC of BCS - lsb 0x%02X msb - 0x%02X\n", crc_lsb, crc_msb);
	if((spd[SPD_CRC_BCS_LSB] == crc_lsb) && (spd[SPD_CRC_BCS_MSB] == crc_msb))	{
		SPDT_PRINTF("CRC of BCS OK\n");
	} else	{
		SPDT_PRINTF("CRC of BCS BROKEN!\nUpdate BCS CRC...\n");
		spd[SPD_CRC_BCS_LSB] = crc_lsb;
		spd[SPD_CRC_BCS_MSB] = crc_msb;
	}

	calc_spd_crc_mss(spd, &crc_lsb, &crc_msb);
	SPDT_PRINTF("RE-CALC CRC of section#2 - lsb 0x%02X msb - 0x%02X\n", crc_lsb, crc_msb);
	if((spd[SPD_CRC_MSS_LSB] == crc_lsb) && (spd[SPD_CRC_MSS_MSB] == crc_msb))	{
		SPDT_PRINTF("CRC of section#2 OK\n");
	} else	{
		SPDT_PRINTF("CRC of section#2 BROKEN!\nUpdate BCS CRC...\n");
		spd[SPD_CRC_MSS_LSB] = crc_lsb;
		spd[SPD_CRC_MSS_MSB] = crc_msb;
	}
}

/* just update CRC */
int encode_spd(uint8_t *spd, int size)
{
	update_crc(spd);

	return 0;
}

int decode_spd(uint8_t *spd, int show_fild_names, uint16_t from, uint16_t to, int size)
{
	spd_bcs *bcs;
	msb_udimm *msb;
	spd_msd *msd;
	uint16_t pos = from;

	bcs = malloc(sizeof(spd_bcs));
	memset((void *)bcs, 0x0, sizeof(spd_bcs));

	SPDT_PRINTF("\n*** SPD BASE CONFIGURATION SECTION ***\n");

	do{
		pos = decode_bcs_case(spd, bcs, pos, show_fild_names);
	}while(pos != to);

	if(is_udimm_memory(spd))	{
		SPDT_PRINTF("\n*** SPD MODULE SPECIFIC BYTES FOR UNBUFFERED MEMORY ***\n");
		msb = malloc(sizeof(msb_udimm));
		pos = SPD_MSB_START_OFFS;
		do{
			pos = decode_msb_udimm_case(spd, msb, pos, show_fild_names);
		}while(pos != to);
	} else	{
		SPDT_PRINTF("\nCurrent modules type don't support in this version of SPD_TOOL\n");
		decode_bcs_case(spd, bcs, SPD_MODULE_TYPE, 0);
		decode_bcs_case(spd, bcs, SPD_DEVTYPE, 0);
	}

	if(size > SDP_MSD_END)	{
		SPDT_PRINTF("\n*** SPD Extended Function Parameters Block ***\n");
		msd = malloc(sizeof(spd_msd));
		pos = SPD_MSD_START_OFFS;
		do{
			pos = decode_msd_case(spd, msd, pos,  show_fild_names  );
		}while(pos != to);
	}
	SPDT_PRINTF("\n*** SPD CRC check ***\n");
	update_crc(spd);

	return 0;
}

