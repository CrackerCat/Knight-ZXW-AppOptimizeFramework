//
// Created by Knight-ZXW on 2024/4/3.
//
#pragma once

#define DCHECK(x) \
  if (::android::base::kEnableDChecks) CHECK(x)
#define DCHECK_EQ(x, y) \
  if (::android::base::kEnableDChecks) CHECK_EQ(x, y)
#define DCHECK_NE(x, y) \
  if (::android::base::kEnableDChecks) CHECK_NE(x, y)
#define DCHECK_LE(x, y) \
  if (::android::base::kEnableDChecks) CHECK_LE(x, y)
#define DCHECK_LT(x, y) \
  if (::android::base::kEnableDChecks) CHECK_LT(x, y)
#define DCHECK_GE(x, y) \
  if (::android::base::kEnableDChecks) CHECK_GE(x, y)
#define DCHECK_GT(x, y) \
  if (::android::base::kEnableDChecks) CHECK_GT(x, y)
#define DCHECK_STREQ(s1, s2) \
  if (::android::base::kEnableDChecks) CHECK_STREQ(s1, s2)
#define DCHECK_STRNE(s1, s2) \
  if (::android::base::kEnableDChecks) CHECK_STRNE(s1, s2)