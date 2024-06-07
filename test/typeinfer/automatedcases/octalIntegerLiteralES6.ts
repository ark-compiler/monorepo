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

// === tests/cases/conformance/es6/binaryAndOctalIntegerLiteral/octalIntegerLiteralES6.ts ===
declare function AssertType(value:any, type:string):void;
let oct1 = 0o45436;
AssertType(oct1, "number");
AssertType(0o45436, "int");

let oct2 = 0O45436;
AssertType(oct2, "number");
AssertType(0O45436, "int");

let oct3 = 0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777;
AssertType(oct3, "number");
AssertType(0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777, "int");

let oct4 = 0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777;
AssertType(oct4, "number");
AssertType(0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777, "int");

let obj1 = {
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType({    0o45436: "Hello",    a: 0o45436,     b: oct1,    oct1,    0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777: true}, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");

    0o45436: "Hello",
AssertType(0o45436, "string");
AssertType("Hello", "string");

    a: 0o45436, 
AssertType(a, "number");
AssertType(0o45436, "int");

    b: oct1,
AssertType(b, "number");
AssertType(oct1, "number");

    oct1,
AssertType(oct1, "number");

    0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777: true
AssertType(0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777, "boolean");
AssertType(true, "boolean");
}

let obj2 = {
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType({    0O45436: "hi",    a: 0O45436,     b: oct2,    oct2,    0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777: false,}, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");

    0O45436: "hi",
AssertType(0O45436, "string");
AssertType("hi", "string");

    a: 0O45436, 
AssertType(a, "number");
AssertType(0O45436, "int");

    b: oct2,
AssertType(b, "number");
AssertType(oct2, "number");

    oct2,
AssertType(oct2, "number");

    0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777: false,
AssertType(0o7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777, "boolean");
AssertType(false, "boolean");
}

obj1[0o45436];     // string
AssertType(obj1[0o45436], "string");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType(0o45436, "int");

obj1["0o45436"];   // any
AssertType(obj1["0o45436"], "error");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType("0o45436", "string");

obj1["19230"];     // string
AssertType(obj1["19230"], "string");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType("19230", "string");

obj1[19230];       // string
AssertType(obj1[19230], "string");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType(19230, "int");

obj1["a"];         // number
AssertType(obj1["a"], "number");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType("a", "string");

obj1["b"];         // number
AssertType(obj1["b"], "number");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType("b", "string");

obj1["oct1"];      // number
AssertType(obj1["oct1"], "number");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType("oct1", "string");

obj1["Infinity"];  // boolean
AssertType(obj1["Infinity"], "boolean");
AssertType(obj1, "{ 19230: string; a: number; b: number; oct1: number; Infinity: boolean; }");
AssertType("Infinity", "string");

obj2[0O45436];    // string
AssertType(obj2[0O45436], "string");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType(0O45436, "int");

obj2["0O45436"];  // any
AssertType(obj2["0O45436"], "error");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType("0O45436", "string");

obj2["19230"];    // string
AssertType(obj2["19230"], "string");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType("19230", "string");

obj2[19230];      // string
AssertType(obj2[19230], "string");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType(19230, "int");

obj2["a"];        // number
AssertType(obj2["a"], "number");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType("a", "string");

obj2["b"];        // number
AssertType(obj2["b"], "number");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType("b", "string");

obj2["oct2"];     // number
AssertType(obj2["oct2"], "number");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType("oct2", "string");

obj2[5.462437423415177e+244];    // boolean
AssertType(obj2[5.462437423415177e+244], "boolean");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType(5.462437423415177e+244, "int");

obj2["5.462437423415177e+244"];  // boolean
AssertType(obj2["5.462437423415177e+244"], "boolean");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType("5.462437423415177e+244", "string");

obj2["Infinity"];  // any
AssertType(obj2["Infinity"], "error");
AssertType(obj2, "{ 19230: string; a: number; b: number; oct2: number; 5.462437423415177e+244: boolean; }");
AssertType("Infinity", "string");


