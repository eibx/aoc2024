#include "basic.c"

u64 num_digits(u64 n) {
    return
(n > 999999999999999999) ? 19 :
(n > 99999999999999999) ? 18 :
(n > 9999999999999999) ? 17 :
(n > 999999999999999) ? 16 :
(n > 99999999999999) ? 15 :
(n > 9999999999999) ? 14 :
(n > 999999999999) ? 13 :
(n > 99999999999) ? 12 :
(n > 9999999999) ? 11 :
(n > 999999999) ? 10 :
(n > 99999999) ? 9 :
(n > 9999999) ? 8 :
(n > 999999) ? 7 :
(n > 99999) ? 6 :
(n > 9999) ? 5 :
(n > 999) ? 4 :
(n > 99) ? 3 :
(n > 9) ? 2 : 1;
}

u64 power10(u64 n) {
    return
(n == 0)  ? 1 :
(n == 1)  ? 10 :
(n == 2)  ? 100 :
(n == 3)  ? 1000 :
(n == 4)  ? 10000 :
(n == 5)  ? 100000 :
(n == 6)  ? 1000000 :
(n == 7)  ? 10000000 :
(n == 8)  ? 100000000 :
(n == 9)  ? 1000000000 :
(n == 10) ? 10000000000 :
(n == 11) ? 100000000000 :
(n == 12) ? 1000000000000 :
(n == 13) ? 10000000000000 :
(n == 14) ? 100000000000000 :
(n == 15) ? 1000000000000000 :
(n == 16) ? 10000000000000000 :
(n == 17) ? 100000000000000000 : 1000000000000000000;
}

u64 part_1(u8* content, u32 file_size) {
    u64* stones = (u64*)malloc(200000*sizeof(u64));
    u64 stone_count = 0;

    u64 n = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i]-'0';
        } else {
            stones[stone_count++] = n;
            n = 0;
        }
    }

    for (u64 i = 0; i < 25; i++) {
        u64 current_count = stone_count;
        for (u64 j = 0; j < current_count; j++) {
            u64 number = stones[j];
            if (number == 0) {
                stones[j] = 1;
                continue;
            }

            u64 digits = num_digits(number);
            if (digits % 2 == 0) {
                u64 power = power10(digits / 2);
                u64 left = number / power;
                u64 right = number - (left * power);
                stones[j] = left;
                stones[stone_count++] = right;
            } else {
                stones[j] = number * 2024;
            }
        }
    }

    return stone_count;
}

u64 part_2(u8* content, u32 file_size) {
    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input11.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
