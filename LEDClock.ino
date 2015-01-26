
int clockPinBase = 2;
int dataPinBase = 3;

int latchPinArrow = 5;
int clockPinArrow = 6;
int dataPinArrow = 4;

void setup()
{
	pinMode(clockPinBase, OUTPUT);
	pinMode(dataPinBase, OUTPUT);

	pinMode(latchPinArrow, OUTPUT);
	pinMode(clockPinArrow, OUTPUT);
	pinMode(dataPinArrow, OUTPUT);

	digitalWrite(latchPinArrow, LOW);
	for (int i = 0; i < 64; i++)
	{
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0b00000000);
	}
	shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0b00000001);
	digitalWrite(latchPinArrow, HIGH);
}

void loop()
{
	for (int i = 0; i < 8; i++)
	{
		shiftOut(dataPinBase, clockPinBase, LSBFIRST, 1 << i);
		delay(1000);
	}
	for (int i = 0; i < 8; i++)
	{
		shiftOut(dataPinBase, clockPinBase, LSBFIRST, 0);
	}
	delay(5000);
}
