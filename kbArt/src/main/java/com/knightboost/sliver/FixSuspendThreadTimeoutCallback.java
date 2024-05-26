package com.knightboost.sliver;

import androidx.annotation.Keep;

@Keep
public interface FixSuspendThreadTimeoutCallback {
  public void triggerSuspendTimeout();
  public void onError(String error);

  public void hookSuspendThreadByIdSuccess();
  public void hookSuspendThreadByPeerWarningSuccess();
}
