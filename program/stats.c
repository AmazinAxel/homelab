#include <stdio.h> // fopen(), snprintf()
#include <time.h> // time()
#include <stdbool.h> // booleans
#include <sys/statvfs.h> // drive mounting

int isSynced() {
    FILE *f = fopen("/home/alec/lastSynced", "r");
    if (!f) {
        return false;
    };

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
    FILE *file = fopen("/sys/class/net/wlan0/operstate", "r");
    if (!file) {
        return false;
    };

    char u = fgetc(file); // should be u
    char p = fgetc(file); // should be p
    fclose(file);

    return (u == 'u' && p == 'p');
}

char* storageUsage() {
    static char buf[16];

    struct statvfs s;
    if (statvfs("/media", &s) != 0) {
        snprintf(buf, sizeof(buf), "??");
        return buf;
    };

    unsigned long long totalStorage = (unsigned long long)s.f_blocks * s.f_frsize;
    unsigned long long usedStorage = (unsigned long long)(s.f_blocks - s.f_bfree) * s.f_frsize;

    int percentUsed = totalStorage ? (int)((usedStorage * 100) / totalStorage) : 0;

    snprintf(buf, sizeof(buf), "%d%%", percentUsed);
    return buf;
};
