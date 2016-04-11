#include "stm32f4xx.h"
#include "stdlib.h"
#include "definitions.h"
#include "initPeripheral.h"
#include "userLibrary.h"
#include "VTimer.h"
#include "mbmaster.h"
#include "usart.h"
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
unsigned char emptyCementFbk;
unsigned char emptyWaterFbk;
unsigned char openCloseMixerFbk;
unsigned char cartIsUpFbk;
unsigned char cartIsDownFbk;
unsigned char cartIsReadyFbk;
unsigned char loosedCartRopeFbk;

short inertScaleValue, waterScaleValue, cementScaleValue;

// LED commands
unsigned char manualModeLED;
unsigned char autoModeLED;

//create recipe
ConcreteRecipe recipe;

//State variables
tAutoModeStates autoModeState;

extern tAutoModeStates autoModeState;
extern unsigned char MBMasterResponseBuffer[RESPONSE_MAX_SIZE];

//Initialize all needed paripherals and states
void InitController(void)
{
    InitSwitch(MANUAL_AUTO_MODE_SWITCH);

    //sensors
    InitInput(EMPTY_CEMENT_INPUT);
    InitInput(EMPTY_WATER_INPUT);
    InitInput(OPEN_CLOSE_MIXER_INPUT);
    InitInput(SKIP_CART_UP_INPUT);
    InitInput(SKIP_CART_DOWN_INPUT);
    InitInput(SKIP_CART_READY_INPUT);
    InitInput(LOOSE_SKIP_CART_ROPE_INPUT);
    InitADC(CEMENT_SCALE_ANALOG_INPUT);

    //manual control signal
    InitSwitch(EMPTY_WATER_SWITCH);
    InitButton(DOSE_SAND_BUTTON);
    InitButton(DOSE_GRAVEL_BUTTON);
    InitButton(DOSE_CEMENT_BUTTON);
    InitButton(DOSE_WATER_BUTTON);
    InitButton(EMPTY_CEMENT_BUTTON);
    InitButton(SKIP_CART_UP_BUTTON);
    InitButton(SKIP_CART_DOWN_BUTTON);
    InitButton(OPEN_CLOSE_MIXER_BUTTON);

    //auto control signal
    InitOutput(DOSE_SAND_OUTPUT);
    InitOutput(DOSE_GRAVEL_OUTPUT);
    InitOutput(DOSE_CEMENT_OUTPUT);
    InitOutput(DOSE_WATER_OUTPUT);
    InitOutput(EMPTY_CEMENT_OUTPUT);
    InitOutput(EMPTY_WATER_OUTPUT);
    InitOutput(SKIP_CART_UP_OUTPUT);
    InitOutput(SKIP_CART_DOWN_OUTPUT);
    InitOutput(OPEN_CLOSE_MIXER_OUTPUT);
    
    //Init LEDs
    InitLED(CEMENT_VALVE_LED);
    InitLED(WATER_VALVE_LED);
    InitLED(MIXER_VALVE_LED);
    InitLED(CART_IS_DOWN_LED);
    InitLED(CART_IS_UP_LED);
    InitLED(MANUAL_MODE_LED);
    InitLED(AUTO_MODE_LED);
    
    //initialize timers
    InitVTimers();
    
    //Initialize usart2 for ModBus master communication
    InitUSART2(MB_MASTER_UNIT);
    
    //init auto mode
    autoModeState = ePrepareForRecipeExecution;
        
    //initialize the recipe
    recipe.sandQuantity = SAND_QUANTITY;
    recipe.gravelQuantity = GRAVEL_QUANTITY;
    recipe.cementQuantity = CEMENT_QUANTITY;
    recipe.waterQuantity = WATER_QUANTITY;
    recipe.timeForMixing = MIXING_TIME;
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
    SetDigitalOutput(EMPTY_WATER_OUTPUT, emptyWaterCmd);
    SetDigitalOutput(SKIP_CART_UP_OUTPUT, skipCartUpCmd);
    SetDigitalOutput(SKIP_CART_DOWN_OUTPUT, skipCartDownCmd);
    SetDigitalOutput(OPEN_CLOSE_MIXER_OUTPUT, openCloseMixerCmd);  
    
    manualModeLED = ON;
    autoModeLED = OFF;
}

void ReadScaleValues(void)
{
    // read cement scale's value from analog input
    cementScaleValue = GetAnalogInput(CEMENT_SCALE_ANALOG_INPUT);
    
    // read inert scale's value with ModBus communication
    ReadHoldingRegisters(INERT_SCALE_VALUE_ADDRESS, 0, 1);
    MBMaster();
    inertScaleValue = *(MBMasterResponseBuffer + 3);
    inertScaleValue = (inertScaleValue << 8) | MBMasterResponseBuffer[4];
    
    // read water scale's value with ModBus communication
    ReadHoldingRegisters(WATER_SCALE_VALUE_ADDRESS, 0, 1);
    MBMaster();
    waterScaleValue = *(MBMasterResponseBuffer + 3);
    waterScaleValue = (waterScaleValue << 8) | MBMasterResponseBuffer[4];
}

