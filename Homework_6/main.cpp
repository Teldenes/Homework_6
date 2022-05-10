#include <iostream>
#include <string>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

//using namespace std;

class AnimatedSprite : public sf::Sprite
 {
  public:
    AnimatedSprite(int fps_, std::string path_)
     {
      fps = fps_;
      path = path_;
     }

    void add_animation_frame(sf::Rect<int> frame)
     {
      frames.emplace_back(frame);
     }

    void animation(sf::Time &elapsed)
     {
      slf += elapsed;
      if(slf.asSeconds() >= 1/fps)
       {
        it++;
        if(it==6){it=0;}
        slf = slf-slf;
       }
      setTextureRect(frames[it]);

      speed_y += g*elapsed.asSeconds(); // acceleration
      std::cout << speed_y << std::endl;


      move(0, speed_y*elapsed.asSeconds()); // falling
     }




  private:
    float fps;
    int it=0;
    sf::Time slf; // since last frame
    std::string path;
    std::vector<sf::Rect<int>> frames;
    float g = 200; //px/s
    int speed_x = 0;
    float speed_y = 0;
 };

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;
    sf::Clock clock1;

    sf::Texture tex;
    if (!tex.loadFromFile("sprites\\Character\\character.png")) {
        std::cerr << "Could not load texture" << std::endl;
    }

    AnimatedSprite hero(10, "sprites\\Character\\character.png");
    hero.setTexture(tex);


    //hero.add_animation_frame(sf::IntRect(0, 0, 50, 37)); // hero standing frame 1
    //hero.add_animation_frame(sf::IntRect(50, 0, 50, 37)); // hero standing frame 2
    //hero.add_animation_frame(sf::IntRect(100, 0, 50, 37)); // hero standing frame 3
    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37)); // hero running frame 1

    while (window.isOpen())
     {
      sf::Time elapsed = clock.restart();


      sf::Event event;
      while (window.pollEvent(event))
       {
        if (event.type == sf::Event::Closed)
         {
          window.close();
         }
       }

      window.clear(sf::Color::Black);
      window.draw(hero);

      hero.animation(elapsed);

      window.display();
     }
    return 0;

}
