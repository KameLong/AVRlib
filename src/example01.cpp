#include <util/delay.h>
#include <avr/io.h>
void InitHard( void);   // ポートの初期化
int main(void)
{
    // ポートB初期設定
    DDRB = 0xff;    // PB7～PB0を出力に設定します。
    PORTB = 0;              // PB7～PB0に'L'を出力します。
    while(1)
    {
        PORTB = 0xFF;  // PD7～PD4に'H'を出力してLEDを点灯させます。
        _delay_ms(500);
        PORTB = 0x00;  // PD7～PD4に'L'を出力してLEDを消灯させます。
        _delay_ms(500);
    }
}

