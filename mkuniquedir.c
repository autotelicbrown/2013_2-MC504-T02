#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>

int itoa (int num, char* str)
{
        int i = 0;
        while (num != 0)
        {
                str[i] = num % 10 + '0';
                num /= 10;
                i++;
        }
        str[i] = '\0';
        return i;
}

asmlinkage long sys_mkuniquedir(const char __user *pathname, umode_t mode) {
        char * str = __getname();
        int len;
        int pid = 0;
        int error = 0;
        mm_segment_t fs;
        char * num = __getname();
        char *aux;
        int i;
        pid = sys_getpid();
        copy_from_user(str,pathname,PATH_MAX);
        len = strlen(str);
        str[len] = '-';
        aux = str + len +1;
        len = itoa(pid,num);
        for (i = 0; i < len; i++)
                aux[i] = num[len-i-1];
        aux[len] = '\0';


        fs = get_fs();
        set_fs (get_ds());

        if(sys_mkdir(str, mode) == -1)
                error = -1;

        set_fs(fs);

        __putname(str);
        __putname(num);

        if (error)
                return error;
        return pid;
}
