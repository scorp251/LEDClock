#include "RTC.h"

int latchPinBase = 2;
int clockPinBase = 3;
int dataPinBase = 4;

int latchPinArrow = 7;
int clockPinArrow = 6;
int dataPinArrow = 5;

uint8_t cHour;
uint8_t cMin;
uint8_t cSec;

uint8_t regs[8][8];

uint8_t cMills = 0;

void setup()
{
	pinMode(latchPinBase, OUTPUT);
	pinMode(clockPinBase, OUTPUT);
	pinMode(dataPinBase, OUTPUT);

	pinMode(latchPinArrow, OUTPUT);
	pinMode(clockPinArrow, OUTPUT);
	pinMode(dataPinArrow, OUTPUT);

	clearArrows();
	clearRegs();

	//setDateTime();

	//Serial.begin(9600);
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

	cHour = bcd2bin(rtc.h24.Hour10, rtc.h24.Hour);
	cMin = bcd2bin(rtc.Minutes10, rtc.Minutes);
	cSec = bcd2bin(rtc.Seconds10, rtc.Seconds);
	uint8_t shiftHour = (uint8_t) cMin / 12;
	uint8_t regNum;

	cMills = (millis() % 1000) >> 4;
	
	clearRegs();

	regNum = cMills >> 3;
	regs[7][regNum] |= 1 << (cMills % 8);

	for (int i = 0; i < 6; i++)
	{
		if (cHour >= 12)
		{
			cHour -= 12;
		}
		regNum = (cHour * 5) + shiftHour >> 3;
		regs[i][regNum] |= 1 << ((cHour * 5) + shiftHour) % 8;
	}

	for (int i = 0; i < 8; i++)
	{
		regNum = (uint8_t)cMin >> 3;
		if (i % 2)
		{
			regs[i][regNum] |= 1 << cMin % 8;
		}
		else
		{
			regs[i][regNum] ^= 1 << cMin % 8;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		regNum = (uint8_t)cSec >> 3;
		regs[i][regNum] ^= 1 << (cSec % 8);
	}

	//dots for hour segments
	for (int i = 0; i < 60; i += 5)
	{
		regNum = (uint8_t)i >> 3;
		regs[0][regNum] |= 1 << i % 8;
	}


	//char buffer[80];     // the code uses 70 characters.
	//
	//sprintf(buffer, "%d - %d - %d", cHour, cMin, cSec);
	//Serial.println("---");
	//Serial.println(buffer);

	//sprintf(buffer, "Time = %02d:%02d:%02d, ", \
	//	bcd2bin(rtc.h24.Hour10, rtc.h24.Hour), \
	//	bcd2bin(rtc.Minutes10, rtc.Minutes), \
	//	bcd2bin(rtc.Seconds10, rtc.Seconds));
	//Serial.print(buffer);

	//sprintf(buffer, "Date(day of month) = %d, Month = %d, " \
	//	"Day(day of week) = %d, Year = %d", \
	//	bcd2bin(rtc.Date10, rtc.Date), \
	//	bcd2bin(rtc.Month10, rtc.Month), \
	//	rtc.Day, \
	//	2000 + bcd2bin(rtc.Year10, rtc.Year));
	//Serial.println(buffer);

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
		for (int j = 0; j < 8; j++)
		{
			regs[i][j] = 0;
		}
	}
}

void displayAll()
{
	for (int i = 0; i < 8; i++)
	{
		digitalWrite(latchPinBase, LOW);
		shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x00);
		digitalWrite(latchPinBase, HIGH);

		// clearArrows();
		digitalWrite(latchPinArrow, LOW);
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0x00);
		digitalWrite(latchPinArrow, HIGH);

		digitalWrite(latchPinArrow, LOW);
		for (int j = 7; j >= 0; j--)
		{
			shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, regs[i][j]);
		}
		digitalWrite(latchPinArrow, HIGH);
		digitalWrite(latchPinBase, LOW);
		shiftOut(dataPinBase, clockPinBase, MSBFIRST, 1 << i);
		digitalWrite(latchPinBase, HIGH);
		delay(1);
	}
	digitalWrite(latchPinBase, LOW);
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x00);
	digitalWrite(latchPinBase, HIGH);
}