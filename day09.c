#include "basic.c"

u64 part_1(u8* content, u32 file_size) {
    i32* output = (i32*)malloc(file_size*10*sizeof(i32));
    i32 output_index = 0;

    int id = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '\n') continue;

        u8 n = content[i] - '0';
        if (i % 2 == 0) {
            for (i32 j = 0; j < n; j++) {
                output[output_index++] = id;
            }
            id++;
        } else {
            for (i32 j = 0; j < n; j++) {
                output[output_index++] = -1;
            }
        }
    }

    i32 j = output_index-1;
    for (i32 i = 0; i < output_index; i++) {
        if (j <= i) break;
        if (output[i] > -1) continue;

        for (; j >= 0; j--) {
            if (output[j] > -1) {
                output[i] = output[j];
                output[j] = -1;
                break;
            }
        }
    }

    u64 sum = 0;
    for (i32 i = 0; i < j; i++) {
        sum += i * output[i];
    }
    return sum;
}

typedef struct { i32 id; i32 size; } File;
u64 part_2(u8* content, u32 file_size) {
    File* output = (File*)malloc(file_size*sizeof(File));
    i32 output_index = 0;

    int id = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '\n') continue;

        u8 n = content[i] - '0';
        if (n == 0) continue;

        b8 even = (i % 2 == 0);
        output[output_index++] = (File){ even ? id++ : -1, n };
    }

    for (i32 i = 0; i < output_index; i++) {
        if (output[i].id > -1) continue;

        for (i32 j = output_index-1; j > i; j--) {
            if (output[j].id == -1) continue;

            i32 size_diff = output[i].size - output[j].size;
            if (size_diff >= 0) {
                output[i] = output[j];
                output[j].id = -1;

                if (size_diff > 0) {
                    j++;
                    output_index++;
                    for (i32 k = output_index; k > i; k--) { output[k] = output[k-1]; }
                    output[i+1] = (File){ -1, size_diff };
                }
                break;
            }
        }
    }

    u64 sum = 0;
    u64 x = 0;
    for (i32 i = 0; i < output_index; i++) {
        for (i32 j = 0; j < output[i].size; j++) {
            if (output[i].id > -1) { sum += x * output[i].id; }
            x++;
        }
    }
    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input09.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
