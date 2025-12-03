/* --------------------------------------------------------------------------
 * FreeRTOSConfig.h  -  GD32E23x (Cortex-M23, non-secure)
 *
 * - Static allocation only (no heap)
 * - No software timers (you can enable later if needed)
 * - Optional debug features (high-watermark, run-time stats, queue registry)
 *
 * IMPORTANT:
 *  - Provide implementations for:
 *       void vApplicationStackOverflowHook( TaskHandle_t, char * );
 *    when configCHECK_FOR_STACK_OVERFLOW == 2.
 *  - Provide vConfigureRunTimeStatsTimer() and ulGetRunTimeCounterValue()
 *    if DEBUG is defined (see bottom of file).
 * -------------------------------------------------------------------------- */

#pragma once

#include <stdint.h>
#include "system_gd32e23x.h"

/* --------------------------------------------------------------------------
 * Kernel timing
 * -------------------------------------------------------------------------- */

/* CPU clock used by the kernel (defined by system_gd32e23x.c) */
#define configCPU_CLOCK_HZ                  ( SystemCoreClock )

/* RTOS tick rate (1 kHz = 1 ms tick) */
#define configTICK_RATE_HZ                  ( ( TickType_t ) 1000U )


/* --------------------------------------------------------------------------
 * Scheduling & tasks
 * -------------------------------------------------------------------------- */

/* Preemptive scheduling (1 = preemptive, 0 = cooperative) */
#define configUSE_PREEMPTION                1

/* Round-robin between tasks of equal priority */
#define configUSE_TIME_SLICING              1

/* Maximum number of priorities (0 .. configMAX_PRIORITIES-1) */
#define configMAX_PRIORITIES                5

/* Minimal stack size for tasks created with configMINIMAL_STACK_SIZE
 * (units = words, so 96 * 4 = 384 bytes on Cortex-M). */
#define configMINIMAL_STACK_SIZE            ( ( uint16_t ) 96U )

/* Maximum length of task names (not critical, just ROM/RAM) */
#define configMAX_TASK_NAME_LEN             12

/* 0 = 32-bit tick (recommended), 1 = 16-bit tick */
#define configUSE_16_BIT_TICKS              0

/* Idle task yields CPU if higher-priority ready task exists */
#define configIDLE_SHOULD_YIELD             1

/* Enable task notifications (very lightweight synchronisation primitive) */
#define configUSE_TASK_NOTIFICATIONS        1


/* --------------------------------------------------------------------------
 * Memory model
 *  - We use ONLY static allocation to save RAM and avoid a heap.
 * -------------------------------------------------------------------------- */

#define configSUPPORT_STATIC_ALLOCATION     1
#define configSUPPORT_DYNAMIC_ALLOCATION    0   /* no pvPortMalloc / vPortFree */


/* --------------------------------------------------------------------------
 * RTOS objects: queues, mutexes, events, timers
 * -------------------------------------------------------------------------- */

/* Enable basic mutexes (for mutual exclusion) */
#define configUSE_MUTEXES                   1

/* Recursive mutexes (not needed unless you really want them) */
#define configUSE_RECURSIVE_MUTEXES         0

/* Counting semaphores (can be enabled if needed) */
#define configUSE_COUNTING_SEMAPHORES       0

/* Event groups (bitmask-style events, disabled to save code/RAM) */
#define configUSE_EVENT_GROUPS              0

/* Queue sets (rarely needed, disabled for footprint) */
#define configUSE_QUEUE_SETS                0

/* Software timers: disabled for now to keep things small.
 * If you enable (set to 1), you must define the timer task settings below. */
#define configUSE_TIMERS                    0

/* --- Timer task settings (only used if configUSE_TIMERS == 1) --- */
#define configTIMER_TASK_PRIORITY           ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH            8
#define configTIMER_TASK_STACK_DEPTH        ( configMINIMAL_STACK_SIZE * 2 )


/* --------------------------------------------------------------------------
 * Debug / trace support
 * -------------------------------------------------------------------------- */

/* Basic trace facility (task switch trace macros etc.) – OFF in release.
 * In DEBUG we enable this (see bottom). */
#define configUSE_TRACE_FACILITY            0

/* Queue registry:
 *   0  = disabled (no registry, smallest RAM)
 *   >0 = allow tools to look up queues/semaphores by name.
 *
 * In release we keep this at 0 and enable it only in DEBUG. */
#define configQUEUE_REGISTRY_SIZE           0

/* Record stack high address so tools can show stack end / usage and
 * high-watermarks. Overhead is tiny, so we keep it enabled always. */
#define configRECORD_STACK_HIGH_ADDRESS     1


/* --------------------------------------------------------------------------
 * Hooks
 * -------------------------------------------------------------------------- */

/* Idle hook (call vApplicationIdleHook) – 0 = disabled */
#define configUSE_IDLE_HOOK                 0

/* Tick hook (call vApplicationTickHook) – 0 = disabled */
#define configUSE_TICK_HOOK                 0

/* Stack overflow checking:
 *  0 = off
 *  1 = method 1
 *  2 = method 2 (more thorough for Cortex-M; requires
 *      vApplicationStackOverflowHook() in your code). */
#define configCHECK_FOR_STACK_OVERFLOW      0

/* Malloc failed hook (not used since we have no dynamic alloc) */
#define configUSE_MALLOC_FAILED_HOOK        0

/* ASSERT: halt the system with interrupts disabled */
#define configASSERT( x )                   \
    do {                                    \
        if( ( x ) == 0 ) {                  \
            taskDISABLE_INTERRUPTS();       \
            for( ;; ) { /* hang */ }        \
        }                                   \
    } while( 0 )


