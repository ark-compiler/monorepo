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

// === tests/cases/conformance/es6/for-ofStatements/for-of22.ts ===
declare function AssertType(value:any, type:string):void;
class Foo { }
class FooIterator {
    next() {
AssertType({            value: new Foo,            done: false        }, "{ value: Foo; done: boolean; }");
        return {

            value: new Foo,
AssertType(value, "Foo");
AssertType(new Foo, "Foo");
AssertType(Foo, "typeof Foo");

            done: false
AssertType(done, "boolean");
AssertType(false, "boolean");

        };
    }
    [Symbol.iterator]() {
AssertType(this, "this");
        return this;
    }
}

v;
AssertType(v, "Foo");

for (let v of new FooIterator) {
    
}

