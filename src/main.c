//	2022 maddsua | https://gitlab.com/maddsua
//	No warranties are given, etc...


#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>


#define buffsize 32			//	char buffer
#define commsize 4			//	command

#define strtarg_ceil 4		//	top number of start arguments

#define len_readable 19		//	2022/03/12.23:00:25
#define len_unix_long 13	//	1647118825600
#define len_unix 10			//	1647118825
#define len_unix_rt 0

#define asci_range_num_start 46		// '.'
#define asci_range_num_end 58		// '9'
#define asci_range_num_shift 48		// '0'

#define asci_range_comm_start 97	// 'a'
#define asci_range_comm_end 122		// 'z'

#define date_demo_form "year/mon/day.hour:min:sec"
#define date_gen_form "%Y/%m/%d.%H:%M:%S"
#define date_read_form "%a, %d %b %Y, %H:%M:%S"


#define infoline_title "v1.0 Unix Time Converter"
#define infoline_defsets "DST: auto, Local time"
#define newinputmsg "UNIXTIME> "
#define invinpmsg "Incorrect time format"
#define invinptmmsg "This time can not be converted"
#define invcommmsg "Command not found"
#define tmamsg "Too many arguments"
#define inargmsg "An argument was ignored"
#define helpmsg " https://gitlab.com/maddsua\n\n Commands:\n	now	-->	print current time in epoch and readable format\n	dst	-->	toggle DST (summer time)\n	adst	-->	set DST to auto mode (pick system settings)\n	gmt	-->	toggle GMT (Global Coordinated Time)\n	clr	-->	reset all settings\n	exit	-->	?????\n\n\n Inline call [ USER> unixtime arg1 arg2 arg3 ]\n	Available commands: now, dst, adst, gmt\n\n	Call like: USER> unixtime now dst gmt\n	By default DST is off and local time is used (GMT off)\n\n	arg1: Time, timestamp or command [now]\n	arg2, arg3: Any other inline command"


unsigned int dstflag = -1;
unsigned int gmtflag = 0;


// separate function for current time output
void displayTimeModifiers(int check_dst, int check_gmt){
	
	if(dstflag == 1 && check_dst){
		printf("	(summer time)\n");
	}
	
	if(gmtflag && check_gmt){
		printf("	(GMT+0)\n");
	}
	
}


void thistime(int demo){
	
	time_t now;
	struct tm ts;
		
	time(&now);
	
	
	if(gmtflag){
		ts = *gmtime(&now);
	}
	else{
		ts = *localtime(&now);
	}
	
	if(dstflag == 1){
		ts.tm_isdst = 1;
	}
	else if(dstflag == 0){
		ts.tm_isdst = 0;
	}
	
	
	char buf_general[buffsize];
	char buf_readable[buffsize];
			//clear strings for 146% sure
		for(int i = 0; i < buffsize; i++){
			buf_general[i] = 0;
			buf_readable[i] = 0;
		}
	
	
	//create date/time in readable format
	strftime(buf_general, buffsize, date_gen_form, &ts);	
	strftime(buf_readable, buffsize, date_read_form, &ts);
	
	
	if(demo){
		
	/*	struct timezone here;
		printf("	%s <---> %ld000\n	%ld <---> %s\n	%i, %i\n", date_demo_form, (long)now, (long)now, buf_general, here.tz_dsttime, here.tz_minuteswest);*/
		
		printf("	%s <---> %ld000\n	%ld <---> %s\n", date_demo_form, (long)now, (long)now, buf_general);
	}
	else{
		printf("\n	---> %ld\n	---> %ld000\n", (long)now, (long)now);
			displayTimeModifiers(1,0);
		printf("\n	---> %s\n	---> %s\n", buf_general, buf_readable);
			displayTimeModifiers(0,1);
	}
}


