#pragma once
#include <windows.h>

void DelayMicroseconds(LONGLONG microseconds) {
    LARGE_INTEGER frequency, start, current;

    // Get the high-resolution performance counter frequency
    QueryPerformanceFrequency(&frequency);

    // Calculate total ticks required for the delay
    LONGLONG ticks = (microseconds * frequency.QuadPart) / 1000000LL;

    // Get starting time
    QueryPerformanceCounter(&start);

    // Busy-wait loop
    do {
        QueryPerformanceCounter(&current);
    } while (current.QuadPart - start.QuadPart < ticks);
}