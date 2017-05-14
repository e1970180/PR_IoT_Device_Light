#ifndef PR_IoT_Device_Light_h
#define PR_IoT_Device_Light_h

    #include <arduino.h>
    #include <PR_IoT.h>
	
	#ifndef	PR_ADC_MAX
		#define	PR_ADC_MAX			1023		//
	#endif
	
	#ifndef	PR_K_FLTR
		#define	PR_K_FLTR			2		//[1 ... n]
	#endif	
	
	#ifndef	PR_LIGHT_TRHLD
		#define	PR_LIGHT_TRHLD		300		//[%]*10.23
	#endif	
	
	#ifndef	PR_SUN_TRHLD
		#define	PR_SUN_TRHLD		700		//[%]*10.23
	#endif
	
	#ifndef	PR_HYST
		#define	PR_HYST				100		//[%]*10.23
	#endif
	
class PR_IoT_Light : public PR_IoT_DeviceClass {
        public:
            PR_IoT_Light(String name) : PR_IoT_DeviceClass(name)	{ } 

            void 		announce();
            void 		update();
            virtual void 	inMsgCallback(); 
            void 		setupHW(uint8_t pin);			
			virtual void	loopHW();

			void		setupHWTresholds(uint8_t lightTresholdPercent, uint8_t sunTresholdPercent, uint8_t hysteresisPersent);
			
			bool		isDark()				{	return (_currentState == DARK);		}
			bool		isLight()				{	return (_currentState >= LIGHT);	}
			bool		isSun()					{	return (_currentState == SUN);		}
			
        protected:
			enum LightState	{ DARK, LIGHT, SUN };
		
			uint16_t 	_lightTreshold 		= PR_LIGHT_TRHLD;
			uint16_t 	_sunTreshold		= PR_SUN_TRHLD;
			uint16_t 	_hysteresis			= PR_HYST;
			uint8_t 	_pin;
			LightState	_currentState		= DARK;
    };
	
#endif