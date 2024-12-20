#include "basic.c"

i64 a,b,c;
i64 combo_operand(i64 n) {
    if (n >= 0 && n <= 3) return n;
    if (n == 4) return a;
    if (n == 5) return b;
    if (n == 6) return c;
    return -1;
}

void part_1(u8* content, u32 file_size) {
    printf("part 1: ");

    u8 state = 0;
    i64 n = 0;
    i8 opcode, operand;
    i32 program_start = -1;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i] - '0';
        } else if (content[i] == '\n' || content[i] == ',') {
            if      (state == 0) { a = n; n = 0; state++; }
            else if (state == 1) { b = n; n = 0; state++; }
            else if (state == 2) { c = n; n = 0; state++; }
            else if (state == 3) { state++; }
            else if (state == 4) {
                opcode = n; n = 0; state++;
                if (program_start == -1) program_start = i-1;
            }
            else if (state == 5) {
                operand = n;
                if (opcode == 0) {
                    a = a / (1 << combo_operand(operand));
                } else if (opcode == 1) {
                    b = b ^ operand;
                } else if (opcode == 2) {
                    b = combo_operand(operand) % 8;
                } else if (opcode == 3) {
                    if (a > 0) i = program_start + operand * 2 - 1;
                } else if (opcode == 4) {
                    b = b ^ c;
                } else if (opcode == 5) {
                    printf("%i,", (combo_operand(operand) % 8));
                } else if (opcode == 6) {
                    b = a / (1 << combo_operand(operand));
                } else if (opcode == 7) {
                    c = a / (1 << combo_operand(operand));
                }

                n = 0;
                state--;
            }
        }
    }
    printf("\n");
}

void part_2(u8* content, u32 file_size) {
    printf("part 2: ");

    printf("\n");
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input17.txt", &file_size);
    part_1(content, file_size);
    part_2(content, file_size);

    return 0;
}
