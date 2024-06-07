/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


1 < 2 ? (): number => { return 1; } : (): string => { return "1"; }
1 < 2 ? (): number => { return 1; } : (p1: number, p2: string): string => { return "1"; }
1 < 2 ? (p: string): number => { return 1; } : (): string => { return "1"; }

var a = 1, b = 2, c = 3;
1 < 2 ? (a = 1, b = 2) : c
1 < 2 ? (a = 1, b = 2): any => { return b } : c
