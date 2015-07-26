/*
 * oled.h
 *
 *  Created on: Jul 23, 2015
 *      Author: Jian
 */

#pragma once


#define OLED_PAGE_MAX                                   (8)
#define OLED_SEG_MAX                                    (128)
#define OLED_COM_MAX                                    (64)
#define OLED_PIXEL_APPEARANCE_VISIBLE                   (1)
#define OLED_PIXEL_APPEARANCE_INVISIBLE                 (0)
#define OLED_PIXEL_APPEARANCE_COLOR_YELLOW              (OLED_PIXEL_APPEARANCE_VISIBLE)
#define OLED_PIXEL_APPEARANCE_COLOR_BLUE                (OLED_PIXEL_APPEARANCE_INVISIBLE)
#define OLED_PIN_DC                                     (SIU.GPDO[OLED_PIN_DC_PCR].B.PDO)
#define OLED_PIN_DC_PCR                                 (43)
#define OLED_PIN_RST                                    (SIU.GPDO[OLED_PIN_RST_PCR].B.PDO)
#define OLED_PIN_RST_PCR                                (37)
#define OLED_DC_COMMAND                                 (0)
#define OLED_DC_DATA                                    (1)
#define OLED_DSPI                                       (DSPI_1)
#define OLED_DSPI_PUSHR_CONT                            (1)
#define OLED_DSPI_PUSHR_CTAS                            (3)
#define OLED_DSPI_PUSHR_EOQ                             (0)
#define OLED_DSPI_PUSHR_CTCNT                           (0)
#define OLED_DSPI_PUSHR_PCS                             (1)
#define OLED_DSPI_PUSHR                                 ((uint32_t)(0x00000000) | (OLED_DSPI_PUSHR_CONT << 15) | (OLED_DSPI_PUSHR_CTAS << 12) | (OLED_DSPI_PUSHR_EOQ << 11) | (1 << PCS))
#define OLED_DSPI_CTAR_DBR                              (0b0)
#define OLED_DSPI_CTAR_CPOL                             (0b0)
#define OLED_DSPI_CTAR_CPHA                             (0)
#define OLED_DSPI_CTAR_LSBFE                            (0b0)
#define OLED_DSPI_CTAR_PCSSCK                           (0b00)
#define OLED_DSPI_CTAR_PASC                             (0b00)
#define OLED_DSPI_CTAR_PDT                              (0b10)
#define OLED_DSPI_CTAR_PBR                              (0b10)
#define OLED_DSPI_CTAR_CSSCK                            (0b0111)
#define OLED_DSPI_CTAR_ASC                              (0b0111)
#define OLED_DSPI_CTAR_DT                               (0b0010)
#define OLED_DSPI_CTAR_BR                               (0b0100)
#define TASK_STK_SIZE_OLED_FLUSH_MEM                    (1024)
#define TASK_PRIO_OLED_FLUSH_MEM                        (15)
#define TASK_PRIO_MUTEX_OLED_DIS_MEM                    (3)

extern OS_STK TaskStk_OLED_Flush_Mem[];

extern int Init_OLED(void);
extern int Init_OLED_Pin(void);
extern int Fill_OLED_PAGE(int page, int seg_start, int seg_cnt, int pixel_appearance);
extern void Task_OLED_Flush_Mem(void *p_arg);
extern void Task_OLED_Flush_Mem(void *p_arg);
extern void Test_OLED_Init(void);
extern void Resume_Task_OLED_Flush_Mem(void);
