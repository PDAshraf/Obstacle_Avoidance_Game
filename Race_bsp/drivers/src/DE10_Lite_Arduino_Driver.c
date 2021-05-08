/*!
 * \file       DE10_Lite_Arduino_Driver.c
 * \details    Implementation of functions for accessing Arduino devices
 * \author     Jesper Dahlbäck
 * \version    1.0
 * \date       2020
 * \copyright  AGSTU AB
 */

#include <alt_types.h>
#include <altera_avalon_i2c.h>
#include <altera_avalon_spi.h>
#include <altera_avalon_spi_regs.h>
#include <altera_modular_adc.h>
#include <DE10_Lite_Arduino_Driver.h>
#include <io.h>
#include <system.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
static alt_u32 sequencer;
static alt_u32 sample_store;

// Enable Arduino pin D0 and D1 as UART
void arduino_uart_enable(alt_u8 value) {
	IOWR_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE, 8, (value & 0x1));
}

// Enable Arduino pin D10-D13 as SPI
void arduino_spi_enable(alt_u8 value) {
	IOWR_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE, 8, (value & 0x1)<<1);
}

// Set Arduino pin to output or input
void arduino_pin_mode(alt_u8 pin,alt_u8 mode) {
	alt_u32 output_register = 0;
	output_register = IORD_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE, 0);
	output_register = (output_register & ~(1UL << pin)) | (mode << pin);
	IOWR_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE, 0, output_register);
}

// Write to Arduino pin
void arduino_digital_write(alt_u8 pin,alt_u8 value) {
	alt_u32 output_register = 0;
	output_register = IORD_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE, 4);
	output_register = (output_register & ~(1UL << pin)) | (value << pin);
	IOWR_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE, 4, output_register);
}

// Read Arduino pin
alt_u8 arduino_digital_read(alt_u8 pin) {
	return (IORD_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE, 4) >> pin) & 1;
}

// Start ADC in continuous mode
void arduino_analog_init(alt_u32 sequencer_base,alt_u32 sample_store_base) {
	sequencer=sequencer_base;
	sample_store=sample_store_base;
	adc_interrupt_disable(sample_store); // Disable IRQ until user knows how to write the correct code to make it work
	adc_stop(sequencer);
	adc_set_mode_run_continuously(sequencer);
	adc_start(sequencer);
}

// Read analog pin, DE10-Lite accepts up to 5.0v as input.
// Downscaled internally to max 2.5v. See voltage read function
// below for info on how to use values.
alt_u32 arduino_analog_read(alt_u8 pin) {
	alt_u32 reading;
	alt_adc_word_read(sample_store+(pin*4),&reading,1);
	return reading;
}

