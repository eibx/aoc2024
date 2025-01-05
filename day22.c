#include "basic.c"
#include <string.h>

u64 mix(u64 v, u64 s) {
    return v ^ s;
}
u64 prune(u64 s) {
    return s % 16777216;
}

u64 part_1(u8* content, u32 file_size) {
    u64 secret = 0;
    u64 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] != '\n') {
            secret *= 10;
            secret += content[i]-'0';
        } else {
            for (i32 t = 0; t < 2000; t++) {
                u64 n;
                n = secret * 64;
                secret = mix(n, secret);
                secret = prune(secret);
                n = secret / 32;
                secret = mix(n, secret);
                secret = prune(secret);
                n = secret * 2048;
                secret = mix(n, secret);
                secret = prune(secret);
            }
            sum += secret;
            secret = 0;
        }
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    i8 (*cache)[19*19*19*19] = malloc(sizeof(i8[2500][19*19*19*19]));
    for (i32 y = 0; y < 2500; y++) {
        for (i32 x = 0; x < 19*19*19*19; x++) {
            cache[y][x] = -1;
        }
    }

    i32 x = 0;
    u64 secret = 0;
    i32 seller_count = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] != '\n') {
            secret *= 10;
            secret += content[i]-'0';
        } else {
            i8 deltas[4] = {};
            i8 p = secret % 10;
            for (i32 t = 0; t < 2000; t++) {
                u64 n;
                n = secret * 64;
                secret = mix(n, secret);
                secret = prune(secret);
                n = secret / 32;
                secret = mix(n, secret);
                secret = prune(secret);
                n = secret * 2048;
                secret = mix(n, secret);
                secret = prune(secret);
                n = secret % 10;

                deltas[0] = deltas[1];
                deltas[1] = deltas[2];
                deltas[2] = deltas[3];
                deltas[3] = n-p;

                if (t > 2) {
                    i32 index = 0;
                    index += (deltas[0]+9) * 19 * 19 * 19;
                    index += (deltas[1]+9) * 19 * 19;
                    index += (deltas[2]+9) * 19;
                    index += (deltas[3]+9);
                    if (cache[seller_count][index] == -1) {
                        cache[seller_count][index] = n;
                    }
                }

                p = n;
            }
            secret = 0;
            seller_count++;
        }
    }

    u64 sum = 0;
    i8 goal[4] = { -9, -9, -9, -9 };
    i8 best[4] = {};
    while (true) {
        goal[0]++;
        if (goal[0] == 10) { goal[0] = -9; goal[1]++; }
        if (goal[1] == 10) { goal[1] = -9; goal[2]++; }
        if (goal[2] == 10) { goal[2] = -9; goal[3]++; }
        if (goal[3] == 10) { break; }

        i32 index = 0;
        index += (goal[0]+9) * 19 * 19 * 19;
        index += (goal[1]+9) * 19 * 19;
        index += (goal[2]+9) * 19;
        index += (goal[3]+9);

        i32 result = 0;
        for (i32 i = 0; i < seller_count; i++) {
            if (cache[i][index] > 0)
                result += cache[i][index];
        }

        if (result > sum) {
            sum = result;
            best[0] = goal[0];
            best[1] = goal[1];
            best[2] = goal[2];
            best[3] = goal[3];
        }
    }

    i32 index = 0;
    index += (best[0]+9) * 19 * 19 * 19;
    index += (best[1]+9) * 19 * 19;
    index += (best[2]+9) * 19;
    index += (best[3]+9);

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input22.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
