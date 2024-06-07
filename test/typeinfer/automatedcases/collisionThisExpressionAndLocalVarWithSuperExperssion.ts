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

// === tests/cases/compiler/collisionThisExpressionAndLocalVarWithSuperExperssion.ts ===
declare function AssertType(value:any, type:string):void;
class a {
    public foo() {
    }
}
class b extends a {
    public foo() {
        let _this = 10;
AssertType(_this, "number");
AssertType(10, "int");

        let f = () => super.foo();
AssertType(f, "() => void");
AssertType(() => super.foo(), "() => void");
AssertType(super.foo(), "void");
AssertType(super.foo, "() => void");
AssertType(super, "a");
    }
}
class b2 extends a {
    public foo() {
        let f = () => {
AssertType(f, "() => void");
AssertType(() => {            let _this = 10;            return super.foo()        }, "() => void");

            let _this = 10;
AssertType(_this, "number");
AssertType(10, "int");

AssertType(super.foo(), "void");
AssertType(super.foo, "() => void");
AssertType(super, "a");
            return super.foo()
        }
    }
}

