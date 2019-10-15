#include <stdio.h>
#include "logger.h"

int main(){

    // default logging
    infof("INFO std Message %d\n", 1);
    warnf("WARN std Message %d\n", 2);
    errorf("ERROR std Message %d\n", 2);

    // stdout logging
    initLogger("stdout");
    infof("INFO std Message %d\n", 1);
    warnf("WARN std Message %d\n", 2);
    errorf("ERROR std Message %d\n", 2);

    // syslog logging
    initLogger("syslog");
    infof("INFO sys Message %d", 1);
    warnf("WARN sys Message %d", 2);
    errorf("ERROR sys Message %d", 2);

    return 0;
}
