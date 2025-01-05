#include "basic.c"
#include <string.h>

i32 compare(const void* a, const void* b) {
    return (*(i32*)a - *(i32*)b);
}

typedef struct {
    u8 name[4];
} GateName;

GateName i2n(i32 i) {
    GateName n = {};
    n.name[2] = (i % 36);
    n.name[2] += (n.name[2] >= 26) ? '0'-26 : 'a';
    n.name[1] = ((i/36) % 36);
    n.name[1] += (n.name[1] >= 26) ? '0'-26 : 'a';
    n.name[0] = ((i/36/36) % 36);
    n.name[0] += (n.name[0] >= 26) ? '0'-26 : 'a';
    n.name[3] = 0;
    return n;
}

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

typedef struct {
    i32 wire;
    i32 output;
} Output;

typedef struct {
    i32 wire1;
    i32 wire2;
    i32 operator;
    i32 target;
} Gate;

Output outputs[1024];
i32 outputs_count = 0;

Gate gates[1024];
i32 gates_count = 0;

u64 part_2(u8* content, u32 file_size) {
    u8 state = 0;
    Gate gate;

    //---------------------------------------------------------------------------//
    // Read file
    for (i32 i = 0; i < file_size; i++) {
        if (state == 0) {
            if (content[i] == '\n') {
                i32 wire = n2i(content[i-6], content[i-5], content[i-4]);
                u8 output = content[i-1]-'0';
                outputs[outputs_count++] = (Output){ wire, output };

                if (content[i+1] == '\n') {
                    state++;
                    i++;
                }
            }
        } else if (state == 1) {
            gate.wire1 = n2i(content[i+0], content[i+1], content[i+2]);
            i += 3;
            state++;
        } else if (state == 2) {
            if      (content[i] == 'A') { gate.operator = 'A'; i += 3; }
            else if (content[i] == 'X') { gate.operator = 'X'; i += 3; }
            else if (content[i] == 'O') { gate.operator = 'O'; i += 2; }
            state++;
        } else if (state == 3) {
            gate.wire2 = n2i(content[i+0], content[i+1], content[i+2]);
            i += 6;
            state++;
        } else if (state == 4) {
            if (gate.wire2 < gate.wire1) {
                i32 tmp = gate.wire1;
                gate.wire1 = gate.wire2;
                gate.wire2 = tmp;
            }

            gate.target = n2i(content[i+0], content[i+1], content[i+2]);
            gates[gates_count++] = gate;
            i += 3;
            state = 1;
        }
    }

    //---------------------------------------------------------------------------//
    // Process gates
    i32 result[50];
    i32 result_count = 0;
    for (i32 i = 0; i < gates_count; i++) {
        GateName wire1 = i2n(gates[i].wire1);
        GateName wire2 = i2n(gates[i].wire2);
        GateName target = i2n(gates[i].target);
        if (target.name[0] == 'z' && gates[i].operator != 'X' && gates[i].target != n2i('z','4','5')) {
            result[result_count++] = gates[i].target;
        }

        if (target.name[0] != 'z' && gates[i].operator == 'X' && wire1.name[0] != 'x') {
            result[result_count++] = gates[i].target;
        }

        if (gates[i].operator == 'A' && gates[i].wire1 != n2i('x','0','0')) {
            for (i32 j = 0; j < gates_count; j++) {
                if (gates[j].operator == 'O') continue;

                if (gates[j].wire1 == gates[i].target || gates[j].wire2 == gates[i].target) {
                    result[result_count++] = gates[i].target;
                    break;
                }
            }
        }

        if (gates[i].operator == 'O') {
            for (i32 j = 0; j < gates_count; j++) {
                if (gates[j].operator == 'A') continue;

                if (gates[j].target == gates[i].wire1)
                    result[result_count++] = gates[j].target;
                if (gates[j].target == gates[i].wire2)
                    result[result_count++] = gates[j].target;
            }
        }
    }

    qsort(result, result_count, sizeof(i32), compare);
    i32 previous = result[0];
    printf("%s,", i2n(previous).name);

    for (i32 i = 0; i < result_count; i++) {
        if (previous == result[i]) continue;

        printf("%s,", i2n(result[i]).name);
        previous = result[i];
    }
    printf("\n");

    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input24.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: "); part_2(content, file_size);

    return 0;
}
