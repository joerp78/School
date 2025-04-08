// tinyTemp.c
// Add a intro comment


#include <avr/io.h>       // Needed for ATtiny85 registers
#include <string.h>
#include <stdlib.h>       // For itoa()
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "tinyOLED.h"

#define NUMSAMPLES 25   // #ADC Samples to average
#define VREF 1.1        // ADC reference voltage (internal 1.1V)
#define MAXTEMP 70      // Too-Hot warning at this temp (Deg F)

void adc_init(void);
unsigned int get_adc(void);
void WDT_OFF(void);

int main(void)
{
    // Disable any active watchdog timer at startup
    WDT_OFF();
    
    float tempC, tempF;
    unsigned int digitalValue;
    unsigned long int totalValue;
    char buffer[6];

    // Initialize ADC and OLED
    adc_init();
    OLED_init();

    // Dim the OLED to reduce current draw
    OLED_command(0x81);   // Command: Set Contrast
    OLED_command(0x10);   // Contrast value (0x10 = dim, go from Decimal 0 to 255, but enter in hex. Lower is dimmer.)


    // Acquire and average NUMSAMPLES ADC measurements
    totalValue = 0;
    for (int i = 0; i < NUMSAMPLES; i++)
        totalValue += get_adc();
    digitalValue = totalValue / NUMSAMPLES;

    // Convert ADC reading to temperature and voltage
    tempC = digitalValue * VREF / 10.24 - 50.0;
    tempF = tempC * 9.0 / 5.0 + 32.0;

    // Clamp temperatures to safe ranges
    if (tempC < 0.0)
        tempC = 0;
    else if (tempC > 45.0)
        tempC = 45.0;
    if (tempF < 32.0)
        tempF = 32.0;
    else if (tempF > 113.0)
        tempF = 113.0;

    // Prepare temperature values with one decimal place precision
    int tempC10 = (int)(tempC * 10);
    int tempF10 = (int)(tempF * 10);

    // Update the OLED display
    OLED_clear();

    // Display Too-Hot warning if needed
    if (tempF > MAXTEMP) {
        OLED_cursor(20, 0);
        OLED_printC('T');
        OLED_cursor(30, 0);
        OLED_printC('O');
        OLED_cursor(40, 0);
        OLED_printC('O');
        OLED_cursor(60, 0);
        OLED_printC('H');
        OLED_cursor(70, 0);
        OLED_printC('O');
        OLED_cursor(80, 0);
        OLED_printC('T');
        OLED_cursor(85, 0);
        OLED_printC('!');
    }

    // Display Celsius temperature on line 2
    OLED_cursor(20, 2);
    itoa(tempC10 / 10, buffer, 10);
    for (unsigned char i = 0; i < strlen(buffer); i++)
        OLED_printC(buffer[i]);
    OLED_printC('.');
    itoa(tempC10 % 10, buffer, 10);
    for (unsigned char i = 0; i < strlen(buffer); i++)
        OLED_printC(buffer[i]);
    OLED_cursor(45, 2);
    OLED_printC('C');

    // Display Fahrenheit temperature on line 3
    OLED_cursor(60, 2);
    itoa(tempF10 / 10, buffer, 10);
    for (unsigned char i = 0; i < strlen(buffer); i++)
        OLED_printC(buffer[i]);
    OLED_printC('.');
    itoa(tempF10 % 10, buffer, 10);
    for (unsigned char i = 0; i < strlen(buffer); i++)
        OLED_printC(buffer[i]);
    OLED_cursor(85, 2);
    OLED_printC('F');

    // Set up watchdog timer for 8 seconds and enter sleep mode
    wdt_enable(WDTO_8S); // Enable watchdog timer with 8s timeout
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode(); // put the MCU to sleep

    // Once woken up by the watchdog reset, execution starts over.
    while (1) ;
    return 0;
}

// ADC initialization for ATtiny85: set input pin for ADC3 (PB3)
void adc_init(void)
{
    DDRB &= ~(1 << 3);  // Set PB3 as input for ADC
    ADCSRA = 0x83;      // Enable ADC; clock prescaler for ~125 KHz (for 1 MHz clock)
    ADMUX = 0x83;       // Select ADC3 and internal 1.1V reference
}

// Read ADC value from ADC3
unsigned int get_adc(void)
{
    ADCSRA |= (1 << ADSC);             // Start ADC conversion
    while ((ADCSRA & (1 << ADIF)) == 0);  // Wait for conversion to finish
    return ADCL | (ADCH << 8);         // Read low byte first!
}

// Disable watchdog timer and clear the reset flag at startup
void WDT_OFF(void)
{
    MCUSR &= ~(1 << WDRF);         // Use MCUSR on the ATtiny85
    WDTCR = (1 << WDCE) | (1 << WDE);
    WDTCR = 0x00;
}

// End of tinyTemp.c