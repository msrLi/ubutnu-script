/*
 * RC File Functions
 *
 * $Revision: 1.2 $
 * $Date: 2002/06/13 07:00:33 $
 */

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include "ftp.h"
#include "quftp.h"
#include "text.h"
#include "rc.h"

char *config(struct variable_list *list, char *key)
{
    struct variable_list *item;
    if (!key) { return NULL; }
    item = list;
    while (item)
    {
        if (strcasecmp(item->key, key) == 0)
        {
            return strdup(item->value);
        }
        item = item->next;
    }
    return NULL;
}

int setconfig(struct variable_list **list, char *key, char *data)
{
    struct variable_list *item, *lastitem;
    if (!key) { return 0; }
    item = *list;
    lastitem = NULL;
    while (item)
    {
        if (strcmp(item->key, key) == 0)
        {
            if (item->value) { free(item->value); }
            if (!data)                      /* Remove key */
            {
                if (lastitem) { lastitem->next = item->next; }
                else { *list = item->next; }
                free(item);
                return 1;
            }
            item->value = strdup(data);
            return 1;
        }
        lastitem = item;
        item = item->next;
    }
    if (!data) { return 0; }
    item = (struct variable_list *)malloc(sizeof(struct variable_list));
    memset(item, 0, sizeof(*item));
    item->key = strdup(key);
    item->value = strdup(data);
    item->next = NULL;
    if (lastitem) { lastitem->next = item; }
    else { *list = item; }
    return 1;
}

char *alias(struct alias_list *list, char *alias)
{
    struct alias_list *item;
    item = list;
    while (item)
    {
        if (strcasecmp(item->alias, alias) == 0)
        {
            return strdup(item->expanded);
        }
        item = item->next;
    }
    return NULL;
}

int setalias(struct alias_list **list, char *alias, char *expanded)
{
    struct alias_list *item, *lastitem;
    item = *list;
    lastitem = NULL;
    while (item)
    {
        if (strcasecmp(item->alias, alias) == 0)
        {
            if (item->expanded) { free(item->expanded); }
            item->expanded = strdup(expanded);
            return 1;
        }
        lastitem = item;
        item = item->next;
    }
    item = (struct alias_list *)malloc(sizeof(struct alias_list));
    memset(item, 0, sizeof(*item));
    item->alias = strdup(alias);
    item->expanded = strdup(expanded);
    item->next = NULL;
    if (lastitem) { lastitem->next = item; }
    else { *list = item; }
    return 1;
}

