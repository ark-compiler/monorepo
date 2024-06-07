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

// === tests/cases/conformance/expressions/binaryOperators/comparisonOperator/comparisonOperatorWithIdenticalObjects.ts ===
declare function AssertType(value:any, type:string):void;
class A1 {
    public a: string;
    public b: number;
    public c: boolean;
    public d: any;
    public e: Object;
    public fn(a: string): string {
AssertType(null, "null");
        return null;
    }
}
class B1 {
    public a: string;
    public b: number;
    public c: boolean;
    public d: any;
    public e: Object;
    public fn(b: string): string {
AssertType(null, "null");
        return null;
    }
}

class Base {
    private a: string;
    private fn(b: string): string {
AssertType(null, "null");
        return null;
    }
}
class A2 extends Base { }
class B2 extends Base { }

interface A3 { f(a: number): string; }
interface B3 { f(a: number): string; }

interface A4 { new (a: string): A1; }
interface B4 { new (a: string): B1; }

interface A5 { [x: number]: number; }
interface B5 { [x: number]: number; }

interface A6 { [x: string]: string; }
interface B6 { [x: string]: string; }

let a1: A1;
AssertType(a1, "A1");

let a2: A2;
AssertType(a2, "A2");

let a3: A3;
AssertType(a3, "A3");

let a4: A4;
AssertType(a4, "A4");

let a5: A5;
AssertType(a5, "A5");

let a6: A6;
AssertType(a6, "A6");

let b1: B1;
AssertType(b1, "B1");

let b2: B2;
AssertType(b2, "B2");

let b3: B3;
AssertType(b3, "B3");

let b4: B4;
AssertType(b4, "B4");

let b5: B5;
AssertType(b5, "B5");

let b6: B6;
AssertType(b6, "B6");

let base1: Base;
AssertType(base1, "Base");

let base2: Base;
AssertType(base2, "Base");

