/*
 *  Queued FTP Client
 *
 *  $Revision: 1.9 $
 *  $Date: 2003/03/11 01:26:39 $
 *  vim: sw=4 ts=4
 *
 */
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <glob.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#ifndef __GLIBC__
#include <term.h>
#endif
#include <ite_typedef.h>
#include <ite_common.h>
#include <ite_log.h>
#include <ite_msg_key.h>
#include <ITENetSDK.h>
#include <ITENICP.h>
#include <ITENITP.h>
#include <ite_msg_define.h>
#include <jpeg_extheader.h>
#include "itg_config_info.h"
#include <ite_osal.h>

#include "ftp.h"
#include "text.h"
#include "rc.h"
#include "quftp.h"


iConfigInfo         *pFtpserver = NULL;
char ITG_File[1024];
#define SEND_FILE_SHARE_KEY 0x12345             // for  itarge used add by linwei   memory share key
#define SEND_SIZE_MAX 0x100000                  // for  itarge used add by linwei   send info user passwd port serverip
#define ETH_NAME_1  "eth1"                            //Net interface
#define ETH_NAME_0  "eth0"                            //Net interface

/* Increase this for a steadier but less accurate ETA and Speed readout */
#define SPEEDS_AVERAGE      5

/* Maximum number of concurrent connections */
#define MAX_CONNECTIONS     3

/* Define a couple of very handy macros */
#define CURRENT_CONNECTION  connection[atoi(config(variables, "CURRENT_CONNECTION"))]
#define LAST_CONNECTION     connection[atoi(config(variables, "LAST_CONNECTION"))]

struct ftpconnection *connection[MAX_CONNECTIONS];
struct qitem *leftqueue = NULL;
struct variable_list *variables = NULL;
struct alias_list *aliases = NULL;

struct COMMAND command_list[] =
{
    {"ls", cmd_ls, "List current directory", NULL},
    {"lls", cmd_lls, "List local directory", NULL},
    {"dir", cmd_ls, "Synonym for \"ls\"", NULL},
    {"cd", cmd_cd, "Change the current directory", NULL},
    {"lcd", cmd_lcd, "Change the current local directory", NULL},
    {"pwd", cmd_pwd, "Show current working directory", NULL},
    {"get", cmd_get, "Queue a file for download", NULL},
    {"put", cmd_put, "Queue a file for upload", NULL},
    {"fxp", cmd_fxp, "Queue a file to be transferred between connections", NULL},
    {"go", cmd_go, "Start transferring the queue", NULL},
    {"clear", cmd_clear, "Delete all items from the queue", NULL},
    {"queue", cmd_queue, "List all items on the queue", NULL},
    {"quit", cmd_quit, "Exit the program", NULL},
    {"close", cmd_close, "Disconnect from the server", NULL},
    {"open", cmd_open, "Connect to a server", NULL},
    {"status", cmd_status, "Show current status", NULL},
    {"stat", cmd_stat, "Display file statistics", NULL},
    {"nlst", cmd_nlst, "List filenames in current directory only", NULL},
    {"switch", NULL, "Switch to another connection", NULL},
    {"debug", NULL, "Set debugging level", "Specify the debug level from 1 to 5"},
    {"user", cmd_user, "Enter username and password for the server", "Syntax: user username [password]\nPassword is optional and should be separated from username by a space."},
    {"cat", cmd_cat, "Output a text file to stdout", NULL},
    {"more", cmd_more, "Page a text file", NULL},
    {"set", cmd_set, "Set a variable", NULL},
    {"unset", cmd_unset, "Remove a variable", NULL},
    {"rm", cmd_dele, "Delete a remote file", NULL},
    {"run", NULL, "Run a script file", NULL},
    {"alias", cmd_alias, "Define a command alias", NULL},
    {"getlist", cmd_getlist, "Get all files listed in param.txt", NULL},
    {"assert", cmd_assert, "Die if the last operation was not successful", NULL},
    {"quote", cmd_quote, "Send a command directly to the server", NULL},
    {"exit", cmd_quit, "Synonym for quit", NULL},
    {"mkdir", cmd_mkdir, "Make directory", NULL},
    {"rmdir", cmd_rmdir, "Remove directory", NULL},
    {"upload", cmd_upload, "upload File to ftp server", NULL},
    {NULL, NULL, NULL, NULL}
};


int fstep_log(char *logStr)
{
    return 0;
#define FTP_LIBS_LOG "/tmp/ftpexe_log"
    FILE *ftplib_flagFD = fopen(FTP_LIBS_LOG, "a+");
    if (ftplib_flagFD) //创建日志文件记录发送文件名
    {
        int wCnt = 0;
        wCnt = fwrite(logStr, 1, strlen(logStr), ftplib_flagFD);
        fwrite(" \n", 1, 2, ftplib_flagFD);
        fclose(ftplib_flagFD);
    }
    return 0;
}

int fstep_log_delete()
{
#define FTP_LIBS_LOG "/tmp/ftpexe_log"
    unlink(FTP_LIBS_LOG);
    return 0;
}


int main(int argc, char **argv)
{
    int arg, ignoreerrors = 0;
    int i;
    FILE *scriptfile = NULL;
    FILE *rcfilehandle = NULL;
    char *rcfile = NULL;
    char *itgfile = NULL;
    char *filepath = NULL, *filename = NULL;
    alarm(60);
    ITE_OSAL_MSG_INFO_T pFtpshare;
    memset(&pFtpshare, 0x0, sizeof(ITE_OSAL_MSG_INFO_T));
    InitFtpshare(&pFtpshare);
    ITE_SHM_Get_Addr(pFtpshare.shmid, (OS_HANDLE)&pFtpserver);
    log_set_level(2);
    log_show_function = false;
    log_show_pid = false;
    log_show_file = false;
    log_show_level = false;
    log_show_date = false;

    setbuf(stdout, NULL);
    //ftplog(LOG_CRITICAL, "ftp version %s     copyright@iTarge (soft@iTarge.com)\n\n", VERSION);
    leftqueue = NULL;
    ftp_port = pFtpserver->FtpParam.stFTPParam.wPort;
#if 0
    rl_attempted_completion_function = (CPPFunction *)quftp_completion;
#endif
    setconfig(&variables, "PROGRAM_NAME", "quftp");
    setconfig(&variables, "VERSION", VERSION);
    setconfig(&variables, "CURRENT_CONNECTION", "0");
    setconfig(&variables, "LAST_CONNECTION", "1");
    setconfig(&variables, "passive", "true");
    if (access("/mnt/ramdisk/ftplog", 0) == 0)
    {
        log_set_level(6);
    }

    for (i = 0; i < MAX_CONNECTIONS; i++)
    {
        connection[i] = ftp_new();
        connection[i]->variables = variables;
        getcwd((char *)connection[i]->localdir, 256);
    }

    /* Process command line arguments */
    //  ftplog(LOG_CRITICAL, " arg iTarge \n" );
    while ((arg = getopt(argc, argv, "d:::r:s:f:x:p:n:ih")) != -1)
        switch (arg)
        {
            case 'd':
                if (optarg)
                {
                    log_set_level(atoi(optarg));
                }
                else
                {
                    log_set_level(log_get_level() + 1);
                }
                ftplog(log_get_level(), __FILE__, __LINE__, __FUNCTION__, "Debugging level is now %d", log_get_level());
                break;
            case 'i':
                ignoreerrors = 1;
                break;

            case 'r':
                rcfile = strdup(optarg);
                ftplog(LOG_WARNING, "Reading configuration from %s\n", rcfile);
                break;
            case 'f' :                      // iTarge  used  ITG_File is filename
            {
                itgfile = strdup(optarg);
                strcpy(ITG_File, itgfile);
                break;
            }
            case 's':
                if (!(scriptfile = fopen(optarg, "r")))
                {
                    perror("fopen");
                    if (!ignoreerrors) { exit(0); }
                }
                break;
            case 'p':
                filepath = strdup(optarg);
                break;
            case 'n':
                filename = strdup(optarg);
                break;
            case 'x':
            {
                int ret = 1;
                ret = cmd_X();
                ftp_disconnect(CURRENT_CONNECTION);
                ITE_SHM_Detach_Addr(pFtpserver);
                cmd_quit("quit");
                fstep_log_delete();
                if (ret == 0)
                {
                    exit(0);
                }
                else
                {
                    exit(-1);
                }
            }
            break;
            case 'h':
                printf("Usage: quftp [-vhi] [-d[n]] [-rfilename] [-sfilename][-f filename] [URL]\n\n");
                printf("  -h		Print this help, then exit\n");
                printf("  -d		Increase debugging level (optional argument)\n");
                printf("  -i		Ignore errors in a script\n");
                printf("  -r		Read configuration from filename instead of %s\n", DEFAULT_RC_FILENAME);
                printf("  -s		Process script filename\n");
                printf("  -f		upload  filename\n");
                printf("  URL		Connect to any valid ftp url.. Read man 1 quftp for more information\n");
                printf("\n");
                ITE_SHM_Detach_Addr(pFtpserver);
                exit(0);
                break;
        }
#if 0
    if (!rcfile)
    {
        rcfile = strdup(DEFAULT_RC_FILENAME);
    }
#endif
    if (rcfile)
    {
        rcfile = expand_home(rcfile);
        rcfilehandle = fopen(rcfile, "r");
        if (!rcfilehandle) { ftplog(LOG_WARNING, "Can't find rcfile %s", rcfile); }
    }

    if (optind < argc)
    {
        if (!parseurl(argv[optind], CURRENT_CONNECTION))
        {
            ftplog(LOG_ALERT, "There was an error parsing \"%s\"\n", argv[optind]);
        }
        else
        {
            if (strlen(CURRENT_CONNECTION->password) == 0)
            {
                if (config(variables, "email"))
                {
                    strncpy((char *)CURRENT_CONNECTION->password, config(variables, "email"), 32);
                }
                else
                {
                    strncpy((char *)CURRENT_CONNECTION->password, "mantis@itarge.com", 32);
                }
            }
            CURRENT_CONNECTION->status = STATUS_WAIT_CONNECT;
            if (CURRENT_CONNECTION->remotedir[0] != '\0')
            {
                (void)getconnected(CURRENT_CONNECTION);
            }
        }
    }
    ITE_LOG(ITE_LOG_INFO, "[ftp.out][%s]\n", itgfile);

    while (1)
    {
        char *line = NULL, *command_word = NULL;
        struct COMMAND *command = NULL;
        register int i;
        int result;

        if (line)
        {
            free(line);
            line = NULL;
        }
        if (itgfile && filepath)
        {
            cmd_upload_common(ITG_File, filepath, filename);
            ftp_disconnect(CURRENT_CONNECTION);
            ITE_SHM_Detach_Addr(pFtpserver);
            exit(0);
        }
        else if (itgfile)
        {
            // iTarge process   begin upload
            free(itgfile);
            cmd_upload(ITG_File);
            ftp_disconnect(CURRENT_CONNECTION);
            ITE_SHM_Detach_Addr(pFtpserver);
            fstep_log_delete();
            exit(0);
        }
        else if (rcfilehandle)
        {
            char *temp;
            line = (char *)malloc(4096);
            fgets(line, 4096, rcfilehandle);
            if (!*line)
            {
                /* End of script? */
                fclose(rcfilehandle);
                rcfilehandle = NULL;
                continue;
            }
            if (*line == '#') { continue; } // Comment
            temp = strchr(line, '\n');
            if (temp == line) { continue; } // Zero length line?
            else { *temp = '\0'; }      // Strip \n
            ftplog(LOG_INFO, "%s> %s", rcfile, line);
        }
        else if (scriptfile)
        {
            /* Reading from a script or command line? */
            char *temp;
            line = (char *)malloc(4096);
            fgets(line, 4096, scriptfile);
            if (!*line)
            {
                /* End of script? */
                fclose(scriptfile);
                scriptfile = NULL;
                continue;
            }
            if (*line == '#') { continue; }
            temp = strchr(line, '\n');
            if (temp == line) { continue; } // Zero length line?
            else { *temp = '\0'; }      // Strip \n
            add_history(line);
            printf("quftp> %s\n", line);
        }
        else
        {
            // 命令行方式
            line = readline("quftp> ");
            if (!line)              // NULL returned from readline is CTRL-D
            {
                line = strdup("quit");
            }
            else if (!*line)        // "" returned from readline is an empty line
            {
                continue;
            }
            add_history(line);
        }

        line = parseline(line);
        line = expand_alias(line);

        command_word = getnextword(line, GNW_START);
        if (!command_word || !*command_word) { continue; }

        /* Match core command set */
        if (strcasecmp(command_word, "switch") == 0)
        {
            char *temp;
            char *param = getnextword(line, GNW_NEXT);
            if (!param)
            {
                int curr = atoi(config(variables, "CURRENT_CONNECTION"));
                int last = atoi(config(variables, "LAST_CONNECTION"));
                temp = (char *)malloc(4096);
                sprintf(temp, "%d", last);
                setconfig(&variables, "CURRENT_CONNECTION", temp);
                sprintf(temp, "%d", curr);
                setconfig(&variables, "LAST_CONNECTION", temp);
                free(temp);
            }
            else
            {
                int new_connection = atoi(param);
                if (new_connection >= MAX_CONNECTIONS)
                {
                    ftplog(LOG_CRITICAL, "Only %d number of connections avaliable (0-%d)\n",
                           MAX_CONNECTIONS, MAX_CONNECTIONS - 1);
                    continue;
                }
                else
                {
                    temp = (char *)malloc(4096);
                    if (new_connection == atoi(config(variables, "CURRENT_CONNECTION"))) { continue; }
                    setconfig(&variables, "LAST_CONNECTION", config(variables, "CURRENT_CONNECTION"));
                    sprintf(temp, "%d", new_connection);
                    setconfig(&variables, "CURRENT_CONNECTION", temp);
                    free(temp);
                }
            }
            ftplog(LOG_WARNING, "Switched to connection %s\n", config(variables, "CURRENT_CONNECTION"));
            setconfig(&variables, "HOSTNAME", *CURRENT_CONNECTION->hostname ? CURRENT_CONNECTION->hostname : NULL);
            setconfig(&variables, "USERNAME", *CURRENT_CONNECTION->username ? CURRENT_CONNECTION->username : NULL);
            continue;
        }
        else if (strstr(command_word, "!") == command_word)
        {
            char *temp;
            int rval;
            temp = line + 1;
            rval = system(temp);
            if (rval) { ftplog(LOG_ALERT, "Return value from \"%s\": %d\n", temp, rval); }
            continue;
        }
        else if (strcasecmp(command_word, "run") == 0)
        {
            char *filename;
            filename = getnextword(line, GNW_NEXT);
            if (!filename || !*filename)
            {
                ftplog(LOG_CRITICAL, "You must specify a script file to run\n");
                continue;
            }
            if (!(scriptfile = fopen(filename, "r")))
            {
                perror("fopen");
                if (!ignoreerrors) { continue; }
            }
            continue;
        }
        else if (strcasecmp(command_word, "debug") == 0)
        {
            char *param;
            if ((param = getnextword(line, GNW_NEXT)))
            {
                log_set_level(atoi(param));
            }
            else
            {
                log_set_level(log_get_level());
            }
            ftplog(log_get_level(), __FILE__, __LINE__, __FUNCTION__, "Debugging level is now %d", log_get_level());
            continue;
        }
        else if (strcasecmp(command_word, "help") == 0 || strcmp(command_word, "?") == 0)
        {
            char *param;
            int foundit = 0;
            if ((param = getnextword(line, GNW_NEXT)))
            {
                for (i = 0; command_list[i].name && *command_list[i].name; i++)
                {
                    if (strcmp(command_list[i].name, param) == 0)
                    {
                        if (command_list[i].longhelp)
                        {
                            printf("%s\n", command_list[i].longhelp);
                        }
                        else
                        {
                            printf("No extra help available for %s\n", param);
                        }
                        foundit = 1;
                    }
                }
                if (!foundit)
                {
                    printf("Command %s does not exist\n", param);
                }
            }
            else
            {
                for (i = 0; command_list[i].name && *command_list[i].name; i++)
                    printf("%-10s%s\n", command_list[i].name,
                           command_list[i].description);
            }
            continue;
        }

        /* Match dynamic command set */
        for (i = 0; command_list[i].name; i++)
        {
            if (strcasecmp(command_word, command_list[i].name) == 0)
            {
                command = &command_list[i];
                break;
            }
        }
        if (!command)
        {
            printf("Unknown command \"%s\"\n", command_word);
            if (scriptfile && !ignoreerrors) { break; }
            continue;
        }
        result = (command->function)(line);
        if (result < 0)
        {
            break;
        }
        else if (result > 0)        // Successful command
        {
            setconfig(&variables, "SUCCESSFUL_OPERATION", "true");
        }
        else                // Unsuccessful command
        {
            setconfig(&variables, "SUCCESSFUL_OPERATION", "false");
            if (scriptfile && !ignoreerrors)
            {
                ftplog(LOG_CRITICAL, "Error processing script\n");
                break;
            }
        }
        continue;
    }
    ftp_disconnect(CURRENT_CONNECTION);
    ITE_SHM_Detach_Addr(pFtpserver);

    return 0;
}

