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
	_ptrPins = (bool**)malloc(_countSSR * sizeof(bool*));
	
	//Ввод элементов массива
	for(int i = 0; i < _countSSR; i++) { //Цикл по строкам
		//Выделение памяти под хранение строк
		_ptrPins[i] = (bool*)malloc(_bitCountSSR * sizeof(bool));
		
		for(int j = 0; j < _bitCountSSR; j++) { //Цикл по столбцам
			_ptrPins[i][j] = false;
		}
	}
	
	if (_debug) {
		Serial.println("Initialize");
		_PrintArray();
	}
	
	_initialized = true;
}

void SerialShiftRegisters::Set(int indexSSR, int indexPinSSR, bool value) {
	_ptrPins[indexSSR][indexPinSSR] = value;

	if (_debug) {
		Serial.println("Set");
		_PrintArray();
	}
}

void SerialShiftRegisters::SetGlobal(int indexPin, bool value) {
	_ptrPins[indexPin / _bitCountSSR][indexPin - indexPin / _bitCountSSR * _bitCountSSR] = value;

	if (_debug) {
		Serial.println("SetGlobal");
		_PrintArray();
	}
}

void SerialShiftRegisters::Push() {	
	//Устанавливаем синхронизацию "защелки" на LOW
    digitalWrite(_latchPin, LOW);
	
	//"Проталкиваем" байты в регистры
	for(int i = _countSSR - 1; i >= 0 ; i--) { //Цикл по сдвиговым регистрам
		for(int j = _bitCountSSR - 1; j >= 0; j--) { //Цикл по пинам сдвигового регистра
			_ShiftOutExt(_ptrPins[i][j]);
		}
	}

	if (_debug) {
		Serial.println("Push");
		_PrintArray();
	}

	//"Защелкиваем" регистр, тем самым устанавливая значения на выходах
	digitalWrite(_latchPin, HIGH);
}

void SerialShiftRegisters::Debug() {
	_debug = !_debug;
}

void SerialShiftRegisters::_ShiftOutExt(bool val) {
	if (val) {
		digitalWrite(_dataPin, HIGH);
	} else {
		digitalWrite(_dataPin, LOW);
	}
	
	digitalWrite(_clockPin, HIGH);
	digitalWrite(_clockPin, LOW);
}

void SerialShiftRegisters::_PrintArray() {
	char out[256];

	for(int i = 0; i < _countSSR; i++) { //Цикл по сдвиговым регистрам
		for(int j = 0; j < _bitCountSSR; j++) { //Цикл по пинам сдвигового регистра
			sprintf(out, "%s%d%s%d%s%d", "Registr=", i, " Pin=", j, " Value=", _ptrPins[i][j]);
			Serial.println(out);
		}
	}
	Serial.println("");
}