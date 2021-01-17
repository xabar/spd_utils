/*
 * spd_msb_udimm.h
 *
 *  Created on: 09.11.2018
 *      Author: Perepelitsin Roman
 */
#ifndef SPD_MSB_UDIMM_H_
#define SPD_MSB_UDIMM_H_

#include <stdint.h>

typedef struct {
	uint8_t	rce_mnh;	/* 0x80 */
	uint8_t mmt;		/* 0x81 */
	uint8_t rrcu;		/* 0x82 */
	uint8_t addr_map_ec_dram;	/* 0x83 */
	uint8_t reserved_132_253[122];	/* 0x84-0xFC */
	uint8_t crc_lsb;	/* 0xfe */
	uint8_t crc_msb;	/* 0xff */
}msb_udimm;

uint16_t decode_msb_udimm_case(uint8_t *spd, msb_udimm *msb, uint16_t from, int show_fild_names);
int is_udimm_memory(uint8_t *spd);

#endif /* SPD_MSB_UDIMM_H_ */
