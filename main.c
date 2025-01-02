#include <stdio.h>

#include "src/json_gen.h"

void json_constructor(FILE *json_fp, json_obj_t *json_obj) {
  if (json_fp != NULL) {
    fprintf(json_fp, "{\"date\":\"%s\",", json_obj->date);
    fprintf(json_fp, "\"file\":\"%s\",", json_obj->file);
    fprintf(json_fp, "\"process\":\"%s\" ,", json_obj->e_process);
    fprintf(json_fp, "\"event\":\"%s\",", json_obj->e_p_event);
    fprintf(json_fp, "\"state\":\"%s\",", json_obj->e_p_state);
    fprintf(json_fp, "\"umask\":\"%s\",", json_obj->e_p_Umask);
    fprintf(json_fp, "\"username\":\"%s\"}", json_obj->e_username);
  }
}

int main(void) {
  FILE *json_fp = NULL;

  if ((json_fp = init_json_gen()) == NULL) {
    fprintf(stderr, "Failed initialize json_gen : %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  json_obj_t json_obj = {"Thu Oct 17 16:22:01 2024",
                         "/home/Uname/Documents/workspace/filemond/.gitignore",
                         "git",
                         "FILE ACCESSED",
                         "c0d_0x",
                         "0022",
                         "(running)"};

  write_json_obj(json_fp, &json_obj, json_constructor);
  fclose(json_fp);
  return EXIT_SUCCESS;
}
