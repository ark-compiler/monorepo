/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "compilerContext.h"

#include <compiler/core/emitter/emitter.h>
#include <typescript/extractor/typeRecorder.h>

namespace panda::es2panda::compiler {

CompilerContext::CompilerContext(binder::Binder *binder, bool isDebug, bool isDebuggerEvaluateExpressionMode,
                                 bool isMergeAbc, bool isTypeExtractorEnabled, bool isJsonInputFile, 
                                 bool isRecordSource, std::string sourceFile, std::string pkgName, 
                                 util::StringView recordName, util::PatchFix *patchFixHelper)
    : binder_(binder), isDebug_(isDebug), isDebuggerEvaluateExpressionMode_(isDebuggerEvaluateExpressionMode),
      isMergeAbc_(isMergeAbc), isTypeExtractorEnabled_(isTypeExtractorEnabled), isJsonInputFile_(isJsonInputFile),
      isRecordSource_(isRecordSource), sourceFile_(sourceFile), pkgName_(pkgName), recordName_(recordName), 
      patchFixHelper_(patchFixHelper), emitter_(std::make_unique<class Emitter>(this))
{
}

void CompilerContext::SetTypeRecorder(extractor::TypeRecorder *recorder)
{
    ASSERT(emitter_ != nullptr);
    emitter_->FillTypeLiteralBuffers(recorder);
    recorder_ = recorder;
}

void CompilerContext::SetTypeExtractor(extractor::TypeExtractor *extractor)
{
    extractor_ = extractor;
}

}  // namespace panda::es2panda::compiler
