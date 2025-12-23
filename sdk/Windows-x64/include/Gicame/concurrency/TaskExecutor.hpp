#pragma once
#include "common.h"
#include "../utils/NotCopyable.h"
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <future>
#include <type_traits>
#include <exception>
#include <optional>


namespace Gicame::Concurrency {

	/**
	 * @brief A thread waiting for tasks
	 */
	class TaskExecutor {

		NOT_COPYABLE(TaskExecutor)

	public:   // Public types
		enum class CompletionState {
			NOT_DONE, DONE, DONE_WITH_ERRORS
		};

	private:  // Private data members
		std::thread worker;
		std::mutex mtx;
		std::condition_variable cvTaskAvailable;
		std::condition_variable cvTaskDone;
		std::function<void()> task;
		bool taskAvailable;
		bool stopFlag;
		CompletionState taskCompletionState;
		std::optional<std::exception> lastTaskException;

	private:
		// Function run by the worker thread
		void workerThreadBody();

	public:
		GICAME_API TaskExecutor();
		GICAME_API ~TaskExecutor();
		GICAME_API CompletionState waitForTaskCompletion();
		GICAME_API void submitVoidTask(const std::function<void()>&);
		GICAME_API std::optional<std::exception> getLastTaskException();
		
		template <typename Res>
		inline std::future<Res> submitTask(const std::function<Res()>& newTask) {
			std::promise<Res> pr;
			std::future<Res> future = pr.get_future();
			auto taskToSumbit = [&newTask, pr = std::move(pr)]() mutable {
				if constexpr (std::is_same_v<Res, void>) {
					newTask();
					pr.set_value();
				}
				else {
					pr.set_value(newTask());
				}
			};
			submitVoidTask([&]() { taskToSumbit(); });  // Cannot directly pass a mutable lambda
			return future;
		}

	};

};