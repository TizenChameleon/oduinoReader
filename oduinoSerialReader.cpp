#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "oduinoSerialReader.h"

/*
class SerialReader
{
private:
	int fd,c,res;
	struct termios oldtio,newtio;
	char buf[BUF_SIZE];

public:
	SerialReader();
	~SerialReader();
	int isCreated();	//1:Success, 0:Failed

	int listen(char storeTo[]);

};

	*/

SerialReader::SerialReader()
{
	fd = open(DEVICE_NAME1, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if(fd<=0){
		fd = open(DEVICE_NAME2, O_RDWR | O_NOCTTY | O_NONBLOCK);
		if(fd<=0)
		{
			fd = open(DEVICE_NAME3, O_RDWR | O_NOCTTY | O_NONBLOCK);
			if(fd<=0)
				return;
		}
	}

	tcgetattr(fd, &oldtio);
	bzero(&newtio, sizeof(newtio));

	newtio.c_cflag = BAUD_RATE | CRTSCTS | CS8 | CLOCAL | CREAD;

	newtio.c_iflag = IGNPAR | ICRNL;

	newtio.c_oflag = 0;

	newtio.c_lflag = ICANON;

	/*
	   initialize all control characters
	   default values can be found in /usr/include/termios.h, and are given
	   in the comments, but we don't need them here
	 */
	newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
	newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtio.c_cc[VERASE]   = 0;     /* del */
	newtio.c_cc[VKILL]    = 0;     /* @ */
	newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
	newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
	newtio.c_cc[VSWTC]    = 0;     /* '\0' */
	newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
	newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	newtio.c_cc[VEOL]     = 0;     /* '\0' */
	newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	newtio.c_cc[VEOL2]    = 0;     /* '\0' */

	/*
	   now clean the modem line and activate the settings for the port
	 */

}
SerialReader::~SerialReader()
{
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);
}
int SerialReader::isCreated()
{
	if(fd<=0)
		return 0;
	else
		return 1;
}

int SerialReader::listen()
{
	if(isCreated() == 0)
		return 0;

	return parse()==1;
}
int SerialReader::parse()
{
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);

	while((res = read(fd, buf, BUF_SIZE))<=0)


	if(res <= 0)
		return 0;

	buf[res] = 0;

	char tmp_str[BUF_SIZE];
	char *tmp_position = strstr(buf, "TEM:");
	if(tmp_position == 0)
		return 0;

	// TEM:
	tmp_position += 4;
	strcpy(tmp_str, tmp_position);
	tmp_position = strstr(tmp_str, "C");
	*tmp_position = 0;
	temperature = atoi(tmp_str);

	tmp_position = strstr(buf, "HUMI:");
	if(tmp_position == 0)
		return 0;

	// HUMI:
	tmp_position += 5;
	strcpy(tmp_str, tmp_position);
	tmp_position = strstr(tmp_str, "%");
	*tmp_position = 0;
	humidity = atoi(tmp_str);


	return 1;
}
int SerialReader::get_Temperature()
{
	return temperature;
}
int SerialReader::get_Humidity()
{
	return humidity;
}
