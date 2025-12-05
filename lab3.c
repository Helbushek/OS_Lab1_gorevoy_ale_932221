#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/time64.h>
#include <linux/timekeeping.h>

#define PROCFS_NAME "tsulab"

/* Дата гибели Титаника: 15 апреля 1912 года, 00:00 UTC */
static const struct tm titanic_date = {
    .tm_year = 1912,
    .tm_mon  = 4,
    .tm_mday = 15,
    .tm_hour = 0,
    .tm_min  = 0,
    .tm_sec  = 0
};

static int tsulab_proc_show(struct seq_file *m, void *v)
{
    struct timespec64 ts_now;
    time64_t titanic_timestamp;
    time64_t now_timestamp;
    s64 days_diff;

    titanic_timestamp = mktime64(
        titanic_date.tm_year,
        titanic_date.tm_mon,
        titanic_date.tm_mday,
        titanic_date.tm_hour,
        titanic_date.tm_min,
        titanic_date.tm_sec
    );

    ktime_get_real_ts64(&ts_now);
    now_timestamp = ts_now.tv_sec;

    days_diff = div_s64(now_timestamp - titanic_timestamp, 86400LL);

    seq_printf(m, "Days since Titanic sank: %lld\n", days_diff);

    return 0;
}

static int tsulab_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, tsulab_proc_show, NULL);
}

static const struct proc_ops tsulab_proc_fops = {
    .proc_open    = tsulab_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static struct proc_dir_entry *tsulab_proc_entry;

static int __init tsulab_init(void)
{
    pr_info("Welcome to the Tomsk State University\n");

    tsulab_proc_entry = proc_create(PROCFS_NAME, 0644, NULL, &tsulab_proc_fops);
    if (!tsulab_proc_entry) 
    {
        pr_err("Failed to create /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }

    pr_info("/proc/%s created successfully\n", PROCFS_NAME);
    return 0;
}

static void __exit tsulab_exit(void)
{
    if (tsulab_proc_entry)
    {
        proc_remove(tsulab_proc_entry);
    }

    pr_info("Tomsk State University forever!\n");
}

module_init(tsulab_init);
module_exit(tsulab_exit);

MODULE_LICENSE("GPL");
