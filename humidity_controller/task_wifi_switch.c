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
#define SWITCH_PIN 1

// define outputs
#define ON 1 // high voltage
#define OFF 0 // low voltage

// Pullup/pulldown resistors
#define ENABLE_PULLUP 1
#define DISABLE_PULLUP 0

#define ENABLE_PULLDOWN 1
#define DISABLE_PULLDOWN 0

static void wifi_switch_init(void);

TaskHandle_t wifi_Task_Handle_t;

/* LED task */
void task_wifi_switch(void *pvParameters)
{
  printf("LED task started\r\n");

    // define LEDs as outputs
  bl_gpio_enable_output(LED_G_PIN, DISABLE_PULLUP, DISABLE_PULLDOWN);
  bl_gpio_enable_output(LED_R_PIN, DISABLE_PULLUP, DISABLE_PULLDOWN);
  bl_gpio_enable_input(SWITCH_PIN, ENABLE_PULLUP, ENABLE_PULLDOWN );


  uint8_t pin_return_value = 0;

    // wait for 100ms
  vTaskDelay(100 / portTICK_PERIOD_MS);


  int pin_return;
  bool isWifiOn = true;
    //bl_gpio_output_set(LED_G_PIN, OFF);

  while (1) {
      //printf("new loop: \r\n");

    pin_return = bl_gpio_input_get(SWITCH_PIN, & pin_return_value);

      // print switch status
      /*printf("GPIO%d val is %s\r\n",
       SWITCH_PIN,
       0 == pin_return ? (pin_return_value ? "high" : "low") : "Err"
       );*/


    if (pin_return == 0 && pin_return_value){
      vTaskDelay(10 / portTICK_PERIOD_MS);
          //conforming switch press again
      pin_return = bl_gpio_input_get(SWITCH_PIN, & pin_return_value);
      if (pin_return == 0 && pin_return_value){

            if (isWifiOn){ //wifi need to be turn on
              printf("wifi turning on\r\n");

              wifi_switch_init();


              bl_gpio_output_set(LED_G_PIN, ON);
              bl_gpio_output_set(LED_R_PIN, OFF);
              isWifiOn = !isWifiOn;
            } else{ //wifi need to be turn off
              printf("wifi turning off\r\n");


              vTaskDelete(wifi_Task_Handle_t);


              bl_gpio_output_set(LED_G_PIN, OFF);
              bl_gpio_output_set(LED_R_PIN, ON);
              isWifiOn = !isWifiOn;
            }


            vTaskDelay(1000 / portTICK_PERIOD_MS);

          }

        } 


      // wait for 1s
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }

  // should never happen but would delete the task and free allocated resources
      vTaskDelete(NULL);
}

static void wifi_switch_init(void){
  /* Define information containers for tasks */
  static StackType_t wifi_stack[1024];
  static StaticTask_t wifi_task;

  printf("[SYSTEM] Starting WiFi task\r\n");
  extern void task_wifi(void *pvParameters);
  
  wifi_Task_Handle_t = xTaskCreateStatic(
    task_wifi, 
    (char *)"wifi", 
    1024, 
    NULL, 
    16, 
    wifi_stack, 
    &wifi_task
    );

}
