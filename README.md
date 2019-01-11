# Embeded Controller

## IDE

[Visual Studio Code
](https://code.visualstudio.com/) with [PlatformIO](https://platformio.org/) extension.

## Board

[ST STM32](https://docs.platformio.org/en/latest/boards/ststm32/disco_f746ng.html#hardware)

## Sensors

### Button

Button is a digital input sensor.

I use the button (b1 | b2) to pause and resume the counter with a function.

```c++
InterruptIn b1(USER_BUTTON);
InterruptIn b2(D4);

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

b1.rise(&pause);
b2.rise(&pause);
```

I ran into some issues with the sensor button triggering the function multiple times, where the onboard didn't.

Tried adding a wait, but that didn't look good.

### LED Socket Kit

LED Socket Kit is a digital out sensor.

The onboard LED (led1) blink everytime the counter counts to 10.

```cpp
if (count % 10 == 0)
  led1 = 1;
else
  led1 = 0;
```

I use the LED sensor to show when the counter is running (ON when running & OFF when paused).

```cpp
//when running
led2 = 1;

//when paused
led2 = 0;
```

### LCD

Uses LCD_DISCO_F746NG libary.

I use the LCD (lcd) to to show the counter in the center of the the screen.

The number with always show 4 digits:

```cpp
// example
0012
0208
```

When the counter is paused the LCD is changing background color, and puts `--- Paused ---` at the top of the screen.

I had some issues with clearing the screen to change background color and text only clearing partially. I fixed it by adding a line of spaces.

I also had issues with flickering when clearing. I fixed it by pausing the loop, when pausing the counter. When the counter is running it's only the text updating, and that doesn't flicker.

## Other sensors

I got the temperature and humidity sensor to work, but I couldn't find a use for it so I removed again. I tested it just to see how an analog sensor works, and it seems straigt forward.
