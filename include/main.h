#pragma once
#include "stdbool.h"
#include "gd32e23x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "message_buffer.h"
#include "semphr.h"

/* ==== Misc Config ==== */
#ifndef FW_VERSION_MAJOR
#define FW_VERSION_MAJOR 0
#endif

#ifndef FW_VERSION_MINOR
#define FW_VERSION_MINOR 0
#endif

#ifndef FW_VERSION_PATCH
#define FW_VERSION_PATCH 0
#endif

#define FIRMWARE_VERSION ((FW_VERSION_MAJOR << 16) | (FW_VERSION_MINOR << 8) | FW_VERSION_PATCH)

#ifndef PRODUCT_ID
#else PRODUCT_ID  0000000
#endif

/* ==== Flash Configuration ==== */
#define FLASH_APP_OFFSET        0x00002000U
#define RESERVED_BASE_ADDR      0x0800FC00U