in_addr_t net_get_ifaddr(char *ifname)
{
    struct ifreq ifr;
    int skfd;
    struct sockaddr_in *saddr;

    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "socket error\n");
        return -1;
    }

    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0)
    {
        fprintf(stderr, "net_get_ifaddr: ioctl SIOCGIFADDR\n");
        close(skfd);
        return -1;
    }
    close(skfd);

    saddr = (struct sockaddr_in *) &ifr.ifr_addr;
    return saddr->sin_addr.s_addr;
}


/*
****************************************************************
*               iTarge used some item
****************************************************************
*/

int InitFtpshare(ITE_OSAL_MSG_INFO_T *pshareOpt)
{

    if (ITE_MESSAGE_Create(ITE_CONFIG_INFO_KEY, pshareOpt, 0) != 0)
    {
        ITE_LOG(ITE_LOG_ERROR, "Create the FTp share failed\n");
        return ITE_OSAL_FAIL;
    }
    return ITE_OSAL_OK;

}
#if 0
int ITE_SHM_Detach_Addr(void *pSrc)
{
    shmdt(pSrc);
    return 0;
}
#endif
struct qitem *addtoqueue(struct qitem **queue, struct qitem *item)
{
    struct qitem *newitem = 0;
    struct qitem *tempitem = 0, *lastgooditem = 0;
    int id = 0;
    if (!(newitem = (struct qitem *)malloc(sizeof(struct qitem))))
    {
        perror("malloc");
        exit(-1);
    }
    memcpy(newitem, item, sizeof(struct qitem));
    tempitem = *queue; /* Find the last item on the queue */
    while (tempitem)
    {
        lastgooditem = tempitem;
        if (strcmp(newitem->remotefilename, tempitem->remotefilename) == 0) { return NULL; }
        if (tempitem->id > id) { id = tempitem->id; }
        tempitem = tempitem->next;
    }
    newitem->id = ++id;
    if (lastgooditem) { lastgooditem->next = newitem; }
    if (!*queue) { *queue = newitem; }
    newitem->next = 0;
    return newitem;
}

struct qitem *delfromqueue(struct qitem **queue, int id)
{
    struct qitem *item, *last;
    int counter;
    item = last = *queue;
    while (item)
    {
        if (item->id == id)
        {
            last->next = item->next;
            if (item == *queue)
            {
                if (item->next) { *queue = item->next; }
                else { *queue = NULL; }
            }
            free(item);
            item = *queue;
            counter = 1; /* Straighten out counter */
            while (item)
            {
                item->id = counter;
                counter++;
                item = item->next;
            }
            return NULL;
        }
        last = item;
        item = item->next;
    }
    return *queue;
}

void clearqueue(struct qitem *queue)
{
    struct qitem *item, *next = NULL;
    item = queue;
    while (item)
    {
        next = item->next;
        free(item);
        item = next;
    }
}