//actual "main" function
int convertTime(char* inputstring, int data_len){
	
	//time to unix
	if(data_len == len_readable){
		
		struct tm t;
		time_t timestamp;
		
		//parse input data
			t.tm_year = (inputstring[0] - asci_range_num_shift) * 1000;
			t.tm_year += (inputstring[1] - asci_range_num_shift) * 100;
			t.tm_year += (inputstring[2] - asci_range_num_shift) * 10;
			t.tm_year += (inputstring[3] - asci_range_num_shift);
			t.tm_year = (t.tm_year - 1900);
			
			t.tm_mon = (inputstring[5] - asci_range_num_shift) * 10;
			t.tm_mon += (inputstring[6] - asci_range_num_shift);
			t.tm_mon = (t.tm_mon - 1);
			
			t.tm_mday = (inputstring[8] - asci_range_num_shift) * 10;
			t.tm_mday += (inputstring[9] - asci_range_num_shift);
			
			t.tm_hour = (inputstring[11] - asci_range_num_shift) * 10;
			t.tm_hour += (inputstring[12] - asci_range_num_shift);
			
			t.tm_min = (inputstring[14] - asci_range_num_shift) * 10;
			t.tm_min += (inputstring[15] - asci_range_num_shift);
			
			t.tm_sec = (inputstring[17] - asci_range_num_shift) * 10;
			t.tm_sec += (inputstring[18] - asci_range_num_shift);
			
			t.tm_isdst = dstflag;
			
		//check if in range	
		if(t.tm_year < 70 || t.tm_year > 137 || t.tm_mon < 0 || t.tm_mon > 11 || t.tm_mday <= 0 || t.tm_mday > 31 || t.tm_hour < 0 || t.tm_hour > 23 || t.tm_min < 0 || t.tm_min > 59 || t.tm_sec < 0 || t.tm_sec > 60 ){
			return 0;
		}
	
		//create timestamp
		timestamp = mktime(&t);
		
		printf("\n	---> %ld\n", (long)timestamp);
		printf("	---> %ld000\n", (long)timestamp);
			displayTimeModifiers(1,0);
	}
	
	//unix to time
	else{
		
		long unixtime = 0;
		long position = pow(10, (len_unix-1));
		
		//convert input string to long number
		for (int i = 0; i < len_unix; i++){
			unixtime += position * (((int)inputstring[i]) - asci_range_num_shift);
			position /= 10;
		}
		
		//pass timestamp to time structure
		struct tm ts;
		
		if(gmtflag){
			ts = *gmtime(&unixtime);
		}
		else{
			ts = *localtime(&unixtime);
		}
		
		
		//create tmp strings
		char timestr_general[buffsize];
		char timestr_readable[buffsize];
		
			//clear strings for 146% sure
			for(int i = 0; i < buffsize; i++){
				timestr_general[i] = 0;
				timestr_readable[i] = 0;
			}


		//create date/time in readable format
		strftime(timestr_general, buffsize, date_gen_form, &ts);
		strftime(timestr_readable, buffsize, date_read_form, &ts);
		
		
		printf("\n	---> %s\n	---> %s\n", timestr_general, timestr_readable);
			displayTimeModifiers(0,1);
		
	}
	
	return 1;
}


// this function returns lenght of time format, or -1 if it is incorrect
int isTimeValid(char* inputstring){

	int data_len = strlen(inputstring);
	
	//abort if size does not match
	if(data_len != len_readable && data_len != len_unix_long && data_len != len_unix){
		return -1;
	}
	
	//if string have not only numbers and separators
	for (int i = 0; i < data_len; i++){
		
		if(inputstring[i] < asci_range_num_start || inputstring[i] > asci_range_num_end){
			return -1;	
		}
	}

	return data_len;
}


// this function generates kind of unique numeric id of a command, just not to compare strings for every command
long commSnap(char* comm){
	
	long cast = 0;
	long position = pow(10, (commsize - 1));
	
	for (int i = 0; i < commsize; i++){
		cast += position * (int)comm[i];
		position /= 10;
	}
	
	return cast;
}


