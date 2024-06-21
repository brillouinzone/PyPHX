#ifndef SIMPLE_ACQUISITION_H
#define SIMPLE_ACQUISITION_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


	// Declaration of the live function
	int live(
        char* file,
        uint16_t* globalBuffer,
        uint32_t globalBufferWidth,
        uint32_t globalBufferHeight,
        bool stop_loop,
        bool read_buffer,
        bool save_config
    );

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_ACQUISITION_H