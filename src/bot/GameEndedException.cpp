/*
 * Created by liyicheng on 2019/8/26.
 */

#include "GameEndedException.h"

z2::GameEndedException::GameEndedException(const string &arg) : runtime_error(arg) {}

z2::GameEndedException::GameEndedException(const char *string) : runtime_error(string) {}
