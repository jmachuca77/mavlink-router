#pragma once

#include <time.h>

#include "endpoint.h"

class StatsFile {
public:

    StatsFile(const char *_path);
    ~StatsFile();

    void update(Endpoint **g_endpoints);

private:

    char *filename;
    char *filename_new;
};
