#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "system_stm32f4xx.h"
#include "definitions.h"
#include "rcc.h"
#include "initPeripheral.h"
#include "userLibrary.h"
#include "VTimer.h"
#include "usart.h"
#include "mytim.h"
#include "serial.h"
#include "mbslave.h"
#include "rs232.h"
#include "mbmaster.h"
#include "concreteController.h"

extern RCC_ClocksTypeDef MYCLOCKS;
extern ModBusSlaveUnit ModBusSlaves[MAX_MODBUS_SLAVE_DEVICES];
extern tAutoModeStates autoModeState;

int controllerWorkMode; 

int main()
{   
    InitRCC();
    
    InitController();
    
    controllerWorkMode = GetSwitchState(MANUAL_AUTO_MODE_SWITCH);
    
    while(1)
    {
        ReadSensorSignals();
        ReadScaleValues();
        
        if(controllerWorkMode == OFF)
        {
            //manual mode
            ControllerInManualMode();
            controllerWorkMode = GetSwitchState(MANUAL_AUTO_MODE_SWITCH);
            
            autoModeState = ePrepareForRecipeExecution;
        }
        else
        {
            //auto mode
            ControllerInAutoMode();
            if (autoModeState == eOpenMixer)
            {
                //The recipe is completed.
                controllerWorkMode = GetSwitchState(MANUAL_AUTO_MODE_SWITCH);
            }
        }
        
        SetLEDs();
        
    }
}

