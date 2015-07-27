/*
 * SDCard.h
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */

#pragma once


#define SDCARD_DSPI_PUSHR_CONT                          (1)
#define SDCARD_DSPI_PUSHR_PCS                           (DSPI_PUSHR_PCS_0)
#define SDCARD_DSPI_CTAR_DBR                            (0b0)
#define SDCARD_DSPI_CTAR_CPOL                           (0b0)
#define SDCARD_DSPI_CTAR_CPHA                           (0)
#define SDCARD_DSPI_CTAR_LSBFE                          (0b0)
#define SDCARD_DSPI_CTAR_PCSSCK                         (0b00)
#define SDCARD_DSPI_CTAR_PASC                           (0b00)
#define SDCARD_DSPI_CTAR_PDT                            (0b10)
#define SDCARD_DSPI_CTAR_PBR                            (0b00)
#define SDCARD_DSPI_CTAR_CSSCK                          (0b0111)
#define SDCARD_DSPI_CTAR_ASC                            (0b0111)
#define SDCARD_DSPI_CTAR_DT                             (0b0010)
#define SDCARD_DSPI_CTAR_BR                             (0b0110)


extern      int                 Test_SDCard(void);
extern      int                 Test_SDCard_Send_Cmd(int cmd, uint32_t var, uint32_t *rev);
extern      int                 Reset_SDCard(void);