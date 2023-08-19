#include "MyCircle.h";


MyCircle::MyCircle()
{
	hue = 0.0f;
	saturation = 1.0f;
	value = 1.0f;
	this->setRadius(getRandomRad());
	this->initMass();
	this->randomizeSpeed();
	this->randomizeColor();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->setPosition(getRandomPos());
}

MyCircle::MyCircle(sf::Vector2f& pos)
{
	hue = 0.0f;
	saturation = 1.0f;
	value = 1.0f;
	this->setRadius(getRandomRad());
	this->initMass();
	this->randomizeSpeed();
	this->randomizeColor();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->setPosition(pos);
}

MyCircle::MyCircle(float w, float h, float xs, float ys, float rad)
{
	hue = 0.0f;
	saturation = 1.0f;
	value = 1.0f;
	this->setRadius(rad);
	this->setSpeed(xs, ys);
	this->initMass();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->randomizeColor();
	this->setPosition(w, h);
	std::cout << "CONSTRUCTOR" << "\n";
}

MyCircle::~MyCircle()
{

}


sf::Vector2f MyCircle::getSpeed() const
{
	return sf::Vector2f(this->_velocity.x, this->_velocity.y);
}

void MyCircle::setSpeed(float xs, float ys)
{
	this->_velocity.x = xs;
	this->_velocity.y = ys;
}

void MyCircle::invertXSpeed()
{
	this->_velocity.x *= -1.0f;
}
void MyCircle::invertYSpeed()
{
	this->_velocity.y *= -1.0f;
}

bool MyCircle::contains(sf::Vector2f& p) const
{
	float dx = std::pow((this->getPosition().x - p.x), 2);
	float dy = std::pow((this->getPosition().y - p.y), 2);
	float dist = std::sqrt(dx + dy);
	if (dist <= this->getRadius())
	{
		return true;
	}
	return false;
}

void MyCircle::randomizeColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	hue = dis(gen);
	value = 0.0f;
	sf::Color randCol = HSVtoRGB(hue, saturation, value);
	this->setFillColor(randCol);
}

void MyCircle::updateColor()
{
	value += 0.04f;
	hue += 0.1f;

	if (hue >= 360.0f)
	{
		hue -= 360.0f;
	}
	if (value >= 1.0f)
	{
		value = 1.0f;
	}

	sf::Color newColor = HSVtoRGB(hue, saturation, value);
	this->setFillColor(newColor);
}


void MyCircle::randomizeSpeed()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-5.34f, 5.02f);
	float velX = dis(gen);
	float velY = dis(gen);
	this->_velocity.x = velX;
	this->_velocity.y = velY;
}
void MyCircle::updateMovement()
{
	sf::VideoMode res = sf::VideoMode::getDesktopMode();
	int width = res.width;
	int height = res.height;
	if (this->getPosition().x >= width - this->getRadius() || this->getPosition().x <= this->getRadius())
	{
		this->invertXSpeed();
		this->randomizeColor();
	}
	if (this->getPosition().y >= height - this->getRadius() || this->getPosition().y <= this->getRadius())
	{
		this->invertYSpeed();
		this->randomizeColor();
	}
	float newX = std::clamp(this->getPosition().x + _velocity.x, this->getRadius(), width - this->getRadius());
	float newY = std::clamp(this->getPosition().y + _velocity.y, this->getRadius(), height - this->getRadius());
	this->setPosition(newX, newY);
}

bool MyCircle::isIntersect(const MyCircle& c2) const
{
	float dx = std::pow((c2.getPosition().x - this->getPosition().x), 2);
	float dy = std::pow((c2.getPosition().y - this->getPosition().y), 2);
	float dist = std::sqrt(dx + dy);
	if (dist < this->getRadius() + c2.getRadius())
	{
		return true;
	}
	else { return false; }
}

void MyCircle::resolveIntersections(MyCircle& c2)
{
	sf::Vector2f center1 = this->getPosition();
	sf::Vector2f center2 = c2.getPosition();

	float distance = std::sqrt((center2.x - center1.x) * (center2.x - center1.x) + (center2.y - center1.y) * (center2.y - center1.y));
	float sumRadii = this->getRadius() + c2.getRadius();
	float overlapDistance = sumRadii - distance;
	float separationDistance = overlapDistance / 2.0f;

	// Calculate the direction from circle1 to circle2
	sf::Vector2f separationDirection = (center2 - center1) / distance;

	// Move both circles away from each other
	this->setPosition(center1 - separationDirection * separationDistance);
	c2.setPosition(center2 + separationDirection * separationDistance);
	this->calculateCollision(c2);
}

void MyCircle::calculateCollision(MyCircle& c2)
{
	sf::Vector2f newVel1 = c2.getSpeed();
	sf::Vector2f newVel2 = this->getSpeed();
	this->_velocity = newVel1;
	c2._velocity = newVel2;
	this->randomizeColor();
	c2.randomizeColor();
}

float MyCircle::getMass() const
{
	return this->mass;
}

sf::Vector2f MyCircle::getRandomPos() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> xDis(this->getRadius(), sf::VideoMode::getDesktopMode().width - 2 * (this->getRadius()));
	std::uniform_real_distribution<float> yDis(this->getRadius(), sf::VideoMode::getDesktopMode().height - 2 * (this->getRadius()));
	return sf::Vector2f(xDis(gen), yDis(gen));
}

float MyCircle::getRandomRad() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(30.0f, 55.0f);
	return dis(gen);
}


void MyCircle::initMass()
{
	this->mass = this->getRadius() * 0.1f;
}

sf::Color MyCircle::HSVtoRGB(float h, float s, float v)
{
	int hi = static_cast<int>(h / 60) % 6;
	float f = h / 60 - std::floor(h / 60);
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	switch (hi) {
	case 0: return sf::Color(static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(t * 255), static_cast<sf::Uint8>(p * 255));
	case 1: return sf::Color(static_cast<sf::Uint8>(q * 255), static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(p * 255));
	case 2: return sf::Color(static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(t * 255));
	case 3: return sf::Color(static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(q * 255), static_cast<sf::Uint8>(v * 255));
	case 4: return sf::Color(static_cast<sf::Uint8>(t * 255), static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(v * 255));
	default: return sf::Color(static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(q * 255));
	}
}




