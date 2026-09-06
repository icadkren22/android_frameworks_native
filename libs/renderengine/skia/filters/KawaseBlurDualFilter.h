/*
 * Copyright 2024 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <SkCanvas.h>
#include <SkImage.h>
#include <SkRuntimeEffect.h>
#include <SkSurface.h>
#include "BlurFilter.h"

#include "RuntimeEffectManager.h"

#include <chrono>

namespace android {
namespace renderengine {
namespace skia {

/**
 * An algorithm for doing Kawase dual blur on GPUs using Skia.
 *
 * Kawase is an approximation of Gaussian blur, but behaves differently. It looks fine for
 * not-too-blurry effects, but the results could visually degrade with larger radii.
 */
class KawaseBlurDualFilter : public BlurFilter {
public:
    explicit KawaseBlurDualFilter(RuntimeEffectManager& effectManager);
    virtual ~KawaseBlurDualFilter() {}

    // Execute blur, saving it to a texture
    sk_sp<SkImage> generate(SkiaGpuContext* context, const uint32_t radius,
                            const sk_sp<SkImage> blurInput, const SkRect& blurRect) const override;

private:
    sk_sp<SkRuntimeEffect> mLowSampleBlurEffect;
    sk_sp<SkRuntimeEffect> mHighSampleBlurEffect;

    mutable sk_sp<SkImage> mCachedImage;
    mutable std::chrono::steady_clock::time_point mLastBlurTime;
    mutable int mLastWidth = 0;
    mutable int mLastHeight = 0;

    void blurInto(const sk_sp<SkSurface>& drawSurface, const sk_sp<SkImage>& readImage,
                  const float radius, const float alpha, const sk_sp<SkRuntimeEffect>&) const;

    void blurInto(const sk_sp<SkSurface>& drawSurface, const sk_sp<SkShader> input,
                  const float radius, const float alpha, const sk_sp<SkRuntimeEffect>&) const;
};

} // namespace skia
} // namespace renderengine
} // namespace android
