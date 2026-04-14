#include <avr/io.h>
#include <util/delay.h>

// ????
#define ADC_PIN      PA0     // ?????ADC0(PA0)
#define SEG_PORT     PORTC   // ?????
#define DIG_PORT     PORTB   // ?????

// ????????(0-9)
const uint8_t seg_table[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
};

uint16_t adc_value = 0;      // ADC???
uint8_t digits[4] = {0};     // ??4?BCD?

// ???ADC
void ADC_Init() {
    ADMUX = (1 << REFS0);               // AVCC????,ADC0??
    ADCSRA = (1 << ADEN) | (7 << ADPS0); // ??ADC,???128
}

// ??ADC?(10?)
uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (1 << REFS0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC);              // ????
    while (ADCSRA & (1 << ADSC));       // ????
    return ADC;
}

// ?ADC????4?BCD?
void ADC_To_BCD(uint16_t val) {
    uint32_t scaled = (uint32_t)val * 9999 / 1023; // ???0-9999
    digits[0] = scaled / 1000;          // ??
    digits[1] = (scaled / 100) % 10;    // ??
    digits[2] = (scaled / 10) % 10;     // ??
    digits[3] = scaled % 10;            // ??
}

// ?????????
void Display_Dynamic() {
    for (uint8_t i = 0; i < 4; i++) {
        DIG_PORT = 0x00;                // ??????(??)
        SEG_PORT = seg_table[digits[i]]; // ????
        DIG_PORT = (1 << i);            // ??????
        _delay_ms(2);                   // ????
        SEG_PORT = 0xFF;                // ????(???)
    }
}

// ???
int main() {
    // ???
    DDRC = 0xFF;        // PORTC??(??)
    DDRB |= 0x0F;       // PB0-PB3??(??)
    ADC_Init();

    while (1) {
        adc_value = ADC_Read(0);       // ??ADC0
        ADC_To_BCD(adc_value);         // ???BCD?
        Display_Dynamic();             // ????
    }
}