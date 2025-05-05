#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

using namespace std;

struct Button {
	sf::Text label;
	sf::RectangleShape box;
};

std::vector<Button> createButtons(const sf::Font& font) {
	std::vector<Button> buttons;

	std::vector<std::string> labels = { "Play", "Menu", "Quit" };
	float startY = 200;
	float spacing = 200;

	for (size_t i = 0; i < labels.size(); ++i) {
		Button btn;

		btn.box.setSize({ 300, 100 });
		btn.box.setFillColor(sf::Color::Blue);
		btn.box.setPosition(1500 / 2 - 150, startY + spacing * i);

		btn.label.setFont(font);
		btn.label.setString(labels[i]);
		btn.label.setCharacterSize(50);
		btn.label.setFillColor(sf::Color::White);
		btn.label.setOutlineThickness(3);
		btn.label.setOutlineColor(sf::Color::Black);

		sf::FloatRect textBounds = btn.label.getLocalBounds();
		btn.label.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
		btn.label.setPosition(
			btn.box.getPosition().x + btn.box.getSize().x / 2,
			btn.box.getPosition().y + btn.box.getSize().y / 2
		);

		buttons.push_back(btn);
	}

	return buttons;
}


class Player {
private:
	
	float speed = 60.0f;
	float vspeed = 40.0f;
	float gravity = 100.0f;
	bool jumping = false;
	bool crouching = false;
	float jumpstrength = -100.0f;
	sf::Vector2f velocity;
	

public:
	int health = 10;
	sf::RectangleShape shape;
	string name;
	bool Alive = true;
	Player(float x, float y){
		shape.setSize(sf::Vector2f(50.f, 50.0f));
		shape.setFillColor(sf::Color::Blue);
		shape.setPosition(x, y);
		shape.getPosition();


	}
	void update(float dt) {
		sf::FloatRect playerbounds = shape.getGlobalBounds();
	}
	void display() const {
		cout << "you Have " << health << "health and " << speed << "Speed\n\n"; 
	}
	
	void move(float dt) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			shape.move((-speed * 3) * dt, 0); // Move left
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			shape.move((speed * 3) * dt, 0); // Move right
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			jump();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			Duck();
		}
	}
	void applyGravity(float deltaTime) {
		velocity.y += gravity * deltaTime;
		shape.move(0, (velocity.y * 3) * deltaTime);

	}
	void Collision(const sf::RectangleShape& Ground) {
		// basic ground collision
		if (shape.getGlobalBounds().intersects(Ground.getGlobalBounds())) {
			shape.setPosition(shape.getPosition().x, Ground.getPosition().y - shape.getSize().y);
			velocity.y = 0;
			jumping = false;
		}
	}
	void Hit(const sf::CircleShape enemy){
		if (shape.getGlobalBounds().intersects(enemy.getGlobalBounds())) {
			health -= 1;
		}
	}
	void Death() {
		if (health == 0) {
			Alive = false;
		}
	}
	void jump() {
		if (!jumping) {
			velocity.y = jumpstrength;
			jumping = true;
		}
	}
	void Duck() {
		// Crouch
		if (shape.getSize().y == 50) { // Prevent constant resizing
			shape.setSize(sf::Vector2f(50, 25));
			shape.move(0, 25);
		}
		else {
			// Stand up
			if (shape.getSize().y == 25) {
				shape.setSize(sf::Vector2f(50, 50));
				shape.move(0, -25);
			}
		}
	}

	sf::FloatRect const getPosition(){
		return shape.getGlobalBounds();
	}
	void setPosition(float x,float y) {
		return shape.setPosition(x, y);
	}
	void updatespeed() {
		vspeed += 10;
	}

};
class Bullet {
public:
	string name;
	sf::CircleShape shape;
	sf::FloatRect bulletbounds = shape.getGlobalBounds();
	int speed = 300;
	int power = 10;
	bool hit = false;
	sf::Vector2f velocity;
	int spawnpoint = rand() % 4 + 1;

	Bullet(float x, float y){
		shape.setRadius(30.0f);
		shape.setFillColor(sf::Color::Yellow);
		shape.setPosition(x, y);
		speed = 300;
		power = 10;
	}

