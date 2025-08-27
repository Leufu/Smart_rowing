#pragma once
#include <esp_sntp.h>

// Simple helper to query the current time in microseconds.
struct timeval tv_now;

static volatile int64_t timer_0=0;
// lock para proteger la variable del timer 
static portMUX_TYPE t0mux= portMUX_INITIALIZER_UNLOCKED;

// Return time elapsed in microseconds since `timer_0` was set.
int64_t get_local_time_us()
{
        const int64_t now=esp_timer_get_time();
        int64_t t0_copy;

        portENTER_CRITICAL(&t0mux);
        t0_copy=timer_0;
        portEXIT_CRITICAL(&t0mux);

        if(t0_copy==0){return -1;}// nos aseguramos que se hayan sincronizado

        return now-t0_copy;
}


// Return the current system time in microseconds.
int64_t get_time_us()
{
        gettimeofday(&tv_now, NULL);
        int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        return time_us;
}
