/*
 * font.c
 *
 *  Created on: Aug 7, 2015
 *      Author: Jian
 */


#define         __FONT_C_
#include        "includes.h"


const static TCHAR Font_File_Path[] = L"方正像素16_U16.bin";


typedef struct
{
    uint8_t magic[4];
    uint32_t dwFileSize;
    uint8_t nSection;
    uint8_t YSize;
    uint16_t wCpFlag;
    uint16_t nTotalChars;
    uint8_t reserved[2];
} tagFontLibHeader;

typedef struct
{
    uint16_t First;
    uint16_t Last;
    uint32_t OffAddr;
} tagFlSectionInfo;

typedef struct
{
    uint32_t OffAddr : 26;
    uint32_t Width : 6;
} tagUflCharInfo;


int Load_Font_File(void)
{
    FIL fil1;
    UINT br1;
    FRESULT fr1;
    tagFontLibHeader tagFontLibHeader1;
    tagFlSectionInfo tagFlSectionInfo1;
    const char font_mark[] = "UFL";
    const TCHAR load_list[] = L"陈鹏智能车十一";
    
    fr1 = f_open(&fil1, Font_File_Path, FA_READ);
    if (FR_OK != fr1)
    {
        return FONT_RES_ERR_READ_FILE;
    }
    
    fr1 = f_read(&fil1, &tagFontLibHeader1, sizeof(tagFontLibHeader1), &br1);
    if (FR_OK != fr1)
    {
        return FONT_RES_ERR_READ_FILE;
    }
    else if (sizeof(tagFontLibHeader1) != br1)
    {
        f_close(&fil1);
        return FONT_RES_ERR_READ_FILE;
    }
    else if (strncmp((const char *)tagFontLibHeader1.magic, font_mark, 3))
    {
        return FONT_RES_ERR_READ_FILE;
    }
    else if (!tagFontLibHeader1.nSection)
    {
        return FONT_RES_ERR_READ_FILE;
    }
    else if (FONT_HEIGHT != tagFontLibHeader1.YSize)
    {
        return FONT_RES_ERR_READ_FILE;
    }
    else if (!(tagFontLibHeader1.wCpFlag & 0x0200))
    {
        return FONT_RES_ERR_READ_FILE;
    }
    
    fr1 = f_lseek(&fil1, sizeof(tagFlSectionInfo1));
    if (FR_OK != fr1)
    {
        f_close(&fil1);
        return FONT_RES_ERR_READ_FILE;
    }
    fr1 = f_read(&fil1, &tagFlSectionInfo1, sizeof(tagFlSectionInfo1), &br1);
    if (FR_OK != fr1)
    {
        f_close(&fil1);
        return FONT_RES_ERR_READ_FILE;
    }

//    fr1 = f_mount(&fatfs_1, mmc, 1);
//    fr1 = f_open(&fil_1, path2, FA_OPEN_ALWAYS);
//    fr1 = f_close(&fil_1);
//    fr1 = f_open(&fil_2, path2, FA_WRITE);
//    fr1 = f_write(&fil_2, test_line, sizeof(test_line)-1, &bw);
//    fr1 = f_close(&fil_2);
}
