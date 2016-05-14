#include "SceneNode.h"
#include "Command.h"
#include "Foreach.h"

#include <algorithm>
#include <cassert>


SceneNode::SceneNode()
	: _children()
	, _parent(nullptr)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->_parent = this;
	_children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(_children.begin(), _children.end(), [&](Ptr& p) { return p.get() == &node; });
	assert(found != _children.end());

	Ptr result = std::move(*found);
	result->_parent = nullptr;
	_children.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt)
{
	FOREACH(Ptr& child, _children)
		child->update(dt);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	FOREACH(const Ptr& child, _children)
		child->draw(target, states);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->_parent)
		transform = node->getTransform() * transform;

	return transform;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	// Command current node, if category matches
	if (command.category & getCategory())
		command.action(*this, dt);

	// Command children
	FOREACH(Ptr& child, _children)
		child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
	return Category::Scene;
}
