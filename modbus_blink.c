#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include <time.h>

int delay(int ms) {
	clock_t start_time = clock();
	while(clock() < start_time + ms*1000);
}

int main() {
	// Connect arduino to port /dev/ttyACM0 with baud rate 9600, no parity bits, 8 data bits, and 1 stop bit
	modbus_t *arduino;
	arduino = modbus_new_rtu("/dev/ttyACM0", 9600, 'N', 8, 1);
	
	if(arduino == NULL) {
		fprintf(stderr, "Unable to create the libmodbus context\n");
		return -1;
	}
	
	// Set the device id to communicate with
	if(modbus_set_slave(arduino, 1) == -1) {
		fprintf(stderr, "Invalid slave ID\n");
		modbus_free(arduino);
		return -1;
	}
	
	// Test the connection
	if(modbus_connect(arduino) == -1) {
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		modbus_free(arduino);
		return -1;
	}
	
	// Set bits to turn on/off led
	uint8_t led_on[1] = {1};
	uint8_t led_off[1] = {0};

	while(1) {
		// Write 1 bit, starting at address 0 on the arduino, to the led_on buffer (1)
		modbus_write_bits(arduino, 0, 1, led_on);
		delay(1000);

		// Write 1 bit, starting at address 0 on the arduino, to the led_off buffer (0)
		modbus_write_bits(arduino, 0, 1, led_off);
		delay(1000);
	}
	
	// Clean out memory
	modbus_close(arduino);
	modbus_free(arduino);
}