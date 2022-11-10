/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_usart_async.h>
#include <hal_usart_async.h>
#include <hal_usart_async.h>

#include <hal_delay.h>

extern struct usart_async_descriptor SERIAL_AUXTWO;
extern struct usart_async_descriptor SERIAL_AUXONE;
extern struct usart_async_descriptor SERIAL_PC;

void SERIAL_AUXTWO_PORT_init(void);
void SERIAL_AUXTWO_CLOCK_init(void);
void SERIAL_AUXTWO_init(void);

void SERIAL_AUXONE_PORT_init(void);
void SERIAL_AUXONE_CLOCK_init(void);
void SERIAL_AUXONE_init(void);

void SERIAL_PC_PORT_init(void);
void SERIAL_PC_CLOCK_init(void);
void SERIAL_PC_init(void);

void delay_driver_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
