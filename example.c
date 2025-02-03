#include <stdio.h>

#define JSON_GEN_IMPL
#include "json_gen.h"

int main(void) {
  FILE *json_fp = NULL;

  if ((json_fp = init_json_gen("sample.json")) == NULL) {
    fprintf(stderr, "Error: Failed initialize json_gen\n");
    exit(EXIT_FAILURE);
  }

  write_json_fmt(json_fp, "{\"id\": %d, \"name\": \"%s\"}", 1, "Alice");
  write_json_fmt(json_fp, "{\"id\": 2, \"name\": \"Bub\"}");

  close_json_f(json_fp);
  return EXIT_SUCCESS;
}
