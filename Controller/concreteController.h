#ifndef __CONCRETECONTROLLER_H
#define __CONCRETECONTROLLER_H

//Inputs from the plant
#define EMPTY_CEMENT_INPUT                       INPUT_1 // cement valve's state
#define EMPTY_WATER_INPUT                        INPUT_2 // water valve's state
#define OPEN_CLOSE_MIXER_INPUT                   INPUT_3 // mixer valve's state
#define SKIP_CART_UP_INPUT                       INPUT_4 // cart is UP
#define SKIP_CART_DOWN_INPUT                     INPUT_5 // cart is DOWN
#define SKIP_CART_READY_INPUT                    INPUT_6 // cart is READY
#define LOOSE_SKIP_CART_ROPE_INPUT               INPUT_7 // indicates alarm state

#define CEMENT_SCALE_ANALOG_INPUT                ADC_2
#define INERT_SCALE_VALUE_ADDRESS                1
#define WATER_SCALE_VALUE_ADDRESS                2


//Outputs command to the plant
#define DOSE_SAND_OUTPUT                         OUTPUT_1
#define DOSE_GRAVEL_OUTPUT                       OUTPUT_2
#define DOSE_CEMENT_OUTPUT                       OUTPUT_3
#define DOSE_WATER_OUTPUT                        OUTPUT_4
#define EMPTY_CEMENT_OUTPUT                      OUTPUT_5
#define EMPTY_WATER_OUTPUT                       OUTPUT_6
#define OPEN_CLOSE_MIXER_OUTPUT                  OUTPUT_7
#define SKIP_CART_UP_OUTPUT                      OUTPUT_8
#define SKIP_CART_DOWN_OUTPUT                    OUTPUT_9

//Buttons for manual plant controlling
#define DOSE_SAND_BUTTON                         BUTTON_1
#define DOSE_GRAVEL_BUTTON                       BUTTON_2
#define DOSE_CEMENT_BUTTON                       BUTTON_3
#define DOSE_WATER_BUTTON                        BUTTON_4
#define EMPTY_CEMENT_BUTTON                      BUTTON_5
#define OPEN_CLOSE_MIXER_BUTTON                  BUTTON_6
#define SKIP_CART_UP_BUTTON                      BUTTON_7
#define SKIP_CART_DOWN_BUTTON                    BUTTON_8

//LEDs
#define CEMENT_VALVE_LED                        LED_1
#define WATER_VALVE_LED                         LED_2
#define MIXER_VALVE_LED                         LED_3
#define CART_IS_DOWN_LED                        LED_4
#define CART_IS_UP_LED                          LED_5
#define MANUAL_MODE_LED                         LED_6
#define AUTO_MODE_LED                           LED_7

//Switches
#define MANUAL_AUTO_MODE_SWITCH                  SWITCH_1
#define EMPTY_WATER_SWITCH                       SWITCH_2

//Timers
#define MIXER_TIMER                              TIMER_1

// Constant Types
#define OPENED                                   OFF
#define CLOSED                                   ON

//Constants
#define SAND_QUANTITY                            500
#define GRAVEL_QUANTITY                          750
#define CEMENT_QUANTITY                          500
#define WATER_QUANTITY                           250
#define MIXING_TIME                              120 * T_1_S

typedef enum ControllerStates{
    ePrepareForRecipeExecution = 0xAA,
    eDoseSand,
    eDoseGravel,
    eDoseCement,
    eDoseWater,
    eEmptyInertMaterials,
    eEmptyCement,
    eEmptyWater,
    eMixingMaterials,
    eOpenMixer
}tAutoModeStates;

typedef struct RecipeType{
    unsigned short sandQuantity;
    unsigned short gravelQuantity;
    unsigned short cementQuantity;
    unsigned short waterQuantity;
    int timeForMixing;
}ConcreteRecipe;

//functions
void InitController(void);
void ControllerInManualMode(void);
void ReadScaleValues(void);
void ReadSensorSignals(void);
void ControllerInAutoMode(void);
void SetLEDs(void);


#endif

