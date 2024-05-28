package com.knightboost.optimize;

import androidx.annotation.Keep;

@Keep
public interface FixSuspendThreadTimeoutCallback {
  /**
   * 系统触发 suspendTimeout的回调
   */
  public void triggerSuspendTimeout();
  public void onError(String error);

  /**
   * 替换 SuspendThreadByPeer 为 suspendThreadById 成功
   */
  public void hookSuspendThreadByIdSuccess();

  public void hookSuspendThreadByPeerWarningSuccess();
}
