/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using SERIAL_AUXTWO to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_SERIAL_AUXTWO[12] = "Hello World!";

static void tx_cb_SERIAL_AUXTWO(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void SERIAL_AUXTWO_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&SERIAL_AUXTWO, USART_ASYNC_TXC_CB, tx_cb_SERIAL_AUXTWO);
	/*usart_async_register_callback(&SERIAL_AUXTWO, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&SERIAL_AUXTWO, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&SERIAL_AUXTWO, &io);
	usart_async_enable(&SERIAL_AUXTWO);

	io_write(io, example_SERIAL_AUXTWO, 12);
}

/**
 * Example of using SERIAL_PC to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_SERIAL_PC[12] = "Hello World!";

static void tx_cb_SERIAL_PC(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void SERIAL_PC_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&SERIAL_PC, USART_ASYNC_TXC_CB, tx_cb_SERIAL_PC);
	/*usart_async_register_callback(&SERIAL_PC, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&SERIAL_PC, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&SERIAL_PC, &io);
	usart_async_enable(&SERIAL_PC);

	io_write(io, example_SERIAL_PC, 12);
}

void delay_example(void)
{
	delay_ms(5000);
}
