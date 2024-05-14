package com.knightboost.sliver;

public interface HookSuspendThreadTimeoutCallback {
  public void triggerSuspendTimeout();
  public void onError(String error);
}
