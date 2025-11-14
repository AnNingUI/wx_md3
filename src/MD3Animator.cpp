#include "wx_md3/core/MD3Animator.h"
#include <wx/log.h>
#include <cmath>

namespace wx_md3 {

    // Static member initialization
    std::unique_ptr<MD3Animator> MD3Animator::s_instance = nullptr;

    // MD3Animation implementation
    MD3Animation::MD3Animation(MD3AnimationType type, long duration, MD3Easing easing)
        : m_type(type), m_state(MD3AnimationState::Stopped), m_duration(duration),
          m_elapsedTime(0), m_easing(easing) {
    }

    MD3Animation::~MD3Animation() {
        Stop();
    }

    void MD3Animation::Start() {
        if (m_state == MD3AnimationState::Stopped || m_state == MD3AnimationState::Completed) {
            m_state = MD3AnimationState::Playing;
            m_elapsedTime = 0;
            if (m_onStart) m_onStart();
        } else if (m_state == MD3AnimationState::Paused) {
            m_state = MD3AnimationState::Playing;
        }
    }

    void MD3Animation::Stop() {
        m_state = MD3AnimationState::Stopped;
        m_elapsedTime = 0;
    }

    void MD3Animation::Pause() {
        if (m_state == MD3AnimationState::Playing) {
            m_state = MD3AnimationState::Paused;
        }
    }

    void MD3Animation::Resume() {
        if (m_state == MD3AnimationState::Paused) {
            m_state = MD3AnimationState::Playing;
        }
    }

    // Easing function implementation
    float MD3Animation::Ease(float t, MD3Easing easing) {
        switch (easing) {
            case MD3Easing::Linear:
                return t;
            case MD3Easing::EaseIn:
                return t * t;
            case MD3Easing::EaseOut:
                return t * (2.0f - t);
            case MD3Easing::EaseInOut:
                return t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
            case MD3Easing::EaseInQuad:
                return t * t;
            case MD3Easing::EaseOutQuad:
                return 1.0f - (1.0f - t) * (1.0f - t);
            case MD3Easing::EaseInOutQuad:
                return t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
            case MD3Easing::EaseInCubic:
                return t * t * t;
            case MD3Easing::EaseOutCubic:
                return 1.0f - pow(1.0f - t, 3.0f);
            case MD3Easing::EaseInOutCubic:
                return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
            default:
                return t;
        }
    }

    float MD3Animation::Lerp(float start, float end, float t) {
        return start + (end - start) * t;
    }

    // MD3Animator implementation
    MD3Animator::MD3Animator() : m_isRunning(false) {
        Bind(wxEVT_TIMER, &MD3Animator::OnTimer, this);
        // 🔧 设置定时器的所有者为 this (MD3Animator)
        m_timer.SetOwner(this);
    }

    MD3Animator::~MD3Animator() {
        Stop();
        Unbind(wxEVT_TIMER, &MD3Animator::OnTimer, this);
    }

    MD3Animator& MD3Animator::GetInstance() {
        if (!s_instance) {
            s_instance = std::make_unique<MD3Animator>();
        }
        return *s_instance;
    }

    void MD3Animator::AddAnimation(std::shared_ptr<MD3Animation> animation) {
        if (animation) {
            m_animations.push_back(animation);
            if (m_isRunning && animation->GetState() == MD3AnimationState::Stopped) {
                animation->Start();
            }
        }
    }

    void MD3Animator::RemoveAnimation(std::shared_ptr<MD3Animation> animation) {
        m_animations.erase(
            std::remove_if(m_animations.begin(), m_animations.end(),
                          [&animation](const std::shared_ptr<MD3Animation>& anim) {
                              return anim == animation;
                          }),
            m_animations.end());
    }

    void MD3Animator::ClearAnimations() {
        m_animations.clear();
    }

    void MD3Animator::Start() {
        if (!m_isRunning) {
            m_isRunning = true;
            m_timer.Start(16); // ~60 FPS

            // Start all stopped animations
            for (auto& anim : m_animations) {
                if (anim->GetState() == MD3AnimationState::Stopped) {
                    anim->Start();
                }
            }
        }
    }

    void MD3Animator::Stop() {
        if (m_isRunning) {
            m_isRunning = false;
            m_timer.Stop();

            // Stop all animations
            for (auto& anim : m_animations) {
                anim->Stop();
            }
        }
    }

    void MD3Animator::OnTimer(wxTimerEvent& event) {
        if (!m_isRunning) return;

        // Update all animations
        auto it = m_animations.begin();
        while (it != m_animations.end()) {
            auto& anim = *it;
            if (anim->GetState() == MD3AnimationState::Playing) {
                anim->Update(16.0f); // 16ms per frame
            }

            // Remove completed animations
            if (anim->IsCompleted()) {
                it = m_animations.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Utility functions
    wxColour MD3Animator::LerpColour(const wxColour& start, const wxColour& end, float t) {
        unsigned char r = static_cast<unsigned char>(start.Red() + (end.Red() - start.Red()) * t);
        unsigned char g = static_cast<unsigned char>(start.Green() + (end.Green() - start.Green()) * t);
        unsigned char b = static_cast<unsigned char>(start.Blue() + (end.Blue() - start.Blue()) * t);
        unsigned char a = static_cast<unsigned char>(start.Alpha() + (end.Alpha() - start.Alpha()) * t);
        return wxColour(r, g, b, a);
    }

} // namespace wx_md3