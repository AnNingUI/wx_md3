#ifndef MD3ANIMATOR_H
#define MD3ANIMATOR_H

#include <wx/wx.h>
#include <wx/timer.h>
#include <memory>
#include <vector>
#include <functional>
#include <map>

namespace wx_md3 {

    // Animation types as defined in the blueprint
    enum class MD3AnimationType {
        Ripple,
        Elevation,
        Hover,
        LabelFloat,
        ThumbSlide,
        ScaleFade,
        IndicatorSlide,
        CursorBlink,
        ProgressAnimate,
        Custom,
        Count // Must be last
    };

    // Easing functions for animations
    enum class MD3Easing {
        Linear,
        EaseIn,
        EaseOut,
        EaseInOut,
        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic
    };

    // Animation state
    enum class MD3AnimationState {
        Stopped,
        Playing,
        Paused,
        Completed
    };

    // Animation value type - can be float, color, position, etc.
    struct MD3AnimationValue {
        enum class Type {
            Float,
            Int,
            Point,
            Color
        } type;
        union {
            float floatValue;
            int intValue;
            struct {
                float x, y;
            } pointValue;
            struct {
                unsigned char r, g, b, a;
            } colorValue;
        };

        MD3AnimationValue() : type(Type::Float), floatValue(0.0f) {}
        explicit MD3AnimationValue(float f) : type(Type::Float), floatValue(f) {}
        explicit MD3AnimationValue(int i) : type(Type::Int), intValue(i) {}
        MD3AnimationValue(float x, float y) : type(Type::Point) { pointValue.x = x; pointValue.y = y; }
        MD3AnimationValue(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
            : type(Type::Color) { colorValue.r = r; colorValue.g = g; colorValue.b = b; colorValue.a = a; }
    };

    // Base animation class
    class MD3Animation {
    public:
        MD3Animation(MD3AnimationType type, long duration = 300, MD3Easing easing = MD3Easing::EaseInOut);
        virtual ~MD3Animation();

        // Start/stop animation
        void Start();
        void Stop();
        void Pause();
        void Resume();

        // Update animation (called by animator)
        virtual void Update(float deltaTime) = 0;
        virtual bool IsCompleted() const = 0;

        // Getters
        MD3AnimationType GetType() const { return m_type; }
        MD3AnimationState GetState() const { return m_state; }
        long GetDuration() const { return m_duration; }
        long GetElapsedTime() const { return m_elapsedTime; }

        // Callbacks
        using AnimationCallback = std::function<void()>;
        void SetOnStartCallback(AnimationCallback callback) { m_onStart = callback; }
        void SetOnUpdateCallback(AnimationCallback callback) { m_onUpdate = callback; }
        void SetOnCompleteCallback(AnimationCallback callback) { m_onComplete = callback; }

        // Easing function helpers (public for external use)
        static float Ease(float t, MD3Easing easing);
        static float Lerp(float start, float end, float t);

    protected:
        MD3AnimationType m_type;
        MD3AnimationState m_state;
        long m_duration;
        long m_elapsedTime;
        MD3Easing m_easing;

        AnimationCallback m_onStart;
        AnimationCallback m_onUpdate;
        AnimationCallback m_onComplete;
    };

    // Simple property animation
    template<typename T>
    class MD3PropertyAnimation : public MD3Animation {
    public:
        MD3PropertyAnimation(MD3AnimationType type, T* target, T startValue, T endValue,
                           long duration = 300, MD3Easing easing = MD3Easing::EaseInOut)
            : MD3Animation(type, duration, easing), m_target(target), m_startValue(startValue), m_endValue(endValue) {}

        void Update(float deltaTime) override {
            if (m_state != MD3AnimationState::Playing) return;

            m_elapsedTime += static_cast<long>(deltaTime);
            if (m_elapsedTime >= m_duration) {
                if (m_target) *m_target = m_endValue;
                m_state = MD3AnimationState::Completed;
                if (m_onComplete) m_onComplete();
                return;
            }

            float t = static_cast<float>(m_elapsedTime) / static_cast<float>(m_duration);
            t = Ease(t, m_easing);

            if (m_target) *m_target = Lerp(m_startValue, m_endValue, t);
            if (m_onUpdate) m_onUpdate();
        }

        bool IsCompleted() const override {
            return m_state == MD3AnimationState::Completed;
        }

    private:
        T* m_target;
        T m_startValue;
        T m_endValue;

        static T Lerp(const T& start, const T& end, float t) {
            // Default implementation for numeric types
            return start + (end - start) * t;
        }
    };

    // Main animator class
    class MD3Animator : public wxEvtHandler {
    public:
        MD3Animator();
        ~MD3Animator();

        // Singleton access
        static MD3Animator& GetInstance();

        // Animation management
        template<typename T>
        std::shared_ptr<MD3PropertyAnimation<T>> CreatePropertyAnimation(
            MD3AnimationType type, T* target, T startValue, T endValue,
            long duration = 300, MD3Easing easing = MD3Easing::EaseInOut);

        void AddAnimation(std::shared_ptr<MD3Animation> animation);
        void RemoveAnimation(std::shared_ptr<MD3Animation> animation);
        void ClearAnimations();

        // Start/stop the animator
        void Start();
        void Stop();

        // Update all animations (called by timer)
        void OnTimer(wxTimerEvent& event);

        // Utility functions
        static wxColour LerpColour(const wxColour& start, const wxColour& end, float t);

    private:
        wxTimer m_timer;
        std::vector<std::shared_ptr<MD3Animation>> m_animations;
        bool m_isRunning;
        static std::unique_ptr<MD3Animator> s_instance;
    };

    // Template implementation for CreatePropertyAnimation
    template<typename T>
    std::shared_ptr<MD3PropertyAnimation<T>> MD3Animator::CreatePropertyAnimation(
        MD3AnimationType type, T* target, T startValue, T endValue,
        long duration, MD3Easing easing) {
        auto animation = std::make_shared<MD3PropertyAnimation<T>>(
            type, target, startValue, endValue, duration, easing);
        AddAnimation(animation);
        return animation;
    }

} // namespace wx_md3

#endif // MD3ANIMATOR_H