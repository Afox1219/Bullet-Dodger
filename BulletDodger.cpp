#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

using namespace std;

bool enableNormalBullets = true;
bool enableSmallBullets = true;
bool enableBigBullets = true;
bool enableWeirdBullets = true;
bool enableExplodingBullets = true;

struct Button {
	sf::Text label;
	sf::RectangleShape box;
};

std::vector<Button> createButtonsTitle(const sf::Font& font) {
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

std::vector<Button> createButtonsMenu(const sf::Font& font) {
	std::vector<Button> buttons;

	std::vector<std::string> labels = { "Back", "Bullets"};
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

std::vector<Button> createButtonsBullets(const sf::Font& font) {
	std::vector<Button> bullettypes;

	std::vector<std::string> labels = { "Normal", "Small" ,"Big", "Wierd" ,"Exploding"};
	float startX = 20;
	float spacing = 200;

	for (size_t i = 0; i < labels.size(); ++i) {
		Button toggle;

		toggle.box.setSize({ 100, 100 });
		toggle.box.setFillColor(sf::Color::Blue);
		toggle.box.setPosition((startX + spacing * i) + 250 , 800/2 - 150);

		toggle.label.setFont(font);
		toggle.label.setString(labels[i]);
		toggle.label.setCharacterSize(20);
		toggle.label.setFillColor(sf::Color::White);
		toggle.label.setOutlineThickness(3);
		toggle.label.setOutlineColor(sf::Color::Black);

		sf::FloatRect textBounds = toggle.label.getLocalBounds();
		toggle.label.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
		toggle.label.setPosition(
			toggle.box.getPosition().x + toggle.box.getSize().x / 2,
			toggle.box.getPosition().y + toggle.box.getSize().y / 2
		);

		bullettypes.push_back(toggle);
	}

	return bullettypes;
}
enum GameState {
	Title,
	Game,
	Menu,
};
enum BulletType {
	Normal,
	Small,
	Big,
	Weird,
	Exploding
};
enum Settings{
	Main,
	BulletMenu
};



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
	float health = 10;
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
			setCrouch(true);
		}
		else {
			setCrouch(false);
		}
	}
	void applyGravity(float deltaTime) {
		velocity.y += gravity * deltaTime;
		shape.move(0, (velocity.y * 3) * deltaTime);

	}
	void Collision(const sf::RectangleShape& Ground) {
		if (shape.getGlobalBounds().intersects(Ground.getGlobalBounds())) {
			shape.setPosition(shape.getPosition().x, Ground.getPosition().y - shape.getSize().y );
			velocity.y = 0;
			jumping = false;
		}
	}
	void Hit(const sf::CircleShape enemy,float dmg){
		if (shape.getGlobalBounds().intersects(enemy.getGlobalBounds())) {
			health -= dmg;
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
	void setCrouch(bool isCrouching) {
		if (isCrouching && !crouching) {
			// Crouch
			crouching = true;
			shape.setSize(sf::Vector2f(50, 25));
			shape.move(0, 25); // lower player
		}
		else if (!isCrouching && crouching) {
			// Stand up
			crouching = false;
			shape.setSize(sf::Vector2f(50, 50));
			shape.move(0, -25); // raise player
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
	BulletType type;
	sf::CircleShape shape;
	sf::FloatRect bulletbounds = shape.getGlobalBounds();
	int speed = 300;
	float power = 1;
	sf::Vector2f velocity;
	int spawnpoint = rand() % 4 + 1;

	Bullet(BulletType bulletType, float x, float y){
		type = bulletType;
		configureBullet(bulletType);
		shape.setPosition(x, y);
	}

	void configureBullet(BulletType bulletType) {
		switch (bulletType) {
		case Normal:
		if(enableNormalBullets){
			shape.setRadius(30.f);
			shape.setFillColor(sf::Color::Yellow);
			speed = 300;
			power = 1;
		}
			break;
		case Small:
		if(enableSmallBullets){
			shape.setRadius(15.f);
			shape.setFillColor(sf::Color::Green);
			speed = 500;
			power = 0.5;
		}
			break;
		case Big:
		if(enableBigBullets){
			shape.setRadius(50.f);
			shape.setFillColor(sf::Color::Red);
			speed = 200;
			power = 2;
		}
			break;
		case Weird:
		if(enableWeirdBullets){
			shape.setRadius(25.f);
			shape.setFillColor(sf::Color::Magenta);
			speed = 250;
			power = 1;
		}
			break;
		case Exploding:
		if(enableExplodingBullets){
			shape.setRadius(20.f);
			shape.setFillColor(sf::Color::Black);
			speed = 300;
			power = 1;
		}
			break;
		}
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
	float speed = 300.0f;

	Floor(float x, float y) {
		shape.setSize(sf::Vector2f(width, length));
		shape.setFillColor(sf::Color::Black);
		shape.setPosition(x, y);


	}
	sf::FloatRect getPosition() {
		return shape.getGlobalBounds();
	}
	void move(float dt){
		shape.move(speed * dt, 0);
	}
};


void TitleScreen(sf::Font& font, sf::RenderWindow& window, sf::Vector2f mouseWorldPos, GameState& state, bool Play);
void Levelloop(sf::RenderWindow& window, Player& Hero, Floor& floor, std::vector<Bullet>& bullets, sf::Text& Time, sf::Text& Level, sf::Text& Health, std::vector<Floor>& Platforms, sf::Clock& clock, sf::Clock& bulletClock, sf::Clock& Gametime, sf::Clock& TimeofLevel, GameState& state, float& Speed_diff, int& Levels);
void MenuScreen(sf::Font& font, sf::RenderWindow& window, sf::Vector2f mouseWorldPos, GameState& state, Settings& Sets);
void GameOverScreen();



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
	Player Hero(1500 / 2, 710);
	Floor floor(0, 760);

	Platforms.push_back(Floor(0, 600));
	Platforms.push_back(Floor(375, 600));
	Platforms.push_back(Floor(750, 600));
	Platforms.push_back(Floor(1125, 600));


	for (auto& plat : Platforms) {
		plat.shape.setSize(sf::Vector2f(150.0f, 50.f));
	}

	GameState state = GameState::Title;
	Settings Sets = Settings::Main;
	
		
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
				Levelloop(window, Hero, floor, bullets, Time, Level, Health, Platforms, clock, bulletClock, Gametime, TimeofLevel, state, Speed_diff, Levels);
				break;
			case Menu:
				MenuScreen(font, window, mouseWorldPos, state, Sets);
			default:
				break;
			}

			
			window.display();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
				window.close();
				return 0;
			}
		}
	return 0;
}

