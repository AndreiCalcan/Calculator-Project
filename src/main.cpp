#include <avr/io.h>
#include <util/delay.h>
#include <LiquidCrystal_I2C.h>

extern "C"
{
#include "usart.h"
#include "eval.h"
}

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define PM_BAUD 9600
#define DEBOUNCE_TIME 500
#define DEBUG 1

char input[30];
int pos = 0;

char keypad[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '('},
    {'*', '0', '/', '.'}};

void reset_input()
{
    for (int i = 0; i < 30; i++)
    {
        input[i] = '\0';
    }
    pos = 0;
}

void proc_keypad_press(int row, int column)
{
    if (pos >= 16)
    {
        pos = 17;
        lcd.clear();
        lcd.print(input);
        lcd.setCursor(0, 1);
        lcd.print("ERR_LEN");
        if (DEBUG == 1)
        {
            printf("Eroare lungime prea mare\n");
        }
        _delay_ms(DEBOUNCE_TIME);
        return;
    }
    char c = keypad[row][column];
    char last_char = input[pos - 1];
    if (c == '(' && (isdigit(last_char) || last_char == ')'))
    {
        c = ')';
    }
    if (DEBUG == 1)
    {
        printf("S-a apasat %c\n", c);
    }
    input[pos] = c;
    pos++;
    lcd.clear();
    lcd.print(input);
    _delay_ms(DEBOUNCE_TIME);
}

void scan_row(int row)
{
    for (int i = 2; i < 6; i++)
    {
        if ((PIND & (1 << i)) == 0)
        {
            proc_keypad_press(row, 5 - i);
        }
    }
}

void read_keypad()
{
    PORTB &= ~(1 << PB1);
    scan_row(0);
    PORTB |= (1 << PB1);

    PORTB &= ~(1 << PB0);
    scan_row(1);
    PORTB |= (1 << PB0);

    PORTD &= ~(1 << PD7);
    scan_row(2);
    PORTD |= (1 << PD7);

    PORTD &= ~(1 << PD6);
    scan_row(3);
    PORTD |= (1 << PD6);

    if ((PINB & (1 << PB2)) == 0 && pos <= 16)
    {
        if (DEBUG == 1)
        {
            printf("S-a apasat B1\n");
        }
        char output[30];
        sprintf(output, "%.3f", evaluate(input));
        lcd.clear();
        lcd.print(input);
        lcd.setCursor(0, 1);
        lcd.print(output);
        reset_input();
        _delay_ms(DEBOUNCE_TIME);
    }

    if ((PINB & (1 << PB3)) == 0 && pos > 0)
    {
        if (DEBUG == 1)
        {
            printf("S-a apasat B2\n");
        }
        input[pos - 1] = '\0';
        pos--;
        lcd.clear();
        lcd.print(input);
        _delay_ms(DEBOUNCE_TIME);
    }
}

void setup_keypad()
{
    // Put Row 1&2 pins to output
    DDRB |= (1 << PB1) | (1 << PB0);

    // Put Control Button 1&2 to input
    DDRB &= ~(1 << PB2) & ~(1 << PB3);

    // Put Row 3&4 pins to output
    DDRD |= (1 << PD6) | (1 << PD7);

    // Put Columns 1-4 pins to input
    DDRD &= ~(1 << PD2) & ~(1 << PD3) & ~(1 << PD4) & ~(1 << PD5);

    // Activate Pull-Ups for input pins and set logic level to HIGH for outputs
    PORTB |= (1 << PB1) | (1 << PB0) | (1 << PB2) | (1 << PB3);
    PORTD |= (1 << PD6) | (1 << PD7) | (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
}

int main()
{
    init();
    lcd.init();
    lcd.clear();
    lcd.backlight();

    lcd.setCursor(2, 0);
    lcd.print("Hello World");

    lcd.setCursor(2, 1);
    lcd.print("Andrei C");

    if (DEBUG == 1)
    {
        USART0_init(CALC_USART_UBRR(PM_BAUD));
        USART0_use_stdio();
    }

    setup_keypad();
    reset_input();
    while (1)
    {
        read_keypad();
    }

    return 0;
}