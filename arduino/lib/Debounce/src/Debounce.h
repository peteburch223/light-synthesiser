#ifndef _Debounce_h
	#define _Debounce_h

	class Debounce {
	public:
		Debounce(unsigned char defaultState);
		unsigned char debounce(unsigned char reading);

	protected:
		int counter;
		unsigned char debouncedValue;
	};

#endif
