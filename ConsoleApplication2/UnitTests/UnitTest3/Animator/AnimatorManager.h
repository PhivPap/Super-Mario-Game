#pragma once
#include "Animator.h"
#include <assert.h>
#include <set>

class AnimatorManager {
private:
    std::set<Animator*>     running, suspended;
    static AnimatorManager  singleton;
    AnimatorManager(void) = default;
    AnimatorManager(const AnimatorManager&) = delete;
    AnimatorManager(AnimatorManager&&) = delete;

public:
    void                            Register(Animator*);
    void                            Cancel(Animator*);
    void                            MarkAsRunning(Animator*);
    void                            MarkAsSuspended(Animator*);
    void                            Progress(timestamp_t);
    static AnimatorManager&         GetSingleton(void);
    static const AnimatorManager&   GetSingletonConst(void);
};