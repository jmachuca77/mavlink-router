#include <mavlink-router/statsfile.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StatsFile::StatsFile(const char *_filename)
{
    if (_filename[0] == '/') {
        // absolute filepath
        filename = strdup(_filename);
        filename_new = strdup(filename);
        filename_new = (char*)realloc(filename_new, strlen(filename_new+4));
        if (filename_new == nullptr) {
            ::fprintf(stderr, "realloc failed");
            abort();
        }
        memset(&filename_new[strlen(filename)], '\0', 4);
        strcpy(&filename_new[strlen(filename)], ".tmp");
    } else {
        // relative to log path
        ::fprintf(stderr, "stats filepath must be absolute");
        abort();
    }
    // ::fprintf(stderr, "filepath=%s\n", filename);
    // ::fprintf(stderr, "filepath_new=%s\n", filename_new);
}

StatsFile::~StatsFile()
{
    free(filename);
    free(filename_new);
}

void StatsFile::update(Endpoint **g_endpoints)
{
    FILE *out = fopen(filename_new, "w+");
    if (out == nullptr) {
        ::fprintf(stderr, "Failed to open (%s): %s\n", filename_new, strerror(errno));
        return;
    }
    for (Endpoint **e = g_endpoints; *e != nullptr; e++) {
        (*e)->update_statsfile(out);
    }
    fclose(out);
    if (rename(filename_new, filename) == -1) {
        ::fprintf(stderr, "Failed to rename (%s) to %s: %s\n", filename_new,
                  filename, strerror(errno));
        return;
    }
}
