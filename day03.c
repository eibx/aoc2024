#include "basic.c"
#include <string.h>

u8* c;
u32 fs;
u8 str_eq(i32 i, const char* str) {
    i32 str_len = strlen(str);
    if (i+str_len >= fs) return false;
    for (i32 si = 0; si < str_len; si++) {
        if (c[i+si] != str[si]) return false;
    }
    return true;
}

i32 part_1(u8* content, u32 file_size) {
    c = content;
    fs = file_size;
    i32 sum = 0;
    for (i32 i = 0; i < fs; i++) {
        if (str_eq(i, "mul(")) {
            i += 4;
            i32 number1 = 0;
            i32 number2 = 0;

            while (c[i] >= '0' && c[i] <= '9') {
                number1 *= 10;
                number1 += c[i] - '0';
                i++;
            }

            if (c[i] != ',') continue;
            i++;

            while (c[i] >= '0' && c[i] <= '9') {
                number2 *= 10;
                number2 += c[i] - '0';
                i++;
            }

            if (c[i] != ')') continue;

            sum += number1 * number2;
        }
    }

    return sum;
}

i32 part_2(u8* content, u32 file_size) {
    c = content;
    fs = file_size;
    i32 sum = 0;
    b8 active = true;
    for (i32 i = 0; i < fs; i++) {
        if (str_eq(i, "do()"))
            active = true;

        if (str_eq(i, "don't()"))
            active = false;

        if (str_eq(i, "mul(")) {
            i += 4;
            i32 number1 = 0;
            i32 number2 = 0;

            while (c[i] >= '0' && c[i] <= '9') {
                number1 *= 10;
                number1 += c[i] - '0';
                i++;
            }

            if (c[i] != ',') continue;
            i++;

            while (c[i] >= '0' && c[i] <= '9') {
                number2 *= 10;
                number2 += c[i] - '0';
                i++;
            }

            if (c[i] != ')') continue;

            if (active)
                sum += number1 * number2;
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input03.txt", &file_size);

    printf("part 1: %i\n", part_1(content, file_size));
    printf("part 2: %i\n", part_2(content, file_size));

    return 0;
}
