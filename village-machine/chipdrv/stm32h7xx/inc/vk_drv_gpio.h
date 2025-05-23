//###########################################################################
// vk_drv_gpio.h
// Hardware Layer class that manages a single GPIO pin
//
// $Copyright: Copyright (C) village
//###########################################################################
#ifndef __VK_DRV_GPIO_H__
#define __VK_DRV_GPIO_H__

#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_bus.h"

/// @brief Gpio
class Gpio
{
protected:
    //Members
    GPIO_TypeDef* GPIOx;
    uint32_t GPIO_PIN_x;
public:
	//Enumerations
	enum GpioChannel
	{
		_ChA = 0,
		_ChB = 1,
		_ChC = 2,
		_ChD = 3,
		_ChE = 4,
		_ChF = 5,
		_ChG = 6,
		_ChH = 7,
		_ChI = 8,
		_ChJ = 9,
		_ChK = 10,
	};
	
     enum GpioPin
    { 
        _Pin0  = 0,
        _Pin1  = 1,
        _Pin2  = 2,
        _Pin3  = 3,
        _Pin4  = 4,
        _Pin5  = 5,
        _Pin6  = 6,
        _Pin7  = 7,
        _Pin8  = 8,
        _Pin9  = 9,
        _Pin10 = 10,
        _Pin11 = 11,
        _Pin12 = 12,
        _Pin13 = 13,
        _Pin14 = 14,
        _Pin15 = 15,
    };

	enum GpioState
	{
        _Low = GPIO_PIN_RESET,
        _High = GPIO_PIN_SET,
	};

	enum GpioMode
	{
        _Input  = LL_GPIO_MODE_INPUT,
        _Output = LL_GPIO_MODE_OUTPUT,
        _Altera = LL_GPIO_MODE_ALTERNATE,
        _Analog = LL_GPIO_MODE_ANALOG,
	};

	enum GpioOutType
	{
        _PushPull = LL_GPIO_OUTPUT_PUSHPULL,
        _OpenDrain = LL_GPIO_OUTPUT_OPENDRAIN,
	};

	enum GpioPullType
	{
        _NoPull   = LL_GPIO_PULL_NO,
        _PullUp   = LL_GPIO_PULL_UP,
        _PullDown = LL_GPIO_PULL_DOWN,
	};

    enum GpioAlternate
    {
        _AF0   = 0,
        _AF1   = 1,
        _AF2   = 2,
        _AF3   = 3,
        _AF4   = 4,
        _AF5   = 5,
        _AF6   = 6,
        _AF7   = 7,
        _AF8   = 8,
        _AF9   = 9,
        _AF10  = 10,
        _AF11  = 11,
        _AF12  = 12,
        _AF13  = 13,
        _AF14  = 14,
        _AF15  = 15,
    };

	enum GpioSpeed
	{
        _LowSpeed  = LL_GPIO_SPEED_FREQ_LOW,
        _MedSpeed  = LL_GPIO_SPEED_FREQ_MEDIUM,
        _HighSpeed = LL_GPIO_SPEED_FREQ_HIGH,
		_SuperSpeed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
	};

	struct Config
	{
		uint16_t ch;
		uint16_t pin;
		uint16_t mode;
		uint16_t altfun;
		uint16_t value;
	};
public:
	//Methods
	Gpio();
	void Initialize(Config config);
	void ConfigMode(GpioMode mode);
	void ConfigPullType(GpioPullType inputType);
	void ConfigOutputType(GpioOutType outputType);
	void ConfigAltFunc(GpioAlternate altMode);
    void ConfigSpeed(GpioSpeed speed);
	
    ///Drives the pin high
    inline void Set() { LL_GPIO_SetOutputPin(GPIOx, GPIO_PIN_x); }

    ///Drives the pin low
    inline void Clear() { LL_GPIO_ResetOutputPin(GPIOx, GPIO_PIN_x); }

    ///Toggles the pin value
    inline void Toggle() { LL_GPIO_TogglePin(GPIOx, GPIO_PIN_x); }

    ///Writes a specified state into the pin
    inline void Write(GpioState state) { (state == _High) ? Set() : Clear(); }

    ///Reads the value of the pin
    inline uint32_t Read() { return LL_GPIO_IsInputPinSet(GPIOx, GPIO_PIN_x); }

    ///Gets the current input state
    inline GpioState GetInputState() { return (GpioState)LL_GPIO_IsInputPinSet(GPIOx, GPIO_PIN_x); }

    ///Gets the current output state
    inline GpioState GetOutputState() { return (GpioState)LL_GPIO_IsOutputPinSet(GPIOx, GPIO_PIN_x); }
};

#endif // !__VK_DRV_GPIO_H__
