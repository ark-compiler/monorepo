/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

declare function AssertType(value:any, type:string):void;
{
    class Student {
        age : number;
        name : string;

        constructor(ageIn : number, nameIn : string) {
            this.age = ageIn;
            this.name = nameIn;
        }

        get studentAge() : number {
            return this.age;
        }

        get studentName() : string {
            return this.name
        }
    }
    let obj = new Student(15, "xiaoming");
    let a = obj.studentAge;
    let b = obj.studentName;
    AssertType(a, "number");
    AssertType(b, "string");
}