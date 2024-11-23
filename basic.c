#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef bool b8;

u8* load_file(const char* filename, u32* file_size) {
    FILE* file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    u8* buffer = (u8*)malloc(*file_size + 1);
    fread(buffer, 1, *file_size, file);
    buffer[*file_size] = '\0';

    return buffer;
}
