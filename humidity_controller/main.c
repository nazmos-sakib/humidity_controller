#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>


#include <bl_dma.h>
#include <bl_gpio.h>
#include <bl_irq.h>
#include <bl_sec.h>
#include <bl_sys_time.h>
#include <bl_uart.h>
#include <hal_boot2.h>
#include <hal_board.h>
#include <hal_hwtimer.h>

#include <blog.h>
#include <lwip/tcpip.h>


/* Define heap regions */
extern uint8_t _heap_start;
extern uint8_t _heap_size;
extern uint8_t _heap_wifi_start;
extern uint8_t _heap_wifi_size;

/* Size of the stack for the task */
#define WIFI_SWITCH_STACK_SIZE 512


static HeapRegion_t xHeapRegions[] =
{
  { &_heap_start, (unsigned int) &_heap_size},
  { &_heap_wifi_start, (unsigned int) &_heap_wifi_size },
  { NULL, 0},
  { NULL, 0}
};

static void system_init(void);
static void wifi_switch_init(void);

/* main function, execution starts here */
void bfl_main(void)
{

  /* Initialize UART
   * Ports: 16+7 (TX+RX)
   * Baudrate: 2 million
   */
  bl_uart_init(0, 16, 7, 255, 255, 2 * 1000 * 1000);
  printf("[SYSTEM] Starting up!\r\n");
  
  /* (Re)define Heap */
  vPortDefineHeapRegions(xHeapRegions);
  
  /* Initialize system */
  system_init();
  /* Start tasks */

	//wifi switch
 	wifi_switch_init();
  

  /* Start TCP/IP stack */
  printf("[SYSTEM] Starting TCP/IP stack\r\n");
  tcpip_init(NULL, NULL);
  
  /* Start scheduler */
  printf("[SYSTEM] Starting scheduler\r\n");
  vTaskStartScheduler();
}


static void system_init(void){
	/* Initialize system */
	blog_init();
	bl_irq_init();
	bl_sec_init();
	bl_dma_init();
	hal_boot2_init();
	//board config is set after system is initialized
	hal_board_cfg(0);
}

static void wifi_switch_init(void){
	/* Define information containers for tasks */
	static StackType_t wifi_switch_stack[WIFI_SWITCH_STACK_SIZE];
	static StaticTask_t wifi_switch_task;

	printf("[SYSTEM] Starting WiFi SWITCH task\r\n");
	extern void task_wifi_switch(void *pvParameters);
	
	 
	xTaskCreateStatic(
		task_wifi_switch, 
		(char*)"wifi_switch", 
		WIFI_SWITCH_STACK_SIZE, 
		NULL, 
		16, 
		wifi_switch_stack, 
		&wifi_switch_task
	);

}