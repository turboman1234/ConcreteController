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
#include "LCD.h"

extern RCC_ClocksTypeDef MYCLOCKS;
extern ModBusSlaveUnit ModBusSlaves[MAX_MODBUS_SLAVE_DEVICES];
extern tAutoModeStates autoModeState;

extern int controllerWorkMode = OFF; //init in manual state; 

//Test Functions
//void TestOutputsAndButtons(void);
//void InitOutputsAndButtons(void);



int main()
{   
    InitRCC();
    
    InitController();
    
    controllerWorkMode = GetSwitchState(MANUAL_AUTO_MODE_SWITCH);
    
    InitLCD();
//    InitOutputsAndButtons();
    
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
                if(controllerWorkMode == ON)
                {
                    autoModeState = ePrepareForRecipeExecution;
                }
            }
        }
        
        SetLEDs();
        
        if(IsVTimerElapsed(LCD_REFRESH_TIMER) == ELAPSED)
        {
            LCDTask();
            SetVTimerValue(LCD_REFRESH_TIMER, T_500_MS);
        }
        
        
        //TestOutputsAndButtons();
    }
}

//int b1, b2, b3, b4, b5, b6, b7, b8;
//int o1, o2, o3, o4, o5, o6, o7, o8, o9;
//int i1, i2, i3, i4, i5, i6, i7, i8, i9;
//
//void TestOutputsAndButtons(void)
//{
//    b1 = GetButtonState(BUTTON_1);
//    b2 = GetButtonState(BUTTON_2);
//    b3 = GetButtonState(BUTTON_3);
//    b4 = GetButtonState(BUTTON_4);
//    b5 = GetButtonState(BUTTON_5);
//    b6 = GetButtonState(BUTTON_6);
//    b7 = GetButtonState(BUTTON_7);
//    b8 = GetButtonState(BUTTON_8);
//    
//    SetDigitalOutput(OUTPUT_9, b1);
//    SetDigitalOutput(OUTPUT_10, b2);
//    SetDigitalOutput(OUTPUT_11, b3);
//    SetDigitalOutput(OUTPUT_12, b4);
//    SetDigitalOutput(OUTPUT_13, b5);
//    SetDigitalOutput(OUTPUT_14, b6);
//    SetDigitalOutput(OUTPUT_15, b7);
//    SetDigitalOutput(OUTPUT_16, b8);    
//}
//
//void InitOutputsAndButtons(void)
//{
//    InitButton(BUTTON_1);
//    InitButton(BUTTON_2);
//    InitButton(BUTTON_3);
//    InitButton(BUTTON_4);
//    InitButton(BUTTON_5);
//    InitButton(BUTTON_6);
//    InitButton(BUTTON_7);
//    InitButton(BUTTON_8);
//    
//    InitOutput(OUTPUT_1);
//    InitOutput(OUTPUT_2);
//    InitOutput(OUTPUT_3);
//    InitOutput(OUTPUT_4);
//    InitOutput(OUTPUT_5);
//    InitOutput(OUTPUT_6);
//    InitOutput(OUTPUT_7);
//    InitOutput(OUTPUT_8);
//    InitOutput(OUTPUT_9);
//    InitOutput(OUTPUT_10);
//    InitOutput(OUTPUT_11);
//    InitOutput(OUTPUT_12);
//    InitOutput(OUTPUT_13);
//    InitOutput(OUTPUT_14);
//    InitOutput(OUTPUT_15);
//    InitOutput(OUTPUT_16);
//    
//}