// operator <
let r1a1 = a1 < b1;
AssertType(r1a1, "boolean");
AssertType(a1 < b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r1a2 = base1 < base2;
AssertType(r1a2, "boolean");
AssertType(base1 < base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r1a3 = a2 < b2;
AssertType(r1a3, "boolean");
AssertType(a2 < b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r1a4 = a3 < b3;
AssertType(r1a4, "boolean");
AssertType(a3 < b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r1a5 = a4 < b4;
AssertType(r1a5, "boolean");
AssertType(a4 < b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r1a6 = a5 < b5;
AssertType(r1a6, "boolean");
AssertType(a5 < b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r1a7 = a6 < b6;
AssertType(r1a7, "boolean");
AssertType(a6 < b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r1b1 = b1 < a1;
AssertType(r1b1, "boolean");
AssertType(b1 < a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r1b2 = base2 < base1;
AssertType(r1b2, "boolean");
AssertType(base2 < base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r1b3 = b2 < a2;
AssertType(r1b3, "boolean");
AssertType(b2 < a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r1b4 = b3 < a3;
AssertType(r1b4, "boolean");
AssertType(b3 < a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r1b5 = b4 < a4;
AssertType(r1b5, "boolean");
AssertType(b4 < a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r1b6 = b5 < a5;
AssertType(r1b6, "boolean");
AssertType(b5 < a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r1b7 = b6 < a6;
AssertType(r1b7, "boolean");
AssertType(b6 < a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");

// operator >
let r2a1 = a1 > b1;
AssertType(r2a1, "boolean");
AssertType(a1 > b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r2a2 = base1 > base2;
AssertType(r2a2, "boolean");
AssertType(base1 > base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r2a3 = a2 > b2;
AssertType(r2a3, "boolean");
AssertType(a2 > b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r2a4 = a3 > b3;
AssertType(r2a4, "boolean");
AssertType(a3 > b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r2a5 = a4 > b4;
AssertType(r2a5, "boolean");
AssertType(a4 > b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r2a6 = a5 > b5;
AssertType(r2a6, "boolean");
AssertType(a5 > b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r2a7 = a6 > b6;
AssertType(r2a7, "boolean");
AssertType(a6 > b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r2b1 = b1 > a1;
AssertType(r2b1, "boolean");
AssertType(b1 > a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r2b2 = base2 > base1;
AssertType(r2b2, "boolean");
AssertType(base2 > base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r2b3 = b2 > a2;
AssertType(r2b3, "boolean");
AssertType(b2 > a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r2b4 = b3 > a3;
AssertType(r2b4, "boolean");
AssertType(b3 > a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r2b5 = b4 > a4;
AssertType(r2b5, "boolean");
AssertType(b4 > a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r2b6 = b5 > a5;
AssertType(r2b6, "boolean");
AssertType(b5 > a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r2b7 = b6 > a6;
AssertType(r2b7, "boolean");
AssertType(b6 > a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");

// operator <=
let r3a1 = a1 <= b1;
AssertType(r3a1, "boolean");
AssertType(a1 <= b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r3a2 = base1 <= base2;
AssertType(r3a2, "boolean");
AssertType(base1 <= base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r3a3 = a2 <= b2;
AssertType(r3a3, "boolean");
AssertType(a2 <= b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r3a4 = a3 <= b3;
AssertType(r3a4, "boolean");
AssertType(a3 <= b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r3a5 = a4 <= b4;
AssertType(r3a5, "boolean");
AssertType(a4 <= b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r3a6 = a5 <= b5;
AssertType(r3a6, "boolean");
AssertType(a5 <= b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r3a7 = a6 <= b6;
AssertType(r3a7, "boolean");
AssertType(a6 <= b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r3b1 = b1 <= a1;
AssertType(r3b1, "boolean");
AssertType(b1 <= a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r3b2 = base2 <= base1;
AssertType(r3b2, "boolean");
AssertType(base2 <= base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r3b3 = b2 <= a2;
AssertType(r3b3, "boolean");
AssertType(b2 <= a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r3b4 = b3 <= a3;
AssertType(r3b4, "boolean");
AssertType(b3 <= a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r3b5 = b4 <= a4;
AssertType(r3b5, "boolean");
AssertType(b4 <= a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r3b6 = b5 <= a5;
AssertType(r3b6, "boolean");
AssertType(b5 <= a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r3b7 = b6 <= a6;
AssertType(r3b7, "boolean");
AssertType(b6 <= a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");

// operator >=
let r4a1 = a1 >= b1;
AssertType(r4a1, "boolean");
AssertType(a1 >= b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r4a2 = base1 >= base2;
AssertType(r4a2, "boolean");
AssertType(base1 >= base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r4a3 = a2 >= b2;
AssertType(r4a3, "boolean");
AssertType(a2 >= b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r4a4 = a3 >= b3;
AssertType(r4a4, "boolean");
AssertType(a3 >= b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r4a5 = a4 >= b4;
AssertType(r4a5, "boolean");
AssertType(a4 >= b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r4a6 = a5 >= b5;
AssertType(r4a6, "boolean");
AssertType(a5 >= b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r4a7 = a6 >= b6;
AssertType(r4a7, "boolean");
AssertType(a6 >= b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r4b1 = b1 >= a1;
AssertType(r4b1, "boolean");
AssertType(b1 >= a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r4b2 = base2 >= base1;
AssertType(r4b2, "boolean");
AssertType(base2 >= base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r4b3 = b2 >= a2;
AssertType(r4b3, "boolean");
AssertType(b2 >= a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r4b4 = b3 >= a3;
AssertType(r4b4, "boolean");
AssertType(b3 >= a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r4b5 = b4 >= a4;
AssertType(r4b5, "boolean");
AssertType(b4 >= a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r4b6 = b5 >= a5;
AssertType(r4b6, "boolean");
AssertType(b5 >= a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r4b7 = b6 >= a6;
AssertType(r4b7, "boolean");
AssertType(b6 >= a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");

// operator ==
let r5a1 = a1 == b1;
AssertType(r5a1, "boolean");
AssertType(a1 == b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r5a2 = base1 == base2;
AssertType(r5a2, "boolean");
AssertType(base1 == base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r5a3 = a2 == b2;
AssertType(r5a3, "boolean");
AssertType(a2 == b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r5a4 = a3 == b3;
AssertType(r5a4, "boolean");
AssertType(a3 == b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r5a5 = a4 == b4;
AssertType(r5a5, "boolean");
AssertType(a4 == b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r5a6 = a5 == b5;
AssertType(r5a6, "boolean");
AssertType(a5 == b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r5a7 = a6 == b6;
AssertType(r5a7, "boolean");
AssertType(a6 == b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r5b1 = b1 == a1;
AssertType(r5b1, "boolean");
AssertType(b1 == a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r5b2 = base2 == base1;
AssertType(r5b2, "boolean");
AssertType(base2 == base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r5b3 = b2 == a2;
AssertType(r5b3, "boolean");
AssertType(b2 == a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r5b4 = b3 == a3;
AssertType(r5b4, "boolean");
AssertType(b3 == a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r5b5 = b4 == a4;
AssertType(r5b5, "boolean");
AssertType(b4 == a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r5b6 = b5 == a5;
AssertType(r5b6, "boolean");
AssertType(b5 == a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r5b7 = b6 == a6;
AssertType(r5b7, "boolean");
AssertType(b6 == a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");

// operator !=
let r6a1 = a1 != b1;
AssertType(r6a1, "boolean");
AssertType(a1 != b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r6a2 = base1 != base2;
AssertType(r6a2, "boolean");
AssertType(base1 != base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r6a3 = a2 != b2;
AssertType(r6a3, "boolean");
AssertType(a2 != b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r6a4 = a3 != b3;
AssertType(r6a4, "boolean");
AssertType(a3 != b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r6a5 = a4 != b4;
AssertType(r6a5, "boolean");
AssertType(a4 != b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r6a6 = a5 != b5;
AssertType(r6a6, "boolean");
AssertType(a5 != b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r6a7 = a6 != b6;
AssertType(r6a7, "boolean");
AssertType(a6 != b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r6b1 = b1 != a1;
AssertType(r6b1, "boolean");
AssertType(b1 != a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r6b2 = base2 != base1;
AssertType(r6b2, "boolean");
AssertType(base2 != base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r6b3 = b2 != a2;
AssertType(r6b3, "boolean");
AssertType(b2 != a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r6b4 = b3 != a3;
AssertType(r6b4, "boolean");
AssertType(b3 != a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r6b5 = b4 != a4;
AssertType(r6b5, "boolean");
AssertType(b4 != a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r6b6 = b5 != a5;
AssertType(r6b6, "boolean");
AssertType(b5 != a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r6b7 = b6 != a6;
AssertType(r6b7, "boolean");
AssertType(b6 != a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");

// operator ===
let r7a1 = a1 === b1;
AssertType(r7a1, "boolean");
AssertType(a1 === b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r7a2 = base1 === base2;
AssertType(r7a2, "boolean");
AssertType(base1 === base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r7a3 = a2 === b2;
AssertType(r7a3, "boolean");
AssertType(a2 === b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r7a4 = a3 === b3;
AssertType(r7a4, "boolean");
AssertType(a3 === b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r7a5 = a4 === b4;
AssertType(r7a5, "boolean");
AssertType(a4 === b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r7a6 = a5 === b5;
AssertType(r7a6, "boolean");
AssertType(a5 === b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r7a7 = a6 === b6;
AssertType(r7a7, "boolean");
AssertType(a6 === b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r7b1 = b1 === a1;
AssertType(r7b1, "boolean");
AssertType(b1 === a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r7b2 = base2 === base1;
AssertType(r7b2, "boolean");
AssertType(base2 === base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r7b3 = b2 === a2;
AssertType(r7b3, "boolean");
AssertType(b2 === a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r7b4 = b3 === a3;
AssertType(r7b4, "boolean");
AssertType(b3 === a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r7b5 = b4 === a4;
AssertType(r7b5, "boolean");
AssertType(b4 === a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r7b6 = b5 === a5;
AssertType(r7b6, "boolean");
AssertType(b5 === a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r7b7 = b6 === a6;
AssertType(r7b7, "boolean");
AssertType(b6 === a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");

// operator !==
let r8a1 = a1 !== b1;
AssertType(r8a1, "boolean");
AssertType(a1 !== b1, "boolean");
AssertType(a1, "A1");
AssertType(b1, "B1");

let r8a2 = base1 !== base2;
AssertType(r8a2, "boolean");
AssertType(base1 !== base2, "boolean");
AssertType(base1, "Base");
AssertType(base2, "Base");

let r8a3 = a2 !== b2;
AssertType(r8a3, "boolean");
AssertType(a2 !== b2, "boolean");
AssertType(a2, "A2");
AssertType(b2, "B2");

let r8a4 = a3 !== b3;
AssertType(r8a4, "boolean");
AssertType(a3 !== b3, "boolean");
AssertType(a3, "A3");
AssertType(b3, "B3");

let r8a5 = a4 !== b4;
AssertType(r8a5, "boolean");
AssertType(a4 !== b4, "boolean");
AssertType(a4, "A4");
AssertType(b4, "B4");

let r8a6 = a5 !== b5;
AssertType(r8a6, "boolean");
AssertType(a5 !== b5, "boolean");
AssertType(a5, "A5");
AssertType(b5, "B5");

let r8a7 = a6 !== b6;
AssertType(r8a7, "boolean");
AssertType(a6 !== b6, "boolean");
AssertType(a6, "A6");
AssertType(b6, "B6");

let r8b1 = b1 !== a1;
AssertType(r8b1, "boolean");
AssertType(b1 !== a1, "boolean");
AssertType(b1, "B1");
AssertType(a1, "A1");

let r8b2 = base2 !== base1;
AssertType(r8b2, "boolean");
AssertType(base2 !== base1, "boolean");
AssertType(base2, "Base");
AssertType(base1, "Base");

let r8b3 = b2 !== a2;
AssertType(r8b3, "boolean");
AssertType(b2 !== a2, "boolean");
AssertType(b2, "B2");
AssertType(a2, "A2");

let r8b4 = b3 !== a3;
AssertType(r8b4, "boolean");
AssertType(b3 !== a3, "boolean");
AssertType(b3, "B3");
AssertType(a3, "A3");

let r8b5 = b4 !== a4;
AssertType(r8b5, "boolean");
AssertType(b4 !== a4, "boolean");
AssertType(b4, "B4");
AssertType(a4, "A4");

let r8b6 = b5 !== a5;
AssertType(r8b6, "boolean");
AssertType(b5 !== a5, "boolean");
AssertType(b5, "B5");
AssertType(a5, "A5");

let r8b7 = b6 !== a6;
AssertType(r8b7, "boolean");
AssertType(b6 !== a6, "boolean");
AssertType(b6, "B6");
AssertType(a6, "A6");


