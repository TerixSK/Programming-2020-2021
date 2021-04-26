
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

unsigned int get_file_size(char *file_name) {
    FILE *file = fopen(file_name, "rb");
    fseek(file, 0, SEEK_END);
    unsigned int file_size = ftell(file);
    fclose(file);
    return file_size;
}

void copy_file(FILE *from, FILE *to) {
    int c;
    while ((c = getc(from)) != EOF) {
        putc(c, to);
    }
}

union archive_header {
    char buf[12];

    struct {
        unsigned int header_size;
        unsigned int archive_size;
        unsigned int table_size;
    } arch_data;
};

union file_header {
    char buf[12];

    struct {
        unsigned int header_size;
        unsigned int file_size;
        unsigned int file_size_compressed;
    } file_data;
};

struct MinHeapNode {


    char data;


    unsigned freq;


    struct MinHeapNode *left, *right;
};

struct MinHeap {

    unsigned size;

    unsigned capacity;

    struct MinHeapNode **array;
};

struct MinHeapNode *newNode(char data, unsigned freq) {
    struct MinHeapNode *temp
            = (struct MinHeapNode *) malloc
                    (sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

struct MinHeap *createMinHeap(unsigned capacity) {

    struct MinHeap *minHeap
            = (struct MinHeap *) malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array
            = (struct MinHeapNode **) malloc(minHeap->
            capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a,
                     struct MinHeapNode **b) {

    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx) {

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->
            freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->
            freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap *minHeap) {

    return (minHeap->size == 1);
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap) {

    struct MinHeapNode *temp = minHeap->array[0];
    minHeap->array[0]
            = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap *minHeap,
                   struct MinHeapNode *minHeapNode) {

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printArr(const int arr[], int n, FILE *table) {
    char symbol = 0;
    int i, c = 0;
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
    }
    for (i = 0; i < n; i++) {
        c++;
        symbol <<= 1;
        if (arr[i]) {
            symbol += 1;
        }
        if (c % 8 == 0) {
            fputc(symbol, table);
            symbol = 0;
            c = 0;
        }
    }
    symbol <<= 8 - c;
    fputc(symbol, table);
    printf("\n");
}

int isLeaf(struct MinHeapNode *root) {

    return !(root->left) && !(root->right);
}

struct MinHeap *createAndBuildMinHeap(char data[], int freq[], int size) {

    struct MinHeap *minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

struct MinHeapNode *buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;

    struct MinHeap *minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {

        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void printCodes(struct MinHeapNode *root, int arr[], int top, FILE *table) {

    // Assign 0 to left edge and recur 
    if (root->left) {

        arr[top] = 0;
        printCodes(root->left, arr, top + 1, table);
    }

    // Assign 1 to right edge and recur 
    if (root->right) {

        arr[top] = 1;
        printCodes(root->right, arr, top + 1, table);
    }

    // If this is a leaf node, then 
    // it contains one of the input 
    // characters, print the character 
    // and its code from arr[] 
    if (isLeaf(root)) {
        printf("%c : ", root->data);
        fputc(root->data, table);
        fprintf(table, "%c", top);
        printArr(arr, top, table);
    }
}

void HuffmanCodes(char data[], int freq[], int size, FILE *table) {
    struct MinHeapNode *root
            = buildHuffmanTree(data, freq, size);

    int arr[MAX_TREE_HT], top = 0;

    printCodes(root, arr, top, table);
}

void create(char *arch_name, int argc, char *argv[]) {
    FILE *table = fopen("table.txt", "wb+");
    unsigned symbols[256];
    for (int i = 0; i < 256; i++) {
        symbols[i] = 0;
    }
    int counter = 0;
    for (int i = 4; i < argc; i++) {
        FILE *file = fopen(argv[i], "rb");
        int c;
        while ((c = fgetc(file)) != EOF) {
            if (symbols[c] == 0) {
                counter++;
            }
            symbols[c]++;
        }
        fclose(file);
    }
    char *arr = malloc(counter * sizeof(char));
    int freq[counter];
    counter = 0;
    for (int i = 0; i < 256; i++) {
        if (symbols[i] > 0) {
            arr[counter] = i;
            freq[counter] = symbols[i];
            counter++;
        }
    }

    HuffmanCodes(arr, freq, counter, table);

    FILE *arch_file = fopen(arch_name, "wb");

    union archive_header arc_header;
    arc_header.arch_data.header_size = 12 + strlen(arch_name);

    rewind(table);
    unsigned t_size = get_file_size("table.txt");

    arc_header.arch_data.archive_size = arc_header.arch_data.header_size + t_size;
    arc_header.arch_data.table_size = t_size;

    //printf("\n%d", arc_header.arch_data.archive_size);
    for (int i = 4; i < argc; i++) {
        FILE *file = fopen(argv[i], "rb");
        unsigned f_size = get_file_size(argv[i]);
        unsigned sum = 0;
        while (ftell(file) < f_size) {
            rewind(table);
            unsigned y = fgetc(file);
            unsigned x = fgetc(table);
            while (x != y) {
                unsigned jump = fgetc(table);
                if (jump % 8 > 0) {
                    jump = jump / 8 + 1;
                } else {
                    jump /= 8;
                }
                fseek(table, jump, SEEK_CUR);
                x = fgetc(table);
            }
            unsigned d = fgetc(table);
            sum += d;
        }
        if (sum % 8 > 0) {
            sum = sum / 8 + 1;
        } else {
            sum /= 8;
        }
        arc_header.arch_data.archive_size += sum;
        arc_header.arch_data.archive_size += 12 + strlen(argv[i]);
        fclose(file);
    }
    fwrite(arc_header.buf, sizeof(char), 12, arch_file);
    fwrite(arch_name, sizeof(char), strlen(arch_name), arch_file);

    rewind(table);
    copy_file(table, arch_file);
    for (int i = 4; i < argc; i++) {
        FILE *file = fopen(argv[i], "rb");
        FILE *copy = fopen("copy.txt", "wb+");
        unsigned f_size = get_file_size(argv[i]);
        rewind(file);
        char w_symb = 0;
        int c = 0;
        while (ftell(file) < f_size) {
            rewind(table);
            unsigned y = fgetc(file);
            unsigned x = fgetc(table);
            while (x != y) {
                unsigned jump = fgetc(table);
                if (jump % 8 > 0) {
                    jump = jump / 8 + 1;
                } else {
                    jump /= 8;
                }
                fseek(table, jump, SEEK_CUR);
                x = fgetc(table);
            }
            unsigned d = fgetc(table);

            unsigned code = fgetc(table), d_copy = d;

            while (d_copy > 8) {
                code <<= 8;
                unsigned temp = fgetc(table);
                code += temp;
                d_copy -= 8;
            }

            d_copy = d;
            if (d_copy % 8 > 0) {
                d_copy = d_copy / 8 + 1;
            } else {
                d_copy /= 8;
            }

            unsigned cd = 1;
            while (d_copy > 0) {
                cd *= 128;
                d_copy--;
            }

            for (int j = 0; j < d; j++) {
                w_symb <<= 1;
                w_symb += code / cd;
                code <<= 1;
                code %= cd * 2;
                c++;
                if (c == 8) {
                    fputc(w_symb, copy);
                    c = 0;
                    w_symb = 0;
                }
            }
        }
        if (c > 0) {
            while (c % 8 != 0) {
                w_symb <<= 1;
                c++;
            }
            fputc(w_symb, copy);
        }

        rewind(copy);
        union file_header copy_header;
        copy_header.file_data.file_size = get_file_size("copy.txt");
        copy_header.file_data.header_size = 12 + strlen(argv[i]);
        rewind(file);

        f_size = get_file_size(argv[i]);
        unsigned sum = 0;
        while (ftell(file) < f_size) {
            rewind(table);
            unsigned y = fgetc(file);
            unsigned x = fgetc(table);
            while (x != y) {
                unsigned jump = fgetc(table);
                if (jump % 8 > 0) {
                    jump = jump / 8 + 1;
                } else {
                    jump /= 8;
                }
                fseek(table, jump, SEEK_CUR);
                x = fgetc(table);
            }
            unsigned d = fgetc(table);
            sum += d;
        }
        copy_header.file_data.file_size_compressed = sum;
        /*printf("%d ", copy_header.file_data.file_size);
        printf("%d\n", copy_header.file_data.header_size);*/
        fwrite(copy_header.buf, sizeof(char), 12, arch_file);
        fwrite(argv[i], sizeof(char), strlen(argv[i]), arch_file);

        copy_file(copy, arch_file);

        fclose(copy);
        fclose(file);

        remove(argv[i]);
    }
    fclose(arch_file);
    fclose(table);
    remove("copy.txt");
    remove("table.txt");
}

void list(char *arch_name) {
    FILE *arch_file = fopen(arch_name, "rb");

    union archive_header arc_header;
    fread(arc_header.buf, sizeof(char), 12, arch_file);

    fseek(arch_file, arc_header.arch_data.table_size + strlen(arch_name), SEEK_CUR);

    while (ftell(arch_file) < arc_header.arch_data.archive_size) {
        union file_header file_header;
        fread(file_header.buf, sizeof(char), 12, arch_file);

        char *file_name = malloc((file_header.file_data.header_size - 12) * sizeof(char));
        fread(file_name, sizeof(char), file_header.file_data.header_size - 12, arch_file);

        printf("%s\n", file_name);
        fseek(arch_file, file_header.file_data.file_size, SEEK_CUR);
        free(file_name);
    }

    fclose(arch_file);
}

void extract(char *arch_name) {
    FILE *arch_file = fopen(arch_name, "rb");

    union archive_header arc_header;
    fread(arc_header.buf, sizeof(char), 12, arch_file);
    fseek(arch_file, strlen(arch_name), SEEK_CUR);

    FILE *table = fopen("table.txt", "wb+");
    for (int i = 0; i < arc_header.arch_data.table_size; i++) {
        unsigned c = fgetc(arch_file);
        fputc(c, table);
    }

    while (ftell(arch_file) < arc_header.arch_data.archive_size) {
        union file_header file_header;
        fread(file_header.buf, sizeof(char), 12, arch_file);

        char *file_name = malloc((file_header.file_data.header_size - 12) * sizeof(char));
        fread(file_name, sizeof(char), file_header.file_data.header_size - 12, arch_file);

        FILE *output_file = fopen(file_name, "wb");
        FILE *copy = fopen("copy.txt", "wb+");

        for (int i = 0; i < file_header.file_data.file_size; i++) {
            unsigned c = fgetc(arch_file);
            fputc(c, copy);
        }

        rewind(copy);
        rewind(table);
        unsigned c_size = get_file_size("copy.txt");
        unsigned t_size = get_file_size("table.txt");
        rewind(copy);
        char symb = 0;
        unsigned symb_counter = 1, counter, big_counter = 0;
        for (int i = 0; i < c_size; i++) {
            unsigned y = fgetc(copy);
            counter = 1;
            rewind(table);
            symb += y / 128;
            y <<= 1;
            y %= 256;
            while (counter < 9) {
                unsigned x = fgetc(table);
                unsigned d = fgetc(table);
                unsigned code = fgetc(table), d_copy = d;

                while (d_copy > 8) {
                    code <<= 8;
                    unsigned temp = fgetc(table);
                    code += temp;
                    d_copy -= 8;
                }

                d_copy = d;
                if (d_copy % 8 > 0) {
                    d_copy = d_copy / 8 + 1;
                } else {
                    d_copy /= 8;
                }

                unsigned c = 1;
                for (int j = 0; j < 8 * d_copy - d; j++) {
                    c *= 2;
                }
                code /= c;

                if (code == symb && symb_counter == d) {
                    fputc(x, output_file);
                    symb = 0;
                    symb_counter = 0;
                    big_counter += d;
                    if (big_counter == file_header.file_data.file_size_compressed) {
                        break;
                    }
                }

                if (ftell(table) == t_size) {
                    counter++;
                    symb <<= 1;
                    symb += y / 128;
                    y <<= 1;
                    y %= 256;
                    rewind(table);
                    symb_counter++;
                }
            }
        }
        //printf("%d %d %d", symb, symb_counter, big_counter);

        fclose(output_file);
        fclose(copy);
        free(file_name);
    }

    fclose(arch_file);
    fclose(table);
    remove("copy.txt");
    remove("table.txt");
}


int main(int argc, char *argv[]) {
    char *arch_name = argv[2];

    if (argv[3][2] == 'c') { //create
        create(arch_name, argc, argv);
    } else if (argv[3][2] == 'e') { //extract
        extract(arch_name);
        remove(arch_name);
    } else { //list
        printf("File in %s:\n", arch_name);
        list(arch_name);
    }

    return 0;
} 