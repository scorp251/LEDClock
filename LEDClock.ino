
int clockPinBase = 2;
int dataPinBase = 3;

int latchPinArrow = 6;
int clockPinArrow = 5;
int dataPinArrow = 4;

void setup()
{
	pinMode(clockPinBase, OUTPUT);
	pinMode(dataPinBase, OUTPUT);

	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0xff);

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
}

uint8_t globalBase = 0;

void loop()
{

	digitalWrite(latchPinArrow, LOW);
	for (int j = 0; j < 8; j++)
	{
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0b10000001);
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0b10000001);
	}
	digitalWrite(latchPinArrow, HIGH);

	delay(500);

	digitalWrite(latchPinArrow, LOW);
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0 << i);
		}
	}
	digitalWrite(latchPinArrow, HIGH);
	delay(500);
}
