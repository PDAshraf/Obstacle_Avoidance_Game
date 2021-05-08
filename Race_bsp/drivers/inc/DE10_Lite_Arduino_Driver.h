/*!
 * \file       DE10_Lite_Arduino_Driver.h
 * \details    Interface defining functions for accessing Arduino devices
 * \author     Linus Eriksson
 * \author     Jesper Dahlbäck
 * \version    2.0
 * \date       2017-2020
 * \copyright  AGSTU AB
 */

#ifndef _DE10_LITE_ARDUINO_DRIVER_H_
#define _DE10_LITE_ARDUINO_DRIVER_H_

#include <alt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#define D14 14
#define D15 15
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define PIN_INPUT 0
#define PIN_OUTPUT 1
#define SIGNAL_LOW 0
#define SIGNAL_HIGH 1
#define ANALOG_VOLTAGE_REFERENCE 2.5f

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void arduino_uart_enable(alt_u8 value);
extern void arduino_spi_enable(alt_u8 value);
extern void arduino_pin_mode(alt_u8 pin,alt_u8 mode);
extern void arduino_digital_write(alt_u8 pin,alt_u8 value);
extern alt_u8 arduino_digital_read(alt_u8 pin);
extern void arduino_analog_init(alt_u32 sequencer_base,alt_u32 sample_store_base);
extern alt_u32 arduino_analog_read(alt_u8 pin);
extern float arduino_analog_read_voltage(alt_u8 pin);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accelerometer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ACCELEROMETER_ADDRESS 0x53
extern void accelerometer_open_dev(void);
extern void accelerometer_init(void);
extern void accelerometer_read(alt_u32 *accelerometer_x, alt_u32 *accelerometer_y, alt_u32 *accelerometer_z);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I2C
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void i2c_open_dev(void);
extern void i2c_set_slave_address(alt_u32 slaveAddress);
extern void i2c_transmit(alt_u8 numBytes, alt_u8 *data);
extern void i2c_receive(alt_u8 numBytes, alt_u8 *data);
extern void i2c_set_normal_mode(void);
extern void i2c_set_fast_mode(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SPI
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern int spi_command(alt_u32 write_length, const alt_u8 * wdata, alt_u32 read_length, alt_u8 * read_data);
extern void spi_tx(alt_u8 data);

#ifdef __cplusplus
}
#endif

#endif // _DE10_LITE_ARDUINO_DRIVER_H_
