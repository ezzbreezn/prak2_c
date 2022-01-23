#include <sys/stat.h>
#include <sys/types.h>
#include <stddef.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    UID_SHIFT = 6,
    GROUP_SHIFT = 3
};

static int
gid_search(gid_t gid, unsigned *gids, int gid_count)
{
    if (gids == NULL) {
        return 0;
    }
    for (int i = 0; i < gid_count; ++i) {
        if (gids[i] == gid) {
            return 1;
        }
    }
    return 0;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (stb == NULL || task == NULL) {
        return 0;
    }
    if (task->uid == 0) {
        return 1;
    } else if (task->uid == stb->st_uid) {
        return (((unsigned int) stb->st_mode & (access << UID_SHIFT)) == (unsigned int) access << UID_SHIFT);
    } else if (gid_search(stb->st_gid, task->gids, task->gid_count)) {
        return (((unsigned int) stb->st_mode & (access << GROUP_SHIFT)) == (unsigned int) access << GROUP_SHIFT);
    } else {
        return (((unsigned int) stb->st_mode & access) == (unsigned int) access);
    }
}

