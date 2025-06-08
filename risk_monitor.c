#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

#define PROC_NAME "risk_monitor"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seu Nome");
MODULE_DESCRIPTION("Modulo do kernel para avaliacao de risco de processos com base em CPU e IO");
MODULE_VERSION("0.2");

// Função para calcular tempo de CPU (user + system)
unsigned long get_cpu_time(const struct task_struct *task)
{
    return task->utime + task->stime;
}

// Função para obter escrita em disco
unsigned long get_write_io_bytes(const struct task_struct *task)
{
    return task->ioac.write_bytes;
}

// Função de avaliação de risco combinando várias métricas
const char *get_risk_level_advanced(const struct task_struct *task)
{
    long nice = task_nice(task);
    unsigned long cpu = get_cpu_time(task);
    unsigned long io_write = get_write_io_bytes(task);

    if (cpu > 10000 || io_write > 500000 || nice < 0)
        return "Alto";
    else if (cpu > 5000 || io_write > 100000)
        return "Medio";
    else
        return "Baixo";
}

static int proc_show(struct seq_file *m, void *v)
{
    struct task_struct *task;

    seq_printf(m, "%-6s %-20s %-5s %-12s %-16s %s\n",
               "PID", "Nome", "Nice", "CPU_Time", "IO_Write(Bytes)", "Risco");

    for_each_process(task)
    {
        seq_printf(m, "%-6d %-20s %-5d %-12lu %-16lu %s\n",
                   task->pid,
                   task->comm,
                   task_nice(task),
                   get_cpu_time(task),
                   get_write_io_bytes(task),
                   get_risk_level_advanced(task));
    }

    return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_show, NULL);
}

static const struct proc_ops proc_file_ops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init risk_monitor_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &proc_file_ops);
    printk(KERN_INFO "risk_monitor: modulo carregado.\n");
    return 0;
}

static void __exit risk_monitor_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "risk_monitor: modulo descarregado.\n");
}

module_init(risk_monitor_init);
module_exit(risk_monitor_exit);
