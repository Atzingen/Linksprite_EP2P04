   /*********************************************************************
Arduino Library for the LinkSprite 2.04" E-Paper Display
Written by Michael Niemi / mvniemi  http://github.com/mvniemi
Implements the Adafruit GFX Class , Adapted from the Sharp Memory display library written by Limor Fried/Ladyada for Adafruit Industries.
*********************************************************************/

#include "Linksprite_EP2P04.h"
#include <Arduino.h>

#if defined(ESP32)
#include <pgmspace.h>
#define _delay_ms(ms) delayMicroseconds((ms) * 1000)
#endif
#if defined(ESP8266)
#include <pgmspace.h>
#define _delay_ms(ms) delayMicroseconds((ms) * 1000)
#endif
#ifdef avr
#include <util/delay.h>
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif



//SPI Macros
#define nDC_H digitalWrite(_dc,HIGH)
#define nDC_L digitalWrite(_dc,LOW)
#define nCS_H digitalWrite(_cs,HIGH)
#define nCS_L digitalWrite(_cs,LOW)
#define SDA_H digitalWrite(_da,HIGH)
#define SDA_L digitalWrite(_da,LOW)
#define SCLK_H digitalWrite(_sck,HIGH)
#define SCLK_L digitalWrite(_sck,LOW)
#define nRST_H digitalWrite(_rs,HIGH)
#define nRST_L digitalWrite(_rs,LOW)
#define read_stat digitalRead(_stat)
//

//Waveform
const unsigned char init_data[]={
        0x82,0x00,0x00,0x00,0xAA,0x00,0x00,0x00,
        0xAA,0xAA,0x00,0x00,0xAA,0xAA,0xAA,0x00,
        0x55,0xAA,0xAA,0x00,0x55,0x55,0x55,0x55,
        0xAA,0xAA,0xAA,0xAA,0x55,0x55,0x55,0x55,
        0xAA,0xAA,0xAA,0xAA,0x15,0x15,0x15,0x15,
        0x05,0x05,0x05,0x05,0x01,0x01,0x01,0x01,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x41,0x45,0xF1,0xFF,0x5F,0x55,0x01,0x00,
        0x00,0x00,};//waveform
//

//unsigned char buff[1548];

