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

// === tests/cases/conformance/statements/breakStatements/forInBreakStatements.ts ===
declare function AssertType(value:any, type:string):void;
for(let x in {}) {
    break;
}

ONE:
for(let x in {}) {
    break ONE;
AssertType(ONE, "any");
}

TWO:
THREE:
for(let x in {}) {
    break THREE;
AssertType(THREE, "any");
}

FOUR:
for(let x in {}) {
    FIVE:
AssertType(FIVE, "any");

    for(let x in {}) {
AssertType(x, "string");
AssertType({}, "{}");

        break FOUR;
AssertType(FOUR, "any");
    }
}

for(let x in {}) {
    SIX:
AssertType(SIX, "any");

    for(let x in {}) break SIX;
AssertType(x, "string");
AssertType({}, "{}");
AssertType(SIX, "any");
}

SEVEN:
for (let x in {}) for (let x in {}) for (let x in {}) break SEVEN;

EIGHT:
for (let x in {}){
    let fn = function () { 
AssertType(fn, "() => void");

AssertType(function () { }, "() => void");
}

    break EIGHT;
AssertType(EIGHT, "any");
}