	sf::FloatRect getPosition() {
		return shape.getGlobalBounds();
	}
	void Spawn(const sf::RectangleShape& Player) {
		spawnpoint = rand() % 4 + 1;
		switch (spawnpoint)
		{
		case 1: // From top
			shape.setPosition(Player.getPosition().x, 0);
			velocity = sf::Vector2f(0, speed); // move down
			break;
		case 2: // From bottom
			shape.setPosition(Player.getPosition().x, 800);
			velocity = sf::Vector2f(0, -speed); // move up
			break;
		case 3: // From left
			shape.setPosition(0, Player.getPosition().y);
			velocity = sf::Vector2f(speed, 0); // move right
			break;
		case 4: // From right
			shape.setPosition(1500, Player.getPosition().y);
			velocity = sf::Vector2f(-speed, 0); // move left
			break;
		default:
			shape.setPosition(Player.getPosition().x, 0);
			velocity = sf::Vector2f(0, speed);
			break;
		}
	}
	void update(float dt) {
		shape.move(velocity * dt);
	}
};

class Floor {
public:
	string name;
	sf::RectangleShape shape;
	sf::FloatRect base = shape.getGlobalBounds();
	int length = 200.0f;
	int width = 1500.0f;

	Floor(float x, float y) {
		shape.setSize(sf::Vector2f(width, length));
		shape.setFillColor(sf::Color::Black);
		shape.setPosition(x, y);


	}
	sf::FloatRect getPosition() {
		return shape.getGlobalBounds();
	}
};

enum GameState {
	Title,
	Game,
};

void TitleScreen(sf::Font& font, sf::RenderWindow& window, sf::Vector2f mouseWorldPos, GameState& state, bool Play);
void Levelloop(sf::RenderWindow& window, Player& Hero, Floor& floor, std::vector<Bullet>& bullets,
	sf::Text& Time, sf::Text& Level, sf::Text& Health, std::vector<Floor>& Platforms,
	sf::Clock& clock, sf::Clock& bulletClock, sf::Clock& Gametime, sf::Clock& TimeofLevel,
	float& Speed_diff, int& Levels);
void MenuScreen(bool pause, int Play, int Quit);
void GameOverScreen(int Play, bool pause);



int main() {
	bool Play = true;
	bool Quit = false;
	bool pause = false;
	sf::Clock clock;
	sf::Clock bulletClock;
	sf::Clock Gametime;
	sf::Clock TimeofLevel;


	float Speed_diff = 300.0f;
	int Levels = 0;

	sf::Font font;
	font.loadFromFile("Fonts/Daydream.ttf");
	
	sf::Text Time;
	Time.setFont(font);
	Time.setFillColor(sf::Color::White);
	Time.setPosition(100, 5);
	Time.setCharacterSize(30);
	Time.setOutlineThickness(5);
	Time.setOutlineColor(sf::Color::Black);

	sf::Text Level;
	Level.setFont(font);
	Level.setFillColor(sf::Color::White);
	Level.setPosition(1200/2, 0);
	Level.setCharacterSize(50);
	Level.setOutlineThickness(5);
	Level.setOutlineColor(sf::Color::Black);

	sf::Text Health;
	Health.setFont(font);
	Health.setFillColor(sf::Color::Blue);
	Health.setCharacterSize(10);
	Health.setOutlineThickness(5);
	Health.setOutlineColor(sf::Color::Black);

	std::vector <Bullet> bullets;
	std::vector <Floor> Platforms;
	Player Hero(1500 / 2, 400);
	Floor floor(0, 760);

	Platforms.push_back(Floor(100, 600));
	Platforms.push_back(Floor(400, 600));
	Platforms.push_back(Floor(700, 600));
	Platforms.push_back(Floor(1000, 600));
	Platforms.push_back(Floor(1300, 600));

	for (auto& plat : Platforms) {
		plat.shape.setSize(sf::Vector2f(100.0f, 50.f));
	}

	GameState state = GameState::Title;
	while (Play) {
		
		sf::RenderWindow window(sf::VideoMode({ 1500, 800 }), "SFML works!");
		sf::RectangleShape background({ 1500, 800 });
		background.setFillColor(sf::Color::Cyan);





		while (window.isOpen())
		{

			sf::Event event;
			while (window.pollEvent(event))
			{

			}
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

			window.clear();
			window.draw(background);

			switch (state)
			{
			case Title:
				TitleScreen(font, window, mouseWorldPos, state, Play);
				break;
			case Game:
				std::cout << "Drawing player at: " << Hero.shape.getPosition().x << ", " << Hero.shape.getPosition().y << std::endl;
				Levelloop(window, Hero, floor, bullets, Time, Level, Health, Platforms, clock, bulletClock, Gametime, TimeofLevel, Speed_diff, Levels);
				break;
			default:
				break;
			}

			
			window.display();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || Hero.Alive != true){
				Play = false;
				window.close();
			}
		}
	}
	return 0;
}