struct qitem *runqueue(struct qitem *queue)
{
    struct qitem *item;
    int id, n;
    double starttime, timelength, endtime, speed;
    struct timeval tv;
    item = queue;
    while (item)
    {
        id = item->id;
        if (item->direction == DIRECTION_GET)
        {
            int result;
            result = chdir(item->localdir);
            if (result) switch (errno)
                {
                    case ENOENT  :
                        ftplog(LOG_ERROR, "Creating directory %s\n", item->localdir);
                        mkdir(item->localdir, 0755);
                        break;
                    case ENOTDIR :
                        ftplog(LOG_ERROR, "A component of path is not a directory.\n");
                        item = item->next;
                        delfromqueue(&queue, id);
                        break;
                    case EACCES  :
                    default      :
                        ftplog(LOG_ERROR, "Can't change to %s: ", item->localdir, strerror(errno));
                        item = item->next;
                        delfromqueue(&queue, id);
                        continue;
                        break;
                }
            result = chdir(item->localdir);
            ftp_cwd(item->remoteconnection, item->remotedir);
            gettimeofday(&tv, NULL);
            if ((item->permissions & 2048) == 2048)
            {
                struct qitem *newitem, *newitem2;
                char *param, *filenamelist[500];
                char path[4096];
                struct filedetails details;
                int count = 0;
                sprintf(path, "%s/%s", item->remotedir, item->remotefilename);
                ftp_cwd(item->remoteconnection, path);
                memset(filenamelist, 0, 500 * sizeof(char *));
                ftp_nlst(item->remoteconnection, NULL, filenamelist, 500);
                while (count < 500 && filenamelist[count])
                {
                    param = filenamelist[count];
                    if (!param || !*param)
                    {
                        free(param);
                        count++;
                        continue;
                    }
                    if (!(newitem = malloc(sizeof(struct qitem))))
                    {
                        perror("malloc");
                        exit(1);
                    }
                    memset(newitem, 0, sizeof(struct qitem));
                    strncpy(newitem->localfilename, param, 256);
                    strncpy(newitem->remotefilename, param, 256);

                    result = ftp_exists(CURRENT_CONNECTION, param);
                    if (result == 0)
                    {
                        ftplog(LOG_ALERT, "%s does not exist\n", param);
                        free(newitem);
                        free(param);
                        continue;
                    }
                    if (result < 0)
                    {
                        ftplog(LOG_ALERT, "Can't verify existance of %s.. Don't blame me if this fails!\n", param);
                    }
                    if (result > 0)
                    {
                        ftp_stat(CURRENT_CONNECTION, param, &details);
                        newitem->permissions = details.permissions;
                        newitem->size = details.size;
                    }
                    newitem->direction = DIRECTION_GET;
                    sprintf(newitem->remotedir, "%s/%s", item->remotedir,
                            item->remotefilename);
                    sprintf(newitem->localdir, "%s/%s", item->localdir,
                            item->localfilename);
                    newitem->remoteconnection = item->remoteconnection;
                    newitem->localconnection = item->localconnection;
                    if ((newitem2 = addtoqueue(&queue, newitem)))
                    {
                        ftplog(LOG_INFO, "Queued %s/%s for download, position %d\n", newitem2->remotedir, newitem2->localfilename, newitem2->id);
                    }
                    free(newitem);
                    free(param);
                    count++;
                }
                item = item->next;
                delfromqueue(&queue, id);
                continue;
            }

            starttime = ((tv.tv_sec * 1.0) + (tv.tv_usec / 1000000.0));
            n = ftp_get_file(item->remoteconnection, item);
            if (n > 0)
            {
                gettimeofday(&tv, NULL);
                endtime = ((double)tv.tv_sec + (tv.tv_usec / 1000000));
                timelength = abs(endtime - starttime);
                if (timelength == 0) { timelength = 1; }
                ftplog(LOG_INFO, "Received %s (%d bytes)",
                       item->remotefilename, n);
                speed = (n / 1000) / timelength;
                ftplog(LOG_INFO, " in %lu seconds (%1.2fKb/s)\n",
                       (unsigned long)timelength, speed);
            }
        }
        else if (item->direction == DIRECTION_PUT)
        {
            if (!ftp_cwd(item->remoteconnection, item->remotedir))
            {
                if (!ftp_mkd(item->remoteconnection, item->remotedir))
                {
                    item = item->next;
                    delfromqueue(&queue, id);
                    continue;
                }
                if (!ftp_cwd(item->remoteconnection, item->remotedir))
                {
                    item = item->next;
                    delfromqueue(&queue, id);
                    continue;
                }
            }
            chdir(item->localdir);
            if (S_ISDIR(item->permissions))
            {
                struct qitem *newitem, *newitem2;
                struct dirent *dirent;
                DIR *dir;
                char path[4096];
                int count = 0;
                sprintf(path, "%s/%s", item->localdir, item->localfilename);
                dir = opendir(path);
                if (!dir)
                {
                    ftplog(LOG_ALERT, "Can't recursively put %s\n", path);
                    item = item->next;
                    delfromqueue(&queue, id);
                    continue;
                }
                while ((dirent = readdir(dir)))
                {
                    char fullname[4096];
                    struct stat statbuf;
                    if (strcmp(dirent->d_name, ".") == 0) { continue; }
                    if (strcmp(dirent->d_name, "..") == 0) { continue; }
                    if (strlen(dirent->d_name) == 0) { continue; }
                    if (!(newitem = malloc(sizeof(struct qitem))))
                    {
                        perror("malloc");
                        exit(1);
                    }
                    sprintf(fullname, "%s/%s", path, dirent->d_name);
                    if (stat(fullname, &statbuf))
                    {
                        ftplog(LOG_ALERT, "%s does not exist\n", fullname);
                        free(newitem);
                        continue;
                    }
                    memset(newitem, 0, sizeof(struct qitem));
                    strncpy(newitem->localfilename, dirent->d_name, 256);
                    strncpy(newitem->remotefilename, dirent->d_name, 256);
                    newitem->permissions = statbuf.st_mode;
                    newitem->size = statbuf.st_size;
                    newitem->direction = DIRECTION_PUT;
                    sprintf(newitem->remotedir, "%s/%s", item->remotedir,
                            item->remotefilename);
                    sprintf(newitem->localdir, "%s/%s", item->localdir,
                            item->localfilename);
                    newitem->remoteconnection = item->remoteconnection;
                    newitem->localconnection = item->localconnection;
                    if ((newitem2 = addtoqueue(&queue, newitem)))
                    {
                        ftplog(LOG_INFO, "Queued %s/%s for upload, position %d\n", newitem2->remotedir, newitem2->localfilename, newitem2->id);
                    }
                    free(newitem);
                    count++;
                }
                item = item->next;
                delfromqueue(&queue, id);
                continue;
            }
            starttime = ((tv.tv_sec * 1.0) + (tv.tv_usec / 1000000.0));
            n = ftp_put_file(item->remoteconnection, item);
            if (n > 0)
            {
                gettimeofday(&tv, NULL);
                endtime = ((double)tv.tv_sec + (tv.tv_usec / 1000000));
                timelength = abs(endtime - starttime);
                if (timelength == 0) { timelength = 1; }
                ftplog(LOG_INFO, "Sent %s (%d bytes)",
                       item->remotefilename, n);
                speed = (n / 1000) / timelength;
                ftplog(LOG_INFO, " in %lu seconds (%1.2fKb/s)\n",
                       (unsigned long)timelength, speed);
            }

        }
        else if (item->direction == DIRECTION_FXP)
        {
            char sourcefile[4096], destfile[4096];
            if ((item->permissions & 2048) == 2048)
            {
                struct qitem *newitem, *newitem2;
                char *param, *filenamelist[500];
                char path[4096];
                struct filedetails details;
                int count = 0;
                sprintf(path, "%s/%s", item->remotedir, item->remotefilename);
                ftp_cwd(item->localconnection, path);
                memset(filenamelist, 0, 500 * sizeof(char *));
                ftp_nlst(item->localconnection, NULL, filenamelist, 500);
                while (count < 500 && filenamelist[count])
                {
                    int result;
                    param = filenamelist[count];
                    if (!param || !*param)
                    {
                        free(param);
                        count++;
                        continue;
                    }
                    if (!(newitem = malloc(sizeof(struct qitem))))
                    {
                        perror("malloc");
                        exit(1);
                    }

                    result = ftp_exists(CURRENT_CONNECTION, param);
                    if (result == 0)
                    {
                        ftplog(LOG_WARNING, "%s does not exist\n", param);
                        free(newitem);
                        free(param);
                        continue;
                    }
                    if (result < 0)
                    {
                        ftplog(LOG_WARNING, "Can't verify existance of %s.. Don't blame me if this fails!\n", param);
                    }
                    if (result > 0)
                    {
                        ftp_stat(CURRENT_CONNECTION, param, &details);
                        newitem->permissions = details.permissions;
                        newitem->size = details.size;
                    }

                    memset(newitem, 0, sizeof(struct qitem));
                    strncpy(newitem->localfilename, param, 256);
                    strncpy(newitem->remotefilename, param, 256);
                    newitem->direction = DIRECTION_GET;
                    sprintf(newitem->remotedir, "%s/%s", item->remotedir,
                            item->remotefilename);
                    sprintf(newitem->localdir, "%s/%s", item->localdir,
                            item->localfilename);
                    newitem->remoteconnection = item->remoteconnection;
                    newitem->localconnection = item->localconnection;
                    if ((newitem2 = addtoqueue(&queue, newitem)))
                    {
                        ftplog(LOG_INFO, "Queued %s/%s for FxP, position %d\n", newitem2->remotedir, newitem2->localfilename, newitem2->id);
                    }
                    free(newitem);
                    free(param);
                    count++;
                }
                item = item->next;
                delfromqueue(&queue, id);
                continue;
            }
            sprintf(sourcefile, "%s/%s", item->localdir, item->localfilename);
            sprintf(destfile, "%s/%s", item->remotedir, item->remotefilename);
            starttime = ((tv.tv_sec * 1.0) + (tv.tv_usec / 1000000.0));
            ftplog(LOG_WARNING, "Progress information not available for FxP transfers\n");
            n = ftp_fxp(item->localconnection, item->remoteconnection,
                        sourcefile, destfile);
            if (n == 0)
            {
                gettimeofday(&tv, NULL);
                endtime = ((double)tv.tv_sec + (tv.tv_usec / 1000000));
                timelength = abs(endtime - starttime);
                if (timelength == 0) { timelength = 1; }
                ftplog(LOG_INFO, "Sent %s (%lu bytes)",
                       item->remotefilename, item->size);
                speed = (item->size / 1000) / timelength;
                ftplog(LOG_INFO, " in %lu seconds (%1.2fKb/s)\n",
                       (unsigned long)timelength, speed);
            }
        }
        else
        {
            ftplog(LOG_CRITICAL, "Corrupted item %d\n", id);
        }
        item = item->next;
        delfromqueue(&queue, id);
    }
    return queue;
}

void printqueue(struct qitem *queue)
{
    struct qitem *item;
    char fullname[200];
    long totalsize = 0;
    ftplog(LOG_CALLS, "%p", queue);
    item = queue;
    printf("%-3s    %-30s %-10s%-30s\n", "ID", "Source",
           "Size", "Destination");
    while (item)
    {
        if (item->direction == DIRECTION_GET)
        {
            sprintf(fullname, "%s/%s", item->remotedir, item->remotefilename);
            printf("%-3d <- %-30s %-10lu%-30s\n", item->id, fullname, item->size,
                   item->localdir);
        }
        else if (item->direction == DIRECTION_PUT)
        {
            sprintf(fullname, "%s/%s", item->localdir, item->localfilename);
            printf("%-3d -> %-30s %-10lu%-30s\n", item->id, fullname,
                   item->size, item->remotedir);
        }
        else if (item->direction == DIRECTION_FXP)
        {
            char remotename[200];
            sprintf(fullname, "%s:%s%s%s%s", item->localconnection->hostname,
                    item->localdir, (item->remotefilename[0] == '/')
                    ? "" : "/", item->localfilename,
                    ((item->permissions & 2048) == 2048) ? "/" : "");
            sprintf(remotename, "%s:%s%s%s%s", item->remoteconnection->hostname,
                    item->remotedir, (item->remotefilename[0] == '/')
                    ? "" : "/", item->remotefilename,
                    ((item->permissions & 2048) == 2048) ? "/" : "");
            printf("%-3d -> %-30s %-10lu%-30s\n", item->id, fullname,
                   item->size, remotename);
        }
        else
        {
            ftplog(LOG_CRITICAL, "Bad entry %d\n", item->id);
        }
        totalsize += item->size;
        item = item->next;
    }
    printf("\nTotal queue size: %li bytes\n\n", totalsize);
}

int getconnected(struct ftpconnection *connection)
{
    char *dir = NULL;
    ftplog(LOG_CALLS, "%p", connection);
    if (!connection) { return 0; }
    if (!connection->hostname) { return 0; }
    if (connection->status > STATUS_WAIT_CONNECT) { return 1; }
    if (connection->remotedir[0] != '\0')
    {
        dir = strdup(connection->remotedir);
    }
    if (ftp_connect(connection))
    {
        if (dir) { free(dir); }
        exit(-1);
        return 0;
    }
    if (ftp_login(connection))
    {
        if (dir) { free(dir); }
        ftp_disconnect(connection);
        exit(-1);
        return 0;
    }
    if (dir)
    {
        if (!ftp_cwd(connection, dir)) { ftplog(LOG_ALERT, "Can't change directory to %s\n", dir); }
        free(dir);
    }
    connection->status = STATUS_IDLE;
    setconfig(&variables, "USERNAME", connection->username);
    return 1;
}

char *complete_remote_filename(const char *text, int state)
{
    static char *filenamelist[500];
    static int index = 0;
    int response;
    char *retstring = (char *)NULL;

    if (state == 0)
    {
        char *path;
        memset(&filenamelist[0], 0, sizeof(char *) * 500);
        if (!text || !*text) { return NULL; }
        path = getpath(text);
        response = ftp_nlst(CURRENT_CONNECTION, path, filenamelist, 500);
        if (response < 0) { return NULL; }
        index = 0;
    }

    while (filenamelist[index])
    {
        if (strstr(filenamelist[index], text) == filenamelist[index])
        {
            retstring = strdup(filenamelist[index]);
            free(filenamelist[index]);
            index++;
            return retstring;
        }
        free(filenamelist[index]);
        index++;
    }
    index = 0;
    return NULL;
}

char *complete_command(const char *text, int state)
{
    /* Don't know how I'm going to do this yet */
    return NULL;
}

char **quftp_completion(char *text, int start, int end)
{
    char **matches;
    matches = (char **)NULL;
    if (start == 0)
    {
        /* It's a command */
        matches = rl_completion_matches(text, complete_command);
    }
    else if (strstr(text, "put") != text)
    {
        /* It's a filename */
        matches = rl_completion_matches(text, complete_remote_filename);
    }
    return (matches);
}

int cmd_ls(char *command_line)
{
    int result;
    char *params = NULL;
    params = getnextword(command_line, GNW_REST);
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "ls: Not connected\n");
        return 0;
    }
    result = ftp_list(CURRENT_CONNECTION, params, 1);
    printf("\n");
    if (result < 0) { return 0; }
    return 1;
}


int cmd_cd(char *command_line)
{
    char *param;
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "cd: Not connected\n");
        return 0;
    }
    getnextword(command_line, GNW_START);
    param = getnextword(command_line, GNW_NEXT);
    if (!param || !*param)
    {
        ftplog(LOG_ALERT, "Must specify a directory to change\n");
        return 0;
    }
    else
    {
        if (!ftp_cwd(CURRENT_CONNECTION, param))
        {
            return 0;
        }
    }
    return 1;
}

int cmd_cdup(char *command_line)
{
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "cdup: Not connected\n");
        return 0;
    }
    if (!ftp_cwd(CURRENT_CONNECTION, "..")) { return 0; }
    return 1;
}

int cmd_pwd(char *command_line)
{
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "pwd: Not connected\n");
        return 0;
    }
    ftp_pwd(CURRENT_CONNECTION);
    ftplog(LOG_INFO, "The current remote directory is %s\n", CURRENT_CONNECTION->remotedir);
    ftplog(LOG_INFO, "The current local directory is %s\n", CURRENT_CONNECTION->localdir);
    return 1;
}

int cmd_lcd(char *command_line)
{
    char *param = NULL;
    int rval;
    getnextword(command_line, GNW_START);
    param = getnextword(command_line, GNW_NEXT);
    if (!param || !*param)
    {
        ftplog(LOG_ALERT, "Specify a directory to change to\n");
        return 0;
    }
    rval = chdir(param);
    if (rval)
    {
        ftplog(LOG_ALERT, "Can't change to \"%s\"\n", param);
        return 0;
    }
    getcwd(CURRENT_CONNECTION->localdir, 256);
    ftplog(LOG_INFO, "Current local directory is now \"%s\"\n", CURRENT_CONNECTION->localdir);
    return 1;
}

