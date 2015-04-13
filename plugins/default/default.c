#include <stdio.h>
#include "../../ontom/error.h"
#include "../../lib/lj/config.h"
#include "../../lib/lj/log.h"
#include "../../ontom/charge.h"
#include "../../thirdpart/extso/exso.h"

int exso_default_init(void *p)
{
    printf("******  Plugin  default  initialized  ******\n");
    const char *exso_path = config_read("exso_path");
    if ( exso_path == NULL ) {
        exso_path = config_write("exso_path", "/usr/zeus/plugins/");
        if ( exso_path == NULL ) return ERR_ERR;
    }
    return ERR_OK;
}

int exso_default_main_loop(void *p)
{
    struct charge_task *t = (struct charge_task *)p;

    if ( t == NULL ) return ERR_ERR;
    const char *load_exso_name = config_read("load_exso");
    const char *exso_name = config_read("load_name");
    const char *unload_exso_name = config_read("unload_exso");

    if ( load_exso_name != NULL && exso_name != NULL  && 0 != strcmp("N/A", load_exso_name) ) {
        char exso_path[256];
        struct exso_struct *thiz;
        const char *plugins_path = config_read("exso_path");

        sprintf(exso_path, "%s%s", plugins_path, load_exso_name);
        thiz = exso_load( &(t->exsos), exso_name, exso_path, p);
        if ( thiz == NULL ) {
            log_printf(WRN, "EXSO: load <%s:%s> faile!!!!", exso_name, plugins_path);
            return ERR_ERR;
        } else {
            log_printf(INF, "EXSO: <%s:%s> loaded.", exso_name, plugins_path);
            return ERR_OK;
        }
    } else if ( unload_exso_name != NULL && 0 != strcmp("N/A", unload_exso_name)  ) {
    } else {}

    return ERR_OK;
}

int exso_default_exit(void *p)
{
    printf("******  Plugin  default  exited  ******\n");
    return ERR_OK;
}
