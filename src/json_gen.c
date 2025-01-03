#include "json_gen.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// TODO: improve error checking and logging
static int first_write;
size_t validate_json(char *json_file) {
  char CC;
  int len, json_fd = -1;
  if ((json_fd = open(json_file, O_RDONLY)) == -1) {
    return NOT_FOUND;
  }

  len = read(json_fd, &CC, sizeof(char));
  if (len == -1) {
    close(json_fd);
    fprintf(stderr, "Error: read failed: %s", strerror(errno));
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
    fprintf(stderr, "Error: read failed: %s", strerror(errno));
    return CUSTOM_ERR;
  }

  close(json_fd);
  if (CC != ']') return INVALID_JSON;
  return VALID_JSON;
}

/*functions constructs the json object.*/
void write_json_obj(FILE *json_fp, json_obj_t *json_obj,
                    void (*json_constructor)(FILE *, json_obj_t *)) {
  fseek(json_fp, -(long)sizeof(char) * OFFSET, SEEK_END);
  if (first_write) {
    fputc(',', json_fp);
  }
  json_constructor(json_fp, json_obj);
  fwrite("]\r\n", sizeof(char) * OFFSET, 1, json_fp);
  first_write = 1;
}

void close_json_file(FILE *json_fp) {
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

size_t get_file_size(char *file_path) {
  struct stat buf;
  if (stat(file_path, &buf) != 0) {
    fprintf(stderr, "Error: File  stats failed: %s", strerror(errno));
    return CUSTOM_ERR;
  }
  return buf.st_size;
}

void backup_file(char *file_path) {
  char buffer[64];
  if (access(file_path, F_OK) != 0) return;

  char *time_date = get_locale_time();
  snprintf(buffer, 63, "%s.json", time_date);
  rename(file_path, buffer);
  if (!time_date) free(time_date);
}

FILE *create_new_log(const char *file_path, const char *begin_symbol) {
  FILE *fp = NULL;
  if ((fp = fopen(file_path, "w+")) == NULL) {
    fprintf(stderr, "Error : Failed to create_new_log: %s", strerror(errno));
    return NULL;
  }
  if (begin_symbol) fputs(begin_symbol, fp);
  return fp;
}

FILE *init_json_gen() {
  FILE *json_fp = NULL;
  size_t flag = validate_json(JSON_FILE);
  ssize_t file_size;
  switch (flag) {
    case VALID_JSON:
      file_size = get_file_size(JSON_FILE);
      if (file_size >= FILE_SIZE_MAX) {
        close_json_file(json_fp);
        backup_file(JSON_FILE);
        json_fp = create_new_log(JSON_FILE, BEGIN_SYMBOL);
        return json_fp;
      }

      if ((json_fp = fopen(JSON_FILE, "r+")) == NULL) {
        fprintf(stderr, "Error : Failed to create_new_log: %s",
                strerror(errno));
        return NULL;
      }
      first_write = 1;
      break;
    case INVALID_JSON:
      fprintf(stderr, "Invalid json format: %s", JSON_FILE);
      backup_file(JSON_FILE);
      json_fp = create_new_log(JSON_FILE, BEGIN_SYMBOL);
      break;
    default:
      json_fp = create_new_log(JSON_FILE, BEGIN_SYMBOL);
  }
  return json_fp;
}