void TitleScreen(sf::Font& font, sf::RenderWindow& window, sf::Vector2f mouseWorldPos, GameState& state, bool Play){
	
	sf::Text Title;
	Title.setFont(font);
	Title.setFillColor(sf::Color::White);
	Title.setPosition(1200 / 2, 0);
	Title.setCharacterSize(100);
	Title.setOutlineThickness(5);
	Title.setOutlineColor(sf::Color::Black);

	std::vector<Button> buttons = createButtons(font);


	window.draw(Title);
	for (Button& btn : buttons) {

		window.draw(btn.box);
		window.draw(btn.label);


		if (btn.box.getGlobalBounds().contains(mouseWorldPos)) {
			btn.box.setFillColor(sf::Color::Yellow);


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				std::string label = btn.label.getString();
				if (label == "Play") {
					state = GameState::Game;
				}
				else if (label == "Quit") {
					Play = false;
					window.close();
				}
			}
		}
		else {
			btn.box.setFillColor(sf::Color::Blue);
		}
	}

}
void Levelloop(sf::RenderWindow& window, Player& Hero, Floor& floor, std::vector<Bullet>& bullets, sf::Text& Time, sf::Text& Level, sf::Text& Health, std::vector<Floor>& Platforms, sf::Clock& clock, sf::Clock& bulletClock, sf::Clock& Gametime, sf::Clock& TimeofLevel, float& Speed_diff, int& Levels){
	
	
	float dt = clock.restart().asSeconds();

	float speed = 200.0f;

	float elapsed = TimeofLevel.getElapsedTime().asSeconds();
	std::stringstream Timer;
	Timer << "Time:" << elapsed;
	Time.setString(Timer.str());

	float bulletTime = bulletClock.getElapsedTime().asSeconds();
	std::stringstream level;
	level << "Level " << Levels;
	Level.setString(level.str());

	float remainingH = 10;
	std::stringstream lives;
	lives << "Health: " << Hero.health << "/" << remainingH;
	Health.setString(lives.str());
	Health.setPosition(Hero.shape.getPosition().x, Hero.shape.getPosition().y - 30);

	if (Gametime.getElapsedTime().asSeconds() >= 10) {
		Speed_diff += 10;
		Gametime.restart();
	}

	if (elapsed >= 15) {
		Levels += 1;
		TimeofLevel.restart();
	}
	if (bulletTime >= .9f) {
		Bullet b(0, 0);
		b.speed = Speed_diff;
		b.Spawn(Hero.shape);
		bullets.push_back(b);
		bulletClock.restart();
	}


	Hero.move(dt);
	Hero.Death();
	Hero.applyGravity(dt);
	Hero.Collision(floor.shape);
	for (auto& plat : Platforms) {
		Hero.Collision(plat.shape);
	}


	for (auto& b : bullets) {
		Hero.Hit(b.shape);
	}

	for (auto it = bullets.begin(); it != bullets.end(); ) {
		if (it->shape.getPosition().x < 0 || it->shape.getPosition().x > 1500 ||
			it->shape.getPosition().y < 0 || it->shape.getPosition().y > 800) {
			it = bullets.erase(it);
		}
		else if (it->shape.getGlobalBounds().intersects(Hero.shape.getGlobalBounds())) {
			Hero.Hit(it->shape);
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}



	for (auto& plat : Platforms) {
		window.draw(plat.shape);
	}
	window.draw(floor.shape);
	window.draw(Hero.shape);
	window.draw(Health);

	for (auto& b : bullets) {
		b.update(dt);
		window.draw(b.shape);
	}
	window.draw(Time);
	window.draw(Level);
}
