#ifndef JSON_GEN_H
#define JSON_GEN_H

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#define OFFSET 4
#elif __linux__
#define OFFSET 3
#endif  // _WIN32 or __linux__

#define CUSTOM_ERR (-1)
#define JSON_FILE "sample_gen.json"

/*10 MB max */
#define FILE_SIZE_MAX 10485760
/*{}\r\n: json obj file or []\r\n: json array file*/
#define BEGIN_SYMBOL "[]\r\n"
enum FILE_STATE { EMPTY_FILE = 0, NOT_FOUND, VALID_JSON, INVALID_JSON };

int rotate_json_f(FILE *json_fp, char *file_name);
void write_json_fmt(FILE *json_fp, char *fmt, ...);
void backup_json_f(char *file_path);
size_t get_json_f_size(char *file_path);
FILE *create_new_json_f(const char *file_path, const char *begin_symbol);
FILE *init_json_gen(void);
void close_json_f(FILE *json_fp);

#endif  // !JSON_GEN_H
