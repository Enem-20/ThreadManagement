#ifndef C_TASK_HPP
#define C_TASK_HPP

#include <functional>
#include <future>
#include <memory>
#include <type_traits>
#include <utility>

class TaskBase {
public:
    virtual ~TaskBase() = default;
    virtual void invoke() = 0;
};

using TaskPtr = std::unique_ptr<TaskBase>;

template<typename R>
class TypedTask : public TaskBase {
public:
    using TaskType = std::packaged_task<R()>;

    explicit TypedTask(TaskType&& task)
        : _task(std::move(task)) {}

    void invoke() override {
        _task();
    }

    std::future<R> getFuture() {
        return _task.get_future();
    }

private:
    TaskType _task;
};

class Task {
public:
    Task() = default;

    template<typename Fn, typename... Args,
             typename R = std::invoke_result_t<std::decay_t<Fn>, std::decay_t<Args>...>>
    static std::pair<Task, std::future<R>> create(Fn&& fn, Args&&... args) {
        auto bound = std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...);
        std::packaged_task<R()> task(std::move(bound));
        auto future = task.get_future();
        Task t;
        t._task = std::make_unique<TypedTask<R>>(std::move(task));
        return { std::move(t), std::move(future) };
    }

    void operator()() {
        if (_task) _task->invoke();
    }

    bool valid() const { return static_cast<bool>(_task); }

private:
    TaskPtr _task;
};

#endif
