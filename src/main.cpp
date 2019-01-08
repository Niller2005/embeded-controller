#include "mbed.h"
#include "LCD_DISCO_F746NG.h"
#include <iostream>
#include <string>

LCD_DISCO_F746NG lcd;

DigitalOut led1(LED1);
InterruptIn b1(USER_BUTTON);
InterruptIn b2(D4);
Serial pc(USBTX, USBRX);

bool running = true;
bool clicked = false;
int i = 0;

void pause()
{
	running = !running;

	pc.printf("\r\n----- Counter Paused -----\r\n");
}

void reset()
{
	i = 0;
	lcd.Clear(LCD_COLOR_BLUE);
}

int main()
{
	b1.rise(&pause);
	b2.rise(&pause);

	lcd.Clear(LCD_COLOR_BLUE);
	lcd.SetBackColor(LCD_COLOR_BLUE);
	lcd.SetTextColor(LCD_COLOR_WHITE);
	lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"Let the count begin!", CENTER_MODE);
	wait(1);
	lcd.Clear(LCD_COLOR_BLUE);

	uint32_t count;
	while (1)
	{

		if (running)
		{
			count = i++;
		}

		char screen_int[32];
		sprintf(screen_int, "Count: %lu", count);

		if (count >= 10000)
		{
			reset();
		}
		else if (running)
		{
			pc.printf("Count: %d\r\n", count);
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)&screen_int, CENTER_MODE);
		}
	}
}
