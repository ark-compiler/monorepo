/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <grp.h>
#include "functionalext.h"

#define TEST_GROUP_ID 1089

/**
 * @tc.name      : initgroups_0100
 * @tc.desc      : The group_id of the initialization group usr_test is 1089
 * @tc.level     : Level 0
 */
void initgroups_0100(void)
{
    int rev = initgroups("usr_test", TEST_GROUP_ID);
    EXPECT_EQ("initgroups_0100", rev, 0);
}

/**
 * @tc.name      : initgroups_0200
 * @tc.desc      : The group_id of the initialization group usr_test is -1
 * @tc.level     : Level 2
 */
void initgroups_0200(void)
{
    int rev = initgroups("usr_test", -1);
    EXPECT_EQ("initgroups_0200", rev, -1);
}

int main(void)
{
    initgroups_0100();
    initgroups_0200();
    return t_status;
}