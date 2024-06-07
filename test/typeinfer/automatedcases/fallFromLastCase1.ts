/*
* Copyright (c) Microsoft Corporation. All rights reserved.
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
*
* This file has been modified by Huawei to verify type inference by adding verification statements.
*/

// === tests/cases/compiler/fallFromLastCase1.ts ===
declare function AssertType(value:any, type:string):void;
declare function use(a: string);

function foo1(a: number) {
    switch (a) {
AssertType(a, "number");

        case 1:
AssertType(1, "int");

            use("1");
AssertType(use("1"), "any");
AssertType(use, "(string) => any");
AssertType("1", "string");

            break;
        case 2:
AssertType(2, "int");

            use("2");
AssertType(use("2"), "any");
AssertType(use, "(string) => any");
AssertType("2", "string");
    }
}


function foo2(a: number) {
    switch (a) {
AssertType(a, "number");

        case 1:
AssertType(1, "int");

            use("1");
AssertType(use("1"), "any");
AssertType(use, "(string) => any");
AssertType("1", "string");

            break;
        default:
            use("2");
AssertType(use("2"), "any");
AssertType(use, "(string) => any");
AssertType("2", "string");
    }
}

