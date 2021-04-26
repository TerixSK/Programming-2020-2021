#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union archive_header // Объеденение для хранения буффера и структуря для хедера архива
{
    char buf[8]; // буффер
    struct
    {
        char archive_type[4]; // 4 байта для хедера архива
        unsigned int archive_size; // размер хедера архива
    } arch_data;
};

union file_header // Объеденение для хранения буффера и структуря для хедера файлов
{
    char buf[8]; // буффер
    struct
    {
        unsigned int header_size; // размер хедера для файла
        unsigned int file_size; // размер файла
    } file_data;
};

void copy_file(FILE *from, FILE *to) // Процедура для копирования файлов в архив
{
    int c; // вводим вс
    while ((c = getc(from)) != EOF) // пока файл не кончиться копируем данные
    {
        putc(c, to);
    }
}

unsigned int get_file_size(char *file_name) // Функция для получения размера исходного файла
{
    FILE *file = fopen(file_name, "rb"); // открываем файл
    fseek(file, 0, SEEK_END); // ставим указатель на начало файла
    unsigned int file_size = ftell(file);  // инициализируем переменую для размера файла и записываем в неё размер
    fclose(file); // закрываем
    return file_size; // возвращаем размер файла
}

void create(char *arch_name, int argc, char *argv[]) // Процкдура для создания архива
{
    FILE *arch_file = fopen(arch_name, "wb"); // создаём пустой архив
    union archive_header arc_header; // создаём структуру для хедера архива
    arc_header.arch_data.archive_type[0] = 'A'; //
    arc_header.arch_data.archive_type[1] = 'R'; // записываем разрешения файла
    arc_header.arch_data.archive_type[2] = 'C'; //
    arc_header.arch_data.archive_type[3] = 'H'; //
    arc_header.arch_data.archive_size = 8;
    for (int i = 4; i < argc; i++) // цикл для аодсчёта размера архива
    {
        arc_header.arch_data.archive_size += get_file_size(argv[i]) + 8 + strlen(argv[i]);
    }
    fwrite(arc_header.buf, sizeof(char), 8, arch_file); // записываем 8 байт данных архива
    for (int i = 4; i < argc; i++) // ЦИКЛ для занесения файлов в архив
    {
        union file_header file_header;
        file_header.file_data.file_size = get_file_size(argv[i]);
        file_header.file_data.header_size = 8 + strlen(argv[i]);
        fwrite(file_header.buf, sizeof(char), 8, arch_file);
        fwrite(argv[i], sizeof(char), strlen(argv[i]), arch_file);
        FILE *file;
        file = fopen(argv[i], "rb");
        copy_file(file, arch_file);
        fclose(file);
        remove(argv[i]);
    }
    fclose(arch_file);
}

void list(char *arch_name) // Процудара для вывода данных о создании архива на консоль
{
    FILE *arch_file = fopen(arch_name, "rb");
    union archive_header arc_header;
    fread(arc_header.buf, sizeof(char), 8, arch_file);
    while (ftell(arch_file) < arc_header.arch_data.archive_size)
    {
        union file_header file_header;
        fread(file_header.buf, sizeof(char), 8, arch_file);
        char *file_name = malloc((file_header.file_data.header_size - 8) * sizeof(char));
        fread(file_name, sizeof(char), file_header.file_data.header_size - 8, arch_file);
        printf("%s\n", file_name);
        fseek(arch_file, file_header.file_data.file_size, SEEK_CUR);
        free(file_name);
    }
    fclose(arch_file);
}

void extract(char *arch_name) // Процедура для извлечения файлов из архива
{
    FILE *arch_file = fopen(arch_name, "rb");
    union archive_header arc_header;
    fread(arc_header.buf, sizeof(char), 8, arch_file);
    while (ftell(arch_file) < arc_header.arch_data.archive_size)
    {
        union file_header file_header;
        fread(file_header.buf, sizeof(char), 8, arch_file);
        char *file_name = malloc((file_header.file_data.header_size - 8) * sizeof(char));
        fread(file_name, sizeof(char), file_header.file_data.header_size - 8, arch_file);
        FILE *output_file = fopen(file_name, "wb");
        for (unsigned int i = 0; i < file_header.file_data.file_size; i++)
        {
            int c = getc(arch_file);
            putc(c, output_file);
        }
        fclose(output_file);
        free(file_name);
    }
    fclose(arch_file);
}

int main(int argc, char *argv[]) {
    char *arch_name = argv[2];

    if (argv[3][2] == 'c')
    {
        create(arch_name, argc, argv);
    }
    else if (argv[3][2] == 'e')
    {
        extract(arch_name);
        remove(arch_name);
    }
    else
    {
        printf("File in %s:\n", arch_name);
        list(arch_name);
    }
    return 0;
}