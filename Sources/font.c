/*
 * font.c
 *
 *  Created on: Aug 7, 2015
 *      Author: Jian
 */


#define         __FONT_C_
#include        "includes.h"


const static TCHAR Font_File_Path[] = L"方正像素16_U16.bin";
static uint8_t Font_Data[FONT_DATA_BUFF_LENGTH][FONT_HEIGHT * FONT_WIDTH / 8];


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
    uint32_t Width : 6;
    uint32_t OffAddr : 26;
} tagUflCharInfo;


static uint32_t __bswap_32(uint32_t x);
static uint16_t __bswap_16(uint16_t x);


int Load_Font_File(void)
{
    FIL fil1;
    UINT br1;
    FRESULT fr1;
    tagFontLibHeader tagFontLibHeader1;
    tagFlSectionInfo tagFlSectionInfo1;
    tagUflCharInfo tagUflCharInfo1;
    const char font_mark[] = "UFL";
    const TCHAR load_list[] = L"估E一陈鹏智能车十";
    uint32_t tmp = 0x00000000;
    int i;
    
    fr1 = f_open(&fil1, Font_File_Path, FA_READ);
    if (FR_OK != fr1)
    {
        return FONT_RES_ERR_READ_FILE;
    }
    
    /* 加载文件头 */
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
    
    /* 加载段信息 */
    fr1 = f_lseek(&fil1, sizeof(tagFontLibHeader1));
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
    tagFlSectionInfo1.First     = __bswap_16(tagFlSectionInfo1.First);
    tagFlSectionInfo1.Last      = __bswap_16(tagFlSectionInfo1.Last);
    tagFlSectionInfo1.OffAddr   = __bswap_32(tagFlSectionInfo1.OffAddr);
    
    /* 加载检索表 */
    for (i = 0; i < FONT_DATA_BUFF_LENGTH; i++)
    {
        if (load_list[i] > tagFlSectionInfo1.Last || load_list[i] < tagFlSectionInfo1.First)
        {
            f_close(&fil1);
            return FONT_RES_ERR_READ_FILE;
        }
        tmp = tagFlSectionInfo1.OffAddr + (load_list[i] - tagFlSectionInfo1.First) * sizeof(tagUflCharInfo);
        fr1 = f_lseek(&fil1, tmp);
        if (FR_OK != fr1)
        {
            f_close(&fil1);
            return FONT_RES_ERR_READ_FILE;
        }
        fr1 = f_read(&fil1, &tagUflCharInfo1, sizeof(tagUflCharInfo1), &br1);
        if (FR_OK != fr1)
        {
            f_close(&fil1);
            return FONT_RES_ERR_READ_FILE;
        }
        *(uint32_t *)&tagUflCharInfo1 = __bswap_32(*(uint32_t *)&tagUflCharInfo1);
        
        /* 加载点阵 */
        fr1 = f_lseek(&fil1, tagUflCharInfo1.OffAddr);
        if (FR_OK != fr1)
        {
            f_close(&fil1);
            return FONT_RES_ERR_READ_FILE;
        }
        fr1 = f_read(&fil1, (void *)Font_Data[i], sizeof(Font_Data[i]), &br1);
        if (FR_OK != fr1)
        {
            f_close(&fil1);
            return FONT_RES_ERR_READ_FILE;
        }
    }
}


static uint32_t __bswap_32(uint32_t x)
{
    uint32_t tmp = 0x00000000;
    
    tmp |= (x >> 24) & 0x000000FF;
    tmp |= (x >> 8 ) & 0x0000FF00;
    tmp |= (x << 8 ) & 0x00FF0000;
    tmp |= (x << 24) & 0xFF000000;
    return tmp;
}


static uint16_t __bswap_16(uint16_t x)
{
    uint16_t tmp = 0x0000;
    
    tmp |= (x >> 8) & 0x00FF;
    tmp |= (x << 8) & 0xFF00;
    return tmp;
}