void ReadSensorSignals(void)
{
    emptyCementFbk = GetDigitalInput(EMPTY_CEMENT_INPUT);
    emptyWaterFbk = GetDigitalInput(EMPTY_WATER_INPUT);
    openCloseMixerFbk = GetDigitalInput(OPEN_CLOSE_MIXER_INPUT);
    cartIsUpFbk = GetDigitalInput(SKIP_CART_UP_INPUT);
    cartIsDownFbk = GetDigitalInput(SKIP_CART_DOWN_INPUT);
    cartIsReadyFbk = GetDigitalInput(SKIP_CART_READY_INPUT);
    loosedCartRopeFbk = GetDigitalInput(LOOSE_SKIP_CART_ROPE_INPUT);
}

void SetLEDs(void)
{
    SetLED(CEMENT_VALVE_LED, emptyCementFbk);
    SetLED(WATER_VALVE_LED, emptyWaterFbk);
    SetLED(MIXER_VALVE_LED, openCloseMixerFbk);
    SetLED(CART_IS_DOWN_LED, cartIsDownFbk);
    SetLED(CART_IS_UP_LED, cartIsUpFbk);
    
    SetLED(MANUAL_MODE_LED, manualModeLED);
    SetLED(AUTO_MODE_LED, autoModeLED);
}

//check whether simulator is in alarm - if it is - do not call this function, if it is not in alarm - call it
void ControllerInAutoMode(void)
{
    switch(autoModeState)
    {
    case ePrepareForRecipeExecution:
        {
            if(cartIsDownFbk == ON)
            {
                //cart is down
                skipCartDownCmd = OFF;
                autoModeState = eDoseSand;
            }
            else
            {
                skipCartDownCmd = ON;
            }
            
            SetDigitalOutput(SKIP_CART_DOWN_OUTPUT, skipCartDownCmd);
            break;
        }
    case eDoseSand:
        {
            if(inertScaleValue < recipe.sandQuantity)
            {
                doseSandCmd = ON;
            }
            else
            {
                doseSandCmd = OFF;
                    
                recipe.gravelQuantity = inertScaleValue + recipe.gravelQuantity;
                
                autoModeState = eDoseGravel;
            }
            
            SetDigitalOutput(DOSE_SAND_OUTPUT, doseSandCmd);
            
            break;
        }
    case eDoseGravel:
        {
            if(inertScaleValue < recipe.gravelQuantity)
            {
                doseGravelCmd = ON;
            }
            else
            {
                doseGravelCmd = OFF;
                
                autoModeState = eDoseCement;
            }
            
            SetDigitalOutput(DOSE_GRAVEL_OUTPUT, doseGravelCmd);
            
            break;
        }
    case eDoseCement:
        {
            if(cementScaleValue < recipe.cementQuantity)
            {
                doseCementCmd = ON;
            }
            else
            {
                doseCementCmd = OFF;
                
                autoModeState = eDoseWater;
            }
            
            SetDigitalOutput(DOSE_CEMENT_OUTPUT, doseCementCmd);
            
            break;
        }
    case eDoseWater:
        {
            if(waterScaleValue < recipe.waterQuantity)
            {
                doseWaterCmd = ON;
            }
            else
            {
                doseWaterCmd = OFF;
                
                autoModeState = eEmptyInertMaterials;
            }
            
            SetDigitalOutput(DOSE_WATER_OUTPUT, doseWaterCmd);
            break;
        }
    case eEmptyInertMaterials:
        {
            if(cartIsUpFbk == ON)
            {
                skipCartUpCmd = OFF;
                autoModeState = eEmptyCement;
            }
            else
            {
                skipCartUpCmd = ON;
            }
            
            SetDigitalOutput(SKIP_CART_UP_OUTPUT, skipCartUpCmd);
            break;
        }
    case eEmptyCement:
        {
            if(emptyCementFbk == CLOSED)
            {
                emptyCementCmd = ON;
            }
            else
            {
                emptyCementCmd = OFF;
                autoModeState = eEmptyWater;
            }
            
            SetDigitalOutput(EMPTY_CEMENT_OUTPUT, emptyCementCmd);
            
            break;
        }
    case eEmptyWater:
        {
            if(emptyWaterFbk == CLOSED)
            {
                emptyWaterCmd = ON;
            }
            else
            {
                emptyWaterCmd = OFF;
                autoModeState = eMixingMaterials;
            }
            
            SetDigitalOutput(EMPTY_WATER_OUTPUT, emptyWaterCmd);
            break;
        }
    case eMixingMaterials:
        {
            while(IsVTimerElapsed(MIXING_TIME) == NOT_ELAPSED)
            {
                //print message "Mixing!"
            }
            
            autoModeState = eOpenMixer;
            break;
        }
    case eOpenMixer:
        {
            if(openCloseMixerFbk == CLOSED)
            {
                openCloseMixerCmd = ON;
            }
            else
            {
                openCloseMixerCmd = OFF;
//                autoModeState = eEmptyInertMaterials;
            }            
            
            SetDigitalOutput(OPEN_CLOSE_MIXER_OUTPUT, openCloseMixerCmd);
            
            break;
        }
    }
    
    manualModeLED = OFF;
    autoModeLED = ON;
}



