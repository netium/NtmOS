#include "serial_port.h"
#include "kernel_functions.h"

// The implementaiton is refer to https://wiki.osdev.org/Serial_Ports
// And https://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming

int initial_serial(serial_port port) {
    _io_out8(port + COM_PORT_INT_ENABLE_REGISTER_OFFSET, 0x00); // Disable serial port interrupt
    _io_out8(port + COM_PORT_LINE_CONTROL_REGISTER_OFFSET, 0x80); // Enable DLAB
    _io_out8(port + COM_PORT_LSB_DIVISOR_BAUDRATE_OFFSET, 0x01);    // Default Baudrate divided by 3
    _io_out8(port + COM_PORT_MSB_DIVISOR_BAUDRATE_OFFSET, 0x00);    // Dividor high byte = 0
    _io_out8(port + COM_PORT_LINE_CONTROL_REGISTER_OFFSET, 0x00);   // Clear DLAB
    _io_out8(port + COM_PORT_LINE_CONTROL_REGISTER_OFFSET, 0x03);   // Set 8N1
    _io_out8(port + COM_PORT_FCR_OFFSET, 0xc7);   // 
    _io_out8(port + COM_PORT_MODEM_CONTROL_REGISTER_OFFSET, 0x0b);
    _io_out8(port + COM_PORT_MODEM_CONTROL_REGISTER_OFFSET, 0x1F);
    _io_out8(port + COM_PORT_DATA_REGISTER_OFFSET, 0xae);

    int ret = 0;
    if (_io_in8(port + COM_PORT_DATA_REGISTER_OFFSET) != 0xae) {
        // It looks like some of the virtual mode didn't support the loopback mode
        // e.g. the VirtualBox, so in loopback mode it cannot get the data
        ret = -1;
    }

    _io_out8(port + COM_PORT_MODEM_CONTROL_REGISTER_OFFSET, 0x0f);
    ret = 0;
    return ret;
}

int serial_received(serial_port port) {
    return _io_in8(port + COM_PORT_LINE_STATUS_REGISTER_OFFSET) & 1;
}

int read_serial(serial_port port) {
    while (serial_received(port) == 0);

    return _io_in8(port + COM_PORT_DATA_REGISTER_OFFSET);
}

int is_transmit_empty(serial_port port) {
    return _io_in8(port + COM_PORT_LINE_STATUS_REGISTER_OFFSET) & 0x20;
}

void write_serial(serial_port port, int data) {
    while (is_transmit_empty(port) == 0);

    _io_out8(port + COM_PORT_DATA_REGISTER_OFFSET , data);
}

void sp_put_string(char *str) {
    char ch;
    while ((ch = *str++) != 0) {
        write_serial(COM1_PORT, ch);
    }
    write_serial(COM1_PORT, 0x0a);
    write_serial(COM1_PORT, 0x0d);
}