const unsigned PROGMEM char gImage[3096] = { /* 0X11,0X02,0X00,0XAC,0X00,0X48, */
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X01,0X55,0X54,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X03,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFD,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFD,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X17,0XFF,0XFD,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,
        0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X7F,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X7D,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X7D,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7D,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7D,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFD,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XD5,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XC0,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X0F,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X0F,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X0F,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0A,0XAA,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X0A,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0X80,0X00,
        0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,
        0X80,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,
        0XAA,0XAA,0X80,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,
        0X00,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,
        0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,
        0X00,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,
        0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X00,0X00,0X00,0XAA,0XAA,0XAA,0XAE,0XFE,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X00,0X00,0X00,0XAA,0XAA,0XAA,
        0XBF,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X00,0X00,0X00,0XAA,
        0XAA,0XAA,0XBF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X00,0X00,
        0X00,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,
        0X00,0X00,0X00,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,0XFF,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,0XFF,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,
        0XFA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,
        0XAA,0XAF,0XFA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,
        0XAA,0XAA,0XAA,0XAF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,
        0X02,0XAA,0XAA,0XAA,0XAA,0XAF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XFF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XFF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XFE,0XBA,0XAF,0XEA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAF,
        0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XFE,
        0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,
        0XAA,0XFE,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,
        0XAA,0XAA,0XAA,0XFE,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,
        0X02,0XAA,0XAA,0XAA,0XAF,0XFE,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAF,0XFA,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X95,0X7F,0X40,0X02,0XAA,0XAA,0XAA,0XAF,0XAA,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X02,0XAA,0XAA,0XAA,0XAF,0XAA,0XAA,0XAF,0XEA,0XAA,
        0XAB,0XAA,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X02,0XAA,0XAA,0XAA,0XAF,0XAA,0XAA,0XAF,
        0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X2A,0XAA,0XAA,0XAF,0XAA,
        0XAA,0XAF,0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X2A,0XAA,0XAA,
        0XAF,0XEA,0XAA,0XAF,0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X2A,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAF,0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,
        0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAF,0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,
        0XFF,0X40,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAE,0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,
        0XAA,0XA5,0X56,0X40,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X2A,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,
        0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,
        0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X0A,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,
        0X0A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,
        0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X0A,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,
        0X0A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,0X00,0X00,
        0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0XAB,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X00,0X00,0X0A,0XAA,0XAA,0XAA,0XAA,0XAF,0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,
        0XAA,0XBF,0XFF,0X40,0X00,0X00,0X0A,0XAA,0XAF,0XEA,0XAA,0XAF,0XEA,0XAA,0XAA,0XFE,
        0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X00,0X0A,0XAA,0XAF,0XEA,0XAA,0XAF,0XEA,0XAA,
        0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X00,0X0A,0XAA,0XAF,0XAA,0XAA,0XAF,
        0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X00,0X0A,0XAA,0XAF,0XAA,
        0XAA,0XAF,0XEA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X00,0X0A,0XAA,
        0XAF,0XAA,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,0X00,0X00,
        0X0A,0XAA,0XAF,0XAA,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XFF,0X40,
        0X00,0X00,0X0A,0XAA,0XAF,0XAA,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,
        0XFF,0X40,0X00,0X00,0X0A,0XAA,0XAF,0XFE,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X3F,0X40,0X00,0X00,0X0A,0XAA,0XAF,0XFE,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X00,0X00,0X0A,0XAA,0XAA,0XFE,0XAA,0XAF,0XEA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X00,0X00,0X0A,0XAA,0XAA,0XFE,0XAA,0XAF,
        0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XFE,
        0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,
        0XAA,0XFE,0XAA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,
        0XAA,0XAA,0XAA,0XFF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,
        0X02,0XAA,0XAA,0XAA,0XAA,0XFF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,0XFA,0XAF,0XEA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,0XFA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,0XFA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAF,
        0XFA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,
        0XAA,0XAB,0XFF,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XFF,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,
        0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XBF,0XFE,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X3F,0X40,0X02,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X15,0X00,
        0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XFE,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X02,0XAA,0XAA,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X02,0XAA,
        0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,
        0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X02,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,
        0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X2A,
        0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,0XAA,0X80,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X2A,0XAA,0XAA,0XAA,0XAA,
        0XAA,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X6A,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XF4,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X17,0XF4,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
        0XF4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X03,0XF4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X03,0XF4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X03,0XF4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFD,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X6F,0XFF,0XFF,0XFD,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFD,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,
        0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X0F,0XFF,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X0F,0XFF,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,};


/* CONSTRUCTORS  */
/* ************* */
Linksprite_EP2P04::Linksprite_EP2P04(uint8_t DC, uint8_t STAT, uint8_t CS, uint8_t DA, uint8_t RS, uint8_t SCK,
                                     uint8_t MODE)  :
        Adafruit_GFX(EP2P04_LCDWIDTH, EP2P04_LCDHEIGHT) {
    _mode = MODE;
    _sck = SCK;
    _da = DA;
    _dc = DC;
    _rs = RS;
    _stat = STAT;
    _cs = CS;


    if (_mode == 1){buffsize = EP2P04_LCDHEIGHT*EP2P04_LCDWIDTH/4;}
    else {buffsize = EP2P04_LCDHEIGHT*EP2P04_LCDWIDTH/8;}
    mem_buffer = new char[buffsize];
    //mem_buffer = buff;
    pinMode(_dc,OUTPUT);
    pinMode(_cs,OUTPUT);
    pinMode(_da,OUTPUT);
    pinMode(_sck,OUTPUT);
    pinMode(_rs, OUTPUT);
    pinMode(_stat,INPUT);
    RESET();
    INIT(); //while(1);
}

void Linksprite_EP2P04::begin() {
    setRotation(0);
}

/* *************** */
/* PRIVATE METHODS */
/* *************** */

//LS Privates

