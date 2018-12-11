#include "ofApp.h"
#include <iostream>
#include <cstdio>

using namespace snakelinkedlist;
using nlohmann::json;

void snakejson::from_json(const json& j, snakejson::snake& s) {
    j.at("id").get_to(s.id);
    j.at("length").get_to(s.length);
    j.at("alive").get_to(s.alive);
    j.at("direction").get_to(s.direction);
    j.at("color").get_to(s.color);
    j.at("location").get_to(s.coords);
}

// Setup method
void snakeGame::setup(){
    ofSetWindowTitle("Snake126");
    
    srand(static_cast<unsigned>(time(0))); // Seed random with current time
    // SETUP THE NETWORKING HERE AND GET THE ID OF OUR SNAKE
    
    id_ = 5;
}

/*
 Update function called before every draw
 If the function should update when it is called it will:
 1. Check to see if the game is in progress, if it is paused or over it should not update.
 2. Check to see if the current head of the snake intersects the food pellet. If so:
 * The snake should grow by length 1 in its current direction
 * The food should be moved to a new random location
 3. Update the snake in the current direction it is moving
 4. Check to see if the snakes new position has resulted in its death and the end of the game
 */
void snakeGame::update() {
    json json_to_parse = recieve_json();
    
    // get food locations
    food_loc_ = json_to_parse["food"].get<std::vector<std::pair<int, int>>>();
    
    // Get snake locations and info
    
    for (json j : json_to_parse["snakes"]) {
        snakejson::snake s = j.get<snakejson::snake>();
        sneks_.push_back(s);
    }
    
    // Get this snake
    for (int i = 0; i < sneks_.size(); ++i) {
        if (sneks_.at(i).id == id_) {
            // This is our snake
            num_food_eaten_ = sneks_.at(i).length;
            alive_ = sneks_.at(i).alive;
        }
    }
    
    
    // PARSE THE JSON FOOD INTO A VECTOR OF PAIRS OF INTS
    // PARSE THE JSON SNAKES INTO A UNORDERED MAP OF VECTORS OF PAIRS OF INTS
    // WHAT IS THIS ONE'S ID
    
//    if (should_update_) {
//        if (current_state_ == IN_PROGRESS) {
//            ofVec2f snake_body_size = game_snake_.getBodySize();
//            ofVec2f head_pos = game_snake_.getHead()->position;
//            ofRectangle snake_rect(head_pos.x, head_pos.y, snake_body_size.x, snake_body_size.y);
//
//            if (snake_rect.intersects(game_food_.getFoodRect())) {
//                game_snake_.eatFood(ofColor(0,100,0));
//                game_food_.rebase();
//            }
//            game_snake_.update();
//
//            if (game_snake_.isDead()) {
//                current_state_ = FINISHED;
//            }
//        }
//    }
//
//    should_update_ = true;
//}
    
    
    if (current_state_ == GameState::IN_PROGRESS && !alive_) {
        current_state_ = GameState::FINISHED;
    }
}

/*
 Draws the current state of the game with the following logic
 1. If the game is paused draw the pause screen
 2. If the game is finished draw the game over screen and final score
 3. Draw the current position of the food and of the snake
 */
void snakeGame::draw(){
    if (current_state_ == GameState::FINISHED) {
        drawGameOver();
    }
    drawFood();
    drawSnakes();
}

/*
 Function that handles actions based on user key presses
 1. if key == F12, toggle fullscreen
 3. if game is in progress handle WASD action
 4. if key == r and game is over reset it
 
 WASD logic:
 Let dir be the direction that corresponds to a key
 if current direction is not dir (Prevents key spamming to rapidly update the snake)
 and current_direction is not opposite of dir (Prevents the snake turning and eating itself)
 Update direction of snake and force a game update (see ofApp.h for why)
 */
void snakeGame::keyPressed(int key){
    //    if (key == OF_KEY_F12) {
    //        ofToggleFullscreen();
    //        return;
    //    }
    
    int upper_key = toupper(key); // Standardize on upper case
    
    if (current_state_ == GameState::IN_PROGRESS) {
        
        // If current direction has changed to a valid new one, force an immediate update and skip the next frame update
        if (upper_key == 'W') {
//            update();
//            should_update_ = false;
            json_to_send_["id"] = id_;
            json_to_send_["action"] = std::string("UP");
            send_json(json_to_send_);
            json_to_send_.clear();

        } else if (upper_key == 'A') {
//            update();
//            should_update_ = false;
            json_to_send_["id"] = id_;
            json_to_send_["action"] = std::string("LEFT");
            send_json(json_to_send_);
            json_to_send_.clear();
            
        } else if (upper_key == 'S') {
//            update();
//            should_update_ = false;
            json_to_send_["id"] = id_;
            json_to_send_["action"] = std::string("DOWN");
            send_json(json_to_send_);
            json_to_send_.clear();
            
        } else if (upper_key == 'D') {
//            update();
//            should_update_ = false;
            json_to_send_["id"] = id_;
            json_to_send_["action"] = std::string("RIGHT");
            send_json(json_to_send_);
            json_to_send_.clear();
        }
    } else if (upper_key == 'R' && current_state_ == GameState::FINISHED) {
        json_to_send_["id"] = id_;
        json_to_send_["action"] = std::string("RESET");
        send_json(json_to_send_);
        json_to_send_.clear();
    }
}
//
//void snakeGame::reset() {
//
//    // THIS SHOULDN'T MATTER AS LONG AS THE SERVER HAS THE ID ASSOCIATED
////    game_snake_ = Snake();
////    game_food_.rebase();
//    current_state_ = IN_PROGRESS;
//#warning TODO: DO NETWORKING CALL
//}

void snakeGame::windowResized(int w, int h){
//    game_food_.resize(w, h);
//    game_snake_.resize(w, h);
}

void snakeGame::drawFood() {
    for (std::pair<int, int> coords : food_loc_) {
        ofSetColor(ofColor(100,0,0));
        ofDrawRectangle(coords.first * 25, coords.second * 25, 25, 25);
    }
}

void snakeGame::drawSnakes() {
    for (snakejson::snake s : sneks_) {
        // Set color here
        int red = s.color.at(0);
        int green = s.color.at(1);
        int blue = s.color.at(2);
        
        
        ofSetColor(ofColor(red, green, blue));
        for (std::pair<int, int> coord : s.coords) {
            // Draw the coords at each spot
            ofDrawRectangle(coord.first * 25, coord.second * 25, 25, 25);
        }
    }
}

void snakeGame::drawGameOver() {
    string total_food = std::to_string(num_food_eaten_);
    string lose_message = "You Lost! Final Score: " + total_food;
    ofSetColor(0, 0, 0);
    ofDrawBitmapString(lose_message, ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
}

void snakeGame::send_json(json json_to_send) {
    std::ofstream file_output("/Users/matthew/Documents/Xcode/finalproject-MaxProfit/interchange/to_server.json");
    // Pushes the json to the file with a width of
    file_output << std::setw(2) << json_to_send << std::endl;
}

json snakeGame::recieve_json() {
    // Open the file and read into memory
    json json_holder;
    std::ifstream file_input("/Users/matthew/Documents/Xcode/finalproject-MaxProfit/example/nominal_server_to_client.json");
//    std::ifstream file_input("/Users/matthew/Documents/Xcode/finalproject-MaxProfit/interchange/to_client.json");
    
    try {
        file_input >> json_holder;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return json_holder;
}
