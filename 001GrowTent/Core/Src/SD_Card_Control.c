/*
 * 
 * 001GrowTent
 * SD_Card_Control.c
 * 
 * File Purpose:
 *
 *  Created on: 30 May 2022
 *  Author: Armon H. James
 */


#include "main.h"

/* Private variables ------------------------------------------------------*/
/* External variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;
extern SPI_HandleTypeDef hspi1;

//some variables for FatFs
FATFS FatFs; 	//Fatfs handle
FIL fil; 		//File handle
FRESULT fres; //Result after operations

static void myprintf(const char *fmt, ...);


static void myprintf(const char *fmt, ...)
{
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);

}

void SD_Control_Init(void)
{
	myprintf("\r\n~ SD card initialisation ~\r\n\r\n");

	//Open the file system
	fres = f_mount(&FatFs, "", 1); //1=mount now

	if (fres != FR_OK)
	{
		myprintf("f_mount error (%i)\r\n", fres);
		//while(1);
	}

	//Let's get some statistics from the SD card
	DWORD free_clusters, free_sectors, total_sectors;

	FATFS* getFreeFs;

	fres = f_getfree("", &free_clusters, &getFreeFs);

	if (fres != FR_OK)
	{
		myprintf("f_getfree error (%i)\r\n", fres);
		//while(1);
	}
	else
	{

		//Formula comes from ChaN's documentation
		total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
		free_sectors = free_clusters * getFreeFs->csize;

		myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

		//Now let's try to open file "test.txt"
		/*fres = f_open(&fil, "Test.txt", FA_READ);
		if (fres != FR_OK)
		{
			myprintf("f_open error (%i)\r\n", fres);
			while(1);
		}
		myprintf("I was able to open 'Log.txt' for reading!\r\n");

		//Read 30 bytes from "Test.txt" on the SD card
		BYTE readBuf[30];

		//We can either use f_read OR f_gets to get data out of files
		//f_gets is a wrapper on f_read that does some string formatting for us
		TCHAR* rres = f_gets((TCHAR*)readBuf, 30, &fil);
		if(rres != 0)
		{
			myprintf("Read string from 'test.txt' contents: %s\r\n", readBuf);
		}
		else
		{
			myprintf("f_gets error (%i)\r\n", fres);
		} */

		//Be a tidy kiwi - don't forget to close your file!
		//f_close(&fil);

		//Now let's try and write a file
		char fullFileName[20] = {"Log_"};
		char hourFile[] = {(char)systemVariables.timeHours};
		char dateFile[] = {(char)systemVariables.dateDate};
		char monthFile[] = {'_',(char)systemVariables.dateMonth};
		strcat(fullFileName, hourFile);
		strcat(fullFileName, dateFile);
		strcat(fullFileName, monthFile);

		fres = f_open(&fil, fullFileName, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
		if(fres == FR_OK)
		{
			myprintf("I was able to open %s for writing\r\n", fullFileName);
		}
		else
		{
			myprintf("f_open error (%i)\r\n", fres);
		}

		//Copy in a string
		//strncpy((char*)readBuf, "A new file is made!", 19);
		UINT bytesWrote;
		char buff[] = {"Time, \tDate, \tTemperature, \tHumidity"};

		fres = f_write(&fil, buff, sizeof(buff), &bytesWrote);
		if(fres == FR_OK)
		{
			myprintf("Wrote %i bytes to 'Log.txt'!\r\n", bytesWrote);
		}
		else
		{
			myprintf("f_write error (%i)\r\n", fres);
		}

		//Be a tidy kiwi - don't forget to close your file!
		f_close(&fil);

		//We're done, so de-mount the drive
		//f_mount(NULL, "", 0);
	}
}

void SD_Control_Write(void)
{
	/* Write all variables to file */

	fres = f_open(&fil, "Log.txt", FA_WRITE | FA_OPEN_APPEND);

	char buff[3];
	unsigned char characterASCII[] = {':','.',',','\n','\r','\t'};


	fres = f_putc(characterASCII[3], &fil);

	itoa(systemVariables.timeHours, buff, 10);
	fres = f_puts(buff, &fil);

	fres = f_putc(characterASCII[0], &fil);

	itoa(systemVariables.timeMinutes, buff, 10);
	fres = f_puts(buff, &fil);

	/* Comma */
	fres = f_putc(characterASCII[2], &fil);
	fres = f_putc(characterASCII[5], &fil);

	itoa(systemVariables.dateDate, buff, 10);
	fres = f_puts(buff, &fil);

	fres = f_putc(characterASCII[1], &fil);

	itoa(systemVariables.dateMonth, buff, 10);
	fres = f_puts(buff, &fil);

	fres = f_putc(characterASCII[1], &fil);

	itoa(systemVariables.dateYear, buff, 10);
	fres = f_puts(buff, &fil);

	/* Comma */
	fres = f_putc(characterASCII[2], &fil);
	fres = f_putc(characterASCII[5], &fil);
	fres = f_putc(characterASCII[5], &fil);

	itoa(systemVariables.temperature_int, buff, 10);
	fres = f_puts(buff, &fil);

	/* Comma */
	fres = f_putc(characterASCII[2], &fil);
	fres = f_putc(characterASCII[5], &fil);

	itoa(systemVariables.humidity_int, buff, 10);
	fres = f_puts(buff, &fil);

	f_close(&fil);
}


