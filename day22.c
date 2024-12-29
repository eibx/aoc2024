#include "basic.c"

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
    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input22.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
