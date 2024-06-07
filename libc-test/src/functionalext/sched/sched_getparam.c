/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "functionalext.h"
#include "test.h"

/**
 * @tc.name      : sched_getparam
 * @tc.desc      : When param is NULL, call sched_getparam.
 * @tc.level     : Level 2
 */
static void sched_getparam_0010(void)
{
    pid_t pid;
    pid = getpid();
    EXPECT_EQ(sched_getparam_0010, sched_getparam(pid, NULL), -1);
}

int main(void)
{
    sched_getparam_0010();
    return t_status;
}