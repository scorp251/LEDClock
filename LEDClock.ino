#include "RTC.h"

int clockPinBase = 2;
int dataPinBase = 3;

int latchPinArrow = 6;
int clockPinArrow = 5;
int dataPinArrow = 4;

int cHour;
int cMin;
int cSec;

uint64_t regs[8];

void setup()
{
	pinMode(clockPinBase, OUTPUT);
	pinMode(dataPinBase, OUTPUT);

	pinMode(latchPinArrow, OUTPUT);
	pinMode(clockPinArrow, OUTPUT);
	pinMode(dataPinArrow, OUTPUT);

	clearArrows();
	clearRegs();

	Serial.begin(9600);
	//Serial.println(F("DS1302 Real Time Clock"));
	//Serial.println(F("Version 2, March 2013"));


	// Start by clearing the Write Protect bit
	// Otherwise the clock data cannot be written
	// The whole register is written, 
	// but the WP-bit is the only bit in that register.
	DS1302_write(DS1302_ENABLE, 0);

	// Disable Trickle Charger.
	DS1302_write(DS1302_TRICKLE, 0x00);
}

void loop()
{
	ds1302_struct rtc;
	DS1302_clock_burst_read((uint8_t *)&rtc);
	uint8_t cHour = bcd2bin(rtc.h24.Hour10, rtc.h24.Hour);
	uint8_t cMin = bcd2bin(rtc.Minutes10, rtc.Minutes);
	uint8_t cSec = bcd2bin(rtc.Seconds10, rtc.Seconds);
	
	clearRegs();
	for (int i = 0; i < 6; i++)
	{
		regs[i] &= 1 << cHour;
	}

	for (int i = 0; i < 7; i++)
	{
		regs[i] &= 1 << cMin;
	}

	for (int i = 0; i < 8; i++)
	{
		regs[i] &= 1 << cSec;
	}

	char buffer[80];     // the code uses 70 characters.

	sprintf(buffer, "Time = %02d:%02d:%02d, ", \
		bcd2bin(rtc.h24.Hour10, rtc.h24.Hour), \
		bcd2bin(rtc.Minutes10, rtc.Minutes), \
		bcd2bin(rtc.Seconds10, rtc.Seconds));
	Serial.print(buffer);

	sprintf(buffer, "Date(day of month) = %d, Month = %d, " \
		"Day(day of week) = %d, Year = %d", \
		bcd2bin(rtc.Date10, rtc.Date), \
		bcd2bin(rtc.Month10, rtc.Month), \
		rtc.Day, \
		2000 + bcd2bin(rtc.Year10, rtc.Year));
	Serial.println(buffer);

	displayAll();
}

void clearArrows()
{
	digitalWrite(latchPinArrow, LOW);
	for (int i = 0; i < 8; i++)
	{
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0x00);
	}
	digitalWrite(latchPinArrow, HIGH);
}

void clearRegs()
{
	for (int i = 0; i < 8; i++)
	{
		regs[i] = 0;
	}
}

void displayAll()
{
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x00);
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x00);
	clearArrows();

	for (int i = 7; i >= 0; i--)
	{
		digitalWrite(latchPinArrow, LOW);

		shiftOut(dataPinBase, clockPinBase, MSBFIRST, 1 << i);
		shiftOut(dataPinBase, clockPinBase, MSBFIRST, 1 << i);

		uint8_t reg = regs[i] >> i * 8;
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, reg);

		digitalWrite(latchPinArrow, HIGH);
		
		delay(100);
	}
}