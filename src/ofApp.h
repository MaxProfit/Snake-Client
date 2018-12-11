#ifndef OFAPP_H
#define OFAPP_H
#pragma once
#include <ctime>
#include <cstdlib>
#include <utility>
#include <vector>
#include <unordered_map>

#include "json.hpp"
#include "ofMain.h"
#include "snake.h"
#include "SnakeFood.h"
#include <tuple>

namespace snakelinkedlist {
    
    namespace snakejson {
        
        struct snake {
            int id;
            int length;
            bool alive;
            std::string direction;
            std::array<int, 3> color;
            std::vector<std::pair<int, int>> coords;
        };
        
        void from_json(const nlohmann::json& j, snakejson::snake& s);
    }
    
    // Enum to represent the current state of the game
    enum class GameState {
        IN_PROGRESS = 0,
        FINISHED
    };
    
    class snakeGame : public ofBaseApp {
    private:
        
        std::vector<std::pair<int, int>> food_loc_;
        std::vector<snakejson::snake> sneks_;
        
        snakejson::snake our_snake_;
        
        int current_size_;
        bool alive_;
        ofColor color_;
        uint32_t id_;
        int num_food_eaten_;
        
        
        nlohmann::json json_to_send_;
        // THIS WILL BE WHERE WE STORE THE NETWORKING STUFF
        
        
        GameState current_state_ = GameState::IN_PROGRESS; // The current state of the game, used to determine possible actions
        
//        bool should_update_ = true;     // A flag boolean used in the update() function. Due to the frame dependent animation we've
        // written, and the relatively low framerate, a bug exists where users can prefire direction
        // changes faster than a frame update. Our solution is to force a call to update on direction
        // changes and then not update on the next frame to prevent the snake from skipping across the screen.
        
        
        // Private helper methods to render various aspects of the game on screen.
        void drawFood();
        void drawSnakes();
        void drawGameOver();
        
        // Resets the game objects to their original state.
        void reset();
        
        void send_json(nlohmann::json json_to_send);
        
        nlohmann::json recieve_json();
        
    public:
        // Function used for one time setup
        void setup();
        
        // Main event loop functions called on every frame
        void update();
        void draw();
        
        // Event driven functions, called on appropriate user action
        void keyPressed(int key);
        void windowResized(int w, int h);
    };
} // namespace snakelinkedlist
#endif
