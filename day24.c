#include "basic.c"
#include <string.h>

i32 n2i(u8 a, u8 b, u8 c) {
    if (a >= '0' && a <= '9') { a = (a-'0')+26; } else { a = a-'a'; };
    if (b >= '0' && b <= '9') { b = (b-'0')+26; } else { b = b-'a'; };
    if (c >= '0' && c <= '9') { c = (c-'0')+26; } else { c = c-'a'; };
    return c + (b*36) + (a*36*36);
}

u64 part_1(u8* content, u32 file_size) {
    i8* bits = (i8*)malloc(36*36*36);
    memset(bits, -1, 36*36*36);

    u8 state = 0;
    u8 operator = 0;
    i32 section_2 = 0;
    i32 index_a = 0;
    i32 index_b = 0;
    b8 resolvable = true;
    b8 has_unresolved = false;
    i32 i = 0;
    do {
        has_unresolved = false;
        for (; i < file_size; i++) {
            if (state == 0) {
                if (content[i] == '\n') {
                    i32 index = n2i(content[i-6], content[i-5], content[i-4]);
                    u8 value = content[i-1]-'0';
                    bits[index] = value;

                    if (content[i+1] == '\n') {
                        state++;
                        i++;
                        section_2 = i+1;
                    }
                }
            } else if (state == 1) {
                index_a = n2i(content[i+0], content[i+1], content[i+2]);
                if (bits[index_a] == -1) resolvable = false;
                i += 3;
                state++;
            } else if (state == 2) {
                if      (content[i] == 'A') { operator = 'A'; i += 3; }
                else if (content[i] == 'X') { operator = 'X'; i += 3; }
                else if (content[i] == 'O') { operator = 'O'; i += 2; }
                state++;
            } else if (state == 3) {
                index_b = n2i(content[i+0], content[i+1], content[i+2]);
                if (bits[index_b] == -1) resolvable = false;
                i += 6;
                state++;
            } else if (state == 4) {
                i32 index = n2i(content[i+0], content[i+1], content[i+2]);
                if (resolvable) {
                    if      (operator == 'A') { bits[index] = bits[index_a] & bits[index_b]; }
                    else if (operator == 'X') { bits[index] = bits[index_a] ^ bits[index_b]; }
                    else if (operator == 'O') { bits[index] = bits[index_a] | bits[index_b]; }
                } else {
                    has_unresolved = true;
                }
                resolvable = true;
                i += 3;
                state = 1;
            }
        }

        if (has_unresolved) {
            state = 1;
            i = section_2;
        }
    } while (has_unresolved);

    u64 result = 0;
    for (i32 i = n2i('z','9','9'); i >= n2i('z','0','0'); i--) {
        if (bits[i] == -1) continue;
        if (bits[i] == 1) result = result | 1;
        result = result << 1;
    }

    return result >> 1;
}

u64 part_2(u8* content, u32 file_size) {
    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input24.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
