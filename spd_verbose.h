/*
 * spd_verbose.h
 *
 *  Created on: 09.11.2018
 *      Author: Perepelitsin Roman
 */
#ifndef SPD_VERBOSE_H_
#define SPD_VERBOSE_H_

#include <stdio.h>

#define SPD_VERBOSE_NONE		0
#define SPD_VERBOSE_ERROR 	1
#define	SPD_VERBOSE_INFO 	2
#define SPD_VERBOSE_DEBUG 	3

extern int verbose_level;

void set_verbose_level(int vl);

#define WRAP_PRINTF(x...)	printf(x)
#define WRAP_FFLUSH()

#define SPDT_ERROR(args...)			do{	\
										if(verbose_level >= SPD_VERBOSE_ERROR) {\
											WRAP_PRINTF("SPD_TOOL ERROR:") ; \
											WRAP_PRINTF(args); \
											WRAP_FFLUSH(); \
										} \
									}while(0)


#define SPDT_DEBUG(args...)			do{	\
										if(verbose_level >= SPD_VERBOSE_DEBUG) {\
											WRAP_PRINTF("SPD_TOOL DEBUG:") ; \
											WRAP_PRINTF(args); \
											WRAP_FFLUSH(); \
										} \
									}while(0)


#define SPDT_PRINTF(args...)			do{	\
										if(verbose_level >= SPD_VERBOSE_INFO) {\
											WRAP_PRINTF(args); \
											WRAP_FFLUSH(); \
										} \
									}while(0)


#endif /* SPD_VERBOSE_H_ */
