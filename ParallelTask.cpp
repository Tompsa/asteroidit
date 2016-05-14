#include "ParallelTask.h"


ParallelTask::ParallelTask()
: _thread(&ParallelTask::runTask, this)
, _finished(false)
{
}

void ParallelTask::execute()
{
	_finished = false;
	_elapsedTime.restart();
	_thread.launch();
}

bool ParallelTask::isFinished()
{
	sf::Lock lock(_mutex);
	return _finished;
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(_mutex);

	// 100% at 10 seconds of elapsed time
	return _elapsedTime.getElapsedTime().asSeconds() / 10.f;
}

void ParallelTask::runTask()
{
	// Dummy task - stall 10 seconds
	bool ended = false;
	while (!ended)
	{
		sf::Lock lock(_mutex); // Protect the clock 
		if (_elapsedTime.getElapsedTime().asSeconds() >= 10.f)
			ended = true;
	}

	{ // _finished may be accessed from multiple threads, protect it
		sf::Lock lock(_mutex);
		_finished = true;
	}	
}
