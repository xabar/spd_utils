/*
 * spd_tool.c
 *
 *  Created on: 06.11.2018
 *      Author: Perepelitsin Roman
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "spd.h"
#include "spd_verbose.h"


#define INP_FMT_BIN_PARAM	"bin"
#define INP_FMT_BIN_ID		0x0
#define INP_FMT_HEX_PARAM	"hex"
#define INP_FMT_HEX_ID		0x1
#define INP_FMT_UNDEF_ID	0xff



void *parse_input_bin(void *fa, int inp_len, int *out_len)
{
	unsigned char *out = NULL;

	out = malloc(1024);
	memset(out, 0x0, 1024);

	memcpy(out, fa, inp_len);

	return out;
}

void *parse_input_hex(void *fa, int inp_len, int *out_len)
{
	unsigned char *p = fa;
	unsigned char *out = NULL;
	int proc = 0, out_pos = 0;
	unsigned char comment = 0, elem, half = 0;

	SPDT_DEBUG("start parse hex SPD...\n");


	out = malloc(1024);
	memset(out, 0x0, 1024);


	do{

		if(comment){
			if(p[proc] == 0x0a)	{
				comment = 0;
			}
			proc++;
			continue;
		}

		if( p[proc] == '#'){
			comment = 1;
			proc++;
			continue;
		}

		if(p[proc] >= 0x30)	{
			if(isxdigit(elem = toupper(p[proc])))	{
				elem = elem - 0x30;
				if(elem > 9)
					elem = elem - 7;
			}
			else	{
				SPDT_ERROR("Error in SPD HEX file format! elem %d\n", elem);
				goto _err;
			}

			if(half == 0)	{
				out[out_pos] = ( elem << 4 ) & 0xf0;
				half = 1;
			} else	{
				out[out_pos] = out[out_pos] | elem;
				half = 0;
				out_pos++;
			}
			proc++;
			continue;
		}

		proc++;

	}while(proc < inp_len );


	*out_len = out_pos;
	SPDT_DEBUG("SPD length (BIN) - %d\n", out_pos);

	return (void*)out;

_err:
	free(out);
	return NULL;
}


int parse_input_spd(char *spd_file, char input_fmt, void **spd)
{
	int fd, len = 0;
	struct stat sb;
	ssize_t fsize;
	void *fa;

	fd = open(spd_file, O_RDONLY);
	if(fd < 0){
		SPDT_ERROR("Can't open input SDP file - %s\n", strerror(fd));
		return -1;
	}

	if(fstat(fd, &sb) == -1)	{
		SPDT_ERROR("fstat error - %s", strerror(errno));
		return -1;
	}

	fsize = sb.st_size;
	SPDT_DEBUG("input fsize - %ld bytes\n", fsize);

	fa = mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0x0);
	if(fa == NULL) {
		SPDT_ERROR("mmap failed\n");
		return -1;
	}

	SPDT_DEBUG("mmap OK\n");

	if(input_fmt == INP_FMT_HEX_ID)	{
		*spd = parse_input_hex(fa, fsize, &len);
	} else if(input_fmt == INP_FMT_BIN_ID)	{
		*spd = parse_input_bin(fa, fsize, &len);
	}

	munmap(fa, fsize);

	if(*spd == NULL)	{
		SPDT_ERROR("Error at parsing SDP file\n");
		return 0;
	}
	SPDT_DEBUG("SPD ptr %p, sz - %d\n", spd, len);
	return len;
}


int spd_to_file(char *out_file, void *spd, unsigned int spd_len, int bin_fmt)
{
	int ret = 0;
	FILE *f;


	if(out_file == NULL){
		return -1;
	}

	f = fopen(out_file, "w");

	if(f == NULL){
		SPDT_ERROR("Can't open out file - %s\n", strerror(errno));
		return -1;
	}

	if(bin_fmt){
		ret = fwrite(spd, spd_len, 1, f);
		if(ret == 1){
			SPDT_DEBUG("Write to file %s %d bytes\n", out_file, spd_len);
		} else	{
			SPDT_DEBUG("Error at writing to file %s\n", out_file);
		}
	} else	{
		for(ret = 0; ret < spd_len; ret++)	{

			fprintf(f ,"%02X ", ((unsigned char *)spd)[ret]);
			if((ret & 0x0f) == 0x0f)	{
				fprintf(f,"\n");
			}
		}
	}

	fclose(f);

	return 0;


}



void show_help()
{
	printf("Usage:\n");
	printf("\t -D - decode whole SPD from input file\n");
	printf("\t -i - SPD input file\n");
	printf("\t -f - SPD input file format: hex, bin\n");
	printf("\t -s - start decode SPD from byte\n");
	printf("\t -e - decode SPD to byte\n");
	printf("\t -x - show extra information on decode\n");
	printf("\t -E - convert SPD from input file to output file (with re-calc CRC sections)\n");
	printf("\t -o - output file\n");
	printf("\t -F - SPD output file format: hex, bin\n");
	printf("\t -v - verbose level (default - 2)\n\n");

}

void show_about()
{
	printf("\nSPD-TOOL utility for DDR4 (unbuffered).\n\n");
	printf("SPD HEX format - CoreBoot SPD (# - comment line, hex values w/o 0x) \n");
	printf("SPD BIN format - binary file w/o extra information\n");
	printf("For full and detailed bytes description use JEDEC DDR4 SDRAM Annex L\n");
	show_help();
	printf("example: \n"
			"decode HEX SPD - ./spd_tool -D -i ./spd.hex -f hex -x\n"
			"update CRC in HEX - ./spd_tool -E -i ./spd.hex -f hex -o ./spd.hex -F hex\n\n");
	printf("written by Roman Perepelitsin (perepelitsin.roman@gmail.com)\n");
}


int main(int argc, char *argv[])
{
	int c, val, spd_bin_len, show_xtra = 0;
	char *input_file = NULL, *output_file = NULL;
	char decode_opt = 0, encode_opt = 0, input_fmt = INP_FMT_UNDEF_ID, output_fmt = INP_FMT_UNDEF_ID;
	void *spd_bin = NULL;
	uint16_t s_offs = 0, e_offs = 0;

	set_verbose_level(SPD_VERBOSE_INFO);

	while ((c = getopt(argc, argv, "hDEi:o:f:F:xs:e:v:")) != -1) {
		switch (c) {
			case 'f':
				if(strcmp(optarg, INP_FMT_BIN_PARAM) == 0){
					SPDT_DEBUG("Using input bin fmt\n");
					input_fmt = INP_FMT_BIN_ID;
				} else	if(strcmp(optarg, INP_FMT_HEX_PARAM) == 0)	{
					SPDT_DEBUG("Using input hex fmt\n");
					input_fmt = INP_FMT_HEX_ID;
				} else	{
					SPDT_ERROR("Unknown input format!\n");
					show_help();
					goto _error;
				}
				break;
			case 'F':
				if(strcmp(optarg, INP_FMT_BIN_PARAM) == 0){
					SPDT_DEBUG("Using output bin fmt\n");
					output_fmt = INP_FMT_BIN_ID;
				} else	if(strcmp(optarg, INP_FMT_HEX_PARAM) == 0)	{
					SPDT_DEBUG("Using output hex fmt\n");
					output_fmt = INP_FMT_HEX_ID;
				} else	{
					SPDT_ERROR("Unknown output format!\n");
					show_help();
					goto _error;
				}
				break;
			case 'D':
				decode_opt = 1;
				break;
			case 'E':
				encode_opt = 1;
				break;
			case 'i':
				val = strlen(optarg);
				SPDT_DEBUG("fname len = %d\n", val);
				if(val > 0)	{
					if((input_file = malloc(val))== NULL)	{
						SPDT_ERROR("malloc error\n");
						goto _error;
					}
					strcpy(input_file, optarg);
					SPDT_DEBUG("input file - %s\n", input_file);
				}
				break;
			case 'o':
				val = strlen(optarg);
				SPDT_DEBUG("fname len = %d\n", val);
				if(val > 0)	{
					if((output_file = malloc(val))== NULL)	{
						SPDT_ERROR("malloc error\n");
						goto _error;
					}
					strcpy(output_file, optarg);
					SPDT_DEBUG("output file - %s\n", output_file);
				}
				break;
			case 'x':
				show_xtra = 1;
				break;
			case 's':
				s_offs = atoi(optarg);
				break;
			case 'e':
				e_offs = atoi(optarg);
				break;
			case 'v':
				set_verbose_level(atoi(optarg));
				break;
			case 'h':
				show_about();
				return 0;
			default:
				printf("No args\n");
				break;
		}
	}

	if(decode_opt & encode_opt)	{
		SPDT_ERROR("You must specify only one options D or E\n");
		show_help();
		goto _error;
	}


	if(decode_opt || encode_opt)	{
		if(input_fmt == INP_FMT_UNDEF_ID || input_file == NULL )	{
			SPDT_ERROR("wrong input param args\n");
			show_help();
			goto _error;
		}

		spd_bin_len = parse_input_spd(input_file, input_fmt, &spd_bin);
		if(spd_bin == NULL || spd_bin_len == 0){

			goto _error;
		}
		SPDT_DEBUG("Input SPD file converted done - size %d bytes\n", spd_bin_len);

		if(decode_opt)	{
			decode_spd((uint8_t *)spd_bin, show_xtra, s_offs, e_offs, spd_bin_len);
		} else	{
			if(output_fmt == INP_FMT_UNDEF_ID || output_file == NULL )	{
				SPDT_ERROR("wrong output param args\n");
				show_help();
				goto _error;
			}

			encode_spd((uint8_t *)spd_bin, spd_bin_len);
			spd_to_file(output_file, spd_bin, spd_bin_len, !output_fmt);
		}


	}



	/* clean here */
_error:
	if(input_file != NULL)	{
		free(input_file);
	}

	if(output_file != NULL)	{
		free(output_file);
	}

	if(spd_bin != NULL)	{
		free(spd_bin);
	}

	return 0;

}
