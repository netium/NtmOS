#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

typedef enum {
    COM1_PORT = 0x3F8,
    COM2_PORT = 0x2F8,
    COM3_PORT = 0x3E8,
    COM4_PORT = 0x2E8
} serial_port;

/*
#define COM1_PORT (0x3F8)
#define COM2_PORT (0x2F8)
#define COM3_PORT (0x3E8)
#define COM4_PORT (0x2E8)
*/

#define COM_PORT_DATA_REGISTER_OFFSET (0)
#define COM_PORT_INT_ENABLE_REGISTER_OFFSET (1)
#define COM_PORT_LSB_DIVISOR_BAUDRATE_OFFSET (0)
#define COM_PORT_MSB_DIVISOR_BAUDRATE_OFFSET (1)
#define COM_PORT_FCR_OFFSET (2)
#define COM_PORT_LINE_CONTROL_REGISTER_OFFSET (3)
#define COM_PORT_MODEM_CONTROL_REGISTER_OFFSET (4)
#define COM_PORT_LINE_STATUS_REGISTER_OFFSET (5)
#define COM_PORT_MODEM_STATUS_REIGSTER_OFFSET (6)
#define COM_PORT_SCRATCH_REGISTER_OFFSET (7)

int initial_serial(serial_port port);
int serial_received(serial_port port);
int read_serial(serial_port port);
int is_transmit_empty(serial_port port);
void write_serial(serial_port port, int data);

#endif
