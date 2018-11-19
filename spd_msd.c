/*
 * spd_msd.c
 *
 *  Created on: 12.11.2018
 *      Author: Perepelitsin Roman
 */

#include <strings.h>
#include "spd_msd.h"
#include "spd.h"
#include "spd_verbose.h"



static char *msd_field_names[]	=	{
		"Module Manufacturer ID Code, LSB",
		"Module Manufacturer ID Code, MSB",
		"Module Manufacturing Location",
		"Module Manufacturing Date",
		"",
		"Module Serial Number",
		"",	"",	"",
		"Module Part Number",
		"",	"","","","","","","","",
		"",	"","","","","","","","","",
		"Module Revision Code",
		"DRAM Manufacturer ID Code, LSB",
		"DRAM Manufacturer ID Code, MSB",
		"DRAM Stepping",
		"Manufacturer’s Specific Data",
};


uint16_t decode_msd_case(uint8_t *spd, spd_msd *msd, uint16_t from, int show_fild_names)
{
	int i;


	if(show_fild_names){
		if((from >= SPD_MM_SPECIFIC_START) && (from <= (SPD_MM_SPECIFIC_START + SPD_MM_SPECIFIC_LEN)))	{
			SPDT_PRINTF("\n[0x%02X:%d]\t%s\n", from, from, msd_field_names[SPD_MM_SPECIFIC_START - SPD_MSD_START_OFFS]);
		}
		else	if(from > SPD_MM_SPECIFIC_START + SPD_MM_SPECIFIC_LEN){
			SPDT_PRINTF("\n[0x%02X:%d]\t%s\n", from, from, "reserved");
		} else	{
			SPDT_PRINTF("\n[0x%02X:%d]\t%s\n", from, from, msd_field_names[from - SPD_MSD_START_OFFS]);
		}
	}

	switch(from){
		case SPD_MM_ID_LSB:
			msd->mm_id_lsb = spd[SPD_MM_ID_LSB];
			SPDT_PRINTF("MM ID Code, LSB \t 0x%02x\n", msd->mm_id_lsb );
			return SPD_MM_ID_LSB + 1;

		case  SPD_MM_ID_MSB:
			msd->mm_id_msb = spd[SPD_MM_ID_MSB];
			SPDT_PRINTF("MM ID Code, MSB \t 0x%02x\n", msd->mm_id_msb );
			return SPD_MM_ID_MSB + 1;

		case SPD_MM_LOC:
			msd->mm_loc = spd[SPD_MM_LOC];
			SPDT_PRINTF("MM  Location \t 0x%02x\n", msd->mm_loc);
			return SPD_MM_LOC + 1;

		case SPD_MM_DATE0:
			msd->mm_date0 = spd[SPD_MM_DATE0];
			SPDT_PRINTF("MM date0 \t 0x%02x\n", msd->mm_date0);
			return SPD_MM_DATE0 + 1;

		case SPD_MM_DATE1:
			msd->mm_date1 = spd[SPD_MM_DATE1];
			SPDT_PRINTF("MM date1 \t 0x%02x\n", msd->mm_date1);
			return SPD_MM_DATE1 + 1;

		case SPD_M_SN0:
			SPDT_PRINTF("MSerial number: ");
			for(i = 0; i < 5; i++)	{
				msd->m_sn[i] = spd[SPD_M_SN0 + 1];
				SPDT_PRINTF("0x%02x ", msd->m_sn[i]);
			}
			SPDT_PRINTF("\n");
			return SPD_M_SN3 + 1;

		case SPD_M_PNUM_START:
			SPDT_PRINTF("MPart number number: ");
			for(i = 0; i <= SPD_M_PNUM_LEN; i++)	{
				msd->m_pn[i] = spd[SPD_M_PNUM_START + i];
				SPDT_PRINTF("%c",msd->m_pn[i]);
			}
			SPDT_PRINTF("\n");
			return SPD_M_PNUM_START + SPD_M_PNUM_LEN + 1;

		case SPD_M_REVCODE:
			msd->m_revcode = spd[SPD_M_REVCODE];
			SPDT_PRINTF("revcode \t 0x%02x\n", msd->m_revcode);

			return SPD_M_REVCODE + 1;

		case SPD_DRAMM_ID_LSB:
			msd->dram_mm_id_lsb = spd[SPD_DRAMM_ID_LSB];
			SPDT_PRINTF("DRAM MM ID, LSB \t 0x%02x\n", msd->dram_mm_id_lsb);
			return SPD_DRAMM_ID_LSB + 1;

		case SPD_DRAMM_ID_MSB:
			msd->dram_mm_id_msb = spd[SPD_DRAMM_ID_MSB];
			SPDT_PRINTF("DRAM MM ID, MSB \t 0x%02x\n", msd->dram_mm_id_msb);
			return SPD_DRAMM_ID_MSB + 1;

		case SPD_DRAM_STEPPING:
			msd->dram_stepping = spd[SPD_DRAM_STEPPING];
			SPDT_PRINTF("DRAM stepping \t 0x%02x\n", msd->dram_stepping);
			return SPD_DRAM_STEPPING + 1;

		case SPD_MM_SPECIFIC_START:
			SPDT_PRINTF("MM spicific data: ");
			for(i = 0; i <= SPD_MM_SPECIFIC_LEN; i++)	{
				msd->m_spec[i] = spd[SPD_MM_SPECIFIC_START + i];
				SPDT_PRINTF("%c", msd->m_spec[i]);
			}
			SPDT_PRINTF("\n");
			return SPD_MM_SPECIFIC_START + SPD_MM_SPECIFIC_LEN + 1;

		case SDP_MSD_END:
			return 0;

		default:
			return from + 1;

	}

	return 0;

}
