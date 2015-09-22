

#include "UART0.h"

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);

#ifdef SDCC
 U8 getkey (void);
 void putchar (char output);
#endif

void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x87;                     // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC  = 0x04;                     // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

#if   ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
      SBRL0 = -(SYSCLK / BAUDRATE / 2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // Set prescaler to 4
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 12)
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 12);
      SBCON0 &= ~0x03;                 // Set prescaler to 12
#else
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // Set prescaler to 48
#endif

   SBCON0 |= 0x40;                     // Enable baud rate generator

   SFRPAGE = ACTIVE_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear RI0 and TI0 bits

   TI0 = 1;                            // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

#ifdef SDCC

// SDCC does not include a definition for putchar(), which is used in printf()
// and so it is defined here.  The prototype does not need to be explicitly
// defined because it is provided in stdio.h

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : character to send to UART
//
// This function outputs a character to the UART.
//-----------------------------------------------------------------------------
void putchar (char output)
{
   if (output == '\n')
   {
      while (!TI0);
      TI0 = 0;
      SBUF0 = 0x0D;
   }
   while (!TI0);
   TI0 = 0;
   SBUF0 = output;
}

// SDCC does not include a definition for getkey(), and so it is defined here

//-----------------------------------------------------------------------------
// getkey
//-----------------------------------------------------------------------------
//
// Return Value : character received from UART
// Parameters   : None
//
// This function returns a character received from the UART
// Assumes SFR page has been set correctly by the application
//-----------------------------------------------------------------------------
U8 getkey (void)
{
   U8 received;

                                          
										  
	while (!RI0);                       // Wait for character to be received
   	received = SBUF0;                   // Read it
   	RI0 = 0;                            // Clear received data indicator

   return received;
}

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
