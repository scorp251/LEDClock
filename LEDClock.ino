#include "RTC.h"

int clockPinBase = 2;
int dataPinBase = 3;

int latchPinArrow = 6;
int clockPinArrow = 5;
int dataPinArrow = 4;

void setup()
{
	pinMode(clockPinBase, OUTPUT);
	pinMode(dataPinBase, OUTPUT);

	clearArrows();

	pinMode(latchPinArrow, OUTPUT);
	pinMode(clockPinArrow, OUTPUT);
	pinMode(dataPinArrow, OUTPUT);

	digitalWrite(latchPinArrow, LOW);
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0 << i);
		}
	}
	digitalWrite(latchPinArrow, HIGH);

	Serial.begin(9600);
	Serial.println(F("DS1302 Real Time Clock"));
	Serial.println(F("Version 2, March 2013"));


	// Start by clearing the Write Protect bit
	// Otherwise the clock data cannot be written
	// The whole register is written, 
	// but the WP-bit is the only bit in that register.
	DS1302_write(DS1302_ENABLE, 0);

	// Disable Trickle Charger.
	DS1302_write(DS1302_TRICKLE, 0x00);
}

uint8_t globalBase = 0;
uint64_t globalArrow = 1;

void loop()
{
	ds1302_struct rtc;
	// Read all clock data at once (burst mode).
	DS1302_clock_burst_read((uint8_t *)&rtc);

	int cHour = bcd2bin(rtc.h24.Hour10, rtc.h24.Hour);
	int cMin = bcd2bin(rtc.Minutes10, rtc.Minutes);
	int cdigit1 = cHour / 10;
	int cdigit2 = cHour - (cdigit1 * 10);
	int cdigit3 = cMin / 10;
	int cdigit4 = cMin - (cdigit3 * 10);
	int digiStatus1[4] = { cdigit1, cdigit2, cdigit3, cdigit4 };

	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x00);
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x00);

	clearArrows();
	
	digitalWrite(latchPinArrow, LOW);
	for (int i = 7; i >= 0; i--)
	{
		uint8_t reg = globalArrow >> i * 8;
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, reg);
	}
	digitalWrite(latchPinArrow, HIGH);

	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0xff);
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0xff);

	if (globalBase++ == 7)
	{
		globalBase = 0;
	}

	globalArrow *= 2;
	if ((globalArrow >> 56) == 8)
	{
		globalArrow = 1;
	}

	char buffer[80];     // the code uses 70 characters.

	// Read all clock data at once (burst mode).
	DS1302_clock_burst_read((uint8_t *)&rtc);

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

	delay(300);
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