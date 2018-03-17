#ifndef __THREAD_H_
#define __THREAD_H_

#include "nocopyable.h"

#include "process.h"
#include "windows.h"

namespace jpp {
	class CThread : protected nocopyable
	{
	public:
		CThread() : m_hThread(INVALID_HANDLE_VALUE), m_running(false) {}

		virtual ~CThread() { stop(); };

		bool start() {
			if (isRunning()) {
				return false;
			}

			setRunning(true);

			m_hThread = (HANDLE)_beginthreadex(NULL, 0, threadProc, this, 0, NULL);

			return INVALID_HANDLE_VALUE != m_hThread;
		}

		bool stop() {
			if (isRunning()) {
				setRunning(false);
				WaitForSingleObject(m_hThread, INFINITE);
			}
			if (INVALID_HANDLE_VALUE != m_hThread) {
				CloseHandle(m_hThread);
				m_hThread = INVALID_HANDLE_VALUE;
			}
			return true;
		}

		void join() {
			if (INVALID_HANDLE_VALUE != m_hThread) {
				WaitForSingleObject(m_hThread, INFINITE);
			}
		}

		void detach() {
			if (INVALID_HANDLE_VALUE != m_hThread) {
				CloseHandle(m_hThread);
			}
		}

		bool isRunning() {
			if (INVALID_HANDLE_VALUE != m_hThread && WaitForSingleObject(m_hThread, 0) == WAIT_TIMEOUT) {
				return true;
			}
			return false;
		}

	protected:
		virtual void running() = 0;

		bool looping() {
			return InterlockedCompareExchange(&m_running, false, false);
		}

	private:

		void setRunning(bool flag) { InterlockedExchange( &m_running, flag); };

		static unsigned __stdcall threadProc(PVOID param)
		{
			((CThread *)param)->running();
			return 0;
		}

	private:
		HANDLE									m_hThread;
		volatile unsigned int					m_running;
	};
}
#endif