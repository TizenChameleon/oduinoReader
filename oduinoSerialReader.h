#include <termios.h>

//
#define DEVICE_NAME1 "/dev/ttyACM0"
#define DEVICE_NAME2 "/dev/ttyACM1"
#define DEVICE_NAME3 "/dev/ttyACM2"

#define BAUD_RATE B115200
#define BUF_SIZE 256

class SerialReader
{
private:
	int fd,c,res;
	int temperature, humidity;
	struct termios oldtio,newtio;
	char buf[BUF_SIZE];

public:
	SerialReader();
	~SerialReader();
	int isCreated();	//1:Success, 0:Failed

	int listen();
	int parse();
	int get_Temperature();
	int get_Humidity();

};
