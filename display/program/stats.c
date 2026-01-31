#include <stdio.h> // fopen()
#include <time.h> // time()
#include <stdbool.h> // booleans

int isSynced() {
    FILE *f = fopen("/home/alec/lastSynced", "r");

    long long last_sync = 0;
    if (fscanf(f, "%lld", &last_sync) != 1) {
        fclose(f);
        return false;
    };
    fclose(f);

    time_t now = time(NULL);
    if (now == (time_t)-1) {
        perror("time");
        return 1;
    };

    long long diff = (long long)now - last_sync;

    if (diff > (24 * 60 * 60)) {
        return false; // Not synced
    } else {
        return true; // Synced
    };
};

bool isNetworkConnected() {
    return false; // todo
};

char* storageUsage() {
    return "6/32GB"; // TODO finish
};
