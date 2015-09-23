/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include "compiler_defs.h"
#include "C8051F580_defs.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void PCA0_Init()
{
    PCA0CN    = 0x40;
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x01;
    PCA0CPM0  = 0x21;
    PCA0CPM1  = 0x21;
    PCA0CPM2  = 0x21;
    PCA0CPM3  = 0x42;
    PCA0CPM4  = 0x42;
    PCA0CPM5  = 0x42;
    PCA0CPL3  = 0xFF;
    PCA0CPH3  = 0xFF;
    PCA0CPL4  = 0xFF;
    PCA0CPH4  = 0xFF;
    PCA0CPL5  = 0xFF;
    PCA0CPH5  = 0xFF;
}

void Timer_Init()
{
    TMR2CN    = 0x04;
    TMR2RLL   = 0xE0;
    TMR2RLH   = 0xB1;
    TMR2L     = 0xE0;
    TMR2H     = 0xB1;
}

void Timer_Init3()
{
    TMR3CN    = 0x04;
    TMR3RLL   = 0xE0;
    TMR3RLH   = 0xB1;
    TMR3L     = 0xE0;
    TMR3H     = 0xB1;
}

void SPI_Init()
{
    SPI0CFG   = 0x40;
    SPI0CN    = 0x09;
    SPI0CKR   = 0x0B;
}

void ADC_Init()
{
    ADC0MX    = 0x00;
    ADC0CF    = 0x30;
    ADC0CN    = 0x80;
}

void Voltage_Reference_Init()
{
    REF0CN    = 0x0A;
}

void Port_IO_Init()
{
    // P0.0  -  Skipped,     Open-Drain, Analog
    // P0.1  -  Skipped,     Open-Drain, Analog
    // P0.2  -  Skipped,     Open-Drain, Analog
    // P0.3  -  Skipped,     Open-Drain, Analog
    // P0.4  -  UART_TX (UART0), Push-Pull,  Digital
    // P0.5  -  UART_RX (UART0), Open-Drain, Digital
    // P0.6  -  CAN_TX (CAN0), Push-Pull,  Digital
    // P0.7  -  CAN_RX (CAN0), Push-Pull,  Digital

    // P1.0  -  SCK  (SPI0), Push-Pull,  Digital
    // P1.1  -  MISO (SPI0), Open-Drain, Digital
    // P1.2  -  MOSI (SPI0), Push-Pull,  Digital
    // P1.3  -  NSS  (SPI0), Push-Pull,  Digital
    // P1.4  -  Skipped,     Push-Pull,  Digital
    // P1.5  -  Skipped,     Push-Pull,  Digital
    // P1.6  -  Skipped,     Push-Pull,  Digital
    // P1.7  -  Skipped,     Push-Pull,  Digital

    // P2.0  -  Skipped,     Push-Pull,  Digital
    // P2.1  -  Skipped,     Open-Drain, Digital
    // P2.2  -  Skipped,     Push-Pull,  Digital
    // P2.3  -  Skipped,     Push-Pull,  Digital
    // P2.4  -  Skipped,     Push-Pull,  Digital
    // P2.5  -  Skipped,     Push-Pull,  Digital
    // P2.6  -  Skipped,     Push-Pull,  Digital
    // P2.7  -  Skipped,     Open-Drain, Digital

    // P3.0  -  Skipped,     Open-Drain, Digital
    // P3.1  -  Skipped,     Open-Drain, Digital
    // P3.2  -  Skipped,     Open-Drain, Digital
    // P3.3  -  Skipped,     Open-Drain, Digital
    // P3.4  -  Skipped,     Open-Drain, Digital
    // P3.5  -  Skipped,     Open-Drain, Digital
    // P3.6  -  Skipped,     Open-Drain, Digital
    // P3.7  -  Skipped,     Open-Drain, Digital

    // P4.0  -  CEX0 (PCA0), Open-Drain, Digital [Fixed]
    // P4.1  -  CEX1 (PCA0), Open-Drain, Digital [Fixed]
    // P4.2  -  CEX2 (PCA0), Open-Drain, Digital [Fixed]
    // P4.3  -  CEX3 (PCA0), Push-Pull,  Digital [Fixed]
    // P4.4  -  CEX4 (PCA0), Push-Pull,  Digital [Fixed]
    // P4.5  -  CEX5 (PCA0), Push-Pull,  Digital [Fixed]
    // P4.6  -  Unassigned,  Push-Pull,  Digital [Fixed]
    // P4.7  -  Unassigned,  Push-Pull,  Digital [Fixed]

    SFRPAGE   = CONFIG_PAGE;
    P0MDIN    = 0xF0;
    P0MDOUT   = 0xD0;
    P1MDOUT   = 0xFD;
    P2MDOUT   = 0x7D;
    P4MDOUT   = 0xF8;
    P0SKIP    = 0x0F;
    P1SKIP    = 0xF0;
    P2SKIP    = 0xFF;
    P3SKIP    = 0xFF;
    XBR0      = 0x07;
    XBR1      = 0x18;
    XBR2      = 0x40;
    SFRPAGE   = ACTIVE_PAGE;
}

void Oscillator_Init()
{
    SFRPAGE   = CONFIG_PAGE;
    OSCICN    = 0xC7;
    SFRPAGE   = ACTIVE_PAGE;
}

void Interrupts_Init()
{
    EIE1      = 0x0C;
    IE        = 0xA0;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA0_Init();
    Timer_Init();
    SPI_Init();
    ADC_Init();
    Voltage_Reference_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();
}