int cmd_get(char *command_line)
{
    struct qitem *item, *newitem;
    char *expanded_line;
    char *param;
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "get: Not connected\n");
        return 0;
    }
    expanded_line = expand_wildcard(command_line);
    getnextword(expanded_line, GNW_START);
    while ((param = getnextword(expanded_line, GNW_NEXT)))
    {
        char path[4096], filename[4096];
        int index;
        struct filedetails details;
        if (!param || !*param) { continue; }
        if (ftp_stat(CURRENT_CONNECTION, param, &details))
        {
            ftplog(LOG_ALERT, "%s does not exist\n", param);
            continue;
        }
        if (!(item = malloc(sizeof(struct qitem))))
        {
            perror("malloc");
            exit(1);
        }
        memset(item, 0, sizeof(struct qitem));
        memset(path, 0, 4096);
        memset(filename, 0, 4096);
        item->permissions = details.permissions;
        item->size = details.size;
        /* Split the path and filename from the paramater */
        index = strlen(param);
        while (index)
        {
            if (param[index] == '/')
            {
                strncpy(path, param, index);
                strncpy(filename, &param[index + 1], 4096);
                break;
            }
            index--;
        }
        if (strlen(filename) == 0) { strncpy(filename, param, 4096); }
        strncpy(item->localfilename, filename, 256);
        strncpy(item->remotefilename, filename, 256);
        item->direction = DIRECTION_GET;
        if (path[0] == '/')
        {
            strncpy(item->remotedir, path, 256);
        }
        else
        {
            sprintf(item->remotedir, "%s%s%s", CURRENT_CONNECTION->remotedir,
                    (CURRENT_CONNECTION->remotedir[strlen(CURRENT_CONNECTION->remotedir) - 1] != '/') ? "/" : "", path);
        }
        strncpy(item->localdir, CURRENT_CONNECTION->localdir, 256);
        if (item->remotedir[strlen(item->remotedir) - 1] == '/')
        {
            item->remotedir[strlen(item->remotedir) - 1] = '\0';
        }
        if (item->localdir[strlen(item->localdir) - 1] == '/')
        {
            item->localdir[strlen(item->localdir) - 1] = '\0';
        }
        item->remoteconnection = CURRENT_CONNECTION;
        item->localconnection = NULL;
        newitem = addtoqueue(&leftqueue, item);
        if (newitem) ftplog(LOG_INFO, "Queued %s/%s for download, position %d\n",
                                item->remotedir, item->localfilename, newitem->id);
        free(item);
    }
    if (config(variables, "queuefiles"))
    {
        if (strcasecmp(config(variables, "queuefiles"), "false") == 0)
        {
            cmd_go(expanded_line);
        }
    }
    free(expanded_line);
    return 1;
}

int cmd_put(char *command_line)
{
    struct qitem *item, *newitem;
    char *param, *expanded_line;
    char *pjpg = NULL, *pavi = NULL;
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "put: Not connected\n");
        return 0;
    }
    expanded_line = expand_wildcard(command_line);
    getnextword(expanded_line, GNW_START);
    while ((param = getnextword(expanded_line, GNW_NEXT)))
    {
        char path[4096], filename[4096];
        struct stat stat_buf;
        int index;
        if (!param || !*param) { continue; }
        if (strcmp(param, "End") == 0) { continue; }
        if (stat(param, &stat_buf) != 0)
            switch (errno)
            {
                case ENOENT   :
                    ftplog(LOG_ALERT, "File %s does not exist\n", param);
                    continue;
                    break;
                case EACCES   :
                    ftplog(LOG_ALERT, "Permission denied to stat\n.");
                    continue;
                    break;
                case ENOMEM   :
                    ftplog(LOG_ALERT, "Out of memory\n");
                    exit(1);
                    break;
                case ENAMETOOLONG :
                    ftplog(LOG_ALERT, "File name \"%s\" too long\n", param);
                    continue;
                    break;
                default       :
                    ftplog(LOG_ALERT, "Unknown error doing stat: %d\n", errno);
                    continue;
                    break;
            }
        if (!(item = malloc(sizeof(struct qitem))))
        {
            perror("malloc");
            exit(1);
        }
        memset(item, 0, sizeof(struct qitem));
        memset(path, 0, 4096);
        memset(filename, 0, 4096);
        /* Split the path and filename from the paramater */
        index = strlen(param);
        while (index)
        {
            if (param[index] == '/')
            {
                strncpy(path, param, index);
                strncpy(filename, &param[index + 1], 4096);
                break;
            }
            index--;
        }
        if (strlen(filename) == 0) { strncpy(filename, param, 4096); }
        strncpy(item->localfilename, filename, 256);
        pjpg = strstr(filename, ".jpg");
        pavi = strstr(filename, ".avi");
        if (pjpg)
        {
            if (strlen(pjpg) > 5)
            {
                strncpy(item->remotefilename, filename, (strlen(filename) - 8));
            }
            else
            {
                strncpy(item->remotefilename, filename, 256);
            }
        }
        else if (pavi)
        {
            if (strlen(pavi) > 5)
            {
                strncpy(item->remotefilename, filename, (strlen(filename) - 8));
            }
            else
            {
                strncpy(item->remotefilename, filename, 256);
            }
        }
        else
        {
            strncpy(item->remotefilename, filename, 256);
        }


        item->direction = DIRECTION_PUT;
        if (path[0] == '/')
        {
            strncpy(item->localdir, path, 256);
        }
        else
        {
            sprintf(item->localdir, "%s%s%s", CURRENT_CONNECTION->localdir,
                    (item->localdir[strlen(item->localdir)] != '/') ? "/" : "",
                    path);
        }
        strncpy(item->remotedir, CURRENT_CONNECTION->remotedir, 256);
        if (strlen(item->remotedir) > 1)
            if (item->remotedir[strlen(item->remotedir) - 1] == '/')
            {
                item->remotedir[strlen(item->remotedir) - 1] = '\0';
            }
        if (strlen(item->localdir) > 1)
            if (item->localdir[strlen(item->localdir) - 1] == '/')
            {
                item->localdir[strlen(item->localdir) - 1] = '\0';
            }
        item->remoteconnection = CURRENT_CONNECTION;
        item->localconnection = NULL;
        item->permissions = stat_buf.st_mode;
        item->size = stat_buf.st_size;
        newitem = addtoqueue(&leftqueue, item);
        if (newitem) ftplog(LOG_INFO, "Queued %s/%s for upload, position %d\n",
                                item->remotedir, param, newitem->id);
        free(item);
    }
    if (config(variables, "queuefiles"))
    {
        if (strcasecmp(config(variables, "queuefiles"), "false") == 0)
        {
            cmd_go(expanded_line);
        }
    }
    free(expanded_line);
    return 1;
}


int cmd_put_common(char *command_line, char *remotename)
{
    struct qitem *item, *newitem;
    char *param, *expanded_line;
    char *pjpg = NULL, *pavi = NULL;
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "put: Not connected\n");
        return 0;
    }
    expanded_line = expand_wildcard(command_line);
    getnextword(expanded_line, GNW_START);
    while ((param = getnextword(expanded_line, GNW_NEXT)))
    {
        char path[4096], filename[4096];
        struct stat stat_buf;
        int index;
        if (!param || !*param) { continue; }
        if (strcmp(param, "End") == 0) { continue; }
        if (stat(param, &stat_buf) != 0)
            switch (errno)
            {
                case ENOENT   :
                    ftplog(LOG_ALERT, "File %s does not exist\n", param);
                    continue;
                    break;
                case EACCES   :
                    ftplog(LOG_ALERT, "Permission denied to stat\n.");
                    continue;
                    break;
                case ENOMEM   :
                    ftplog(LOG_ALERT, "Out of memory\n");
                    exit(1);
                    break;
                case ENAMETOOLONG :
                    ftplog(LOG_ALERT, "File name \"%s\" too long\n", param);
                    continue;
                    break;
                default       :
                    ftplog(LOG_ALERT, "Unknown error doing stat: %d\n", errno);
                    continue;
                    break;
            }
        if (!(item = malloc(sizeof(struct qitem))))
        {
            perror("malloc");
            exit(1);
        }
        memset(item, 0, sizeof(struct qitem));
        memset(path, 0, 4096);
        memset(filename, 0, 4096);
        /* Split the path and filename from the paramater */
        index = strlen(param);
        while (index)
        {
            if (param[index] == '/')
            {
                strncpy(path, param, index);
                strncpy(filename, &param[index + 1], 4096);
                break;
            }
            index--;
        }
        if (strlen(filename) == 0) { strncpy(filename, param, 4096); }
        strncpy(item->localfilename, filename, 256);
        pjpg = strstr(filename, ".jpg");
        pavi = strstr(filename, ".avi");
        if (pjpg)
        {
            if (strlen(pjpg) > 5)
            {
                strncpy(item->remotefilename, filename, (strlen(filename) - 8));
            }
            else
            {
                strncpy(item->remotefilename, filename, 256);
            }
        }
        else if (pavi)
        {
            if (strlen(pavi) > 5)
            {
                strncpy(item->remotefilename, filename, (strlen(filename) - 8));
            }
            else
            {
                strncpy(item->remotefilename, filename, 256);
            }
        }
        else
        {
            strncpy(item->remotefilename, filename, 256);
        }


        item->direction = DIRECTION_PUT;
        if (path[0] == '/')
        {
            strncpy(item->localdir, path, 256);
        }
        else
        {
            sprintf(item->localdir, "%s%s%s", CURRENT_CONNECTION->localdir,
                    (item->localdir[strlen(item->localdir)] != '/') ? "/" : "",
                    path);
        }
        strncpy(item->remotedir, CURRENT_CONNECTION->remotedir, 256);
        if (strlen(item->remotedir) > 1)
            if (item->remotedir[strlen(item->remotedir) - 1] == '/')
            {
                item->remotedir[strlen(item->remotedir) - 1] = '\0';
            }
        if (strlen(item->localdir) > 1)
            if (item->localdir[strlen(item->localdir) - 1] == '/')
            {
                item->localdir[strlen(item->localdir) - 1] = '\0';
            }
        item->remoteconnection = CURRENT_CONNECTION;
        item->localconnection = NULL;
        item->permissions = stat_buf.st_mode;
        item->size = stat_buf.st_size;
        if (remotename) { strcpy(item->remotefilename, remotename); }
        newitem = addtoqueue(&leftqueue, item);
        if (newitem) ftplog(LOG_INFO, "Queued %s/%s for upload, position %d\n",
                                item->remotedir, param, newitem->id);
        free(item);
    }
    if (config(variables, "queuefiles"))
    {
        if (strcasecmp(config(variables, "queuefiles"), "false") == 0)
        {
            cmd_go(expanded_line);
        }
    }
    free(expanded_line);
    return 1;
}



int cmd_fxp(char *command_line)
{
    struct qitem *item, *newitem;
    char *param, *expanded_line;

    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "fxp: Not connected\n");
        return 0;
    }
    if (!getconnected(LAST_CONNECTION))
    {
        ftplog(LOG_ALERT, "fxp: Not connected\n");
        return 0;
    }
    expanded_line = expand_wildcard(command_line);
    getnextword(expanded_line, GNW_START);
    while ((param = getnextword(expanded_line, GNW_NEXT)))
    {
        char path[4096], filename[4096];
        int index;
        struct filedetails details;
        if (!param || !*param) { continue; }
        ftplog(LOG_NOTICE, "FxP'ing \"%s\" from %s to %s", param,
               CURRENT_CONNECTION->hostname,
               LAST_CONNECTION->hostname);
        if (!(item = malloc(sizeof(struct qitem))))
        {
            perror("malloc");
            exit(1);
        }
        if (ftp_stat(CURRENT_CONNECTION, param, &details))
        {
            ftplog(LOG_ALERT, "%s does not exist\n", param);
            free(item);
            continue;
        }
        memset(item, 0, sizeof(struct qitem));
        memset(path, 0, 4096);
        memset(filename, 0, 4096);
        /* Split the path and filename from the paramater */
        index = strlen(param);
        while (index)
        {
            if (param[index] == '/')
            {
                strncpy(path, param, index);
                strncpy(filename, &param[index + 1], 4096);
                break;
            }
            index--;
        }
        if (strlen(filename) == 0) { strncpy(filename, param, 4096); }
        strncpy(item->localfilename, param, 256);
        strncpy(item->remotefilename, param, 256);
        item->permissions = details.permissions;
        item->size = details.size;
        item->direction = DIRECTION_FXP;
        if (path[0] == '/')
        {
            strncpy(item->remotedir, path, 256);
        }
        else
        {
            sprintf(item->remotedir, "%s%s%s", LAST_CONNECTION->remotedir, (LAST_CONNECTION->remotedir[strlen(LAST_CONNECTION->remotedir) - 1] != '/') ? "/" : "", path);
        }
        strncpy(item->localdir, CURRENT_CONNECTION->remotedir, 256);
        if (item->remotedir[strlen(item->remotedir) - 1] == '/')
        {
            item->remotedir[strlen(item->remotedir) - 1] = '\0';
        }
        if (item->localdir[strlen(item->localdir) - 1] == '/')
        {
            item->localdir[strlen(item->localdir) - 1] = '\0';
        }
        item->remoteconnection = LAST_CONNECTION;
        item->localconnection = CURRENT_CONNECTION;
        newitem = addtoqueue(&leftqueue, item);
        if (newitem) ftplog(LOG_INFO, "Queued %s/%s for FxP, position %d\n",
                                item->remotedir, item->localfilename, newitem->id);
        free(item);
    }
    if (config(variables, "queuefiles"))
    {
        if (strcasecmp(config(variables, "queuefiles"), "false") == 0)
        {
            cmd_go(expanded_line);
        }
    }
    free(expanded_line);
    return 1;
}

