# M5Stack-Tab5-LCD-Tutorial

by Bryan A. "CrazyUncleBurton" Thompson
Last Updated 2/19/2026

## Concepts

In this project we show you how to download a project from GitHub, then build the project and upload to the M5Stack / Tab5 microcontroller.  The program is based on LVGL, a popular and free UI creation tool.  The program shows a button widget, a slider widget and an arc slider widget.  Changing the arc changes the slider and vv.  

## Hardware

Microcontroller:  M5Stack Tab5 (ESP32-P4NRW32@RISC-V 32-bit Dual-core 360MHz + LP Single-core 40MHz)
Display:  5" (1280 x 720) IPS TFT LCD and ST7213 controller and capacitive touch controller.

## Project Documentation

See the project files / docs folder for a PDF of the tutorial.

## ESP32 Arduino Core 3.x Info

This project updates the Arduino Core to v3.x.  This enables all kinds of things in ESP32 like the high-res GPTimer, the high-res RMT transmit/receive peripheral, and FreeRTOS.  Make sure the platform= line in the platformio.ini file looks like this:

platform = https://github.com/pioarduino/platform-espressif32.git#54.03.20; (Arduino Core 3.x)

If your other projects stop building after this update, it may be because they don't explicitly state that they are intended for an older version of the Arduino Core. To do that, update the platformio.ini file and replace the platform line in the older project(s) with this:

platform = espressif32@~6.5.0; (or whatever version you want. 6.5–6.8 map to Arduino core 2.0.x)

## M5Stack Tab5 Information

### Turning the M5Stack Tab5 On and Off

1. Press the square white button once to turn on the device.

2. Press the square white button twice to turn off the device.

### Charging the Battery

The battery only charges when the device is on and configured.

### Programming the M5Stack Tab5

1. Enter Download Mode - With USB cable or battery connected, long‑press the Reset button (2 seconds) until the internal green LED rapidly blinks; release to enter download mode and await firmware flashing.

2. Program like you would any other VS Code Project.  Click the PlatformIO:Upload button (shaped like a right arrow).

### Tab5 Pin Map

#### Camera

G32 - Camera SCL
G33 - Camera SDA
G36 - Camera MCLK
CSI_DATAP1 (Dedicated) - CAM_D1P
CSI_DATAN1 (Dedicated) - CAM_D1N
CSI_CLKP (Dedicated) - CAM_CSI_CKP
CSI_CLKN (Dedicated) - CAM_CSI_CKN
CSI_DATAP0 (Dedicated) - CSI_DOP
CSI_DATAN0 (Dedicated) - CSI_DON

#### ES8388 2 Channel Audio Codec

ES8388 (0x10)
G30 - MCLK
G27 - SCLK
G26 - DSDIN
G29 - LRCK
G32 - SCL
G33 - SDA

#### ES7210 4 CH ADC 24-bit Possible Mic Array

ES7210 (0x40)
G30 - MCLK
G27 - SCLK
G26 - ASDOUT
G29 - LRCK
G32 - SCL
G33 - SDA

#### LCD Controller ST7123

G22 - LEDA
DSI_CLKN (Dedicated) - DSI_CK_N
DSI_CLKP (Dedicated) - DSI_CK_P
DSI_DATAN1 (Dedicated) - DSI_D1_N
DSI_DATAP1 (Dedicated) - DSI_D1_P
DSI_DATAN0 (Dedicated) - DSI_D0_N
DSI_DATAP0 (Dedicated) - DSI_D0_P

#### Touch

GT911 (0x14) / ST7123 (0x55)
G31 - SDA
G32 - SCL
G23 - TP_INT
E1.P5 - TP_RST

#### BMI270

BMI270 (0x68)
G32 - SCL
G31 - SDA

#### RTC(RX8130CE)

RX8130CE (0x32)
G32 - SCL
G31 - SDA

#### INA226

INA226 (0x40)
G32 - SCL
G31 - SDA

#### ESP32-C6

G11 - SDIO2_D0
G10 - SDIO2_D1
G9 - SDIO2_D2
G8 - SDIO2_D3
G13 - SDIO2_CMD
G12 - SDIO2_CK
G15 - RESET
G14 - IO2
G35 - BOOT

#### microSD

microSD SPI Mode
G9 - MISO
G42 - CS
G43 - SCK
G44 - MOSI

microSD SDIO Mode
G39 - DAT0
G40 - DAT1
G41 - DAT2
G42 - DAT3
G43 - CLK
G44 - CMD

#### RS485

SIT3088
G21 - RX
G20 - TX
G34 - DIR

#### HY2.0-4P PORT A

COnnected to the External I2C pins and bus.  Refer to this as Wire() in Arduino.

Black - GND
Red - +5V
Yellow - G53 - Ext SDA
White - G54 - Ext SCL

### I2C (Internal Bus - sensors inside the Tab5)

Refer to this as Wire1() in Arduino.

G32 - Int SCL
G33 - Int SDA

#### I2C (External Bus - Port A)

Refer to this as Wire() in Arduino.

G53 - Ext SCL
G54 - Ext SDA

#### SPI

G18 - MOSI
G19 - MISO
G5 - SCK

#### Serial

RXD0 - G38
TXD0 - G37

#### M5Bus

Note:  The bus is the same layout as the old CORE2 bus with old GPIO map
Pin 1 - GND
Pin 2 - G16 - GPIO
Pin 3 - GND
Pin 4 - G17 - PB_IN
Pin 5 - GND
Pin 6 - RST - EN
Pin 7 - G18 - SPI_MOSI
Pin 8 - G45 - GPIO
Pin 9 - G19 - SPI_MISO
Pin 10 - G52 - PB_OUT
Pin 11 - G5 - SPI_SCK
Pin 12 - 3V3
Pin 13 - G38 - RXD0
Pin 14 - G37 - TXD0
Pin 15 - G7 - PC-RX
Pin 16 - G6 - PC_TX
Pin 17 - G31 - Int SDA
Pin 18 - G32 - Int SCL
Pin 19 - G3 - GPIO
Pin 20 - G4 - GPIO
Pin 21 - G2 - GPIO
Pin 22 - G48 - GPIO
Pin 23 - G47 - GPIO
Pin 24 - G35 - GPIO - BOOT
Pin 25 - HVIN
Pin 26 - G51 - GPIO
Pin 27 - HVIN
Pin 28 - 5V
Pin 29 - HVIN
Pin 30 - BAT

#### Ext Port 1

G50
G1
3V3
GND
HVIN
GND
GND
EXT 5V
G0
G49

#### Ext Port 2 - RS485

Pin 1 - Black - GND
Pin 2 - Red - HVIN
Pin 3 - YEL - 485A
Pin 4 - Green - 485B
Pin 5 - White - SDA - G32
Pin 6 - Blue - SCL - G32

#### USB C Ext

Pin 1 - USB1_D+
Pin 2 - USB1_D-
Pin 3 - GND
Pin 4 - 5VIN

## References

Microcontroller Info:
<https://docs.m5stack.com/en/core/Tab5>

M5GFX Display Library:
<https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_functions>

M5Canvas:
<https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_canvas>

LVGL Docs:
<https://docs.lvgl.io/master/>
