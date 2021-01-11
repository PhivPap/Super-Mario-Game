#include "AnimatorManager.h"

AnimatorManager AnimatorManager::singleton;

void AnimatorManager::Register(Animator* a) {
    assert(a->HasFinished());
    suspended.insert(a);
}

void AnimatorManager::Cancel(Animator* a) {
    assert(a->HasFinished());
    suspended.erase(a);
}

void AnimatorManager::MarkAsRunning(Animator* a) {
    assert(!a->HasFinished());
    suspended.erase(a);
    running.insert(a);
}

void AnimatorManager::MarkAsSuspended(Animator* a) {
    assert(a->HasFinished());
    running.erase(a);
    suspended.insert(a);
}

void AnimatorManager::Progress(timestamp_t curr_time) {
    auto copied(running);
    for (auto* a : copied)
        a->Progress(curr_time);
    GarbageCollect();
}

AnimatorManager& AnimatorManager::GetSingleton(void) {
    return singleton;
}

const AnimatorManager& AnimatorManager::GetSingletonConst(void) {
    return singleton;
}

void AnimatorManager::GarbageCollect() {
    while (!garbage.empty()) {
        auto* anim = garbage.front();
        garbage.pop_front();
        if (anim->HasFinished()) {
            Cancel(anim);
        }
        else {
            running.erase(anim);
        }
        delete anim;
    }
}

void AnimatorManager::AddGarbage(Animator* anim) {
    assert(anim);
    garbage.push_front(anim);
}