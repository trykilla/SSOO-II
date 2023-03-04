/* Process info */
#define PA_CLASS "PA"
#define PB_CLASS "PB"
#define PC_CLASS "PC"
#define PD_CLASS "PD"

#define PA_PATH "./exec/pa"
#define PB_PATH "./exec/pb"
#define PC_PATH "./exec/pc"
#define PD_PATH "./exec/pd"

enum ProcessClass_t
{
    PA,
    PB,
    PC,
    PD
};

struct TProcess_t
{

    enum ProcessClass_t class;
    /* PA or PB */
    pid_t pid;
    /* Process ID */
    char *str_process_class;
    /* String representation of the process class */
};