#include "main.h"

static StaticTask_t idle_tcb;
static StackType_t  idle_stack[configMINIMAL_STACK_SIZE];

int main(void)
{
    __enable_irq();
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, FLASH_APP_OFFSET);

    vTaskStartScheduler();

    for (;;)
    {
        __NOP();
    }
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &idle_tcb;
    *ppxIdleTaskStackBuffer = idle_stack;
    *pulIdleTaskStackSize   = (uint32_t)(sizeof idle_stack / sizeof idle_stack[0]);
}