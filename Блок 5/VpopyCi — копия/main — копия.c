#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#define _CRT_SECURE_NO_WARNINGS

unsigned int rever_byt(unsigned int n) // Функция для обработки байтов
{
    return ((n >> 24) & 0x000000ff) | ((n >> 8) & 0x0000ff00) | ((n << 8) & 0x00ff0000) |((n << 24) & 0xff000000);
}

typedef union tag_taghead // Union для хранения главных тегов
{
    char buf[12]; // буфер
    struct {
        unsigned short  empty;
        unsigned char   version[3];
        unsigned char   v1;
        unsigned char   v2;
        unsigned char   flags;
        unsigned int    size;
    } data;
} taghead;

typedef union tag_framehead // Union для главных кадров
{
    char buf[10]; // буфер
    struct {
        unsigned char   name[4];
        unsigned int    size;
        unsigned short  flags;
    } data;
} framehead;

void copy_file(FILE* input, FILE* output) // Функция копирования файлов
{
    int c;
    while ((c = getc(input)) != EOF)
        putc(c, output);
}

void show(char* file_name) // Функция для простра метаинформации
{
    FILE* file;
    file = fopen(file_name, "rb"); // Открываем mp3 файл
    if (file == NULL) // если файл не удалось открыть
    {
        printf("При чтения файла произошла ошибка (-_-)\n");
        exit(404);
    }
    fseek(file, 0, SEEK_SET); // просмотр файла
    taghead tag_head;
    fread(tag_head.buf + 2, sizeof(char), 10, file); // чтение главных тегов из буффера
    unsigned int tag_size = rever_byt(tag_head.data.size); // размер тегов
    printf("%sv%d.%d\n", tag_head.data.version, tag_head.data.v1, tag_head.data.v2);
    while (ftell(file) < tag_size + 10)
    {
        framehead frame_head;
        fread(frame_head.buf, sizeof(char), 10, file); // чтения главных фреймов
        if (frame_head.data.name[0] == 0)
            break;
        printf("%s: ", frame_head.data.name);
        unsigned int frame_size = rever_byt(frame_head.data.size);
        unsigned char* frame_data;                        //
        frame_data = malloc(sizeof(char) * frame_size);   // заполнение инфы о фреймах
        fread(frame_data, sizeof(char), frame_size, file);//
        unsigned int i;
        for (i = 0; i < frame_size; ++i)
        {
            printf("%c", frame_data[i]); // вывод
        }
        printf("\n");
        free(frame_data);
    }
    fclose(file);
}
void get(char* file_name, char frame_name[4]) // Функция для получения конкретного тега
{
    FILE* file;
    file = fopen(file_name, "rb");
    taghead tag_head;
    fread(tag_head.buf + 2, sizeof(char), 10, file); // чтение главных тегов из буффера
    unsigned int tag_size = rever_byt(tag_head.data.size);
    while (ftell(file) < tag_size + 10)
    {
        framehead frame_head;
        fread(frame_head.buf, sizeof(char), 10, file);
        unsigned int frame_size = rever_byt(frame_head.data.size);
        if (strcmp(frame_head.data.name, frame_name) == 0)
        {
            printf("%s: ", frame_head.data.name);
            unsigned char* frame_data;
            frame_data = malloc(sizeof(char) * frame_size);
            fread(frame_data, sizeof(char), frame_size, file);
            unsigned int i;
            for (i = 0; i < frame_size; ++i)
            {
                printf("%c", frame_data[i]);
            }
            printf("\n");
            free(frame_data);
            fclose(file);
            return;
        }
        fseek(file, frame_size, SEEK_CUR);
    }
    fclose(file);
    printf("Не найдено значения для %s!", frame_name);
}

void set(char* file_name, char frame_name[4], char* frame_value)
{
    FILE* file;
    file = fopen(file_name, "rb");
    taghead tag_head;
    fread(tag_head.buf + 2, sizeof(char), 10, file);
    unsigned int tag_size = rever_byt(tag_head.data.size);
    unsigned int old_frame_pos = 0;
    unsigned int old_frame_size = 0;
    while (ftell(file) < tag_size + 10)
    {
        framehead frame_header;
        fread(frame_header.buf, sizeof(char), 10, file);
        unsigned int frameSize = rever_byt(frame_header.data.size);
        if (strcmp(frame_header.data.name, frame_name) == 0)
        {
            old_frame_pos = ftell(file) - 10;
            old_frame_size = frameSize;
            break;
        }
        fseek(file, frameSize, SEEK_CUR);
    }
    unsigned int value_size = strlen(frame_value);
    unsigned int new_tag_size = tag_size - old_frame_size + value_size + 10 * (old_frame_pos != 0);
    if (old_frame_pos == 0)
    {
        old_frame_pos = ftell(file);
    }
    if (value_size == 0)
    {
        new_tag_size -= 10;
    }
    FILE* file_copy;
    file_copy = fopen("temp.mp3", "wb");
    fseek(file, 0, SEEK_SET);
    fseek(file_copy, 0, SEEK_SET);
    copy_file(file, file_copy);
    fclose(file);
    fclose(file_copy);
    file_copy = fopen("temp.mp3", "rb");
    file = fopen(file_name, "wb");
    tag_head.data.size = rever_byt(new_tag_size);
    fwrite(tag_head.buf + 2, sizeof(char), 10, file);
    fseek(file_copy, 10, SEEK_SET);
    unsigned int i;
    for (i = 0; i < old_frame_pos - 10; ++i)
    {
        int c;
        c = getc(file_copy);
        putc(c, file);
    }
    if (value_size > 0)
    {
        framehead frame_head;
        unsigned int i;
        for (i = 0; i < 4; ++i)
        {
            frame_head.data.name[i] = frame_name[i];
        }
        frame_head.data.size = rever_byt(value_size);
        frame_head.data.flags = 0;
        fwrite(frame_head.buf, sizeof(char), 10, file);
    }
    fwrite(frame_value, sizeof(char), value_size, file);
    fseek(file_copy, old_frame_pos + 10 + old_frame_size, SEEK_SET);
    for (i = ftell(file); i < new_tag_size; ++i)
    {
        unsigned short int c;
        c = getc(file_copy);
        putc(c, file);
    }
    printf("Новое значение для фрейма %s: %s\n", frame_name, frame_value);
    copy_file(file_copy, file);
    fclose(file);
    fclose(file_copy);
    remove("temp.mp3");
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Rus");
    char* file_name;
    char* frame_name;
    char* value;
    char show_flag = 0;
    char set_flag = 0;
    char get_flag = 0;
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--show") == 0) {
            show_flag = 1;
            continue;
        }
        if (argv[i][2] == 'f')
        {
            file_name = strpbrk(argv[i], "=") + 1;
        }
        if (argv[i][2] == 'g') {
            get_flag = 1;
            frame_name = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 's') {
            set_flag = 1;
            frame_name = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 'v') {
            value = strpbrk(argv[i], "=") + 1;
            continue;
        }
    }
    if (show_flag)
    {
        show(file_name);
    }
    if (get_flag)
    {
        get(file_name, frame_name);
    }
    if (set_flag)
    {
        set(file_name, frame_name, value);
    }
    return 0;
}