void Linksprite_EP2P04::INIT()
{
    SPI4W_WRITECOM(0x03);//set PREVGH,PREVGL
    SPI4W_WRITEDATA(0x00);
    SPI4W_WRITECOM(0x10);//set no deep sleep mode
    SPI4W_WRITEDATA(0x00);
    SPI4W_WRITECOM(0x11);//data enter mode
    SPI4W_WRITEDATA(0x01);
    SPI4W_WRITECOM(0x44);//setRAM x address start/end
    SPI4W_WRITEDATA(0x00);//RAM x address start at 00h;
    SPI4W_WRITEDATA(0x11);//RAMx address end at 11h(17)->72
    SPI4W_WRITECOM(0x45);//set RAM y address start/end
    SPI4W_WRITEDATA(0xAB);//RAM y address start at 00h;
    SPI4W_WRITEDATA(0x00);//RAM y address start at ABh(171)->172
    SPI4W_WRITECOM(0x4E);//set RAM x address count to 0;
    SPI4W_WRITEDATA(0x00);
    SPI4W_WRITECOM(0x4F);//set RAM y address count to 0;
    SPI4W_WRITEDATA(0xAB);
    SPI4W_WRITECOM(0x21);//bypass RAM data
    SPI4W_WRITEDATA(0x03);
    SPI4W_WRITECOM(0xF0);//booster feedback used
    SPI4W_WRITEDATA(0x1F);
    SPI4W_WRITECOM(0x2C);//vcom voltage
    SPI4W_WRITEDATA(0xA0);
    SPI4W_WRITECOM(0x3C);//board voltage
    SPI4W_WRITEDATA(0x63);
    SPI4W_WRITECOM(0x22);//display updata sequence option ,in page 33
    SPI4W_WRITEDATA(0xC4);//enable sequence: clk -> CP -> LUT -> pattern display
    WRITE_LUT();
}

void Linksprite_EP2P04::DELAY_mS(int delaytime) //????
{
    while(delaytime--)_delay_ms(1);
}

void Linksprite_EP2P04::RESET(int delay)
{   if (delay == NULL){delay = 100;}
    nRST_L;
    DELAY_mS(delay);//1ms
    nRST_H;
    DELAY_mS(delay);//1ms
}

void Linksprite_EP2P04::SPI4W_WRITECOM(unsigned char INIT_COM)
{
    unsigned char TEMPCOM;
    unsigned char scnt;

    TEMPCOM=INIT_COM;
    nCS_H;
    nCS_L;
    SCLK_L;
    nDC_L;
    for(scnt=0;scnt<8;scnt++)
    {
        if(TEMPCOM&0x80)  SDA_H;
        else SDA_L;
        SCLK_H;
        SCLK_L;
        TEMPCOM=TEMPCOM<<1;
    }
    nCS_H;
}

void Linksprite_EP2P04::SPI4W_WRITEDATA(unsigned char INIT_DATA)
{
    unsigned char TEMPCOM;
    unsigned char scnt;
    TEMPCOM=INIT_DATA;
    nCS_H;
    nCS_L;
    SCLK_L;
    nDC_H;
    for(scnt=0;scnt<8;scnt++)
    {
        if(TEMPCOM&0x80){
            SDA_H;
            SCLK_H;
            SCLK_L;
        }
        else  {
            SDA_L;
            SCLK_H;
            SCLK_L;
        }

        TEMPCOM=TEMPCOM<<1;
    }
    nCS_H;
    nDC_L;
}




void Linksprite_EP2P04::WRITE_LUT()
{
    unsigned char i;
    SPI4W_WRITECOM(0x32);//write LUT register
    for(i=0;i<90;i++)
    {
        SPI4W_WRITEDATA(init_data[i]);
    }
}

//LS PUBLICS

static const uint8_t PROGMEM
set[] = {  128,  64,  32,  16,  8,  4,  2,  1 },
clr[] = { ~1, ~2, ~4, ~8, ~16, ~32, ~64, ~128 };
//clr[] = { ~1, ~2, ~4, ~8, ~16, ~32, ~64, ~128 };


void Linksprite_EP2P04::entersleep()
{
    SPI4W_WRITECOM(0x22);//display updata sequence option
    SPI4W_WRITEDATA(0x03);
    SPI4W_WRITECOM(0x20);
}

