#ifndef LOADINGSTATE_H
#define LOADINGSTATE_H

#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ParallelTask.h"


class LoadingState : public State
{
public:
	LoadingState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

	void					setCompletion(float percent);

private:
	sf::Text				_loadingText;
	sf::RectangleShape		_progressBarBackground;
	sf::RectangleShape		_progressBar;

	ParallelTask			_loadingTask;
};

#endif // LOADINGSTATE_H