// this function checks if input is a command, executes it and returns: 0 - not a command, 1 - command executed, -1 - command invalid, -2 command: exit app
int isCommand(char* inputstring){
	
	int comm_len = strlen(inputstring);
	
	//	abort if size does not math
	if(comm_len > commsize){
		return 0;	
	}

	//	... or if string have non-letters
	for (int i = 0; i < comm_len; i++){
		
		if(inputstring[i] < asci_range_comm_start || inputstring[i] > asci_range_comm_end){
			return 0;	
		}
	}
	
	//	get "cast" of a command
	long commID = commSnap(inputstring);
	
	// compare it to commands
	if(commID == commSnap("adst")){
		
		dstflag = -1;
		printf("	DST: auto\n");
		
	}
	else if(commID == commSnap("dst")){
		
		if(dstflag == -1 || dstflag == 0){
			
			dstflag = 1;
			printf("	DST: on\n");
		}
		else{
			
			dstflag = 0;
			printf("	DST: off\n");
		}
		
	}
	else if(commID == commSnap("gmt")){
		
		gmtflag = !gmtflag;
		
		if(gmtflag){
			printf("	GMT: on\n");
		}
		else{
			printf("	GMT: off\n");
		}
		
	}
	else if(commID == commSnap("now")){
		thistime(0);
	}
	else if(commID == commSnap("clr")){
		
		dstflag = -1;
		gmtflag = 0;
		printf("	%s\n", infoline_defsets);
		
	}
	else if(commID == commSnap("help")){
		printf("\n%s\n\n", helpmsg);
	}
	else if(commID == commSnap("exit")){
		return -2;
	}
	else{
		return -1;
	}
	//	long construction, shoud've used switch/case?
	
	return 1;
}


//shorter version of isCommand for inline call
//0 - not a command, 1 - command executed, -1 - command invalid, -2 - command: now
int isCommInline(char* inputstring, int isFirst){
	
	
	int comm_len = strlen(inputstring);
	
	//	abort if size does not math
	if(comm_len > commsize){
		return 0;	
	}
	
	//	... or if string have non-letters
	for (int i = 0; i < comm_len; i++){
		
		if(inputstring[i] < asci_range_comm_start || inputstring[i] > asci_range_comm_end){
			return 0;	
		}
	}
	
	//	get "cast" of a command
	long commID = commSnap(inputstring);
	
	//	if this argument is first, it has only two ways
	if(isFirst){
		
		if(commID == commSnap("now")){
		
			thistime(0);
			return -2;
		}
		else{
			return -1;
		}
		
	}
	//	..if not, compare to other commands
	else{
		
		if(commID == commSnap("adst")){
			dstflag = -1;
		}
		else if(commID == commSnap("dst")){			
			dstflag = 1;
		}
		else if(commID == commSnap("gmt")){
			gmtflag = 1;
		}
		else{
			return -1;
		}
	}
	
	return 1;
}


void appLoop(void){
	
	char inputbuffer[buffsize];

	//say hello
	printf("\n  %s\n\n", infoline_title);
		thistime(1);
	printf("\n	%s\n\n", infoline_defsets);
	
while(1){
	
	//	get input
	printf("\n%s", newinputmsg);
	scanf("%s", &inputbuffer);
	
	
	//	process input
	int ifcommand = isCommand(inputbuffer);
	
	if(ifcommand == 0){
		
		int inputtype = isTimeValid(inputbuffer);
	
		if(inputtype != -1){
			if(!convertTime(inputbuffer, inputtype)){
				printf("  %s\n", invinptmmsg);
			}
		}
		else{
			printf("  %s\n", invinpmsg);
		}
		
	}
	//	invalid input
	else if(ifcommand == -1){
		
		printf("  %s\n", invcommmsg);
	}
	//	exit app
	else if(ifcommand == -2){
		
		break;
	}

}
	return;
}


int main(int argc, char** argv) {
	
	
	if(argc > strtarg_ceil){
		printf("  %s\n", tmamsg);
	}
	else if(argc > 1){
		
	//	set time flags for this call mode
		dstflag = 0;
		gmtflag = 0;
		
		
	//	proccess start arguments
		for(int i = 2; i < argc; i++){
			if(isCommInline(argv[i], 0) != 1){
				printf("  %s\n", inargmsg);
			}
		}


	//	proccess first start argument separately
	//	first argument should be "now" or be a date/time
		int ifcommand = isCommInline(argv[1], 1);
	
		if(ifcommand == 0){
		
			int inputtype = isTimeValid(argv[1]);
	
			if(inputtype != -1){
				
				if(!convertTime(argv[1], inputtype)){
					printf("  %s\n", invinptmmsg);
				}
				
			}
			else{
				printf("  %s\n", invinpmsg);
			}
		}
		else if(ifcommand == -1){
				printf("  %s\n", inargmsg);
		}
		
	}
	else{
		// go to the input loop mode
		appLoop();
	}

	return 0;
}
