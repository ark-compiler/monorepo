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

// === tests/cases/conformance/statements/for-ofStatements/ES5For-of10.ts ===
declare function AssertType(value:any, type:string):void;
function foo() {
AssertType({ x: 0 }, "{ x: number; }");
AssertType(x, "number");
AssertType(0, "int");
    return { x: 0 };
}
for (foo().x of []) {
    for (foo().x of [])
AssertType(foo().x, "number");
AssertType(foo(), "{ x: number; }");
AssertType(foo, "() => { x: number; }");
AssertType([], "undefined[]");

        let p = foo().x;
AssertType(p, "number");
AssertType(foo().x, "number");
AssertType(foo(), "{ x: number; }");
AssertType(foo, "() => { x: number; }");
}
