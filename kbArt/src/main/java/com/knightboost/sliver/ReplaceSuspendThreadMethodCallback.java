package com.knightboost.sliver;

import androidx.annotation.Keep;

@Keep
public interface ReplaceSuspendThreadMethodCallback {
    public void onReplaceSuccess();
    public void onError(String error);
}
