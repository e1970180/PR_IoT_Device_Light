#include "PR_IoT_Device_Light.h"

    #include <arduino.h>    

	void 	PR_IoT_Light::announce() {
		postMsg("analog", "HELLO"); 
	}
	
	void 	PR_IoT_Light::setupHW(uint8_t pin) {
		_pin = pin;
	}   
	
	void	PR_IoT_Light::setupHWTresholds(uint8_t lightTresholdPercent, uint8_t sunTresholdPercent, uint8_t hysteresisPersent) {
		_lightTreshold 	= (uint16_t)(lightTresholdPercent * 10.23);
		_sunTreshold	= (uint16_t)(sunTresholdPercent * 10.23);
		_hysteresis 	= (uint16_t)(hysteresisPersent * 10.23);		
	}
	
inline void 	PR_IoT_Light::loopHW() {
		static uint16_t	light = 0;
		
		uint16_t adc = analogRead(_pin);
		delay(5);							//delay is a must to solve analogRead interference with WiFi like connection lost (https://github.com/esp8266/Arduino/issues/1634)
		light = ( (PR_K_FLTR-1)*light + adc) / PR_K_FLTR ;

		
		if 		( light > (_sunTreshold + _hysteresis) )	_currentState = SUN;
		else if ( light < (_sunTreshold - _hysteresis) && light > (_lightTreshold + _hysteresis)  ) _currentState = LIGHT;
		else if ( light < (_lightTreshold - _hysteresis) ) 	_currentState = DARK;
		
	}   
	
	void	PR_IoT_Light::update() {
		switch (_currentState) {
			case DARK:
				postMsg ("light", "dark");
				break;
			case LIGHT:
				postMsg ("light", "light");
				break;
			case SUN:
				postMsg ("light", "sun");
				break;
		}
	}
	
	void	PR_IoT_Light::inMsgCallback() {

		PR_DBGTLN("Swithc: Msg got")
		PR_DBGVLN(inMsg.payload)
		
		if (inMsg.payload == "GET")	{
			update();
		}
	}
	
