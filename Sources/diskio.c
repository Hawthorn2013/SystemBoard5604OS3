/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

//#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */
#include    "includes.h"


/* Definitions of physical drive number for each drive */
#define ATA		1	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		0	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
	    stat = STA_NOINIT;
		return stat;

	case MMC :
	    result = Get_SDCard_Status();
	    if (SDCARD_STATUS_READY == result)
	    {
	        stat = RES_OK;
	    }
	    else
	    {
	        stat = STA_NOINIT;
	    }

		return stat;

	case USB :
	    stat = STA_NOINIT;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
	    stat = STA_NOINIT;
		return stat;

	case MMC :
	    result = Init_SDCard();
	    if (SDCARD_RES_OK == result)
	    {
	        stat = RES_OK;
	    }
	    else
	    {
	        stat = STA_NOINIT;
	    }
		return stat;

	case USB :
	    stat = STA_NOINIT;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

	    res = RES_PARERR;

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

	    if (count > 1 && count <= INT_MAX)
	    {
	        result = SDCard_Read_Mult_Block(sector, (uint8_t (*)[SDCARD_SECTOR_SIZE])buff, (int)count);
	        if (SDCARD_RES_OK == result)
	        {
	            res = RES_OK;
	        }
	        else
	        {
	            res = RES_ERROR;
	        }
	    }
	    else if (1 == count)
	    {
	        result = SDCard_Read_Single_Block(sector, buff);
            if (SDCARD_RES_OK == result)
            {
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
	    }
	    else
	    {
	        res = RES_PARERR;
	    }

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

	    res = RES_PARERR;

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

	    res = RES_PARERR;

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

	    if (count > 1 && count <= INT_MAX)
        {
            result = SDCard_Write_Mult_Blocks(sector, (uint8_t (*)[SDCARD_SECTOR_SIZE])buff, (int)count);
            if (SDCARD_RES_OK == result)
            {
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
        }
        else if (1 == count)
        {
            result = SDCard_Write_Single_Block(sector, buff);
            if (SDCARD_RES_OK == result)
            {
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
        }
        else
        {
            res = RES_PARERR;
        }

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

	    res = RES_PARERR;

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
	    res = RES_PARERR;
		return res;

	case MMC :

		// Process of the command for the MMC/SD card
	    if (CTRL_SYNC == cmd)
	    {
	        res = RES_OK;
	    }
	    else if (GET_SECTOR_COUNT == cmd)
	    {
	        uint32_t cnt = 0;
	        result = Get_SDCard_Sector_Count(&cnt);
	        if (SDCARD_RES_OK == result)
	        {
	            *(uint32_t *)buff = cnt;
	            res = RES_OK;
	        }
	        else
	        {
	            res = RES_ERROR;
	        }
	    }
        else if (GET_SECTOR_SIZE == cmd)
        {
            uint16_t size = 0;
            result = Get_SDCard_Sector_Size(&size);
            if (SDCARD_RES_OK == result)
            {
                if (    512 == size
                    || 1024 == size
                    || 2048 == size
                    || 4096 == size )
                {
                    *(uint16_t *)buff = size;
                    res = RES_OK;
                }
                else
                {
                    res = RES_ERROR;
                }
            }
            else
            {
                res = RES_ERROR;
            }
        }
        else if (GET_BLOCK_SIZE == cmd)
        {
            res = RES_PARERR;
        }
        else if (CTRL_TRIM == cmd)
        {
            res = RES_PARERR;
        }
        else
        {
            res = RES_PARERR;
        }
		return res;

	case USB :
	    res = RES_PARERR;
		return res;
	}

	return RES_PARERR;
}
#endif
