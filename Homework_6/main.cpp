#include <iostream>
#include <string>
#include <vector>
#include <cmath>

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

    bool Collision_T(sf::Sprite wall)
     {
      sf::FloatRect guy_bounds = getGlobalBounds();
      sf::FloatRect wall_bounds = wall.getGlobalBounds();

      if ((guy_bounds.top+guy_bounds.height >= wall_bounds.top-3) && (guy_bounds.top < wall_bounds.top)
      && (guy_bounds.left+guy_bounds.width > wall_bounds.left+3) && (guy_bounds.left < wall_bounds.left+wall_bounds.width-3))
       {
        return 1;
       }
      else{return 0;}
     }

    bool Collision_R(sf::Sprite wall)
     {
      sf::FloatRect guy_bounds = getGlobalBounds();
      sf::FloatRect wall_bounds = wall.getGlobalBounds();

      if ((guy_bounds.left <= wall_bounds.left+wall_bounds.width+3) && (guy_bounds.left+guy_bounds.width > wall_bounds.left+wall_bounds.width)
       && (guy_bounds.top+guy_bounds.height > wall_bounds.top+3) && (guy_bounds.top < wall_bounds.top+wall_bounds.height-3))
       {
        return 1;
       }
      else{return 0;}
     }

    bool Collision_B(sf::Sprite wall)
     {
      sf::FloatRect guy_bounds = getGlobalBounds();
      sf::FloatRect wall_bounds = wall.getGlobalBounds();

      if ((guy_bounds.top <= wall_bounds.top + wall_bounds.height+3) && (guy_bounds.top+guy_bounds.height > wall_bounds.top+wall_bounds.height)
       && (guy_bounds.left+guy_bounds.width > wall_bounds.left+3) && (guy_bounds.left < wall_bounds.left+wall_bounds.width-3))
       {
        return 1;
       }
      else{return 0;}
     }

    bool Collision_L(sf::Sprite wall)
     {
      sf::FloatRect guy_bounds = getGlobalBounds();
      sf::FloatRect wall_bounds = wall.getGlobalBounds();

      if ((guy_bounds.left+guy_bounds.width >= wall_bounds.left-3) && (guy_bounds.left < wall_bounds.left)
       && (guy_bounds.top+guy_bounds.height > wall_bounds.top+3) && (guy_bounds.top < wall_bounds.top+wall_bounds.height-3))
       {
        return 1;
       }
      else{return 0;}
     }



    void animation(sf::Time &elapsed, const std::vector<sf::Sprite> &platforms)
     {
      sf::FloatRect b_guy = getGlobalBounds();

      speed_y += g*elapsed.asSeconds(); // acceleration for falling

      bool top=0, left=0, bottom=0, right=0;
      for(auto &platform : platforms)
      {
       if(Collision_T(platform) == 1){top = 1;}
       if(Collision_L(platform) == 1){left = 1;}
       if(Collision_B(platform) == 1){bottom = 1;}
       if(Collision_R(platform) == 1){right = 1;}
      }

      if((b_guy.top+b_guy.height >= bound_bottom) || (top == 1)) // ground detection
       {
        on_ground = true;
       }
      else
       {
        on_ground = false;
       }

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // movement controls
       {
        //crickets
       }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //left
       {
        if(turn == 0)
         {
          setScale(-1,1);
          turn = 1;
          move(50,0);
         }

        acc_x=-run_acceleration;
        if(on_ground==true){acc_x *= 3;}
       }
      else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //right
       {
        if(turn == 1)
         {
          setScale(1,1);
          turn = 0;
          move(-50,0);
         }
        acc_x=run_acceleration;
        if(on_ground==true){acc_x *= 3;}
       }
      else
       {
        acc_x =0;

       }

      if(fabs(speed_x) < speed_cap) // acceleration running
      {
       speed_x += acc_x*elapsed.asSeconds();
      }

      if(fabs(speed_x) > 25)
       {
        if(speed_x > 0)
         {
          speed_x += -drag_x*speed_x*(on_ground+1)*elapsed.asSeconds();
         }
        if(speed_x < 0)
         {
          speed_x += -drag_x*speed_x*(on_ground+1)*elapsed.asSeconds();
         }
       }
      else if(acc_x == 0)
       {
        speed_x =0;
       }

      if(on_ground == true)// some very poor collision, scrap later
       {
        speed_y = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
         {
          speed_y = -jump_strength;
         }
       }

      if(speed_y > 0) // jumping animation
       {
        setTextureRect(frames[4]);
       }
      else if(speed_y < 0) // fallling animation
       {
        setTextureRect(frames[3]);
       }
      else if(fabs(speed_x) < 5)
       {
        slf += elapsed;
        if(slf.asSeconds() >= 2/fps)
         {
          it++;
          if(it>=3){it=0;}
          slf = slf-slf;
         }
        setTextureRect(frames[it]);
       }
      else  // running animation
       {
        slf += elapsed;
        if(slf.asSeconds() >= 1/fps)
         {
          it++;
          if(it>=9){it=3;}
          slf = slf-slf;
         }
        setTextureRect(frames[it]);
       }

      std::cout << it << std::endl;



      move(0, speed_y*elapsed.asSeconds()); // falling
      move(speed_x*elapsed.asSeconds(), 0); // movement <- ->
     }




  private:
    bool turn = 0; // o for left, 1 for right
    float fps;
    int it=0, bound_top, bound_bottom, bound_right, bound_left;
    sf::Time slf; // since last frame
    std::string path;
    std::vector<sf::Rect<int>> frames;
    float g = 1000; //px/s
    int jump_strength = 500;
    int run_acceleration = 400;
    int speed_cap = 500;
    float acc_x=0, drag_x=1.4;
    float speed_x = 0;
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

    sf::Texture tex_platforms;
    if (!tex_platforms.loadFromFile("sprites\\Map\\tileset.png")) {
        std::cerr << "Could not load texture" << std::endl;
    }

    AnimatedSprite hero(10, "sprites\\Character\\character.png");
    hero.setTexture(tex);

    hero.setBounds(0, window.getSize().x, 0, window.getSize().y);

    hero.add_animation_frame(sf::IntRect(0, 0, 50, 37)); // hero standing frame 1
    hero.add_animation_frame(sf::IntRect(50, 0, 50, 37)); // hero standing frame 2
    hero.add_animation_frame(sf::IntRect(100, 0, 50, 37)); // hero standing frame 3
    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37)); // hero running frame 1
    hero.setPosition(375, 500);

    std::vector<sf::Sprite> platforms;

    sf::Sprite platform1;
    sf::Sprite box1;
    platform1.setTexture(tex_platforms);
    box1.setTexture(tex_platforms);
    platform1.setScale(1, 1);
    platform1.setTextureRect(sf::IntRect(25, 24, 120, 76));
    box1.setTextureRect(sf::IntRect(150, 0, 60, 40));
    platform1.setPosition(400, 500);
    box1.setPosition(435, 510);
    platforms.emplace_back(box1);

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
      window.draw(box1);
      window.draw(platform1);

      window.draw(hero);
      hero.animation(elapsed, platforms);

      window.display();
     }
    return 0;

}
