/* quftp include
   $Revision: 1.8 $
   $Date: 2003/03/11 01:26:39 $
*/

#ifndef __QFTP_H__
#define __QFTP_H__

#define VERSION         "1.0.7"
#define DEFAULT_RC_FILENAME "~/.quftprc"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <ite_typedef.h>
#include <ite_common.h>
#include <ite_osal.h>
#include <jpeg_extheader.h>
#include "rc.h"
#include "log.h"


struct COMMAND
{
    char *name;
    int (*function)();
    //Function *function;
    char *description;
    char *longhelp;
};

struct qitem
{
    int  id;
    int  permissions;
    char direction;
    char command;
    char status;
    char remotedir[256];
    char localdir[256];
    char localfilename[256];
    char remotefilename[256];
    struct ftpconnection *remoteconnection;
    struct ftpconnection *localconnection;
    unsigned long size;
    struct qitem *next;
};



#define DIRECTION_NONE      0
#define DIRECTION_OUT       1
#define DIRECTION_PUT       1
#define DIRECTION_IN        2
#define DIRECTION_GET       2
#define DIRECTION_FXP       3

#define COMMAND_NONE        0

#define STATREGEX1      "^ *([-a-z]+) +([0-9]+) +([a-z]+) +([a-z]+) +([0-9]+) (.{12}) (.*)$"

extern int ftp_debug;
unsigned short ftp_port;
struct qitem *addtoqueue(struct qitem **queue, struct qitem *item);
struct qitem *delfromqueue(struct qitem **queue, int id);
void clearqueue(struct qitem *queue);
struct qitem *runqueue(struct qitem *queue);
void printqueue(struct qitem *queue);
int getconnected(struct ftpconnection *connection);
char *completion_match_generator(char *text, int state);
char *complete_command(const char *text, int state);
char *complete_remote_filename(const char *text, int state);
char **quftp_completion(char *text, int start, int end);
int InitFtpshare(ITE_OSAL_MSG_INFO_T *pshareOpt);
int cmd_ls(char *command_line);
int cmd_cd(char *command_line);
int cmd_lcd(char *command_line);
int cmd_cdup(char *command_line);
int cmd_pwd(char *command_line);
int cmd_get(char *command_line);
int cmd_put(char *command_line);
int cmd_put_common(char *command_line, char *remotename);
int cmd_fxp(char *command_line);
int cmd_go(char *command_line);
int cmd_clear(char *command_line);
int cmd_queue(char *command_line);
int cmd_queue(char *command_line);
int cmd_quit(char *command_line);
int cmd_close(char *command_line);
int cmd_open(char *command_line);
int cmd_user(char *command_line);
int cmd_stat(char *command_line);
int cmd_status(char *command_line);
int cmd_nlst(char *command_line);
int cmd_lls(char *command_line);
int cmd_cat(char *command_line);
int cmd_more(char *command_line);
int cmd_set(char *command_line);
int cmd_unset(char *command_line);
int cmd_assert(char *command_line);
int cmd_quote(char *command_line);
char *parseline(char *line);
int cmd_dele(char *command_line);
int cmd_alias(char *command_line);
char *expand_wildcard(char *inputstring);
char *expand_alias(char *string);
char *print_permissions(int permissions);
int progress_bar(unsigned long current, unsigned long max, double speed);
char *timestring(unsigned long seconds);
int cmd_getlist(char *command_line);
int cmd_mkdir(char *command_line);
int cmd_rmdir(char *command_line);
int cmd_upload(char *command_line_File);
int cmd_upload_common(char *command_line_File, char *path, char *filename);

int fstep_log(char *logStr);
int fstep_log_delete();
int cmd_X();//查询FTP服务器是否在线

#endif
