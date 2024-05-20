package com.knightboost.sliver;

import androidx.annotation.Keep;

@Keep
public interface HookSuspendThreadTimeoutCallback {
  public void triggerSuspendTimeout();
  public void onError(String error);
}
