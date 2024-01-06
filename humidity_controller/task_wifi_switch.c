// FreeRTOS
#include <FreeRTOS.h>
#include <task.h>

// Input/output
#include <stdio.h>
#include <stdbool.h>


// GPIO library
#include <bl_gpio.h>

// Define LED pins
#define LED_R_PIN 17
#define LED_G_PIN 14
#define LED_B_PIN 11

//switch
#define SWITCH_PIN 3

// define outputs
#define ON 1 // high voltage
#define OFF 0 // low voltage

// Pullup/pulldown resistors
#define ENABLE_PULLUP 1
#define DISABLE_PULLUP 0

#define ENABLE_PULLDOWN 1
#define DISABLE_PULLDOWN 0

/* LED task */
void task_wifi_switch(void *pvParameters)
{
  printf("LED task started\r\n");
  
  // define LEDs as outputs
  bl_gpio_enable_output(LED_G_PIN, DISABLE_PULLUP, DISABLE_PULLDOWN);
  bl_gpio_enable_input(SWITCH_PIN, DISABLE_PULLUP, ENABLE_PULLDOWN);


  uint8_t pin_return_value = 0;
  
  // wait for 100ms
  vTaskDelay(100 / portTICK_PERIOD_MS);
  

  int pin_return;
  bool isOn = false;
  bl_gpio_output_set(LED_G_PIN, OFF);

  while (1) {
    printf("new loop: \r\n");

     pin_return = bl_gpio_input_get(SWITCH_PIN, & pin_return_value);
     printf("GPIO%d val is %s\r\n",
       SWITCH_PIN,
       0 == pin_return ? (pin_return_value ? "high" : "low") : "Err"
      );


     if (pin_return == 0 && pin_return_value){
          if (isOn){
            bl_gpio_output_set(LED_G_PIN, ON);
            isOn = !isOn;
          } else{
            bl_gpio_output_set(LED_G_PIN, OFF);
            isOn = !isOn;
          }
     } 
     
   /*
    if (counter & 0x1) {
      bl_gpio_output_set(LED_R_PIN, LED_ON);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    } else {
      bl_gpio_output_set(LED_R_PIN, LED_OFF);
    }
   */ 
    
    
    // wait for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  
  // should never happen but would delete the task and free allocated resources
  vTaskDelete(NULL);
}