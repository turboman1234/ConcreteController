#include "stm32f4xx.h"
#include "stdlib.h"
#include "definitions.h"
#include "userLibrary.h"
#include "mbmaster.h"
#include "concreteController.h"

// control signals
unsigned char doseSandCmd;
unsigned char doseGravelCmd;
unsigned char doseCementCmd;
unsigned char doseWaterCmd;
unsigned char emptyCementCmd;
unsigned char emptyWaterCmd;
unsigned char openCloseMixerCmd;
unsigned char skipCartUpCmd;
unsigned char skipCartDownCmd;

// input signals
unsigned char emptyValveFbk;
unsigned char emptyWaterFbk;
unsigned char openCloseMixerFbk;
unsigned char cartIsUpFbk;
unsigned char cartIsDownFbk;
unsigned char cartIsReadyFbk;
unsigned char loosedCartRopeFbk;

short inertScaleValue, waterScaleValue, cementScaleValue;

extern unsigned char MBMasterResponseBuffer[RESPONSE_MAX_SIZE];

void ControllerInAutoMode(void)
{
    ;
}

void ControllerInManualMode(void)
{
    //read buttons' states
    doseSandCmd = GetButtonState(DOSE_SAND_BUTTON);
    doseGravelCmd = GetButtonState(DOSE_GRAVEL_BUTTON);
    doseCementCmd = GetButtonState(DOSE_CEMENT_BUTTON);
    doseWaterCmd = GetButtonState(DOSE_WATER_BUTTON);
    emptyCementCmd = GetButtonState(EMPTY_CEMENT_BUTTON);
    skipCartUpCmd = GetButtonState(SKIP_CART_UP_BUTTON);
    skipCartDownCmd = GetButtonState(SKIP_CART_DOWN_BUTTON);
    openCloseMixerCmd = GetButtonState(OPEN_CLOSE_MIXER_BUTTON);
    
    emptyWaterCmd = GetSwitchState(EMPTY_WATER_SWITCH);
    
    
    //write buttons' states in outputs
    SetDigitalOutput(DOSE_SAND_OUTPUT, doseSandCmd);
    SetDigitalOutput(DOSE_GRAVEL_OUTPUT, doseGravelCmd);
    SetDigitalOutput(DOSE_CEMENT_OUTPUT, doseCementCmd);
    SetDigitalOutput(DOSE_WATER_OUTPUT, doseWaterCmd);
    SetDigitalOutput(EMPTY_CEMENT_OUTPUT, emptyCementCmd);
    SetDigitalOutput(SKIP_CART_UP_OUTPUT, skipCartUpCmd);
    SetDigitalOutput(SKIP_CART_DOWN_OUTPUT, skipCartDownCmd);
    SetDigitalOutput(OPEN_CLOSE_MIXER_OUTPUT, openCloseMixerCmd);    
}

void ReadScaleValues(void)
{
    // read cement scale's value from analog input
    cementScaleValue = GetAnalogInput(CEMENT_SCALE_ANALOG_INPUT);
    
    // read inert scale's value with ModBus communication
    ReadHoldingRegisters(INERT_SCALE_VALUE_ADDRESS, 0, 1);
    MBMaster();
    inertScaleValue = MBMasterResponseBuffer[0];
    
    // read water scale's value with ModBus communication
    ReadHoldingRegisters(WATER_SCALE_VALUE_ADDRESS, 0, 1);
    MBMaster();
    waterScaleValue = MBMasterResponseBuffer[0];
}

void ReadSensorSignals(void)
{
    emptyValveFbk = GetDigitalInput(EMPTY_CEMENT_INPUT);
    emptyWaterFbk = GetDigitalInput(EMPTY_WATER_INPUT);
    openCloseMixerFbk = GetDigitalInput(OPEN_CLOSE_MIXER_INPUT);
    cartIsUpFbk = GetDigitalInput(SKIP_CART_UP_INPUT);
    cartIsDownFbk = GetDigitalInput(SKIP_CART_DOWN_INPUT);
    cartIsReadyFbk = GetDigitalInput(SKIP_CART_READY_INPUT);
    loosedCartRopeFbk = GetDigitalInput(LOOSE_SKIP_CART_ROPE_INPUT);
}