// Read analog pin and convert to voltage level
float arduino_analog_read_voltage(alt_u8 pin) {
	alt_u32 reading;
	alt_adc_word_read(sample_store+(pin*4),&reading,1);
	return ((((float)(reading<<1))*ANALOG_VOLTAGE_REFERENCE)/4096.0f);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accelerometer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ACCELEROMETER_ADDRESS 0x53 // Accelerometer I2C address

static ALT_AVALON_I2C_DEV_t *accelerometer_dev; //pointer to instance structure
static ALT_AVALON_I2C_STATUS_CODE accelerometer_status; // Variable to hold status codes

void accelerometer_open_dev(void)
{
	accelerometer_dev = alt_avalon_i2c_open("/dev/accelerometer_i2c");
	if (!accelerometer_dev)
	{
		printf("Error: Cannot find /dev/i2c_0\n");
	}
}

void accelerometer_init(void)
{
	alt_u8 txbuffer[2];

	// Set the address of the accelerometer
	alt_avalon_i2c_master_target_set(accelerometer_dev, ACCELEROMETER_ADDRESS);


	// Write data to accelerometer
	txbuffer[0]=0x2D; // POWER_CTL Register
	txbuffer[1]=0x08; // Measuring enable

	accelerometer_status = alt_avalon_i2c_master_tx(
			accelerometer_dev,
			txbuffer,
			2,
			ALT_AVALON_I2C_NO_INTERRUPTS);

	if (accelerometer_status != ALT_AVALON_I2C_SUCCESS)
	{
		printf("ACCELEROMETER_I2C TX ERROR!");
	}
}

void accelerometer_read(alt_u32 *accelerometer_x, alt_u32 *accelerometer_y, alt_u32 *accelerometer_z)
{
	alt_u8 txbuffer[1];
	txbuffer[0] = 0x32; // Start with register 0x32 (ACCEL_XOUT_H)
	alt_u8 rxbuffer[6];
	accelerometer_status = alt_avalon_i2c_master_tx_rx(
			accelerometer_dev,
			txbuffer,
			1,									// TX size
			rxbuffer,
			6,									// RX size
			ALT_AVALON_I2C_NO_INTERRUPTS);

	if (accelerometer_status!=ALT_AVALON_I2C_SUCCESS)
	{
		printf("ACCELEROMETER_I2C RX ERROR!");
	}

	*(accelerometer_x) = ( rxbuffer[0]| rxbuffer[1] << 8); // X-axis value
	if (*(accelerometer_x) & 0x00008000)
	{
		*(accelerometer_x) |= 0xFFFF0000;
	}

	*(accelerometer_y) = ( rxbuffer[2]| rxbuffer[3] << 8); // Y-axis value
	if (*(accelerometer_y) & 0x00008000)
	{
		*(accelerometer_y) |= 0xFFFF0000;
	}

	*(accelerometer_z) = ( rxbuffer[4]| rxbuffer[5] << 8); // Z-axis value
	if (*(accelerometer_z) & 0x00008000)
	{
		*(accelerometer_z) |= 0xFFFF0000;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I2C
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static ALT_AVALON_I2C_DEV_t *arduino_i2c_dev; // Pointer to instance structure
static ALT_AVALON_I2C_STATUS_CODE i2c_status; // Variable to hold status codes

void i2c_open_dev(void)
{
	// Get a pointer to the avalon i2c instance
	arduino_i2c_dev = alt_avalon_i2c_open("/dev/arduino_i2c");
}

void i2c_set_slave_address(alt_u32 slaveAddress)
{
	// Set the address of the device using
	alt_avalon_i2c_master_target_set(arduino_i2c_dev, slaveAddress);
}

void i2c_transmit(alt_u8 numBytes, alt_u8 *data)
{
	i2c_status = alt_avalon_i2c_master_tx(
			arduino_i2c_dev,
			data,
			numBytes,
			ALT_AVALON_I2C_NO_INTERRUPTS);

	if (i2c_status != ALT_AVALON_I2C_SUCCESS)
	{
		printf("I2C: TX error!\n");
	}
}

void i2c_receive(alt_u8 numBytes, alt_u8 *data)
{
	alt_u8 rxbuffer[numBytes];
	i2c_status = alt_avalon_i2c_master_rx (
			arduino_i2c_dev,
			data,
			numBytes,
			ALT_AVALON_I2C_NO_INTERRUPTS);

	if (i2c_status != ALT_AVALON_I2C_SUCCESS)
	{
		printf("I2C: RX error!\n");
	}
	*data = (alt_u8)rxbuffer;
}

void i2c_set_normal_mode(void)
{
	alt_u32 status_register;
	status_register = IORD_32DIRECT(ARDUINO_I2C_BASE, 8);
	IOWR_32DIRECT(ARDUINO_I2C_BASE, 8, status_register & (0 << 1));
}
void i2c_set_fast_mode(void)
{
	alt_u32 status_register;
	status_register = IORD_32DIRECT(ARDUINO_I2C_BASE, 8);
	IOWR_32DIRECT(ARDUINO_I2C_BASE, 8, status_register | (1 << 1));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SPI
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Arduino function to send and receive SPI data
int spi_command(alt_u32 write_length, const alt_u8 * wdata, alt_u32 read_length, alt_u8 * read_data)
{
	return alt_avalon_spi_command(SPI_0_BASE, 0 , write_length, &wdata, read_length, &read_data, 0);
}

// Fast function to transmit SPI data
void spi_tx(alt_u8 data)
{
	while (IORD_ALTERA_AVALON_SPI_STATUS(SPI_0_BASE) & 0x20) // Wait until transmission end
	{
		IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_0_BASE, data);
	}
}
