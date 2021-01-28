#pragma once
#include "Animator.h"
#include "Types.h"
#include <assert.h>
#include <set>
#include <list>

class AnimatorManager {
private:
    std::set<Animator*>     running, suspended;
    std::list<Animator*>    garbage;
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
    void                            AddGarbage(Animator*);
    void                            GarbageCollect();
    void                            DestroyAll();
    void                            TimeShift(uint);
};