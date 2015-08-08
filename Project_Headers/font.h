/*
 * font.h
 *
 *  Created on: Aug 7, 2015
 *      Author: Jian
 */

#pragma once


#define FONT_HEIGHT             (16)
#define FONT_WIDTH              (16)
#define FONT_DATA_BUFF_LENGTH   (16)

typedef enum
{
    FONT_RES_OK = 0,
    FONT_RES_ERR_READ_FILE,
    FONT_RES_ERR_FONT_NOT_LOAD
} FONT_RES;


extern FONT_RES Load_Font_File(void);
FONT_RES Get_Font_16x16(TCHAR unicode, uint8_t *buff);