int cmd_go(char *command_line)
{
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "go: Not connected\n");
        return 0;
    }
    leftqueue = runqueue(leftqueue);
    return 1;
}

int cmd_clear(char *command_line)
{
    clearqueue(leftqueue);
    leftqueue = NULL;
    ftplog(LOG_ALERT, "Queue cleared\n");
    return 1;
}

int cmd_queue(char *command_line)
{
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "queue: Not connected\n");
        return 0;
    }
    printqueue(leftqueue);
    return 1;
}

int cmd_rem(char *command_line)
{
    char *param;
    int id;
    getnextword(command_line, GNW_START);
    while ((param = getnextword(command_line, GNW_NEXT)))
    {
        id = atoi(param);
        if (id > 0)
        {
            if (delfromqueue(&leftqueue, id) == 0)
            {
                ftplog(LOG_WARNING, "Deleted item %d\n", id);
                return 1;
            }
            else
            {
                ftplog(LOG_ALERT, "Item %d doesn't exist!\n", id);
                return 0;
            }
        }
    }
    return 1;
}

int cmd_quit(char *command_line)
{
    //printf("\n");
    return -1;
}

int cmd_close(char *command_line)
{
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "close: Not connected\n");
        return 0;
    }
    if (leftqueue)
    {
        ftplog(LOG_ALERT, "You still have untransferred items\n");
        return 0;
    }
    ftp_disconnect(CURRENT_CONNECTION);
    CURRENT_CONNECTION->status = STATUS_WAIT_CONNECT;
    return 1;
}

int cmd_open(char *command_line)
{
    char *param;
    getnextword(command_line, GNW_START);
    if (!(param = getnextword(command_line, GNW_NEXT)))
    {
        ftplog(LOG_ALERT, "No address specified!\n");
        return 1;
    }
    if (!*param) { return 0; }
    if (!parseurl(param, CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "Can't open %s\n", param);
        return 0;
    }
    if (strlen(CURRENT_CONNECTION->password) == 0)
    {
        if (config(variables, "email"))
        {
            strncpy(CURRENT_CONNECTION->password, config(variables, "email"), 32);
        }
        else
        {
            strncpy(CURRENT_CONNECTION->password, "user@quftp.com", 32);
        }
    }
    if (CURRENT_CONNECTION->remotedir[0] != '\0') { getconnected(CURRENT_CONNECTION); }
    setconfig(&variables, "HOSTNAME", CURRENT_CONNECTION->hostname);
    return 1;
}

int cmd_user(char *command_line)
{
    char *param;
    getnextword(command_line, GNW_START);
    if (!(param = getnextword(command_line, GNW_NEXT)))
    {
        ftplog(LOG_ALERT, "No username specified!\n");
        return 0;
    }
    if (CURRENT_CONNECTION->status >= STATUS_IDLE)
    {
        ftp_disconnect(CURRENT_CONNECTION);
    }
    strncpy(CURRENT_CONNECTION->username, param, 32);
    if ((param = getnextword(command_line, GNW_NEXT)))
    {
        strncpy(CURRENT_CONNECTION->password, param, 32);
    }
    else
    {
        strncpy(CURRENT_CONNECTION->password, getpassword(CURRENT_CONNECTION->username), 32);
    }
    CURRENT_CONNECTION->status = STATUS_WAIT_CONNECT;
    return 1;
}

int cmd_stat(char *command_line)
{
    struct filedetails details;
    char *param;
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "stat: Not connected\n");
        return 0;
    }
    getnextword(command_line, GNW_START);
    while ((param = getnextword(command_line, GNW_NEXT)))
    {
        if (!*param) { continue; }
        if (ftp_stat(CURRENT_CONNECTION, param, &details) == 0)
        {
            ftplog(LOG_INFO, "Details for %s\n", details.filename);
            ftplog(LOG_INFO, "File Size: %lu\n", details.size);
            ftplog(LOG_INFO, "Number of Hard Links: %d\n", details.links);
            if (*details.owner) { ftplog(LOG_INFO, "Owner: %s\n", (char *)details.owner); }
            if (*details.group) { ftplog(LOG_INFO, "Group: %s\n", (char *)details.group); }
            if (details.permissions) { ftplog(LOG_INFO, "Permission: %o\n", (unsigned int)details.permissions); }
        }
    }
    return 1;
}

int cmd_status(char *command_line)
{
    int conn;
    ftplog(LOG_INFO, "Current connection: %s\n", config(variables, "CURRENT_CONNECTION"));
    ftplog(LOG_INFO, "\n");
    for (conn = 0; conn < MAX_CONNECTIONS; conn++)
    {
        if (connection[conn]->status == 0) { continue; }
        ftplog(LOG_INFO, "Connection %d:\n", conn);
        ftplog(LOG_INFO, "	Host: %s:%d\n", connection[conn]->hostname, connection[conn]->port);
        ftplog(LOG_INFO, "	Username: %s\n", connection[conn]->username);
        ftplog(LOG_INFO, "	Directory: %s\n", connection[conn]->remotedir);
        ftplog(LOG_INFO, "	Status: ");
        switch (connection[conn]->status)
        {
            case 0 :
                ftplog(LOG_INFO, "No connection\n");
                break;
            case 1 :
                ftplog(LOG_INFO, "Ready to connect\n");
                break;
            case 2 :
                ftplog(LOG_INFO, "Waiting for login\n");
                break;
            case 3 :
                ftplog(LOG_INFO, "Idle\n");
                break;
            case 4 :
                ftplog(LOG_INFO, "Waiting...\n");
                break;
            case 5 :
                ftplog(LOG_INFO, "Busy transferring\n");
                break;
            case 99 :
                ftplog(LOG_INFO, "Error!\n");
                break;
            default :
                ftplog(LOG_INFO, "Unknown!\n");
                break;
        }
    }
    return 1;
}

int cmd_nlst(char *command_line)
{
    char *list[200];
    char *params = NULL;
    register int index = 0;
    getnextword(command_line, GNW_START);
    params = getnextword(command_line, GNW_REST);
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "nlst: Not connected\n");
        return 0;
    }
    if (ftp_nlst(CURRENT_CONNECTION, params, list, 200) < 0) { return 0; }
    for (index = 0; list[index]; index++)
    {
        printf("%s\n", list[index]);
        free(list[index]);
    }
    return 1;
}

int cmd_lls(char *command_line)
{
    char *param = command_line;
    param++;
    system(param);
    return 1;
}

int cmd_cat(char *command_line)
{
    char *param = (char *)NULL;
    char *expanded_line;
    expanded_line = expand_wildcard(command_line);
    param = getnextword(expanded_line, GNW_START);
    while ((param = getnextword(expanded_line, GNW_NEXT)))
    {
        char *buffer;
        int n;
        buffer = (char *)malloc(4096);
        sprintf(buffer, "%s", param);
        if (ftp_read_file(CURRENT_CONNECTION, buffer, 4095, TRANS_START) < 0)
        {
            free(expanded_line);
            return 0;
        }
        while ((n = ftp_read_file(CURRENT_CONNECTION, buffer, 4095, TRANS_TRANS)) > 0)
        {
            buffer[4095] = '\0';
            printf("%s", buffer);
        }
        ftp_read_file(CURRENT_CONNECTION, buffer, 4095, TRANS_CLOSE);
        free(buffer);
    }
    free(expanded_line);
    return 1;
}

int cmd_more(char *command_line)
{
    char *param = (char *)NULL;
    char *expanded_line;
    FILE *fh;
    expanded_line = expand_wildcard(command_line);
    param = getnextword(expanded_line, GNW_START);
    while ((param = getnextword(expanded_line, GNW_NEXT)))
    {
        char *pager;
        char *buffer;
        int n;
        if (ftp_read_file(CURRENT_CONNECTION, param, 4095, TRANS_START) < 0)
        {
            free(expanded_line);
            return 0;
        }
        pager = config(variables, "pager");
        if (!pager) { pager = strdup("/bin/more"); }
        fh = popen(pager, "w");
        if (!fh)
        {
            perror("popen");
            ftp_read_file(CURRENT_CONNECTION, NULL, 0, TRANS_CLOSE);
            free(expanded_line);
            return 0;
        }
        buffer = (char *)malloc(4096);
        while ((n = ftp_read_file(CURRENT_CONNECTION, buffer, 4095, TRANS_TRANS)) > 0)
        {
            buffer[4095] = '\0';
            fputs(buffer, fh);
        }
        ftp_read_file(CURRENT_CONNECTION, buffer, 4095, TRANS_CLOSE);
        free(buffer);
        pclose(fh);
    }
    free(expanded_line);
    return 1;
}

int cmd_set(char *command_line)
{
    char *key = NULL, *value;
    value = getnextword(command_line, GNW_START);
    value = getnextword(command_line, GNW_NEXT);
    if (value)
    {
        key = strdup(value);
        value = getnextword(command_line, GNW_NEXT);
        if (value)
        {
            setconfig(&variables, strlower(key), value);
        }
        else
        {
            char *data = config(variables, key);
            if (data) { printf("%s: %s\n", key, data); }
            else { ftplog(LOG_ALERT, "%s does not exist\n", key); }
        }
    }
    else
    {
        struct variable_list *list = variables;
        while (list)
        {
            printf("%s: %s\n", list->key, list->value);
            list = list->next;
        }
    }
    if (key) { free(key); }
    return 1;
}

int cmd_unset(char *command_line)
{
    char *key = NULL;
    getnextword(command_line, GNW_START);
    key = strlower(getnextword(command_line, GNW_NEXT));
    if (key)
    {
        setconfig(&variables, key, NULL);
        free(key);
        return 1;
    }
    return 0;
}

int cmd_assert(char *command_line)
{
    char *result;
    result = config(variables, "SUCCESSFUL_OPERATION");
    if (result && strcmp(result, "false") == 0)
    {
        ftplog(LOG_CRITICAL, "Assertion failed... quitting\n");
        return -1;
    }
    return 1;
}

char *parseline(char *line)   /* Expand variables */
{
    int index = 0;
    char *linestart = line;
    char *temp;
    temp = (char *)malloc(4096);
    memset(temp, 0, 4096);
    while (*line)
    {
        if (*line == '$')       // Expand variables
        {
            char *endbit;
            line++;
            endbit = line;
            while (endbit && *endbit && *endbit != ' ') { endbit++; }
            *endbit = '\0';
            if (config(variables, line)) { strcat((temp + index), config(variables, line)); }
            index += (endbit - line);
            line = endbit + 1;
        }
        else
        {
            temp[index] = *line;
        }
        line++;
        if (line > (linestart + strlen(linestart))) { break; }
        index++;
    }
    return temp;
}

int cmd_dele(char *command_line)
{
    char *filename, *expanded_line;
    expanded_line = expand_wildcard(command_line);
    getnextword(expanded_line, GNW_START);
    while ((filename = getnextword(expanded_line, GNW_NEXT)))
    {
        ftp_delete(CURRENT_CONNECTION, filename);
    }
    free(expanded_line);
    return 1;
}