/* --------------------------------------------------------------------------
 * API selection
 *  - Set to 1 to include the API, 0 to exclude (saves code size).
 * -------------------------------------------------------------------------- */

#define INCLUDE_vTaskDelay                  1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_xTaskGetIdleTaskHandle      1
#define INCLUDE_xTaskGetSchedulerState      1

/* Stack high-watermark APIs:
 *  - default OFF in release, enabled in DEBUG.
 *  - uxTaskGetStackHighWaterMark / ...HighWaterMark2().
 */
#define INCLUDE_uxTaskGetStackHighWaterMark 0
#define INCLUDE_uxTaskGetStackHighWaterMark2 0


/* --------------------------------------------------------------------------
 * Run-time statistics (disabled in release)
 * -------------------------------------------------------------------------- */

/* Generate run-time stats for tasks (CPU time per task, etc.).
 * OFF in release, enabled in DEBUG block. */
#define configGENERATE_RUN_TIME_STATS          0

/* Formatted stats (vTaskGetRunTimeStats / vTaskList).
 * This requires dynamic allocation in the official implementation,
 * so we MUST leave this at 0 when configSUPPORT_DYNAMIC_ALLOCATION == 0. */
#define configUSE_STATS_FORMATTING_FUNCTIONS   0

/* When configGENERATE_RUN_TIME_STATS == 1, we also define:
 *  - configRUN_TIME_COUNTER_TYPE
 *  - portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
 *  - portGET_RUN_TIME_COUNTER_VALUE()
 * in the DEBUG block below. */


/* --------------------------------------------------------------------------
 * Cortex-M23 interrupt priority configuration (GD32E23x)
 * -------------------------------------------------------------------------- */

/* If CMSIS didn't set it, define the number of implemented priority bits.
 * GD32E23x has 4 priority levels -> 2 bits. */
#ifndef __NVIC_PRIO_BITS
    #define __NVIC_PRIO_BITS                2
#endif

#define configPRIO_BITS                     __NVIC_PRIO_BITS

/* Lowest and highest interrupt priority values in the library notation
 * (0 = highest priority, larger = lower priority). */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY      ( ( 1U << configPRIO_BITS ) - 1U )
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY ( 1U )

/* Kernel interrupt priority (used for SysTick, PendSV, SVC).
 * Shifted into the high bits as required by the ARM NVIC. */
#define configKERNEL_INTERRUPT_PRIORITY      ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8U - configPRIO_BITS ) )

/* Maximum interrupt priority from which FreeRTOS API calls are allowed.
 * Any ISR that uses "FromISR" APIs must have priority numerically >= this. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8U - configPRIO_BITS ) )


/* --------------------------------------------------------------------------
 * Cortex-M handler mapping to CMSIS names
 * -------------------------------------------------------------------------- */

#define vPortSVCHandler                      SVC_Handler
#define xPortPendSVHandler                   PendSV_Handler
#define xPortSysTickHandler                  SysTick_Handler


/* --------------------------------------------------------------------------
 * Port-specific toggles (ARMv8-M / Cortex-M23)
 * -------------------------------------------------------------------------- */

#define configENABLE_TRUSTZONE               0   /* Non-secure only */
#define configENABLE_FPU                     0   /* No FPU on GD32E23x */
#define configENABLE_MPU                     0   /* No MPU on GD32E23x */


/* --------------------------------------------------------------------------
 * DEBUG configuration overrides
 *
 *  - Enable additional diagnostics (high-watermarks, run-time stats, queue
 *    registry, trace facility).
 *  - Still NO dynamic allocation, NO formatted stats.
 * -------------------------------------------------------------------------- */
#ifdef DEBUG

    /* Override release defaults defined above */
    #undef INCLUDE_uxTaskGetStackHighWaterMark
    #undef INCLUDE_uxTaskGetStackHighWaterMark2
    #undef configUSE_TRACE_FACILITY
    #undef configGENERATE_RUN_TIME_STATS
    #undef configQUEUE_REGISTRY_SIZE

    /* Enable stack watermark APIs */
    #define INCLUDE_uxTaskGetStackHighWaterMark    1
    #define INCLUDE_uxTaskGetStackHighWaterMark2   1

    /* Basic trace hooks (e.g. for external tools) */
    // #define configUSE_TRACE_FACILITY               1

    /* Enable run-time stats collection (you must implement the hooks) */
    // #define configGENERATE_RUN_TIME_STATS          1

    /* Queue registry for debugging (queues/semaphores by name).
     * Adjust size as needed. */
    #define configQUEUE_REGISTRY_SIZE              10

    /* Still no formatted stats (avoids heap usage) */
    // #define configUSE_STATS_FORMATTING_FUNCTIONS   0

    /* Type of the run-time counter (e.g. 32-bit timer counter) */
    // #define configRUN_TIME_COUNTER_TYPE            uint32_t

    /* Hooks implemented in your application code (e.g. main.c):
     *
     *   void vConfigureRunTimeStatsTimer( void );
     *   uint32_t ulGetRunTimeCounterValue( void );
     *
     * vConfigureRunTimeStatsTimer() should configure a free-running timer
     * with a reasonably high resolution (e.g. TIMx at a few MHz).
     */
    // extern void     vConfigureRunTimeStatsTimer( void );
    // extern uint32_t ulGetRunTimeCounterValue( void );

    // #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()  vConfigureRunTimeStatsTimer()
    // #define portGET_RUN_TIME_COUNTER_VALUE()          ulGetRunTimeCounterValue()

#endif /* DEBUG */
