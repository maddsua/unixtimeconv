//	2022 maddsua | https://gitlab.com/maddsua
//	No warranties are given, etc...

#define PLATFORM_WIN

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#ifdef PLATFORM_WIN
	#include <windows.h>
	#include <io.h>
	#include <fcntl.h>
	
	#define consoleDefColor 7
	
#endif

#include "unixtime_private.h"

#define inpTypeDate		1
#define inpTypeUnix		-1

#define buffSize 		32


bool valid(char* buf, int len, bool readable);
void colorPrint(const char* str, int color);


int main(int argc, char** argv) {
	
	#ifdef PLATFORM_WIN	
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), consoleDefColor);
	#endif
		
	//	constAntinopol
	const int len_date = 19;		//	2022/03/12.23:00:25
	const int len_unix_ms = 13;		//	1647118825600
	const int len_unix = 10;		//	1647118825
	
	//	main character
	char userInput[buffSize] = {0};
	int userInputType = 0;				// 0 - empty; 1 - date; -1 - unix
	
	//	flags
	bool is_gmt = false;			// local time
	int is_dst = -1;				// auto dst
	
	//	space-time structures
	time_t epoch_time;
	struct tm ts;
		time(&epoch_time);
	
	//	arguments
	if(argc > 1) {
		
		//	help msg
		if(!strcmp(argv[1], "-help") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
			
			char title [128] = {0};
				sprintf(title, "\nv%i.%i.%i %s\n", VER_MAJOR, VER_MINOR, VER_RELEASE, PRODUCT_NAME);
			colorPrint(title, 10);
			
			printf("%s %s\n", LEGAL_TRADEMARKS, LEGAL_COPYRIGHT);
			
			colorPrint("\nUsage:\n", 10);

			printf(" %s 1654627179000 -g		--->	2022/06/07.18:39:39\n %s 2022/06/07.18:39:39 -s	--->	1654616379000\n", ORIGINAL_FILENAME, ORIGINAL_FILENAME);
			colorPrint("\nFlags:\n", 10);
			printf(" -g : Display GMT time\n -s : DST on, summer time\n -w : DST off, winter time\n");
			printf("\nBy default DST is off and the local time is used (GMT off)\n");
			return 0;
		}
		
		const int inlen = strlen(argv[1]);
		int starg = 1;
		
		//	check user input
		if(inlen == len_date || inlen == len_unix_ms || inlen == len_unix) {
			
			if(valid(argv[1], inlen, true)) {
				
				if(inlen == len_date) {
					strncpy(userInput, argv[1], len_date);
					userInputType = inpTypeDate;
				} else {
					strncpy(userInput, argv[1], len_unix);
					userInputType = inpTypeUnix;
				}
				
				starg = 2;
			}
			else {
				printf("Is your input a valid date-time?\n");
				printf("Run %s -help for more\n", ORIGINAL_FILENAME);
				return 0;
			}
		}
		
		//	check arguments
		for(int i = starg; i < argc; i++) {
			
			if(!strcmp(argv[i], "-s")) {
				is_dst = 1;
			}
			else if(!strcmp(argv[i], "-w")) {
				is_dst = 0;
			}
			else if(!strcmp(argv[i], "-g")) {
				is_gmt = true;
			}
		}
		
	}

	// parse inputs
	if(userInputType == inpTypeDate) {
				
			char asci_year[8] = {0};
				strncpy(asci_year, userInput, 4);
				ts.tm_year = atoi(asci_year);
				ts.tm_year = (ts.tm_year - 1900);
				
			char asci_month[4] = {0};
				strncpy(asci_month, userInput + 5, 2);
				ts.tm_mon = atoi(asci_month);
				ts.tm_mon = (ts.tm_mon - 1);
				
			char asci_day[4] = {0};
				strncpy(asci_day, userInput + 8, 2);
				ts.tm_mday = atoi(asci_day);
				
			char asci_hour[4] = {0};
				strncpy(asci_hour, userInput + 11, 2);
				ts.tm_hour = atoi(asci_hour);
			
			char asci_min[4] = {0};
				strncpy(asci_min, userInput + 14, 2);
				ts.tm_min = atoi(asci_min);
			
			char asci_sec[4] = {0};
				strncpy(asci_sec, userInput + 17, 2);
				ts.tm_sec = atoi(asci_sec);
			
		//	set a dst flag
		ts.tm_isdst = is_dst;
			
		//	check if in range	
		if(ts.tm_year < 70 || ts.tm_year > 137 || ts.tm_mon < 0 || ts.tm_mon > 11 || ts.tm_mday <= 0 || ts.tm_mday > 31 ||
				 ts.tm_hour < 0 || ts.tm_hour > 23 || ts.tm_min < 0 || ts.tm_min > 59 || ts.tm_sec < 0 || ts.tm_sec > 60 ){
				 	
			printf("Invalid time format\n");
			printf("Run %s -help for more\n", ORIGINAL_FILENAME);
			return 0;
		}
	
		//	create epoch timestamp
		epoch_time = mktime(&ts);

	}
	else if(userInputType == inpTypeUnix) {		
		long userInputTime = atol(userInput);
		epoch_time = userInputTime;
	}
	
		// fill time structure if we work with epoch time
		if(userInputType != inpTypeDate) {
			
			if(is_gmt) {
				ts = *gmtime(&epoch_time);
			} else {
				ts = *localtime(&epoch_time);
			}
		}

	
	
	//create date/time in readable format
	
	char bur_read_form[buffSize] = {0};
	char bur_read_gen[buffSize] = {0};
		strftime(bur_read_form, buffSize, "%Y/%m/%d.%H:%M:%S", &ts);	
		strftime(bur_read_gen, buffSize, "%a, %d %b %Y, %H:%M:%S", &ts);
	
	
	//	display time
	
	if(userInputType == 0) {
		colorPrint("\n Now:", 10);
	}
	
	if(userInputType != inpTypeDate) {
		
		colorPrint("\n	---> ", 10);
		printf("%s", bur_read_form);
		colorPrint("\n	---> ", 10);
		printf("%s\n", bur_read_gen);
		
		
		if(is_gmt == true) {
			printf("	     (GMT+0)\n", bur_read_form, bur_read_gen);
		}
	}
	
	if(userInputType != inpTypeUnix) {
		
		
		colorPrint("\n	---> ", 10);
		printf("%ld", (long)epoch_time);
		colorPrint("\n	---> ", 10);
		printf("%ld000\n", (long)epoch_time);

			if(is_dst == 1) {
				printf("	     (summer time)\n");
			}
			else if (is_dst == 0) {
				printf("	     (winter time)\n");
			}
	}
	
	
	return 0;
}

void colorPrint(const char* str, int color) {
	#ifdef PLATFORM_WIN
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		printf(str);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), consoleDefColor);
	#endif
}


bool valid(char* buf, int len, bool readable) {
	
	char rangeA = 0x30;		//	0x2E = char. '0'
	char rangeB = 0x39;		//	0x3A = char. '9'
	
	if(readable) {
		char rangeA = 0x2E;		//	0x2E = char. '.'
		char rangeB = 0x3A;		//	0x3A = char. ':'	
	}

	for(int i = 0; i < len; i++) {
		
		if(buf[i] < 0x2E || buf[i] > 0x3A) {
			return false;
		}
	}
	
	return true;
}