int cmd_alias(char *command_line)
{
    struct alias_list *item = aliases, *last = aliases;
    char *alias = NULL, *expanded = NULL;
    getnextword(command_line, GNW_START);
    expanded = getnextword(command_line, GNW_NEXT);
    if (expanded)
    {
        alias = strdup(expanded);
    }
    expanded = getnextword(command_line, GNW_REST);
    if (!alias || !*alias)
    {
        if (!item)
        {
            ftplog(LOG_ALERT, "No aliases have been defined\n");
            return 1;
        }
        printf("%-20s%s\n", "Alias", "Expands to");
        while (item)
        {
            printf("%-20s%s\n", item->alias, item->expanded);
            item = item->next;
        }
        printf("\n");
        return 1;
    }
    if (!expanded || !*expanded)
    {
        while (item)
        {
            if (strcasecmp(item->alias, alias) == 0)
            {
                free(item->expanded);
                free(item->alias);
                last->next = item->next;
                if (item == aliases) { aliases = NULL; }
                free(item);
                return 1;
            }
            last = item;
            item = item->next;
        }
        ftplog(LOG_ALERT, "Can't find \"%s\"\n", alias);
        return 0;
    }
    while (item)
    {
        if (strcasecmp(item->alias, alias) == 0)
        {
            free(item->expanded);
            item->expanded = strdup(expanded);
            item->next = NULL;
            return 1;
        }
        last = item;
        item = item->next;
    }
    item = (struct alias_list *)malloc(sizeof(struct alias_list));
    if (!aliases) { aliases = item; }
    if (last) { last->next = item; }
    item->alias = strdup(alias);
    item->expanded = strdup(expanded);
    item->next = NULL;
    return 1;
}

char *expand_wildcard(char *inputstring)
{
    char *word, *tempstring, *retstring;

    if (!inputstring) { return NULL; }
    if ((strstr(inputstring, "*") == 0) && (strstr(inputstring, "?")) == 0)
    {
        return strdup(inputstring);
    }
    tempstring = (char *)malloc(65535);
    sprintf(tempstring, "%s ", getnextword(inputstring, GNW_START));
    if (strcasecmp(tempstring, "get ") == 0 || strcasecmp(tempstring, "fxp ") == 0)
    {
        while ((word = getnextword(inputstring, GNW_NEXT)))
        {
            int index = 0;
            char *temp;
            while ((temp = ftp_glob(CURRENT_CONNECTION, word, index++)))
            {
                sprintf(tempstring, "%s %s", tempstring, temp);
            }
        }
        retstring = strdup(tempstring);
        free(tempstring);
        return retstring;
    }
    else if (strcasecmp(tempstring, "put ") == 0)
    {
        glob_t globvec;
        sprintf(tempstring, "%s ", getnextword(inputstring, GNW_START));
        while ((word = getnextword(inputstring, GNW_NEXT)))
        {
            int result, index;
            memset(&globvec, 0, sizeof(glob_t));
            result = glob(word, 1, NULL, &globvec); //GLOB_ONLYDIR   //
            switch (result)
            {
                case 0:
                    for (index = 0; index < globvec.gl_pathc; index++)
                        sprintf(tempstring, "%s %s", tempstring,
                                globvec.gl_pathv[index]);
                    globfree(&globvec);
                    break;
                case GLOB_NOSPACE:
                    ftplog(LOG_CRITICAL, "Ran out of memory for glob\n");
                    free(tempstring);
                    return strdup(inputstring);
                case GLOB_ABORTED:
                    ftplog(LOG_CRITICAL, "Glob Read error\n");
                    free(tempstring);
                    return strdup(inputstring);
                case GLOB_NOMATCH:
                    ftplog(LOG_ALERT, "Glob found no matches\n");
                    free(tempstring);
                    return strdup(inputstring);
                default :
                    ftplog(LOG_ALERT, "Unknown glob error %d\n", result);
                    break;
            }
        }
        retstring = strdup(tempstring);
        free(tempstring);
        return retstring;
    }
    else
    {
        free(tempstring);
        return strdup(inputstring);
    }
}

char *expand_alias(char *string)
{
    struct alias_list *item;
    char *returnstring = (char *)NULL;
    char firstword[500], *rest;
    if (!string || !*string) { return string; }
    memset(firstword, 0, 500);
    strncpy(firstword, getnextword(string, GNW_START), 500);
    rest = getnextword(string, GNW_REST);
    item = aliases;
    while (item)
    {
        if (strcasecmp(item->alias, firstword) == 0)
        {
            returnstring = (char *)malloc(strlen(item->expanded) + strlen(rest) + 2);
            sprintf(returnstring, "%s %s", item->expanded, rest);
            return returnstring;
        }
        item = item->next;
    }
    return strdup(string);
}

char *print_permissions(int permissions)
{
    char *returnstring;
    returnstring = (char *)malloc(11);
    memset(returnstring, '-', 10);
    if ((permissions & 512) == 512) { returnstring[0] = 'd'; }
    if ((permissions & 256) == 256) { returnstring[1] = 'r'; }
    if ((permissions & 128) == 128) { returnstring[2] = 'w'; }
    if ((permissions & 64) == 64) { returnstring[3] = 'x'; }
    if ((permissions & 32) == 32) { returnstring[4] = 'r'; }
    if ((permissions & 16) == 16) { returnstring[5] = 'w'; }
    if ((permissions & 8) == 8) { returnstring[6] = 'x'; }
    if ((permissions & 4) == 4) { returnstring[7] = 'r'; }
    if ((permissions & 2) == 2) { returnstring[8] = 'w'; }
    if ((permissions & 1) == 1) { returnstring[9] = 'x'; }
    return returnstring;
}

int progress_bar(unsigned long current, unsigned long max, double speed)
{
    unsigned long etasec;
    int index, width, count = 0;
    double percent;
    static double oldpercent = -1;
    static double speeds[SPEEDS_AVERAGE];
    static int speedindex = 0;
    char *temp;

    /* Don't display a progress bar for a 0 byte file */
    if (max == 0)
    {
        return 0;
    }

    /* If a new progress bar, clear the speed history */
    if (current == 0)
    {
        for (index = 0; index < SPEEDS_AVERAGE; index++) { speeds[index] = 0.0; }
        speedindex = 0;
    }

    /* Record current speed */
    speeds[++speedindex % SPEEDS_AVERAGE] = speed;

    /* Get current average speed */
    for (index = 0; index < SPEEDS_AVERAGE; index++)
    {
        if (speeds[index] == 0.0) { continue; }
        speed += speeds[index];
        count++;
    }
    speed /= (count * 1.0);
    percent = (current * 1.0) / (max * 1.0);

    /* No movement? Return without displaying anything */
    if (percent == oldpercent) { return 0; }

    etasec = ((max - current) / 1024.00) / speed;
    if ((temp = config(variables, "screenwidth"))) { width = atoi(temp); }
    else if ((temp = getenv("COLUMNS"))) { width = atoi(temp); }
    else { width = 80; }
    width -= 20;
    for (index = 0; index <= (width * percent); index++) { printf("#"); }
    for (index = (width * percent) + 1; index <= width; index++) { printf("-"); }
    printf(" %-6.6s %3.1fKb/s", timestring(etasec), speed);
    printf("\r");
    if (current == max)
    {
        for (index = 0; index < (width + 20); index++) { printf(" "); }
        printf("\r");
    }
    oldpercent = percent;
    return 0;
}

char *timestring(unsigned long seconds)
{
    char *returnstring;
    returnstring = (char *)malloc(15);
    sprintf(returnstring, "%02lu:%02lu", seconds / 60, seconds % 60);
    return returnstring;
}

int cmd_getlist(char *command_line)
{
    char *infile = NULL;
    char line[MAX_LINE_SIZE];
    FILE *fh;
    getnextword(command_line, GNW_START);
    infile = getnextword(command_line, GNW_NEXT);
    fh = fopen(infile, "r");
    if (!fh)
    {
        ftplog(LOG_CRITICAL, "Can't open file %s\n", infile);
        return 0;
    }
    while (fgets(line, MAX_LINE_SIZE, fh))
    {
        char path[MAX_LINE_SIZE], filename[MAX_LINE_SIZE], *param;
        int index;
        int result;
        struct qitem *item = NULL, *newitem;
        struct filedetails details;
        param = strchr(line, '\n');
        *param = '\0';
        if (line[0] == 0) { continue; }
        param = getnextword(line, GNW_START);
        result = ftp_stat(CURRENT_CONNECTION, param, &details);
        switch (result)
        {
            case 211 :
            case 212 :
                ftplog(LOG_ALERT, "%s does not exist\n", param);
                continue;
                break;
            case 500 :
            case 501 :
            case 502 :
                break;
            case 421 :
            default  :
                continue;
                break;
        }
        item = (struct qitem *)malloc(sizeof(struct qitem));
        memset(item, 0, sizeof(struct qitem));
        memset(path, 0, MAX_LINE_SIZE);
        memset(filename, 0, MAX_LINE_SIZE);
        /* Split the path and filename from the paramater */
        index = strlen(param);
        while (index)
        {
            if (param[index] == '/')
            {
                strncpy(path, param, index);
                strncpy(filename, &param[index + 1], MAX_LINE_SIZE);
                break;
            }
            index--;
        }
        if (filename[0] == 0) { strncpy(filename, param, MAX_LINE_SIZE); }
        strncpy(item->localfilename, filename, 256);
        strncpy(item->remotefilename, filename, 256);
        item->permissions = details.permissions;
        item->size = details.size;
        item->direction = DIRECTION_GET;
        if (path[0] == '/')
        {
            strncpy(item->remotedir, path, 256);
        }
        else
        {
            sprintf(item->remotedir, "%s%s%s", CURRENT_CONNECTION->remotedir,
                    (CURRENT_CONNECTION->remotedir[strlen(CURRENT_CONNECTION->remotedir) - 1] != '/') ? "/" : "",
                    path);
        }
        strncpy(item->localdir, CURRENT_CONNECTION->localdir, 256);
        if (item->remotedir[strlen(item->remotedir) - 1] == '/')
        {
            item->remotedir[strlen(item->remotedir) - 1] = '\0';
        }
        if (item->localdir[strlen(item->localdir) - 1] == '/')
        {
            item->localdir[strlen(item->localdir) - 1] = '\0';
        }
        item->remoteconnection = CURRENT_CONNECTION;
        item->localconnection = NULL;
        newitem = addtoqueue(&leftqueue, item);
        if (newitem) ftplog(LOG_INFO, "Queued %s/%s for download, position %d\n",
                                item->remotedir, item->localfilename, newitem->id);
        free(item);
    }
    fclose(fh);
    return 1;
}

int cmd_quote(char *command_line)
{
    char command[4096];
    sprintf(command, "%s\r\n", getnextword(command_line, GNW_REST));
    ftp_sendline(CURRENT_CONNECTION, command);
    ftp_getrc(CURRENT_CONNECTION, NULL, 0, 1);
    return 1;
}

int cmd_mkdir(char *command_line)
{
    char *param;
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "mkdir: Not connected\n");
        return 0;
    }
    getnextword(command_line, GNW_START);
    param = getnextword(command_line, GNW_NEXT);
    if (!param || !*param)
    {
        ftplog(LOG_ALERT, "Must specify a directory to create\n");
        return 0;
    }
    else
    {
        if (!ftp_mkd(CURRENT_CONNECTION, param)) { return 0; }
        else ftplog(LOG_INFO, "Created directory %s on %s in %s\n", param, CURRENT_CONNECTION->hostname,
                        CURRENT_CONNECTION->remotedir);
    }
    return 1;
}

int cmd_rmdir(char *command_line)
{
    char *param;
    if (!getconnected(CURRENT_CONNECTION))
    {
        ftplog(LOG_ALERT, "rmdir: Not connected\n");
        return 0;
    }
    getnextword(command_line, GNW_START);
    param = getnextword(command_line, GNW_NEXT);
    if (!param || !*param)
    {
        ftplog(LOG_ALERT, "Must specify a directory to remove\n");
        return 0;
    }
    else
    {
        if (!ftp_rmd(CURRENT_CONNECTION, param)) { return 0; }
        else ftplog(LOG_INFO, "Removed directory %s on %s in %s\n", param, CURRENT_CONNECTION->hostname,
                        CURRENT_CONNECTION->remotedir);
    }
    return 1;
}


