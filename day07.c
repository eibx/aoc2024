#include "basic.c"
#include <math.h>

u64 part_1(u8* content, u32 file_size) {
    u64 n = 0;
    u64 target = 0;
    u64 numbers[32] = {0};
    u64 numbers_index = 0;

    u64 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i] - '0';
        } else if (content[i] == ':') {
            target = n;
            n = 0;
        } else if (content[i] == ' ' || content[i] == '\n') {
            if (n == 0) continue;
            numbers[numbers_index++] = n;
            n = 0;

            if (content[i] == '\n') {
                u64 bitfield = 0;
                u64 power = 1 << numbers_index;
                for (i32 j = 0; j < power; j++) {
                    u64 c = numbers[0];
                    for (i32 k = 1; k < numbers_index; k++) {
                        if ((bitfield & (1 << k-1)) == 0) {
                            c += numbers[k];
                        } else {
                            c *= numbers[k];
                        }
                    }

                    if (c == target) {
                        sum += target;
                        break;
                    }

                    bitfield++;
                }
                numbers_index = 0;
            }
        }
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    u64 n = 0;
    u64 target = 0;
    u64 numbers[32] = {0};
    u64 numbers_index = 0;

    u64 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i] - '0';
        } else if (content[i] == ':') {
            target = n;
            n = 0;
        } else if (content[i] == ' ' || content[i] == '\n') {
            if (n == 0) continue;
            numbers[numbers_index++] = n;
            n = 0;

            if (content[i] == '\n') {
                u8 operators[32] = {0};
                u64 power = pow(3, numbers_index);
                for (i32 j = 0; j < power; j++) {
                    u64 c = numbers[0];
                    for (i32 k = 1; k < numbers_index; k++) {
                        if (operators[k-1] == 0) {
                            c += numbers[k];
                        } else if (operators[k-1] == 1) {
                            c *= numbers[k];
                        } else {
                            u64 number_of_digits = (u64)log10(numbers[k])+1;
                            u64 number_shift = (u64)pow(10, number_of_digits);
                            c = (c * number_shift) + numbers[k];
                        }
                    }

                    if (c == target) {
                        sum += target;
                        break;
                    }

                    for (i32 b = 0; b < 32; b++) {
                        if (operators[b] == 2) { operators[b] = 0; }
                        else { operators[b]++; break; }
                    }
                }
                numbers_index = 0;
            }
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input07.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
