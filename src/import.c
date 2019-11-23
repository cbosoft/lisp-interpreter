#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gc.h"
#include <glob.h>

#include "import.h"
#include "exception.h"

char *search_paths[] = {
  "~/lib",
  "~/.chrisp",
  "/usr/lib/chrisp",
  NULL
};

// Looks around for the file with name $name, extension optional
// returns full path to result
char *search(char *name)
{

  char *search_path = NULL;
  int i;
  char *home = getenv("HOME");

  for (i = 0, search_path = search_paths[0]; search_path != NULL; i++, search_path = search_paths[i]) {
    char *possible_path_noext = malloc( ( strlen(search_path) + 1 + strlen(name) + 3 + 1)*sizeof(char));
    char *possible_path = malloc( ( strlen(search_path) + 1 + strlen(name) + 1)*sizeof(char));

    if (search_path[0] == '~') {
      char *temp = malloc( (strlen(search_path) - 1 + strlen(home) + 1)*sizeof(char));
      sprintf(temp, "%s%s", home, ++search_path);
      search_path = temp;
    }

    sprintf(possible_path_noext, "%s/%s.cl", search_path, name);
    sprintf(possible_path, "%s/%s", search_path, name);

    // TODO glob for matches here, return first match
    glob_t glob_result = {0};
    glob(possible_path_noext, 0, NULL, &glob_result);

    assert_or_error(glob_result.gl_pathc < 2, "search", "Ambiguous result while searching for '%s'", name);
    ERROR_CHECK;

    if (glob_result.gl_pathc == 1) {
      // one single result
      // return it
      return glob_result.gl_pathv[0];
    }

    glob(possible_path, 0, NULL, &glob_result);

    assert_or_error(glob_result.gl_pathc < 2, "search", "Ambiguous result while searching for '%s'", name);
    ERROR_CHECK;

    if (glob_result.gl_pathc == 1) {
      // one single result
      // return it
      return glob_result.gl_pathv[0];
    }

  }

  assert_or_error(0, "search", "No module found with name '%s'", name);
  return NULL;
}
