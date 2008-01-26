#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lcm/lcm.h>

int catchall_handler (const lcm_recv_buf_t *rbuf, void *u)
{
    printf("catchall handler [%s] (content: %s)\n", rbuf->channel, rbuf->data);
    return 0;
}

int main(int argc, char **argv)
{
    lcm_t *lcm = lcm_create("udpm://");
    if (! lcm) {
        fprintf(stderr, "couldn't allocate lcm_t\n");
        return 1;
    }
    lcm_subscribe (lcm, ".*", catchall_handler, NULL);

    while(1) {
		lcm_handle (lcm);
    }

    lcm_destroy (lcm);
    
    return 0;
}