int calcTimetoDate(struct timeval *sysTime, struct tm *tmnow)
{
    tmnow->tm_year = ((sysTime->tv_sec & 0xFFFF0000) >> 16);
    tmnow->tm_mon = ((sysTime->tv_sec & 0x0000FF00) >> 8);
    tmnow->tm_mday = ((sysTime->tv_sec & 0x000000FF));
    tmnow->tm_hour = ((sysTime->tv_usec & 0xFE000000) >> 25);
    tmnow->tm_min  = ((sysTime->tv_usec & 0x01FC0000) >> 18);
    tmnow->tm_sec  = ((sysTime->tv_usec & 0x0003F800) >> 11);
    return 0;
}


/*
***********************************************************
*           iTarge Ftp 读取上传文件的信息
*           解析出文件的生成时间
*           解析出文件的触发类型
*           主要是读取文件的扩展头信息
*           ReadFileIfo
*           真确解析返回0    异常返回1
*           2011-01-05  for barrett
***********************************************************
*/
typedef struct AviFileInfo
{
    struct timeval    systime;            //系统当前时间
    int             dirNum;             //目录分层数量
    char            dirpath[8][64];     //最多8级子目录图片存储路径 数值内容和DSP中的相同，但是结构体的长度不同（不能直接memcpy 的方式赋值）
    char            filename[128];      //file name
    int             resv[16];           //保留字段
} AviFileInfo;


#define JPG_OFFSET 21
#define AVI_HDR_OFFSET 304
int ReadFileInfo(char *strFileName, char *filedate, char *filetimeHour, char *fileType, ITE_Alg_FileInfo *pdir)
{
    FILE *JpgFp = NULL;
    FILE *AviFp = NULL;
    FILE *mp4fd = NULL;
    char head[24];
    int headoffset = 0, loopi = 0;
    struct timeval sysDate;
    struct tm *tmnow_jpg;
    struct tm *dateTm;
    //printf("read %s ext info\n",strFileName);
    ExtJfif JpgJfifInfo;
    AviFileInfo AviHdr;
    OS_S32 mp4_offset = sizeof(ExtJfif) + sizeof(ITE_ALG_ExtJfif);
    ITE_ALG_ExtJfif *jpgExt2Jfif  = (ITE_ALG_ExtJfif *)malloc(sizeof(ITE_ALG_ExtJfif));
    if (jpgExt2Jfif == NULL)
    {
        return -1;
    }
    if (strstr(strFileName, "yuv"))
    {
        gettimeofday(&sysDate, NULL); //若解析文件扩展头中的时间错误，则取系统时间
        tmnow_jpg = localtime(&sysDate.tv_sec);
        sprintf(filedate, "%04d-%02d-%02d", tmnow_jpg->tm_year + 1900, tmnow_jpg->tm_mon + 1, tmnow_jpg->tm_mday);
        sprintf(filetimeHour, "%02d", tmnow_jpg->tm_hour);
        return -1;

    }
    else if (strstr(strFileName, "jpg"))
    {
        JpgFp = fopen(strFileName, "r");
        if (JpgFp)
        {
            fread(head, 2, 1, JpgFp);
           // headoffset = (head[4] << 8 | head[5]) + 4;
           // fread(head + 6, headoffset - 6, 1, JpgFp);
            fread(&JpgJfifInfo, sizeof(ExtJfif), 1, JpgFp);
            if (JpgJfifInfo.Header[0] != 0xFF || JpgJfifInfo.Header[1] != 0xE1)
            {
                printf("read ext1 error \n");
                gettimeofday(&sysDate, NULL); //若解析文件扩展头中的时间错误，则取系统时间
                tmnow_jpg = localtime(&sysDate.tv_sec);
                sprintf(filedate, "%04d-%02d-%02d", tmnow_jpg->tm_year + 1900, tmnow_jpg->tm_mon + 1, tmnow_jpg->tm_mday);
                sprintf(filetimeHour, "%02d", tmnow_jpg->tm_hour);
                fclose(JpgFp);

                goto end;
            }
            fread(jpgExt2Jfif, sizeof(ITE_ALG_ExtJfif), 1, JpgFp);
            memcpy(pdir, &jpgExt2Jfif->PublicInfo.file, sizeof(ITE_Alg_FileInfo));
           // calcTimetoDate(&JpgJfifInfo.ExpoTime, &dateTm);
            gettimeofday(&sysDate, NULL); //若解析文件扩展头中的时间错误，则取系统时间
            dateTm = localtime(&sysDate.tv_sec);

            sprintf(filedate, "%04d-%02d-%02d", dateTm->tm_year+ 1900, dateTm->tm_mon+1, dateTm->tm_mday);
            sprintf(filetimeHour, "%02d", dateTm->tm_hour);

            goto end;
        }
    }
    else if (strstr(strFileName, ".avi"))
    {
        AviFp = fopen(strFileName, "r");
        if (AviFp)
        {
            fseek(AviFp, AVI_HDR_OFFSET, SEEK_SET);
            memset(&AviHdr, 0x0, sizeof(AviFileInfo));
            fread(&AviHdr, sizeof(AviFileInfo), 1, AviFp);
            pdir->dirNum = AviHdr.dirNum;
            for (loopi = 0; loopi < pdir->dirNum; loopi++)
            {
                strcpy((char*)pdir->dirpath[loopi], AviHdr.dirpath[loopi]);
            }
            printf("avi file:%s  dir num :%d  dir[2] %s \n", pdir->pfilename, pdir->dirNum, pdir->dirpath[1]);

            tmnow_jpg = localtime(&AviHdr.systime.tv_sec);
            sprintf(filedate, "%04d-%02d-%02d", tmnow_jpg->tm_year + 1900, tmnow_jpg->tm_mon + 1, tmnow_jpg->tm_mday);
            sprintf(filetimeHour, "%02d", tmnow_jpg->tm_hour);
            
        }
    }
    else if (strstr(strFileName, ".mp4"))
    {
        
        
        mp4fd = fopen(strFileName, "r");
        if(mp4fd)
        {
            fseek(mp4fd, -1L*mp4_offset, SEEK_END);
            fread(&JpgJfifInfo, 1, sizeof(ExtJfif), mp4fd);

            if (JpgJfifInfo.Header[0] != 0xFF || JpgJfifInfo.Header[1] != 0xE1)
            {
                printf("mp4 read ext1 error Header[%x %x %x %x] \n",JpgJfifInfo.Header[0],JpgJfifInfo.Header[1],JpgJfifInfo.Header[2],JpgJfifInfo.Header[3]);
                gettimeofday(&sysDate, NULL); //若解析文件扩展头中的时间错误，则取系统时间
                tmnow_jpg = localtime(&sysDate.tv_sec);
                sprintf(filedate, "%04d-%02d-%02d", tmnow_jpg->tm_year + 1900, tmnow_jpg->tm_mon + 1, tmnow_jpg->tm_mday);
                sprintf(filetimeHour, "%02d", tmnow_jpg->tm_hour);

                goto end;
            }
            
            fread(jpgExt2Jfif, 1, sizeof(ITE_ALG_ExtJfif), mp4fd);
            memcpy(pdir, &jpgExt2Jfif->PublicInfo.file, sizeof(ITE_Alg_FileInfo));

            // calcTimetoDate(&JpgJfifInfo.ExpoTime, &dateTm);
            gettimeofday(&sysDate, NULL); //若解析文件扩展头中的时间错误，则取系统时间
            dateTm = localtime(&sysDate.tv_sec);
            
            sprintf(filedate, "%04d-%02d-%02d", dateTm->tm_year+ 1900, dateTm->tm_mon+1, dateTm->tm_mday);
            sprintf(filetimeHour, "%02d", dateTm->tm_hour);

        }
    }
    else
    {
        gettimeofday(&sysDate, NULL); //若解析文件扩展头中的时间错误，则取系统时间
        tmnow_jpg = localtime(&sysDate.tv_sec);
        sprintf(filedate, "%04d-%02d-%02d", tmnow_jpg->tm_year + 1900, tmnow_jpg->tm_mon + 1, tmnow_jpg->tm_mday);
        sprintf(filetimeHour, "%02d", tmnow_jpg->tm_hour);
        goto end;
    }

end:

    if(AviFp != NULL)
    {
        fclose(AviFp);
    } 

    if(JpgFp != NULL)
    {
        fclose(JpgFp);
    }

    if(mp4fd != NULL)
    {
        fclose(mp4fd);
        mp4fd = NULL;
    }
        
    if(jpgExt2Jfif != NULL)
    {
        free(jpgExt2Jfif);
    }
    
    return 0;
}

