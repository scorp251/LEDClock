
int clockPinBase = 2;
int dataPinBase = 3;

int latchPinArrow = 6;
int clockPinArrow = 5;
int dataPinArrow = 4;

void setup()
{
	pinMode(clockPinBase, OUTPUT);
	pinMode(dataPinBase, OUTPUT);

	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0x00);

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

	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0xff);
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 0xff);

	Serial.begin(9600);
}

uint8_t globalBase = 0;
uint64_t globalArrow = 1;

void loop()
{

	digitalWrite(latchPinArrow, LOW);
	for (int i = 0; i < 8; i++)
	{
		uint8_t reg = globalArrow >> i * 8;
		shiftOut(dataPinArrow, clockPinArrow, MSBFIRST, 0xff);
		Serial.println(reg);
	}
	Serial.println("---");
	digitalWrite(latchPinArrow, HIGH);

	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 1 << globalBase);
	shiftOut(dataPinBase, clockPinBase, MSBFIRST, 1 << globalBase);

	if (globalBase++ == 7)
	{
		globalBase = 0;
	}

	globalArrow *= 2;
	if ((globalArrow >> 56) == 8)
	{
		globalArrow = 1;
	}

	delay(200);
}
