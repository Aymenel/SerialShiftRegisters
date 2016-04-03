#ifndef SerialShiftRegisters_h
#define SerialShiftRegisters_h
 
#include "Arduino.h"

class SerialShiftRegisters {
	public:
		void Initialize(int countSSR, int bitCountSSR, int latchPin, int clockPin, int dataPin);
		void Set(int indexSSR, int indexPinSSR, bool value);
		void SetGlobal(int indexPin, bool value);
		void Push();
		
		void Debug();
	private:
		void _ShiftOutExt(bool val);
		void _PrintArray();
		
		//Отладочный режим
		bool _debug = false;
		
		//Признак инициализации класса
		bool _initialized = false;
		
		//Количество сдвиговых регистров
		int _countSSR = 0;
		//Разрядность сдвиговых регистров
		int _bitCountSSR = 0;
		
		//Пин, подключеный к ST_CP входу 74HC595
		int _latchPin = 0;
		//Пин, подключеный к SH_CP входу 74HC595
		int _clockPin = 0;
		//Пин, подключеный к DS входу 74HC595
		int _dataPin = 0;
		
		//Указатель на указатель на строку (Двумерный массив пинов [номер сдвигового регистра][номер бита]=состояние бита)
		bool **_ptrPins;
};

#endif