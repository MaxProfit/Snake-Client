#ifndef OFAPP_H
#define OFAPP_H
#pragma once
#include <ctime>
#include <cstdlib>
#include <utility>
#include <vector>
#include <unordered_map>
#include <boost/asio.hpp>
#include "chat_client.hpp"
#include <thread>
#include <memory>

#include "json.hpp"
#include "ofMain.h"

namespace snakelinkedlist {
    
    namespace snakejson {
        
        // Used to define how we will parse out the snake from JSON
        struct snake {
            int id;
            int length;
            bool alive;
            std::string direction;
            std::array<int, 3> color;
            std::vector<std::pair<int, int>> coords;
        };
        
        // Allows the snakes to be parsed out into snake structs easily
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
        std::vector<snakejson::snake> snake_vec_;
        
        snakejson::snake our_snake_;
        
        int current_size_;
        bool alive_;
        ofColor color_;
        uint32_t id_;
        int num_food_eaten_;
        
        // The json we wish to send to the server
        nlohmann::json json_to_send_;
        
        // Start us off as alive
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
        
        // sends the JSON to the server through the chat_client
        void send_json(nlohmann::json json_to_send);
        
        // Gets the most recent JSON from the server
        nlohmann::json receive_json();
        
        // Creates unique pointers for the networking things so we can refer to them
        std::unique_ptr<boost::asio::io_context> io_context_;
        std::unique_ptr<chat_client> client_;
        std::unique_ptr<thread> thread_;
        
        // Allows us to use the keyboard and the stuff will be sent
        bool should_update_ = true;
        
        // Used to time the duration between when we're receiving the json
        // to ensure the we can read the server while the sockets are still on
        std::chrono::duration<double> elapsed_;
        std::chrono::high_resolution_clock::time_point start_;
        std::chrono::high_resolution_clock::time_point finish_;
        
        
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

namespace networking {
    // Where we are connecting to
    const std::string kIPADDRESS("127.0.0.1");
    const std::string kPORT("49145");
    
}
#endif
