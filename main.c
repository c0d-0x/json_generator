#include <stdio.h>

#include "src/json_gen.h"

void constructor(FILE *json_fp, json_obj_t json_obj) {
  if (json_fp != NULL) {
    fputs("{", json_fp);
    fprintf(json_fp, "\"date\":\"%s\",", json_obj.date);
    fprintf(json_fp, "\"file\":\"%s\",", json_obj.file);
    fprintf(json_fp, "\"e_process\":\"%s\" ,", json_obj.e_process);
    fprintf(json_fp, "\"e_p_event\":\"%s\",", json_obj.e_p_event);
    fprintf(json_fp, "\"e_p_state\":\"%s\",", json_obj.e_p_state);
    fprintf(json_fp, "\"e_username\":\"%s\"", json_obj.e_username);
    fputs("}", json_fp);
  }
}

int main(int argc, char *argv[]) {
  FILE *json_fp = NULL;
  if (access(JSON_FILE, F_OK) != 0) {
    if ((json_fp = fopen(JSON_FILE, "w")) == NULL) {
      fprintf(stderr, "Failed to create the json file: %s", strerror(errno));
      exit(EXIT_FAILURE);
    }
    fclose(json_fp);
  }

  if ((json_fp = fopen(JSON_FILE, "r+")) == NULL) {
    fprintf(stderr, "Failed to open json file: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  json_obj_t json_obj = {"Thu Oct 17 16:22:01 2024",
                         "/home/Uname/Documents/workspace/filemond/.gitignore",
                         "git",
                         "FILE ACCESSED",
                         "c0d_0x",
                         "0022",
                         "(running)"};

  append_to_file(json_fp, json_obj, constructor);
  fclose(json_fp);

  return EXIT_SUCCESS;
}
