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

    void setBounds(int left, int right, int top, int bottom)
     {
      bound_top = top;
      bound_bottom = bottom;
      bound_right = right;
      bound_left = left;
     }

    void animation(sf::Time &elapsed)
     {
      sf::FloatRect b_guy = getGlobalBounds();

      speed_y += g*elapsed.asSeconds(); // acceleration

      if(b_guy.top+b_guy.height >= bound_bottom)
       {
        on_ground = true;
       }
      else
       {
        on_ground = false;
       }

      if(on_ground == true)// some very poor collision, scrap later
       {
        speed_y = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
         {
          speed_y = -jump_strength;
         }
       }

      if(speed_y > 0)
       {
        setTextureRect(frames[4]);
       }
      else if(speed_y < 0)
       {
        setTextureRect(frames[3]);
       }
      else
       {
        slf += elapsed;
        if(slf.asSeconds() >= 1/fps)
         {
          it++;
          if(it==6){it=0;}
          slf = slf-slf;
         }
        setTextureRect(frames[it]);
       }

      std::cout << speed_y << std::endl;



      move(0, speed_y*elapsed.asSeconds()); // falling
     }




  private:
    float fps;
    int it=0, bound_top, bound_bottom, bound_right, bound_left;
    sf::Time slf; // since last frame
    std::string path;
    std::vector<sf::Rect<int>> frames;
    float g = 1000; //px/s
    int jump_strength = 450;
    float acc_x;
    int speed_x = 0;
    float speed_y = 0;
    bool on_ground=0;
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

    hero.setBounds(0, window.getSize().x, 0, window.getSize().y);

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
