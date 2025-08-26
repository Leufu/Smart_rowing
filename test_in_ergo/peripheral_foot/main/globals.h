#pragma once
// Struct for sending the FSR data
#define FSR_SAMPLE_RATE 100
#include <cstddef>
#include <cstdint>
constexpr int PIN_IRQ = 4;

typedef struct 
{
	int64_t time_stamp;
	uint16_t	  FSR_0;
	uint16_t	  FSR_1;
	uint16_t	  FSR_2;
	uint16_t	  FSR_3;
	uint16_t	  FSR_4;
	uint16_t	  FSR_5;
	uint16_t	  FSR_6;
	uint16_t	  FSR_7;
}FSR_data_t;

struct TickMsg{int64_t stamp_us;};
/*
typedef struct 
{
	int64_t time_stamp;
	float	  acc_x;
	float	  acc_y;
	float	  acc_z;
	float	  gyr_x;
	float	  gyr_y;
	float	  gyr_z;
	float	  mag_x;
	float	  mag_y;
	float	  mag_z;
}IMU_data_t;
*/

#include "util.h"
#include "timer_header.h"
#include "ble_config.h"
#include "task_monitor.h"
#include "rtos_task.h"

