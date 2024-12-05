#include "basic.c"
b8 order_rules[100][100] = {0};

i32 add_ordering_rules(u8* content, i32 file_size) {
    u8 state = 0;
    i32 n1 = 0;
    i32 n2 = 0;

    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            if (state == 0) {
                n1 *= 10;
                n1 += content[i] - '0';
            } else if (state == 1) {
                n2 *= 10;
                n2 += content[i] - '0';
            }
        } else if (content[i] == '|') {
            state = 1;
        } else if (content[i] == '\n') {
            if (state == 0) {
                // Done with ordering rules
                return i;
            } else {
                // New ordering rule
                order_rules[n1][n2] = 1;
                n1 = 0;
                n2 = 0;
                state = 0;
            }
        }
    }

    return 0;
}

i32 part_1(u8* content, u32 file_size) {
    i32 current_rule = 0;

    b8 valid = true;
    i32 sum = 0;

    i32 updates[100];
    i32 updates_count = 0;

    i32 start_index = add_ordering_rules(content, file_size);
    for (i32 i = start_index; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            current_rule *= 10;
            current_rule += content[i] - '0';
        } else if (content[i] == ',' || content[i] == '\n') {
            for (i32 u = 0; u < updates_count; u++) {
                if (order_rules[current_rule][updates[u]]) {
                    valid = false;
                }
            }

            updates[updates_count++] = current_rule;
            current_rule = 0;

            if (content[i] == '\n') {
                if (valid) {
                    sum += updates[updates_count/2];
                }
                valid = true;
                updates_count = 0;
            }
        }
    }

    return sum;
}

i32 part_2(u8* content, u32 file_size) {
    i32 current_rule = 0;

    b8 valid = true;
    i32 sum = 0;

    i32 updates[100];
    i32 updates_count = 0;

    i32 start_index = add_ordering_rules(content, file_size);
    for (i32 i = start_index; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            current_rule *= 10;
            current_rule += content[i] - '0';
        } else if (content[i] == ',' || content[i] == '\n') {
            updates[updates_count++] = current_rule;
            current_rule = 0;

            if (content[i] == '\n') {
                for (i32 u = 0; u < updates_count; u++) {
                    for (i32 u2 = 0; u2 < u; u2++) {
                        if (order_rules[updates[u]][updates[u2]]) {
                            valid = false;
                            i32 temp = updates[u-1];
                            updates[u-1] = updates[u];
                            updates[u] = temp;
                            u = 0;
                            break;
                        }
                    }
                }

                if (!valid) {
                    sum += updates[updates_count/2];
                }
                valid = true;
                updates_count = 0;
            }
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input05.txt", &file_size);

    printf("part 1: %i\n", part_1(content, file_size));
    printf("part 2: %i\n", part_2(content, file_size));

    return 0;
}
