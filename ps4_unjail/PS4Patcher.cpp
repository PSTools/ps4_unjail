#include <stdint.h>
#include 


struct proc_list_entry {
    char p_comm[32];
    int pid;
};

struct sys_proc_list_args {
    struct proc_list_entry *procs;
    uint64_t *num;
};



int sys_proc_list(struct thread *td, struct sys_proc_list_args *uap) {
    struct proc *p;
    int num;
    int r;

    r = 0;

    if(!uap->num) {
        r = 1;
        goto finish;
    }

    if(!uap->procs) {
        // count
        num = 0;
        p = *allproc;
        do {
            num++;
        } while ((p = p->p_forw));
        
        *uap->num = num;
    } else {
        // fill structure
        num = *uap->num;
        p = *allproc;
        for (int i = 0; i < num; i++) {
            memcpy(uap->procs[i].p_comm, p->p_comm, sizeof(uap->procs[i].p_comm));
            uap->procs[i].pid = p->pid;

            if (!(p = p->p_forw)) {
                break;
            }
        }
    }

finish:
    td->td_retval[0] = r;
    return r;
}