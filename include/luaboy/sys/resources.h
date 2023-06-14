#pragma once

#include <freertos/semphr.h>

inline SemaphoreHandle_t xSemaphoreSPI = xSemaphoreCreateMutex();
