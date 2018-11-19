/*
 * spd_msd.h
 *
 *  Created on: 12.11.2018
 *      Author: Perepelitsin Roman
 */
#ifndef SPD_MSD_H_
#define SPD_MSD_H_


#include <stdint.h>

typedef struct	{
	uint8_t mm_id_lsb;	/* 0x140 */
	uint8_t mm_id_msb;  /* 0x141 */
	uint8_t mm_loc;		/* 0x142 */
	uint8_t mm_date0;	/* 0x143 */
	uint8_t mm_date1;	/* 0x144 */
	uint8_t m_sn[0];	/* 0x145 - 0x148*/
	uint8_t m_pn[20];	/* 0x149-0x15C */
	uint8_t	m_revcode;	/* 0x15D */
	uint8_t dram_mm_id_lsb;	/* 0x15E */
	uint8_t dram_mm_id_msb;	/* 0x15F */
	uint8_t dram_stepping;	/* 0x160 */
	uint8_t m_spec[28];		/* 0x161-0x17D */
	uint8_t reserved_382_383[2];	/* 0x17E-0x17F */
}spd_msd;


uint16_t decode_msd_case(uint8_t *spd, spd_msd *msd, uint16_t from, int show_fild_names);

#endif /* SPD_MSD_H_ */
