#include <stdio.h> // fopen()
#include <time.h> // time()
#include <stdbool.h> // booleans
#include <sys/statvfs.h> // drive mounting
#include <string.h> // strcspn()

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
    FILE *file = fopen("/sys/class/net/wlan0/operstate", "r");
    char u = fgetc(file); // should be u
    char p = fgetc(file); // should be p
    fclose(file);

    return (u == 'u' && p == 'p');
}

char* storageUsage() {
    struct statvfs s;
    if (statvfs("/media", &s) != 0)
        return strdup("??");

    FILE *fp = popen("df -P /media | awk 'NR==2 {print $5}'", "r");
    char buf[64] = "??";

    if (fp) {
        fgets(buf, sizeof(buf), fp);
        pclose(fp);
        buf[strcspn(buf, "\n")] = 0;
    };

    return strdup(buf);
};
