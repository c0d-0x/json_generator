# JSON_GEN
This is a lightweight single filed JSON generator library designed to generate JSON files efficiently. It supports the creation of JSON array files and provides utility functions for manipulating JSON data, including file size management, file rotation, and backup creation.
> [!NOTE]
> I frequently found myself generating JSON data, so I decided to create this lightweight library that relies solely on libc.
# Usage
```bash
# Clone this repository
git clone https://github.com/c0d-0x/json_generator.git
cd json_generator

# Build the static library
make static

# Alternatively, you can copy `json_gen.c` and `json_gen.h` directly into your project.
```

# Features
- JSON Generation: Generate JSON array files with customizable formatting.
- File Handling: Supports file size management and rotating JSON files.
- Backup Functionality: Allows creating backups of JSON files.
- Cross-Platform Support: Works on Windows and Linux environments with platform-specific settings.
- File Description
# Constants and Macros
- OFFSET: Defines an offset for Windows (_WIN32) and Linux (__linux__) environments.
- CUSTOM_ERR: Defines a custom error return code (-1).
- JSON_FILE: Default name for the generated JSON file (sample_gen.json).
- FILE_SIZE_MAX: Maximum allowable file size for the JSON file (10 MB).
- BEGIN_SYMBOL: Starting symbol for the JSON structure (`[]\r\n` for JSON array).
- FILE_STATE: Enum for defining various file states (e.g., empty, not found, valid, invalid).
# Functions (APIs)
- rotate_json_f: Rotates the JSON file when it reaches its maximum size.
- write_json_fmt: Writes formatted JSON content to a file.
- backup_json_f: Creates a backup of the specified JSON file.
- get_json_f_size: Returns the size of the JSON file.
- create_new_json_f: Creates a new JSON file with the specified initial content (e.g., empty array or object).
- init_json_gen: Initializes the JSON generator and prepares it for use.
- close_json_f: Closes the JSON file and cleans up resources.

# Example:
```c
#include "src/json_gen.h"

int main(void) {
  FILE *json_fp = NULL;

  if ((json_fp = init_json_gen()) == NULL) {
    fprintf(stderr, "Error: Failed initialize json_gen\n");
    exit(EXIT_FAILURE);
  }

  write_json_fmt(json_fp, "{\"id\": %d, \"name\": \"%s\"}", 1, "Alice");
  write_json_fmt(json_fp, "{\"id\": 2, \"name\": \"Bub\"}");

  close_json_f(json_fp);
  return 0;
}
```
## Create a New JSON File:
```c
FILE *json_fp = create_new_json_f("my_file.json", BEGIN_SYMBOL);
```

Write JSON Formatted Data:
```c
write_json_fmt(json_fp, "{\"key\": \"value\"}");
```

Backup an Existing JSON File:
```c
backup_json_f("my_file.json");
```

## Get the Size of a JSON File:
```c
size_t size = get_json_f_size("my_file.json");
```
## Rotate JSON File When Full:
```c
rotate_json_f(json_fp, "my_file.json");
```

# License
This project is licensed under the MIT License - see the LICENSE file for details.
