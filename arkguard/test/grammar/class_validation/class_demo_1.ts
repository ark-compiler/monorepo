/*
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
 */

import assert = require('assert');

export class Cat {
  private mAge: number;

  constructor() {
    this.mAge = -1;
  }

  getAge() {
    return this.mAge;
  }

  setAge(age) {
    this.mAge = age;
  }

  get age(): number {
    return this.mAge;
  }

  set age(_age) {
    this.mAge = _age;
  }
}

const cat = new Cat();
assert.strictEqual(cat.getAge(), -1);

cat.age = 12;
assert.strictEqual(cat.age, 12);

cat.setAge(13);
assert.strictEqual(cat.age, 13);
