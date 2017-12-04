#pragma once

#include <time.h>
#include <limits.h>

#include "endpoint.h"

class StatsFile {
public:

    StatsFile(const char *_path);
    ~StatsFile();

    void update(Endpoint **g_endpoints);

private:

    char filename[PATH_MAX];
    char filename_new[PATH_MAX];
};
