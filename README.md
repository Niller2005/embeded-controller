# Embeded Controller

## IDE

[Visual Studio Code
](https://code.visualstudio.com/) with [PlatformIO](https://platformio.org/) extension.

## Board

[ST STM32](https://docs.platformio.org/en/latest/boards/ststm32/disco_f746ng.html#hardware)

## Sensors

### Button

I use the button to pause and resume the counter with a function.

```c++
InterruptIn b1(USER_BUTTON);

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

b2.rise(&pause);
```

I ran into some issues with the sensor button triggering the function multiple times, where the onboard didn't.

Tried adding a wait, but that didn't look good.
