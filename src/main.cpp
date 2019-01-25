#include "mbed.h"
#include "DHT.h"
#include "LCD_DISCO_F746NG.h"
#include <iostream>

LCD_DISCO_F746NG lcd;

InterruptIn btn(USER_BUTTON); // Onboard blue button
DigitalOut led1(LED1);				// Onboard LED
DigitalOut led2(D2);					// LED Socket Kit
AnalogIn ls(A0);							// Light sensor
AnalogIn ss(A2);							// Loudness sensor
DHT sensor(A1, SEN51035P);		// Temprature and Humidity sensor

Serial pc(USBTX, USBRX);

Thread ths;
Thread light;
Thread sound;
// create an event queue
EventQueue queue;

char br[64];

eScale corf = CELCIUS;
char temp = 'C';

void temp_switch()
{
	led2 = !led2;
	if (!led2)
	{
		corf = CELCIUS;
		temp = 'C';
		pc.printf("Switched to Celcius\r\n");
	}
	else
	{
		corf = FARENHEIT;
		temp = 'F';
		pc.printf("Switched to Farenheit\r\n");
	}
}

void user_input()
{
	char b[8];
	char r[8];

	lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"Waiting on input..", CENTER_MODE);
	pc.printf("Indtast bygnings nummer: ");
	pc.scanf("%s", b);
	pc.printf("%s\n", b);

	pc.printf("Indtast lokale nummer: ");
	pc.scanf("%s", r);
	pc.printf("%s\n", r);

	sprintf(br, "Bygning: %s - Lokale: %s", b, r);

	lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"                  ", CENTER_MODE);
	lcd.DisplayStringAt(0, LINE(1), (uint8_t *)br, CENTER_MODE);
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
	lcd.Clear(LCD_COLOR_BLUE);
	lcd.SetBackColor(LCD_COLOR_BLUE);
	lcd.SetTextColor(LCD_COLOR_WHITE);
}

void dht_thread()
{

	int err;
	char temp_int[32];
	char humi_int[32];
	bool init = false;

	while (1)
	{
		lcd.SetTextColor(LCD_COLOR_WHITE);

		err = sensor.readData();
		if (err == 0)
		{
			sprintf(temp_int, "Temperature: %.0f %c", sensor.ReadTemperature(corf), temp);
			sprintf(humi_int, "Humidity: %.0f", sensor.ReadHumidity());

			init = true;
		}
		else
		{
			while (err != 0)
			{
				err = sensor.readData();
				if (err == 0)
				{
					init = true;
				}
			}
			sprintf(temp_int, "Temperature: %.0f %c", sensor.ReadTemperature(corf), temp);
			sprintf(humi_int, "Humidity: %.0f", sensor.ReadHumidity());
		}

		if (init)
		{
			lcd.DisplayStringAt(0, LINE(8), (uint8_t *)temp_int, CENTER_MODE);
			lcd.DisplayStringAt(0, LINE(9), (uint8_t *)humi_int, CENTER_MODE);
		}
		else
		{
			lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"Waiting for DHT...", CENTER_MODE);
		}
	}
}

void light_thread()
{
	while (1)
	{
		lcd.SetTextColor(LCD_COLOR_WHITE);
		if (ls.read() > 0.3)
		{
			lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"Goddag", CENTER_MODE);
		}
		else
		{
			lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"Godnat", CENTER_MODE);
		}
	}
}

void sound_thread()
{
	float loudness;
	lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"Loudness Level", CENTER_MODE);
	while (1)
	{
		loudness = ss.read();

		if (loudness > 0.5)
		{
			lcd.SetTextColor(LCD_COLOR_RED);
		}
		else
		{
			lcd.SetTextColor(LCD_COLOR_WHITE);
		}

		if (loudness < 0.1)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"#---------", CENTER_MODE);
		}
		else if (loudness < 0.2)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"##--------", CENTER_MODE);
		}
		else if (loudness < 0.3)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"###-------", CENTER_MODE);
		}
		else if (loudness < 0.4)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"####------", CENTER_MODE);
		}
		else if (loudness < 0.5)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"#####-----", CENTER_MODE);
		}
		else if (loudness < 0.6)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"######----", CENTER_MODE);
		}
		else if (loudness < 0.7)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"#######---", CENTER_MODE);
		}
		else if (loudness < 0.8)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"########--", CENTER_MODE);
		}
		else if (loudness < 0.9)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"#########-", CENTER_MODE);
		}
		else if (loudness <= 1)
		{
			lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"##########", CENTER_MODE);
		}

		ThisThread::sleep_for(150);
	}
}

int main()
{
	Thread eventThread;
	eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

	btn.fall(queue.event(&temp_switch));

	SCB_CleanDCache();
	SCB_DisableDCache();

	// Start screen
	boot();
	user_input();

	ths.start(dht_thread);
	light.start(light_thread);
	sound.start(sound_thread);

	wait(osWaitForever);
}
