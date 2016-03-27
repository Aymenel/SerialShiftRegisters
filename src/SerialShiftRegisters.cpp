#include "Arduino.h"
#include "SerialShiftRegisters.h"

void SerialShiftRegisters::Initialize(int countSSR, int bitCountSSR, int latchPin, int clockPin, int dataPin) {
	_countSSR = countSSR;
	_bitCountSSR = bitCountSSR;
	
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	
	//Устанавливаем режим OUTPUT
	pinMode(_latchPin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, OUTPUT);
	
	//Выделение памяти под указатели на строки
	_pins = (int**)malloc(_countSSR * sizeof(int*));
	
	//Ввод элементов массива
	for(int i = 0; i < _countSSR; i++) { //Цикл по строкам
		//Выделение памяти под хранение строк
		_pins[i] = (int*)malloc(_bitCountSSR * sizeof(bool));
		
		for(int j = 0; j < _bitCountSSR; j++) { //Цикл по столбцам
			_pins[i][j] = 0;
		}
	}
	
	_initialized = true;
}

void SerialShiftRegisters::Set(int indexSSR, int indexPinSSR, bool value) {
	_pins[indexSSR][indexPinSSR] = value;
}

void SerialShiftRegisters::SetGlobal(int indexPin, bool value) {
	_pins[indexPin / bitCountSSR][indexPin - indexPin / bitCountSSR * bitCountSSR] = value;
}

void SerialShiftRegisters::Push() {
	//Устанавливаем синхронизацию "защелки" на LOW
    digitalWrite(_latchPin, LOW);
	
	for(int i = _countSSR - 1; i >= 0 ; i--) { //Цикл по сдвиговым регистрам
		for(int j = _bitCountSSR - 1; j >= 0; j--) { //Цикл по пинам сдвигового регистра
			_shiftOutExt(_pins[i][j]);
		}
	}
	
	//"Проталкиваем" байты в регистры
    //shiftOut(_dataPin, _clockPin, MSBFIRST, registerTwo);
	//_shiftOutExt
	
	//"Защелкиваем" регистр, тем самым устанавливая значения на выходах
    digitalWrite(_latchPin, HIGH);
}

void SerialShiftRegisters::_shiftOutExt(bool val) {
	digitalWrite(_dataPin, val);
	
	digitalWrite(_clockPin, HIGH);
	digitalWrite(_clockPin, LOW);
}