void TitleScreen(sf::Font& font, sf::RenderWindow& window, sf::Vector2f mouseWorldPos, GameState& state, bool Play){
	
	sf::Text Title;
	Title.setFont(font);
	Title.setFillColor(sf::Color::White);
	Title.setPosition(800 / 2, 50);
	Title.setCharacterSize(50);
	Title.setOutlineThickness(5);
	Title.setOutlineColor(sf::Color::Black);
	Title.setString("BULLET DODGERS");
	std::vector<Button> buttons = createButtonsTitle(font);

	window.draw(Title);
	
	for (Button& btn : buttons) {



		if (btn.box.getGlobalBounds().contains(mouseWorldPos)) {
			btn.box.setFillColor(sf::Color::Yellow);


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				std::string label = btn.label.getString();
				if (label == "Play") {
					state = GameState::Game;
				}
				else if (label == "Menu") {
					state = GameState::Menu;
				}
				else if (label == "Quit") {
					Play = false;
					window.close();
					return;
				}
			}
		}
		else {
			btn.box.setFillColor(sf::Color::Blue);
		}
		window.draw(btn.box);
		window.draw(btn.label);

	}

}
void Levelloop(sf::RenderWindow& window, Player& Hero, Floor& floor, std::vector<Bullet>& bullets, sf::Text& Time, sf::Text& Level, sf::Text& Health, std::vector<Floor>& Platforms, sf::Clock& clock, sf::Clock& bulletClock, sf::Clock& Gametime, sf::Clock& TimeofLevel, GameState& state, float& Speed_diff, int& Levels){
	

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

	if (Hero.Alive != true) {
		state = GameState::Title;
		Hero.Alive = true;
		Hero.health = 10;
		Levels = 0;
	}


	if (Gametime.getElapsedTime().asSeconds() >= 10) {
		Speed_diff += 10;
		Gametime.restart();
	}

	if (elapsed >= 15) {
		Levels += 1;
		TimeofLevel.restart();
	}
	if (bulletTime >= .9f) {
		BulletType randomType = static_cast<BulletType>(rand() % 5);
		Bullet b(randomType,0, 0);
		b.speed = Speed_diff;
		b.Spawn(Hero.shape);
		bullets.push_back(b);
		bulletClock.restart();
	}

	if (Hero.shape.getPosition().y > 1000) {
		Hero.setPosition(700/ 2, 710);
	}

	Hero.move(dt);
	Hero.Death();
	Hero.applyGravity(dt);
	Hero.Collision(floor.shape);
	for (auto& plat : Platforms) {
		Hero.Collision(plat.shape);
		//plat.move(dt);
		for (auto diff = Platforms.begin(); diff != Platforms.end(); ) {
			if (diff->shape.getPosition().x < -200 || diff->shape.getPosition().x > 2000) {
				diff = Platforms.erase(diff);
			}
			else {
				++diff;
			}
		}
	}

	/*for (auto& b : bullets) {
		Hero.Hit(b.shape);
	}*/

	for (auto it = bullets.begin(); it != bullets.end(); ) {
		if (it->shape.getPosition().x < 0 || it->shape.getPosition().x > 1500 ||
			it->shape.getPosition().y < 0 || it->shape.getPosition().y > 800) {
			it = bullets.erase(it);
		}
		else if (it->shape.getGlobalBounds().intersects(Hero.shape.getGlobalBounds())) {
			Hero.health -= it->power;
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
void MenuScreen(sf::Font& font, sf::RenderWindow& window, sf::Vector2f mouseWorldPos, GameState& state, Settings& Sets) {
	
	std::vector<Button> bulletButtons;

	if (Sets == Settings::Main) {
		auto buttons = createButtonsMenu(font);
		for (auto& btn : buttons) {

			if (btn.box.getGlobalBounds().contains(mouseWorldPos)) {
				btn.box.setFillColor(sf::Color::Yellow);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (btn.label.getString() == "Back") {
						state = GameState::Title;
					}
					else if (btn.label.getString() == "Bullets") {
						Sets = Settings::BulletMenu;
					}
				}
			}
			window.draw(btn.box);
			window.draw(btn.label);
		}
	}

	if (Sets == Settings::BulletMenu && bulletButtons.empty()) {
		bulletButtons = createButtonsBullets(font);
	}

	for (size_t i = 0; i < bulletButtons.size(); ++i) {
		if (bulletButtons[i].box.getGlobalBounds().contains(mouseWorldPos)) {
			bulletButtons[i].box.setFillColor(sf::Color::Yellow);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				switch (i) {
				case 0: enableNormalBullets = !enableNormalBullets; break;
				case 1: enableSmallBullets = !enableSmallBullets; break;
				case 2: enableBigBullets = !enableBigBullets; break;
				case 3: enableWeirdBullets = !enableWeirdBullets; break;
				case 4: enableExplodingBullets = !enableExplodingBullets; break;
				}
			}
		}
		else {
			bool enabled = false;
			switch (i) {
			case 0: enabled = enableNormalBullets; break;
			case 1: enabled = enableSmallBullets; break;
			case 2: enabled = enableBigBullets; break;
			case 3: enabled = enableWeirdBullets; break;
			case 4: enabled = enableExplodingBullets; break;
			}
			bulletButtons[i].box.setFillColor(enabled ? sf::Color::Green : sf::Color::Red);
		}

		window.draw(bulletButtons[i].box);
		window.draw(bulletButtons[i].label);
	}

		// Optional: Add a Back button
		sf::RectangleShape backButton({ 150, 50 });
		backButton.setPosition(50, 700);
		backButton.setFillColor(sf::Color::Blue);
		sf::Text backText("Back", font, 20);
		backText.setPosition(75, 710);


		if (backButton.getGlobalBounds().contains(mouseWorldPos)){
			backButton.setFillColor(sf::Color::Yellow);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				Sets = Settings::Main;
			}
		}
		window.draw(backButton);
		window.draw(backText);
}