#include "basic.c"

i32 compare(const void* a, const void* b) {
   return (*(i32*)a - *(i32*)b);
}

i32 part_1(u8* content, u32 file_size) {
    i32 column_a[1001] = {0};
    i32 column_b[1001] = {0};
    i32 column_a_index = 0;
    i32 column_b_index = 0;

    i32 n = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i] - '0';
        } else {
            if (n > 0) {
                if (column_a_index < column_b_index) {
                    column_a[column_a_index++] = n;
                } else {
                    column_b[column_b_index++] = n;
                }

                n = 0;
            }
        }
    }

    qsort(column_a, column_a_index, sizeof(i32), compare);
    qsort(column_b, column_b_index, sizeof(i32), compare);

    i32 sum = 0;
    for (i32 i = 0; i < column_a_index; i++) {
        i32 distance = column_b[i] - column_a[i];
        if (distance < 0) distance *= -1;
        sum += distance;
    }

    return sum;
}

i32 part_2(u8* content, u32 file_size) {
    i32 column_a[1001] = {0};
    i32 column_b[1001] = {0};
    i32 column_a_index = 0;
    i32 column_b_index = 0;

    i32 n = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i] - '0';
        } else {
            if (n > 0) {
                if (column_a_index < column_b_index) {
                    column_a[column_a_index++] = n;
                } else {
                    column_b[column_b_index++] = n;
                }

                n = 0;
            }
        }
    }

    i32 sum = 0;
    for (i32 a = 0; a < column_a_index; a++) {
        i32 occurrences = 0;
        for (i32 b = 0; b < column_b_index; b++) {
            if (column_b[b] == column_a[a]) occurrences++;
        }
        sum += column_a[a] * occurrences;
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input01.txt", &file_size);

    printf("part 1: %i\n", part_1(content, file_size));
    printf("part 2: %i\n", part_2(content, file_size));

    return 0;
}
