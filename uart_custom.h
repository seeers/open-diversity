#include <stdio.h>
#include "WProgram.h"


static FILE uartout = {0} ;

static int uart_putchar (char c, FILE *stream)
{
    Serial.write(c) ;
    return 0 ;
}

static void initUART() {
  Serial.begin(9600) ;
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout ;
}
