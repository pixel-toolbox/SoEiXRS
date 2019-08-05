#include <atomic>
#include <thread>

#ifndef CLASSES_UITHREAD_HPP_
#define CLASSES_UITHREAD_HPP_

namespace SoEiXRS {

class UIThread {
	public:
		UIThread();
		virtual ~UIThread();

	private:
		static void loop(std::atomic<bool>* keepRunning);

		std::atomic<bool> keepRunning;

		std::thread threadLoop;
};

} /* namespace SoEiXRS */

#endif /* CLASSES_UITHREAD_HPP_ */
