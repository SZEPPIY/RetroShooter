#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cstdlib>
#include <memory>
#include <string>


class Laser {
private:
    sf::RectangleShape sprite;
    float movementSpeed;

public:
    Laser(sf::Vector2f position) {
        this->movementSpeed = -10.f;
        this->sprite.setSize({ 4.f, 15.f });
        this->sprite.setFillColor(sf::Color::Red);
        this->sprite.setPosition(position);
    }

    void update() {
        this->sprite.move({ 0.f, this->movementSpeed });
    }

    void render(sf::RenderWindow& target) {
        target.draw(this->sprite);
    }

    sf::Vector2f getPosition() const {
        return this->sprite.getPosition();
    }

    sf::FloatRect getBounds() const {
        return this->sprite.getGlobalBounds();
    }
};


class Heart {
private:
    sf::RectangleShape sprite;
    float movementSpeed;

public:
    Heart(sf::Vector2f position) {
        this->movementSpeed = 3.f; 
        this->sprite.setSize({ 20.f, 20.f });
        this->sprite.setFillColor(sf::Color::Red); 
        this->sprite.setPosition(position);
    }

    void update() {
        this->sprite.move({ 0.f, this->movementSpeed });
    }

    void render(sf::RenderWindow& target) {
        target.draw(this->sprite);
    }

    sf::FloatRect getBounds() const {
        return this->sprite.getGlobalBounds();
    }

    sf::Vector2f getPosition() const {
        return this->sprite.getPosition();
    }
};


class Player {
private:
    sf::RectangleShape sprite;
    float movementSpeed;

    float fireCooldown;
    float fireCooldownMax;

    int ammo;
    int hp;
    int maxHp;

    sf::Font font;
    sf::Text uiText;

    void initSprite() {
        sprite.setSize({ 60.f, 40.f });
        sprite.setFillColor(sf::Color::White);
        sprite.setPosition({ 370.f, 500.f });
    }

    void initUI() {
        
        if (!this->font.openFromFile("arial.ttf")) {
            
        }

        this->uiText.setFont(this->font);
        this->uiText.setCharacterSize(24);
        this->uiText.setFillColor(sf::Color::Yellow);
        this->uiText.setPosition({ 10.f, 10.f });
    }

public:
    
    Player() : uiText(this->font) {
        this->movementSpeed = 8.f;
        this->fireCooldownMax = 10.f;
        this->fireCooldown = this->fireCooldownMax;
        this->ammo = 100;
        this->maxHp = 5; 
        this->hp = 3;
        this->initSprite();
        this->initUI();
    }

    void move(const float dirX, const float dirY) {
        this->sprite.move({ dirX * this->movementSpeed, dirY * this->movementSpeed });
    }

    sf::Vector2f getPosition() const {
        return this->sprite.getPosition();
    }

    sf::FloatRect getBounds() const {
        return this->sprite.getGlobalBounds();
    }

    int getHp() const {
        return this->hp;
    }

    void gainAmmo() {
        this->ammo += 2;
    }

    void gainHp() {
        if (this->hp < this->maxHp) {
            this->hp += 1;
        }
    }

    void loseHp() {
        if (this->hp > 0) {
            this->hp -= 1;
        }
    }

    void update(std::vector<Laser>& lasers) {
        sf::Vector2f currentPos = this->sprite.getPosition();

        if (this->fireCooldown < this->fireCooldownMax) {
            this->fireCooldown += 1.f;
        }

        
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) && currentPos.x > 0.f) {
            this->move(-1.f, 0.f);
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) && currentPos.x < 740.f) {
            this->move(1.f, 0.f);
        }

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && this->fireCooldown >= this->fireCooldownMax && this->ammo > 0) {
            lasers.push_back(Laser({ currentPos.x + 28.f, currentPos.y }));
            this->fireCooldown = 0.f;
            this->ammo -= 1;
        }

        
        if (this->hp > 0) {
            this->uiText.setString("Ammo: " + std::to_string(this->ammo) + " | HP: " + std::to_string(this->hp));
        }
        else {
            this->uiText.setString("GAME OVER");
            this->uiText.setFillColor(sf::Color::Red);
            this->uiText.setPosition({ 320.f, 250.f }); 
        }
    }

    void render(sf::RenderWindow& target) {
        if (this->hp > 0) {
            target.draw(this->sprite);
        }
        target.draw(this->uiText);
    }
};


class Enemy {
protected:
    sf::RectangleShape sprite;
    float movementSpeed;

public:
    Enemy(sf::Vector2f position) {
        this->movementSpeed = 0.5f;
        this->sprite.setPosition(position);
    }

    virtual ~Enemy() = default;

    virtual void update() {
        this->sprite.move({ 0.f, this->movementSpeed });
    }