void Linksprite_EP2P04::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;


    switch(rotation) {
        case 1:
        _swap_int16_t(x, y);
            x = EP2P04_LCDWIDTH  - 1 - x;
            break;
        case 2:
            x = EP2P04_LCDWIDTH  - 1 - x;
            y = EP2P04_LCDHEIGHT - 1 - y;
            break;
        case 3:
        _swap_int16_t(x, y);
            y = EP2P04_LCDHEIGHT - 1 - y;
            break;
    }

    if (_mode == 1 || _mode == 2) {

//        if (color){
           // mem_buffer[((x * EP2P04_LCDHEIGHT) + y) / 4] |= (0xC0 >> ((y) % 4) * 2);
            mem_buffer[((x * EP2P04_LCDHEIGHT) + y) / 4] |= ((color<<6) >> ((y) % 4) * 2);
            mem_buffer[((x * EP2P04_LCDHEIGHT) + y) / 4] &= (((color<<6) >> ((y) % 4) * 2) + 0xff-(0xC0 >> ((y) % 4) * 2));

    }

        //Monochrome buffer
    else{
        if(color){
            //mem_buffer[((x*EP2P04_LCDHEIGHT)+y)/8] |= (0x80 >> ((y)%8)) ;
            mem_buffer[((x*EP2P04_LCDHEIGHT)+y)/8] |= pgm_read_byte(&set[y & 7]) ;

        }else{
            mem_buffer[((x*EP2P04_LCDHEIGHT)+y)/8] &= ~(0x80 >> ((y)%8)) ;
           // mem_buffer[((x*EP2P04_LCDHEIGHT)+y)/8] &= pgm_read_byte(&clr[y & 7]);
        }
    }

}



uint8_t Linksprite_EP2P04::getPixel(uint16_t x, uint16_t y){
    return 0;
}


bool Linksprite_EP2P04::refresh(int timeout_thresh){
    //Block if display busy
    if (timeout_thresh == NULL){
        timeout_thresh = 200;
    }
    long begin_time = millis();
    long current;
    while (read_stat){
        current = millis();
        if (current - begin_time > timeout_thresh){
            return false;
        }
     }
    if (_mode) {
        write_buffer_color(mem_buffer);
    }
    else {write_buffer_mono(mem_buffer);}
    return true;
}

void Linksprite_EP2P04::clearDisplay(){
    for (int i = 0; i < buffsize ; ++i) {
        mem_buffer[i]=0x00;
    }
}

void Linksprite_EP2P04::fillBlack(){
    for (int i = 0; i < buffsize ; ++i) {
        mem_buffer[i]=0xff;
    }
}

void Linksprite_EP2P04::setBounds(uint16_t w, uint16_t h) {
    _width = w;
    _height = h;
}

void Linksprite_EP2P04::write_buffer_color(char buffer[])
{
    char data;
    int i;
    SPI4W_WRITECOM(0x24);
    if (_mode == 1) {
        for (i = 0; i < buffsize; i++) {
            data = mem_buffer[i];
            SPI4W_WRITEDATA(~data);
            //SPI4W_WRITEDATA(0x00);
        }
    }
    else {
        for (i = 0; i < buffsize * 2; i++) {
            data = mem_buffer[i % buffsize];
            SPI4W_WRITEDATA(~data);
            //SPI4W_WRITEDATA(0x00);
        }
    }
    SPI4W_WRITECOM(0x20);
  //  entersleep();
}


void Linksprite_EP2P04::write_buffer_mono(char buffer[]){
    char data;
    int i;
    uint16_t data16;
    char lo;
    char hi;
    SPI4W_WRITECOM(0x24);

    for(i=0;i< buffsize;i++)
    {
        uint16_t data16 = 0x0000;
        hi = 0x00;
        lo = 0x00;

        data = buffer[i];
        for (int j = 0; j < 8; j++) {
            if (data & (0x01 << j)) {
                data16 |= (0x03 << (j*2));
            }
        }

        lo = data16 & 0xFF;
        hi = data16 >> 8;

        SPI4W_WRITEDATA(~hi);
        SPI4W_WRITEDATA(~lo);
    }
    SPI4W_WRITECOM(0x20);
  //  entersleep();
}



void Linksprite_EP2P04::image_display(char image[])
{
    if (image == NULL){ image = gImage;}
    char data;
    int i;
    SPI4W_WRITECOM(0x24);
    for(i=0;i<3096;i++)
    {
        data = pgm_read_byte(&image[i]);
        SPI4W_WRITEDATA(~data);
        //SPI4W_WRITEDATA(0x00);
    }
    SPI4W_WRITECOM(0x20);
   // entersleep();
}
