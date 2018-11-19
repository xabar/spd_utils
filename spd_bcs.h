/*
 * spd_bcs.h
 *
 *  Created on: 07.11.2018
 *      Author: Perepelitsin Roman
 */
#ifndef SPD_BCS_H_
#define SPD_BCS_H_

#include <stdint.h>

typedef struct	{
	uint8_t bytes;		 /* 0x0 */
	uint8_t revision; 	 /* 0x1 */
	uint8_t dram_type; 	 /* 0x2 */
	uint8_t module_type; /* 0x3 */
	uint8_t dens_bansks; /* 0x4 */
	uint8_t sdram_addressing; 		/* 0x5 */
	uint8_t prim_sdram_pack_type;	/* 0x6 */
	uint8_t	sdram_opt_feat;			/* 0x7 */
	uint8_t sdram_thermal_and_refresh_opt; /* 0x8 --- reserved always 0x00 */
	uint8_t sdram_other_feat;		/* 0x9 */
	uint8_t secnd_sdram_pack_type; 	/* 0xA */
	uint8_t vdd;					/* 0xB */
	uint8_t module_org;				/* 0xC */
	uint8_t module_mem_buswidth;	/* 0xD */
	uint8_t module_thermal_sens;	/* 0xE */
	uint8_t ext_module_type;		/* 0xF  --- always 0x00*/
	uint8_t byte16;					/* 0x10  --- reseved always 0x00 */
	uint8_t	timebases;				/* 0x11 */
	uint8_t	tckavg_min;				/* 0x12 */
	uint8_t	tckavg_max;				/* 0x13 */
	uint8_t	cas_lat_1;				/* 0x14 */
	uint8_t	cas_lat_2;				/* 0x15 */
	uint8_t	cas_lat_3;				/* 0x16 */
	uint8_t	cas_lat_4;				/* 0x17 */
	uint8_t taa_min;				/* 0x18 */
	uint8_t trcd_min;				/* 0x19 */
	uint8_t	trp_min;				/* 0x1A	*/
	uint8_t tras_trc_up;			/* 0x1B */
	uint8_t tras_min;				/* 0x1C */
	uint8_t trc_min;				/* 0x1D */
	uint8_t trfc1_min_lsb;			/* 0x1E */
	uint8_t trfc1_min_msb;			/* 0x1F */
	uint8_t trfc2_min_lsb;			/* 0x20 */
	uint8_t trfc2_min_msb;			/* 0x21 */
	uint8_t trfc4_min_lsb;			/* 0x22 */
	uint8_t trfc4_min_msb;			/* 0x23 */
	uint8_t tfaw_min_upper;			/* 0x24 */
	uint8_t tfaw_min;				/* 0x25 */
	uint8_t trrd_s_min;				/* 0x26 */
	uint8_t trrd_l_min;				/* 0x27 */
	uint8_t tccd_l_min;				/* 0x28 */
	uint8_t twr_min_upper;			/* 0x29 */
	uint8_t twr_min;				/* 0x2A */
	uint8_t twtr_s_min_upper;		/* 0x2B */
	uint8_t twtr_s_min;				/* 0x2C */
	uint8_t twtr_l_min;				/* 0x2D */
	uint8_t	reserved_46_59[14];		/* 0x2E-0x3B */
	uint8_t	conn_bit_mapping[18];	/* 0x3C-0x4D */
	uint8_t reserved_78_116[39];	/* 0x4E-0x74 */
	uint8_t	tccd_l_min_fine;		/* 0x75 */
	uint8_t	trrd_l_min_fine;		/* 0x76 */
	uint8_t	trrd_s_min_fine;		/* 0x77 */
	uint8_t	trc_min_fine;			/* 0x78 */
	uint8_t	trp_min_fine;			/* 0x79 */
	uint8_t	trcd_min_fine;			/* 0x7A */
	uint8_t	taa_min_fine;			/* 0x7B */
	uint8_t	tckavg_max_fine;		/* 0x7C */
	uint8_t	tckavg_min_fine;		/* 0x7D */
	uint8_t crc_lsb;				/* 0x7E */
	uint8_t crc_msb;				/* 0x7F */

}spd_bcs;

int decode_bcs(uint8_t *spd, spd_bcs *bcs);
uint16_t decode_bcs_case(uint8_t *spd, spd_bcs *bcs, uint16_t from, int show_fild_names);

#endif /* SPD_BCS_H_ */
