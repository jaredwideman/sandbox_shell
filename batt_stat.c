#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/power_supply.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CB&JW");
MODULE_DESCRIPTION("Battery Info Checker");


//struct to hold info regarding /proc file
static struct proc_dir_entry *procFile;

static int batt_stat_open(struct inode *inode, struct file *file) {
	printk(KERN_ERR " \"batt_stat_read\" is open!\n");
	return 0;
}

static int batt_stat_release(struct inode *inodep, struct file *filep) {
	return 0;
}

static ssize_t batt_stat_read(struct file *file, char __user *buffer, size_t count, loff_t *pos) {
	int length = 0;
    char name[] = "BAT0";
    int result = 0;
    unsigned int curr_energy = 0;
    unsigned int max_energy = 0;
    unsigned int batt_perc = 0;
    struct power_supply *p_supp;
    union power_supply_propval currcharge, chargefull;

    p_supp = power_supply_get_by_name(name);

    if(*pos != 0) { return 0; }	

   length += sprintf(buffer+length, "\n*** BATTERY DATA ***\n");

    result = power_supply_get_property(p_supp, POWER_SUPPLY_PROP_CHARGE_NOW, &currcharge);

    if(!result) {
        length += sprintf(buffer+length, "CURRENT ENERGY LEVEL:\t%d Wh\n", currcharge.intval);
        curr_energy = currcharge.intval;
    }

    result = power_supply_get_property(p_supp, POWER_SUPPLY_PROP_CHARGE_FULL, &chargefull);

    if(!result) {
        length += sprintf(buffer+length, "MAX ENERGY:\t\t%d Wh\n", chargefull.intval);
        max_energy = chargefull.intval;
        
    }

    length += sprintf(buffer+length, "***********************************\n");

    batt_perc = (curr_energy * 100) / max_energy;


    length += sprintf(buffer+length, "BATTERY PERCENTAGE:\t%d %%\n", batt_perc);


	*pos = length;

	return length;
}


static const struct file_operations batt_stat_fops = {
	.owner  = THIS_MODULE,
	.open	= batt_stat_open,
	.read	= batt_stat_read,
	.llseek = seq_lseek,
	.release = batt_stat_release,
};


static int __init batt_stat_init(void) {
    procFile = proc_create("batt_stat", 0, NULL, &batt_stat_fops);

	if (!procFile) {
		return -ENOMEM;
	}

	return 0;
}

static void __exit batt_stat_exit(void) {
	remove_proc_entry("batt_stat", NULL);
}

module_init(batt_stat_init);
module_exit(batt_stat_exit);
