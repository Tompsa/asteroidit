#ifndef PARALLELTASK_H
#define PARALLELTASK_H

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>


class ParallelTask
{
	public:
							ParallelTask();
		void				execute();
		bool				isFinished();
		float				getCompletion();

	private:
		void				runTask();

	private:
		sf::Thread			_thread;
		bool				_finished;
		sf::Clock			_elapsedTime;
		sf::Mutex			_mutex;
};

#endif // PARALLELTASK_H
