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
    FONT_RES_ERR_READ_FILE
} FONT_RES;


extern int Load_Font_File(void);
