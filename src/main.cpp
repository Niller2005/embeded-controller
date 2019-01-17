#include "mbed.h"
#include "rtos.h"
#include "DHT.h"
#include "LCD_DISCO_F746NG.h"
#include <iostream>

LCD_DISCO_F746NG lcd;

InterruptIn btn(D4);
DigitalOut led1(LED1);
DigitalOut led2(D2);
Serial pc(USBTX, USBRX);

Thread counter;
EventQueue queue;


volatile bool running = true;
bool paused = false;

void pause()
{
	if (!paused)
	{
		paused = true;
		lcd.Clear(LCD_COLOR_LIGHTCYAN);
		lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
		lcd.SetTextColor(LCD_COLOR_BLACK);
		pc.printf("\r\n----- Counter Paused -----\r\n");
	}
	else if (paused)
	{
		running = true;
		paused = false;
		lcd.Clear(LCD_COLOR_BLUE);
		lcd.SetBackColor(LCD_COLOR_BLUE);
		lcd.SetTextColor(LCD_COLOR_WHITE);
		pc.printf("\r\n----- Counter Resmumed -----\r\n");
	}
}

void boot()
{
	lcd.Clear(LCD_COLOR_BLACK);
	lcd.SetBackColor(LCD_COLOR_BLACK);
	lcd.SetTextColor(LCD_COLOR_WHITE);
	lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"BOOTING", CENTER_MODE);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"-------", CENTER_MODE);
	wait(0.3);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"#------", CENTER_MODE);
	wait(0.3);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"##-----", CENTER_MODE);
	wait(0.3);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"###----", CENTER_MODE);
	wait(0.3);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"####---", CENTER_MODE);
	wait(0.3);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"#####--", CENTER_MODE);
	wait(0.3);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"######-", CENTER_MODE);
	wait(0.3);
	lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"#######", CENTER_MODE);
	wait(1);
}

void start_count()
{
	lcd.Clear(LCD_COLOR_BLUE);
	lcd.SetBackColor(LCD_COLOR_BLUE);
	lcd.SetTextColor(LCD_COLOR_WHITE);
	lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"Let the count begin!", CENTER_MODE);
	wait(2);
	lcd.Clear(LCD_COLOR_BLUE);
	lcd.SetBackColor(LCD_COLOR_BLUE);
	lcd.SetTextColor(LCD_COLOR_WHITE);
}

void counter_thread()
{
	uint32_t count = 0;
	char screen_int[32];

	while (1)
	{
		if (running)
		{
			count++;
			if (count >= 9999)
			{
				count = 0;
			}
			sprintf(screen_int, "Count: %04lu", count);
		}
		if (!paused)
		{
			lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"              ", CENTER_MODE);
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)screen_int, CENTER_MODE);
			led2 = 1;

			// Onboard LED blink every 10 number
			if (count % 100 == 0)
				led1 = 1;
			else
				led1 = 0;

			running = true;
		}
		else
		{
			lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"--- Paused ---", CENTER_MODE);
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)screen_int, CENTER_MODE);
			led2 = 0;

			running = false;
		}
	}
}



int main()
{
	Thread eventThread;
	eventThread.start(callback(&queue, &EventQueue::dispatch_forever));
	
	btn.fall(queue.event(&pause));
	// Start screen
	boot();
	start_count();
	counter.start(counter_thread);

	while (1)
	{
	}
}
