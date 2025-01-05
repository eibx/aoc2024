#include "basic.c"

i64 a,b,c;
u8 program[24] = {};
i32 program_length = 0;

u8 output[24] = {};
i32 output_length = 0;

i64 combo_operand(i64 n) {
    if (n >= 0 && n <= 3) return n;
    if (n == 4) return a;
    if (n == 5) return b;
    if (n == 6) return c;
    return -1;
}

void compute() {
    output_length = 0;
    i8 opcode, operand;
    for (i32 i = 0; i < program_length; i+=2) {
        opcode = program[i];
        operand = program[i+1];
        if (opcode == 0) {
            a = a / (1 << combo_operand(operand));
        } else if (opcode == 1) {
            b = b ^ operand;
        } else if (opcode == 2) {
            b = combo_operand(operand) % 8;
        } else if (opcode == 3) {
            if (a > 0) i = operand - 2;
        } else if (opcode == 4) {
            b = b ^ c;
        } else if (opcode == 5) {
            output[output_length++] = combo_operand(operand) % 8;
        } else if (opcode == 6) {
            b = a / (1 << combo_operand(operand));
        } else if (opcode == 7) {
            c = a / (1 << combo_operand(operand));
        }
    }
}

void part_1(u8* content, u32 file_size) {
    printf("part 1: ");

    program_length = 0;
    u8 state = 0;
    i64 n = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i] - '0';
        } else if (content[i] == '\n' || content[i] == ',') {
            if      (state == 0) { a = n; n = 0; state++; }
            else if (state == 1) { b = n; n = 0; state++; }
            else if (state == 2) { c = n; n = 0; state++; }
            else if (state == 3) { state++; }
            else if (state == 4) { program[program_length++] = n; n = 0; }
        }
    }

    compute();
    for (i32 i = 0; i < output_length; i++) {
        printf("%i,", output[i]);
    }

    printf("\n");
}

i64 search(i64 reg, i64 tail) {
    for (i32 i = 0; i < 8; i++) {
        i64 new_a = reg * 8 + i;
        a = new_a;
        b = 0;
        c = 0;

        compute();

        if (program[program_length-tail] != output[output_length-tail]) { continue; }
        if (program_length == output_length) { return new_a; }

        i64 final = search(new_a, tail + 1);
        if (final != 0) return final;
    }

    return 0;
}

void part_2(u8* content, u32 file_size) {
    printf("part 2: ");

    program_length = 0;
    u8 state = 0;
    i64 n = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i] - '0';
        } else if (content[i] == '\n' || content[i] == ',') {
            if      (state == 0) { a = n; n = 0; state++; }
            else if (state == 1) { b = n; n = 0; state++; }
            else if (state == 2) { c = n; n = 0; state++; }
            else if (state == 3) { state++; }
            else if (state == 4) { program[program_length++] = n; n = 0; }
        }
    }

    printf("%lli\n", search(0, 1));

    printf("\n");
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input17.txt", &file_size);
    part_1(content, file_size);
    part_2(content, file_size);

    return 0;
}
