#include "display.h"
#include "input.h"
#include "utils.h"
#include <stdio.h>
#include "global.h"
#include <string.h>
#include "log.h"
#include "alias.h"
#include "signal_handler.h"

int main() {
    setup_signal_handlers();
    load_myshrc(); // Load the .myshrc file and all function and aliases to memory
    get_home_directory(home);
    // It take Initiaisation of Shell directory as home directory(~)
    strcpy(previous_directory,"");
    strcpy(normal,"NORMAL");
    MAXPATH=1024;
    int process_list_count=0;

    char input[1024];
    while (1) {
        update_process_list();
        display_prompt(home);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Either Error or Interrupt");
            continue;
        }
        handle_input(input);
        add_to_log(input);
        fg_pid = -1;

    }
    return 0;
}
