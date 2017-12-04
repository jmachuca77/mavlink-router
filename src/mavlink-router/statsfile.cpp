#include <mavlink-router/statsfile.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StatsFile::StatsFile(const char *_filename)
{
    if (_filename[0] == '/') {
        // absolute filepath
        strcpy(filename, _filename);
        memset(filename_new, '\0', PATH_MAX);
        snprintf(filename_new, PATH_MAX, "%s.tmp", filename);
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