    void render(sf::RenderWindow& target) {
        target.draw(this->sprite);
    }

    sf::Vector2f getPosition() const {
        return this->sprite.getPosition();
    }

    sf::FloatRect getBounds() const {
        return this->sprite.getGlobalBounds();
    }
};


class Ufo : public Enemy {
public:
    Ufo(sf::Vector2f position) : Enemy(position) {
        this->sprite.setSize({ 40.f, 30.f });
        this->sprite.setFillColor(sf::Color::Green);
    }
};

class Sarkany : public Enemy {
public:
    Sarkany(sf::Vector2f position) : Enemy(position) {
        this->sprite.setSize({ 60.f, 40.f });
        this->sprite.setFillColor(sf::Color::Magenta);
    }
};


int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Retro Shooter");
    window.setFramerateLimit(60);

    Player player;
    std::vector<Laser> lasers;
    std::vector<std::unique_ptr<Enemy>> enemies; 
    std::vector<Heart> hearts;                   

    
    float spawnTimer = 0.f;
    float spawnTimerMax = 40.f;

    
    float heartSpawnTimer = 0.f;
    float heartSpawnTimerMax = 300.f; 

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        
        if (player.getHp() > 0) {
            player.update(lasers);

            
            for (size_t i = 0; i < lasers.size(); i++) {
                lasers[i].update();
            }

            auto laserIt = lasers.begin();
            while (laserIt != lasers.end()) {
                if (laserIt->getPosition().y < -20.f) {
                    laserIt = lasers.erase(laserIt);
                }
                else {
                    ++laserIt;
                }
            }

            
            for (size_t i = 0; i < hearts.size(); i++) {
                hearts[i].update();
            }

            auto heartIt = hearts.begin();
            while (heartIt != hearts.end()) {
                if (heartIt->getPosition().y > 620.f) {
                    heartIt = hearts.erase(heartIt);
                }
                else {
                    ++heartIt;
                }
            }

            
            if (heartSpawnTimer < heartSpawnTimerMax) {
                heartSpawnTimer += 1.f;
            }
            else {
                
                if (rand() % 100 < 40) {
                    float randomX = static_cast<float>(rand() % 740);
                    hearts.push_back(Heart({ randomX, -40.f }));
                }
                heartSpawnTimer = 0.f;
            }

            
            if (spawnTimer < spawnTimerMax) {
                spawnTimer += 1.f;
            }
            else {
                float randomX = static_cast<float>(rand() % 740);
                int enemyType = rand() % 2;

                if (enemyType == 0) {
                    enemies.push_back(std::make_unique<Ufo>(sf::Vector2f({ randomX, -40.f })));
                }
                else {
                    enemies.push_back(std::make_unique<Sarkany>(sf::Vector2f({ randomX, -40.f })));
                }
                spawnTimer = 0.f;
            }

            
            for (size_t i = 0; i < enemies.size(); i++) {
                enemies[i]->update();
            }

            auto enemyIt = enemies.begin();
            while (enemyIt != enemies.end()) {
                if ((*enemyIt)->getPosition().y > 620.f) {
                    player.loseHp(); 
                    enemyIt = enemies.erase(enemyIt);
                }
                else {
                    ++enemyIt;
                }
            }

            
            auto currentLaser = lasers.begin();
            while (currentLaser != lasers.end()) {
                auto enemyIt = enemies.begin();
                bool laserDestroyed = false;

                while (enemyIt != enemies.end()) {
                    if (currentLaser->getBounds().findIntersection((*enemyIt)->getBounds()).has_value()) {
                        player.gainAmmo(); 
                        enemyIt = enemies.erase(enemyIt);
                        currentLaser = lasers.erase(currentLaser);
                        laserDestroyed = true;
                        break;
                    }
                    else {
                        ++enemyIt;
                    }
                }

                if (!laserDestroyed) {
                    ++currentLaser;
                }
            }

            
            auto heartCollisionIt = hearts.begin();
            while (heartCollisionIt != hearts.end()) {
                if (player.getBounds().findIntersection(heartCollisionIt->getBounds()).has_value()) {
                    player.gainHp(); // Extra élet elkapáskor!
                    heartCollisionIt = hearts.erase(heartCollisionIt);
                }
                else {
                    ++heartCollisionIt;
                }
            }
        }
        else {
            
            player.update(lasers);
        }

        
        window.clear(sf::Color::Black);

        player.render(window);

        
        if (player.getHp() > 0) {
            for (size_t i = 0; i < lasers.size(); i++) {
                lasers[i].render(window);
            }

            for (size_t i = 0; i < enemies.size(); i++) {
                enemies[i]->render(window);
            }

            for (size_t i = 0; i < hearts.size(); i++) {
                hearts[i].render(window);
            }
        }

        window.display();
    }

    return 0;
}