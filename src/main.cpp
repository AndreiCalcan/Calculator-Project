
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

void lcd_activate()
{
    lcd.init();
    lcd.clear();
    lcd.backlight();

    lcd.setCursor(2, 0);
    lcd.print("Hello World");

    lcd.setCursor(2, 1);
    lcd.print("Andrei Calc");
}

char input[30];
int pos = 0;

char keypad[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '('},
    {'*', '0', '/', ')'}};

void reset_input()
{
    for (int i = 0; i < 30; i++)
    {
        input[i] = '\0';
    }
    pos = 0;
}

void build_input(char *str)
{
    char c = USART0_receive();
    int i = 0;
    while (c != '\n' && i < 40)
    {
        str[i] = c;
        i++;
        c = USART0_receive();
    }
    str[i - 1] = '\0';
}

void read_keypad()
{
    PORTB &= ~(1 << PB1);
    for (int i = 2; i < 6; i++)
    {
        if ((PIND & (1 << i)) == 0)
        {
            char output[30];
            sprintf(output, "S-a apasat %c", keypad[0][5 - i]);
            printf("%s\n", output);
            input[pos] = keypad[0][5 - i];
            pos++;
            lcd.clear();
            lcd.print(input);
            _delay_ms(DEBOUNCE_TIME);
        }
    }
    PORTB |= (1 << PB1);

    PORTB &= ~(1 << PB0);
    for (int i = 2; i < 6; i++)
    {
        if ((PIND & (1 << i)) == 0)
        {
            char output[30];
            sprintf(output, "S-a apasat %c", keypad[1][5 - i]);
            printf("%s\n", output);
            input[pos] = keypad[1][5 - i];
            pos++;
            lcd.clear();
            lcd.print(input);
            _delay_ms(DEBOUNCE_TIME);
        }
    }
    PORTB |= (1 << PB0);

    PORTD &= ~(1 << PD7);
    for (int i = 2; i < 6; i++)
    {
        if ((PIND & (1 << i)) == 0)
        {
            char output[30];
            sprintf(output, "S-a apasat %c", keypad[2][5 - i]);
            printf("%s\n", output);
            input[pos] = keypad[2][5 - i];
            pos++;
            lcd.clear();
            lcd.print(input);
            _delay_ms(DEBOUNCE_TIME);
        }
    }
    PORTD |= (1 << PD7);

    PORTD &= ~(1 << PD6);
    for (int i = 2; i < 6; i++)
    {
        if ((PIND & (1 << i)) == 0)
        {
            char output[30];
            sprintf(output, "S-a apasat %c", keypad[3][5 - i]);
            printf("%s\n", output);
            input[pos] = keypad[3][5 - i];
            pos++;
            lcd.clear();
            lcd.print(input);
            _delay_ms(DEBOUNCE_TIME);
        }
    }
    PORTD |= (1 << PD6);

    if ((PINB & (1 << PB2)) == 0)
    {
        char output[30];
        sprintf(output, "S-a apasat B1");
        printf("%s\n", output);
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
        char output[30];
        sprintf(output, "S-a apasat B2");
        printf("%s\n", output);
        input[pos - 1] = '\0';
        pos--;
        lcd.clear();
        lcd.print(input);
        _delay_ms(DEBOUNCE_TIME);
    }
}

void setup_keypad()
{

    DDRB |= (1 << PB1) | (1 << PB0);
    DDRB &= ~(1 << PB2) & ~(1 << PB3);
    DDRD |= (1 << PD6) | (1 << PD7);
    DDRD &= ~(1 << PD2) & ~(1 << PD3) & ~(1 << PD4) & ~(1 << PD5);
    PORTB |= (1 << PB1) | (1 << PB0) | (1 << PB2) | (1 << PB3);
    PORTD |= (1 << PD6) | (1 << PD7) | (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
}

// int main()
// {
//     USART0_init(CALC_USART_UBRR(PM_BAUD));
//     USART0_use_stdio();
//     setup_keypad();
//     while (1)
//     {
//         read_keypad();
//         // char input[40];
//         // build_input(input);
//         // printf("Received %s - (len = %d)\n", input, strlen(input));
//         // printf("Evaluates to: %.3f\n", evaluate(input));
//     }

//     return 0;
// }

void setup()
{
    lcd.init();
    lcd.clear();
    lcd.backlight();

    lcd.setCursor(2, 0);
    lcd.print("Hello World");

    lcd.setCursor(2, 1);
    lcd.print("Andrei C");

    USART0_init(CALC_USART_UBRR(PM_BAUD));
    USART0_use_stdio();
    setup_keypad();
    reset_input();
}

void loop()
{
    read_keypad();
}