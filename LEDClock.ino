
int clockPinBase = 2;
int dataPinBase = 3;

int latchPinArrow = 6;
int clockPinArrow = 5;
int dataPinArrow = 4;

void setup()
{
	pinMode(clockPinBase, OUTPUT);
	pinMode(dataPinBase, OUTPUT);

	pinMode(latchPinArrow, OUTPUT);
	pinMode(clockPinArrow, OUTPUT);
	pinMode(dataPinArrow, OUTPUT);

	digitalWrite(latchPinArrow, LOW);
	for (int i = 0; i < 8; i++)
	{
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0);
	}
	digitalWrite(latchPinArrow, HIGH);
}

void loop()
{
	digitalWrite(latchPinArrow, HIGH);
	shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0x01);
	digitalWrite(latchPinArrow, LOW);
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x01);
	for (int i = 0; i < 8; i++)
	{
		digitalWrite(latchPinArrow, LOW);
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0x00);
		digitalWrite(latchPinArrow, LOW);
		delay(1000);
	}
	digitalWrite(latchPinArrow, LOW);
	for (int i = 0; i < 8 ; i++)
	{
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0);
	}
	digitalWrite(latchPinArrow, HIGH);
}