int DirProcess(ITE_Alg_FileInfo *pdir)
{
    int loopdir = 0;
    char cmdLine[1024];
    //printf("pdir->dirNum = %d\n",pdir->dirNum);
    if (pdir->dirNum > 0 && pdir->dirNum < 9)
    {
        for (loopdir = 0; loopdir < pdir->dirNum; loopdir++)
        {
            if (strlen((char *)(pdir->dirpath[loopdir])) > 0)
            {
                memset(cmdLine, 0x0, 1024);
                //printf("dir path %s \n",pdir->dirpath[loopdir]);
                sprintf(cmdLine, "cd %s", pdir->dirpath[loopdir]);
                if (cmd_cd(cmdLine) == 0)
                {
                    sprintf(cmdLine, "mkdir %s", pdir->dirpath[loopdir]);
                    if (cmd_mkdir(cmdLine) == 0)
                    {
                        cmd_quit("");
                        return 1;
                    }
                    else
                    {
                        sprintf(cmdLine, "cd %s", pdir->dirpath[loopdir]);
                        if (cmd_cd(cmdLine) == 0)
                        {
                            return 1;
                        }
                    }
                }

            }
            else
            {
                break;
            }
        }
    }
    else
    {
        printf(" dir mun  %d \n", pdir->dirNum);
        return -1;
    }
    return 0;
}
/*
**********************************************************************
*                   iTarge
*                   登陆ftp 服务器
*                   成功返回0，失败返回1
*                   ITG_logInServer
**********************************************************************
*/
int ITG_logInServer(char *FtpServerIP, char *User, char *Passwd, char *dir)
{
    char cmdLine[256];
    char *pdir = NULL;
    sprintf(cmdLine, "open %s\n", FtpServerIP);
    if (cmd_open(cmdLine) == 0)
    {
        return 1;
    }

    sprintf(cmdLine, "user %s %s", User, Passwd);
    if (cmd_user(cmdLine) == 0)
    {
        return 1;
    }
    if (strlen(dir) > 0)
    {
        if ((strlen(dir) == 1) && (dir[0] == '/')) { return 0; }

        if (dir[0] == '/') // 防止部分FTP 服务器根目录不是/ 的问题
        {
            pdir = dir + 1;
        }
        else
        {
            pdir = dir;
        }

        sprintf(cmdLine, "cd %s", pdir);
        if (cmd_cd(cmdLine) == 0)
        {
            sprintf(cmdLine, "mkdir %s", pdir);
            if (cmd_mkdir(cmdLine) == 0)
            {
                cmd_quit("");
                return 1;
            }
            else
            {
                sprintf(cmdLine, "cd %s", pdir);
                if (cmd_cd(cmdLine) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;

}


int cmd_X()
{
    char serverIP[64];
    char user[32];
    char password[32];
    char folderName[64];
    memset(user, 0x0, 32);
    memset(password, 0x0, 32);
    memset(folderName, 0x0, 64);
    memset(serverIP, 0x0, 64);

    if (pFtpserver == NULL)
    {
        printf("ftp share mem error\n");
        exit(-1);
    }
    strcpy(serverIP, pFtpserver->FtpParam.stFTPParam.FTPAddr);
    strcpy(user, pFtpserver->FtpParam.stFTPParam.szUserName);
    strcpy(password, pFtpserver->FtpParam.stFTPParam.szPassword);
    strcpy(folderName, pFtpserver->FtpParam.stFTPParam.szFolderPath);

    if (ITG_logInServer(serverIP, user, password, folderName) == 1)
    {
        printf("log in server error !! server :%s user :%s passwd :%s dir :%s \n", serverIP, user, password, folderName);
        return -1;
    }
    else
    {
        return 0;
    }

}




int cmd_processXmlFile(char *pJpgFile)
{
    char xmlfile[1024];
    char cmdLine[1024];
    memset(xmlfile, 0x0, 1024);
    memset(cmdLine, 0x0, 1024);
    memcpy(xmlfile, pJpgFile, (strlen(pJpgFile) - 4));
    strcat(xmlfile, ".xml");
    if (access(xmlfile, 0) == 0)
    {
        memset(cmdLine, 0x0, 1024);
        sprintf(cmdLine, "put %s", xmlfile);
        cmd_put(cmdLine);
        return 0;
    }
    return -1;
}
int cmd_processIniFile(char *pJpgFile)
{
    char inifile[1024];
    char cmdLine[1024];
    memset(inifile, 0x0, 1024);
    memset(cmdLine, 0x0, 1024);
    memcpy(inifile, pJpgFile, (strlen(pJpgFile) - 4));
    strcat(inifile, ".ini");
    if (access(inifile, 0) == 0)
    {
        memset(cmdLine, 0x0, 1024);
        sprintf(cmdLine, "put %s", inifile);
        cmd_put(cmdLine);
        return 0;
    }
    return -1;
}
int FindFilename(char *srcfile, char *dstfile)
{
    int loopi = 0, len = 0;
    int ret = -1;
    char tmp[1024];
    memset(tmp, 0x0, 1024);
    len = strlen(srcfile);
    memcpy(tmp, srcfile, len);
    for (loopi = len; loopi > 0; loopi--)
    {
        if (tmp[loopi] == '/')
        {
            memcpy(dstfile, tmp + loopi, (len - loopi));
            ret = 0;
            break;
        }

    }
    return ret;
}

#if 0
int cmd_processAviFile(char *pJpgFile)
{
    int ret = -1;
    int wait = 0;
    char avifile[1024];
    char cmdLine[1024];
    memset(avifile, 0x0, 1024);
    memset(cmdLine, 0x0, 1024);
    ret = FindFilename(pJpgFile, cmdLine);
    if (ret == 0)
    {
        memcpy(avifile, HDD_PRE_VIDEO_DIR, strlen(HDD_MOUNT_DIR));
        memcpy(avifile + strlen(HDD_PRE_VIDEO_DIR), cmdLine, (strlen(cmdLine) - 4));
        strcat(avifile, ".avi");
        //printf("avi filename process %s \n",avifile);
        while (access(avifile, 0) != 0)
        {
            if (wait > 100)
            {
                printf("not find avi file\n");
                break;
            }
            wait++;
            usleep(50000);
        }
        if (access(avifile, 0) == 0)
        {
            memset(cmdLine, 0x0, 1024);
            sprintf(cmdLine, "put %s", avifile);
            cmd_put(cmdLine);
            return 0;
        }
    }
    return ret;
}
#endif

/*
***********************************************************
*           iTarge Ftp cmd for transfer file to server
*           suport mkdir  del error file
*           创建IP   地址子目录可配置
*           创建日期子目录可配置
*           创建违法类型子目录可配置
*           cmd_upload
*           command_line_File  要上传文件的绝对路径
*           2011-01-05  for barrett
***********************************************************
*/
int cmd_upload(char *command_line_File)
{
    char Itg_IP[32];
    char Itg_date[32];
    char Itg_Hour[4];
    char Itg_Type[32];
    char Itg_uid[32] = {0};

    char serverIP[64];
    char user[32];
    char password[32];
    char folderName[64];
    char cmdLine[1024];
    char filename[1024] = {0};
    struct in_addr Inaddr;
    ITE_Alg_FileInfo upload_server_dir;
    Inaddr.s_addr = (net_get_ifaddr(ETH_NAME_1) == (-1) ? net_get_ifaddr(ETH_NAME_0) : net_get_ifaddr(ETH_NAME_1));
    memset(Itg_IP, 0x0, 32);
    memset(Itg_date, 0x0, 32);
    memset(Itg_uid, 0x0, 32);

    memset(Itg_Hour, 0x0, 4);
    memset(Itg_Type, 0x0, 32);
    memset(user, 0x0, 32);
    memset(password, 0x0, 32);
    memset(folderName, 0x0, 64);
    memset(serverIP, 0x0, 64);

    strcpy(serverIP, pFtpserver->FtpParam.stFTPParam.FTPAddr);
    strcpy(user, pFtpserver->FtpParam.stFTPParam.szUserName);
    strcpy(password, pFtpserver->FtpParam.stFTPParam.szPassword);
    strcpy(folderName, pFtpserver->FtpParam.stFTPParam.szFolderPath);

#if 0
    strcpy(serverIP, "192.168.3.13");
    strcpy(user, "jpg");
    strcpy(password, "123456");
    strcpy(folderName, "ydf");
#endif
    //printf("share  server :%s user :%s passwd :%s dir :%s \n",serverIP,user,password,folderName);

    if (ITG_logInServer(serverIP, user, password, folderName) == 1)
    {
        ITE_LOG(ITE_LOG_INFO, "log in server error !! server :%s user :%s passwd :%s dir :%s \n", serverIP, user, password, folderName);
        return -1;
    }
    memset(&upload_server_dir, 0x0, sizeof(ITE_Alg_FileInfo));
    ReadFileInfo(command_line_File, Itg_date, Itg_Hour, Itg_Type, &upload_server_dir);
    FindFilename(command_line_File, filename);
    memcpy(Itg_uid, filename + 1, 8);
    ITE_LOG(ITE_LOG_INFO, "FTP: [%s]   find name [%s]\n", command_line_File, filename);

   // upload_server_dir.dirNum = 1;
   // sprintf(upload_server_dir.dirpath[0], Itg_date);
    // sprintf(upload_server_dir.dirpath[1],Itg_uid);
    /***************************************************
        *   登陆成功后进入IP目录
        *   后期需要配置是否需要该子目录
        ****************************************************/
#if 0
    if (pFtpserver->FtpParam.stUploadUser.enFtpIP == 1)
    {
        strcpy(Itg_IP, inet_ntoa(Inaddr));
        memset(cmdLine, 0x0, 1024);
        sprintf(cmdLine, "cd %s", Itg_IP);
        if (cmd_cd(cmdLine) == 0)
        {
            sprintf(cmdLine, "mkdir %s", Itg_IP);
            if (cmd_mkdir(cmdLine) == 0)
            {
                cmd_quit("");
                return 1;
            }
            else
            {
                sprintf(cmdLine, "cd %s", Itg_IP);
                if (cmd_cd(cmdLine) == 0)
                {
                    return 1;
                }
            }
        }
    }

    /***************************************************
    *   进入IP目录，再进入日期子目录
    *   后期需要配置是否需要改子目录
    ****************************************************/
    if (pFtpserver->FtpParam.stUploadUser.enFtpDate == 1)
    {
        memset(cmdLine, 0x0, 1024);
        sprintf(cmdLine, "cd %s", Itg_date);
        if (cmd_cd(cmdLine) == 0)
        {
            sprintf(cmdLine, "mkdir %s", Itg_date);
            if (cmd_mkdir(cmdLine) == 0)
            {
                cmd_quit("");
                return 1;
            }
            else
            {
                sprintf(cmdLine, "cd %s", Itg_date);
                if (cmd_cd(cmdLine) == 0)
                {
                    return 1;
                }
            }
        }
    }
    if (pFtpserver->FtpParam.stUploadUser.enFtpHour == 1)
    {
        memset(cmdLine, 0x0, 1024);
        sprintf(cmdLine, "cd %s", Itg_Hour);
        if (cmd_cd(cmdLine) == 0)
        {
            sprintf(cmdLine, "mkdir %s", Itg_Hour);
            if (cmd_mkdir(cmdLine) == 0)
            {
                cmd_quit("");
                return 1;
            }
            else
            {
                sprintf(cmdLine, "cd %s", Itg_Hour);
                if (cmd_cd(cmdLine) == 0)
                {
                    return 1;
                }
            }
        }
    }
#endif

    DirProcess(&upload_server_dir);
#if 0
    /***************************************************
     *   进入IP目录，再进入日期子目录
     *   根据配置再进入违法类型子目录
     *   后期需要配置是否需要改子目录
     ****************************************************/
    if (strlen(Itg_Type) > 0)
    {
        sprintf(cmdLine, "cd %s", Itg_Type);
        if (cmd_cd(cmdLine) == 0)
        {
            sprintf(cmdLine, "mkdir %s", Itg_Type);
            if (cmd_mkdir(cmdLine) == 0)
            {
                cmd_quit("");
                return 1;
            }
            else
            {
                sprintf(cmdLine, "cd %s", Itg_Type);
                if (cmd_cd(cmdLine) == 0)
                {
                    return 1;
                }
            }
        }
    }
#endif
    /***************************************************
    *   进入子目录后开始上传文件
    *   先PUT file
    *   在GO  上传文件到服务器
    ****************************************************/
    memset(cmdLine, 0x0, 1024);
    sprintf(cmdLine, "put %s", command_line_File);
    cmd_put(cmdLine);
    if (strstr(command_line_File, ".jpg"))
    {
        cmd_processXmlFile(command_line_File);
        cmd_processIniFile(command_line_File);
    }
    cmd_go("go");
    fstep_log_delete();
    // cmd_processAviFile(command_line_File);
    // cmd_go("go");
    usleep(1000);
    cmd_quit("quit");
    return 0;
}



int path_param_process(ITE_Alg_FileInfo *pFileInfo, char *path, char *filename)
{
    char cmdLine[1024];
    char *pString = NULL;
    const char *delim = "/";
    int dirNum = 0;
    strcpy(cmdLine, path);
    if (filename)
    {
        strcpy(pFileInfo->pfilename, filename);
    }
    pString = strtok(cmdLine, delim);
    while (pString)
    {
        strcpy(pFileInfo->dirpath[dirNum], pString);
        dirNum++;
        pString = strtok(NULL, "/");
    }
    pFileInfo->dirNum = dirNum;

    return 0;
}

/*
***********************************************************
*           iTarge Ftp cmd for transfer file to server
*           suport mkdir  del error file
*           创建IP   地址子目录可配置
*           创建日期子目录可配置
*           创建违法类型子目录可配置
*           cmd_upload
*           command_line_File  要上传文件的绝对路径
*           2011-01-05  for barrett
***********************************************************
*/
int cmd_upload_common(char *command_line_File, char *path, char *filename)
{
    char Itg_IP[32];
    char Itg_date[32];
    char Itg_Hour[4];
    char Itg_Type[32];
    char serverIP[64];
    char user[32];
    char password[32];
    char folderName[64];
    char cmdLine[1024];
    struct in_addr Inaddr;
    ITE_Alg_FileInfo upload_server_dir;
    Inaddr.s_addr = (net_get_ifaddr(ETH_NAME_1) == (-1) ? net_get_ifaddr(ETH_NAME_0) : net_get_ifaddr(ETH_NAME_1));
    memset(Itg_IP, 0x0, 32);
    memset(Itg_date, 0x0, 32);
    memset(Itg_Hour, 0x0, 4);
    memset(Itg_Type, 0x0, 32);
    memset(user, 0x0, 32);
    memset(password, 0x0, 32);
    memset(folderName, 0x0, 64);
    memset(serverIP, 0x0, 64);

    strcpy(serverIP, pFtpserver->FtpParam.stFTPParam.FTPAddr);
    strcpy(user, pFtpserver->FtpParam.stFTPParam.szUserName);
    strcpy(password, pFtpserver->FtpParam.stFTPParam.szPassword);
    strcpy(folderName, pFtpserver->FtpParam.stFTPParam.szFolderPath);

    //printf("share  server :%s user :%s passwd :%s dir :%s \n",serverIP,user,password,folderName);
    if (ITG_logInServer(serverIP, user, password, folderName) == 1)
    {
        printf("log in server error !! server :%s user :%s passwd :%s dir :%s \n", serverIP, user, password, folderName);
        return -1;
    }

    memset(&upload_server_dir, 0x0, sizeof(ITE_Alg_FileInfo));
    path_param_process(&upload_server_dir, path, filename);
    /***************************************************
    *   登陆成功后进入IP目录
    *   后期需要配置是否需要该子目录
    ****************************************************/


    DirProcess(&upload_server_dir);


    /***************************************************
    *   进入子目录后开始上传文件
    *   先PUT file
    *   在GO  上传文件到服务器
    ****************************************************/
    memset(cmdLine, 0x0, 1024);
    sprintf(cmdLine, "put %s", command_line_File);
    if (strlen(upload_server_dir.pfilename) > 0)
    {
        cmd_put_common(cmdLine, upload_server_dir.pfilename);
    }
    else
    {
        cmd_put(cmdLine);
    }
    cmd_go("go");
    usleep(1000);
    cmd_quit("quit");
    return 0;
}


