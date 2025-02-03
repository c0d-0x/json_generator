#ifndef JSON_GEN_H
#define JSON_GEN_H
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
#define OFFSET 4
#elif __linux__
#define OFFSET 3
#endif  // _WIN32 or __linux__

#define CUSTOM_ERR (-1)

/*10 MB max */
#define FILE_SIZE_MAX 10485760
/*{}\r\n: json obj file or []\r\n: json array file*/
#define BEGIN_SYMBOL "[]\r\n"
enum FILE_STATE { EMPTY_FILE = 0, NOT_FOUND, VALID_JSON, INVALID_JSON };

int rotate_json_f(FILE *json_fp, char *file_name);
void write_json_fmt(FILE *json_fp, char *fmt, ...);
void backup_json_f(char *json_file_name);
size_t get_json_f_size(char *json_file_name);
FILE *create_new_json_f(const char *json_file_name, const char *begin_symbol);
FILE *init_json_gen(char *json_file_name);
void close_json_f(FILE *json_fp);

#ifdef JSON_GEN_IMPL
static int write_initialized;
size_t validate_json(char *json_file_name) {
  char CC;
  int len, json_fd = -1;
  if ((json_fd = open(json_file_name, O_RDONLY)) == -1) {
    return NOT_FOUND;
  }

  len = read(json_fd, &CC, sizeof(char));
  if (len == -1) {
    close(json_fd);
    fprintf(stderr, "Error: Failed to read file: %s\n", strerror(errno));
    return CUSTOM_ERR;
  }

  if (CC != '[') {
    close(json_fd);
    if (len == 0)
      return EMPTY_FILE;
    else
      return INVALID_JSON;
  }

  /**
   * seeking to the last three bytes of the file, which normally should be
   * ']\r\n' -> This is necessary for json decoders
   */
  lseek(json_fd, -(long)sizeof(char) * OFFSET, SEEK_END);
  len = read(json_fd, &CC, sizeof(char));
  if (len < 1) {
    close(json_fd);
    fprintf(stderr, "Error: read failed: %s\n", strerror(errno));
    return CUSTOM_ERR;
  }

  close(json_fd);
  if (CC != ']') return INVALID_JSON;
  return VALID_JSON;
}

void write_json_fmt(FILE *json_fp, char *fmt, ...) {
  if (json_fp == NULL || fmt == NULL) {
    fprintf(stderr, "Error: Json obj not written\n");
    return;
  }

  fseek(json_fp, -(long)sizeof(char) * OFFSET, SEEK_END);
  if (write_initialized) {
    fputc(',', json_fp);
  }

  va_list args;
  va_start(args, fmt);
  if (vfprintf(json_fp, fmt, args) < 0) {
    perror("vfprintf failed");
    va_end(args);
    return;
  }

  va_end(args);
  fwrite("]\r\n", sizeof(char) * OFFSET, 1, json_fp);
  write_initialized = 1;
}

void close_json_f(FILE *json_fp) {
  if (json_fp) fclose(json_fp);
}

static char *get_locale_time(void) {
  char *buffer = NULL;
  struct tm tm = *localtime(&(time_t){time(NULL)});
  buffer = asctime(&tm);
  if (buffer[0] == '\0') {
    free(buffer);
    return NULL;
  }

  buffer[strnlen(buffer, 26) - 1] = '\0';
  return buffer;
}

size_t get_json_f_size(char *json_file_name) {
  struct stat buf;
  if (stat(json_file_name, &buf) != 0) {
    fprintf(stderr, "Error: File stats failed: %s\n", strerror(errno));
    return CUSTOM_ERR;
  }
  return buf.st_size;
}

void backup_json_f(char *json_file_name) {
  char buffer[64];
  if (access(json_file_name, F_OK) != 0) return;

  char *time_date = get_locale_time();
  snprintf(buffer, 63, "%s.json", time_date);
  rename(json_file_name, buffer);
  if (!time_date) free(time_date);
}

FILE *create_new_json_f(const char *json_file_name, const char *begin_symbol) {
  FILE *fp = NULL;
  if ((fp = fopen(json_file_name, "w+")) == NULL) {
    fprintf(stderr, "Error: Failed to create_new_json_f: %s\n",
            strerror(errno));
    return NULL;
  }
  if (begin_symbol) fputs(begin_symbol, fp);
  return fp;
}

int rotate_json_f(FILE *json_fp, char *json_file_name) {
  ssize_t file_size;
  file_size = get_json_f_size(json_file_name);
  /*call to log_rotation func*/
  if (file_size >= FILE_SIZE_MAX) {
    close_json_f(json_fp);
    backup_json_f(json_file_name);
    json_fp = create_new_json_f(json_file_name, BEGIN_SYMBOL);
    return 1;
  }
  return CUSTOM_ERR;
}

FILE *init_json_gen(char *json_file_name) {
  FILE *json_fp = NULL;
  size_t flag = validate_json(json_file_name);
  switch (flag) {
    case VALID_JSON:
      if (rotate_json_f(json_fp, json_file_name) != CUSTOM_ERR) {
        return json_fp;
      }

      write_initialized = 1;
      if ((json_fp = fopen(json_file_name, "r+")) == NULL) {
        fprintf(stderr, "Error: Failed to create_new_json_f: %s\n",
                strerror(errno));
        return NULL;
      }
      break;
    case INVALID_JSON:
      fprintf(stderr, "Error: Invalid json format: %s\n", json_file_name);
      backup_json_f(json_file_name);
      json_fp = create_new_json_f(json_file_name, BEGIN_SYMBOL);
      break;
    default:
      json_fp = create_new_json_f(json_file_name, BEGIN_SYMBOL);
  }
  return json_fp;
}

#endif  // JSON_GEN_IMPL
#endif  // !JSON_GEN_H
