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
#define asctime_r asctime_s
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

typedef struct {
  char *date;
  char *file;
  char *e_process;
  char *e_p_event;
  char *e_username;
  char *e_p_Umask;
  char *e_p_state;
} json_obj_t;

void json_constructor(FILE *json_fp, json_obj_t *json_obj);
void write_json_obj(FILE *json_fp, json_obj_t *json_obj,
                    void (*json_constructor)(FILE *, json_obj_t *));

size_t validate_json(char *json_file);
void backup_file(char *file_path);
size_t get_file_size(char *file_path);
FILE *create_new_log(const char *file_path, const char *begin_symbol);
FILE *init_json_gen();
void close_json_file(FILE *json_fp);

#endif  // !JSON_GEN_H
