#include <linux/module.h>       
#include <linux/kernel.h>       
#include <linux/init.h>         

#define MODULE_NAME "lab3"

static int __init lab3_init(void)
{
    pr_info("Welcome to the Tomsk State University\n");
    return 0;
}

static void __exit lab3_exit(void)
{
    pr_info("Tomsk State University forever!\n");
}

module_init(lab3_init);
module_exit(lab3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gorevoy Aleksandr <gorevaya134@gmail.com>");
MODULE_DESCRIPTION("Minimal hello-module for TSU OS Lab #3");
MODULE_VERSION("1.0");