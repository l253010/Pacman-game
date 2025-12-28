#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio/Sound.hpp>
#include "headerfile.h"
#include<SFML/Audio/SoundBuffer.hpp>
#include <optional>
#include<fstream>
#include <cmath> // For std::abs in manhattanDistance

// Global button positions
sf::FloatRect resumeButtonPos, settingButtonPos, scoreButtonPos, color_pinkButtonPos, color_purpleButtonPos, color_greenButtonPos, color_yellowButtonPos, backButtonPos, resetButtonPos, quitButtonPos, pauseButtonPos, startButtonPos, menuButtonPos, exitButtonPos, muteButtonPos, aboutButtonPos;

using namespace sf;
using namespace std;

//window global
RenderWindow window;
//sounds
SoundBuffer button_click;
Sound button_click_sound(button_click);

SoundBuffer dora_die;
Sound dora_die_sound(dora_die);

SoundBuffer dot_eat;
Sound dot_eat_sound(dot_eat);

SoundBuffer oh_man;
Sound oh_man_sound(oh_man);

SoundBuffer pellet_eat;
Sound pellet_eat_sound(pellet_eat);

SoundBuffer start_song;
Sound start(start_song);


SoundBuffer win;
Sound win_sound(win);

//sounds



//reset stats
void reset_stats() {
    lives = 3;
    scores = 0;
    dots_eaten = 0;
    state = 's';
    level = 1;
    total_dots = total_dots1;
    scared_timer = 0;
}//reset stats

void initialize_maze() {
    ifstream maze;
    if (level == 1) { maze.open("files/level1.txt"); }
    else if (level == 2) { maze.open("files/level2.txt"); }
    else if (level == 3) { maze.open("files/level3.txt"); }
    else if (level == 4) { maze.open("files/level4.txt"); }
    else if (level == 5) { maze.open("files/level5.txt"); }
    else if (level == 6) { maze.open("files/level6.txt"); }
    else if (level == 7) { maze.open("files/level7.txt"); }

    for (i = 0; i < GRID_SIZE_X; i++) {
        for (j = 0; j < GRID_SIZE_Y; j++) {
            maze >> arr[i][j];

        }
    }
    maze.close();
}

void previous_game_save() {
    ofstream file;
    file.open("files/resume_stats.txt", ios::trunc);
    file << scores << endl << level << endl << lives << endl << dots_eaten << endl << total_dots << endl << pacman_x << " " << pacman_y << endl;
    file << scared_timer << endl;
    for (i = 0; i < 4; i++) {
        file << ghost_x[i] << " " << ghost_y[i] << " " << ghost_state[i] << ghostDirection[i] << endl;
    }
    file.close();
    file.open("files/resume_maze.txt", ios::trunc);//storing array
    for (i = 0; i < GRID_SIZE_X; i++) {
        for (j = 0; j < GRID_SIZE_Y; j++) {
            file << arr[i][j];
            if (j != (GRID_SIZE_Y - 1)) {
                file << " ";
            }

        }
        file << "\n";
    }
    file.close();
}

void previous_game_load() {//maze load previous game ki
    ifstream file;
    file.open("files/resume_stats.txt");
    file >> scores >> level >> lives >> dots_eaten >> total_dots >> pacman_x >> pacman_y;
    file >> scared_timer;
    for (i = 0; i < 4; i++) {
        file >> ghost_x[i] >> ghost_y[i] >> ghost_state[i] >> ghostDirection[i];
    }
    file.close();
    file.open("files/resume_maze.txt");
    for (i = 0; i < GRID_SIZE_X; i++) {
        for (j = 0; j < GRID_SIZE_Y; j++) {
            file >> arr[i][j];


        }

    }
    file.close();
}

void previous_game_initialize() {
    //if start clicked then previous game end..ab start wali game previous game bnay gi.whatif user lose next game and then
    //resume this game thats why..to avoid cheating
    ofstream file;
    file.open("files/resume_stats.txt", ios::trunc);
    file << 0 << endl << 1 << endl << 3 << endl << 0 << endl << 328 << endl << 15 << " " << 14 << endl;
    file << 0;
    for (i = 0; i < 4; i++) {
        file << reset_ghost_x[i] << " " << reset_ghost_y[i] << " " << reset_ghost_state[i] << " " << reset_ghostDirection[i] << endl;
    }
    file.close();
    //filemaze level one put kro
    ifstream fileI;
    fileI.open("files/level1.txt");
    file.open("files/resume_maze.txt", ios::trunc);
    for (i = 0; i < GRID_SIZE_X; i++) {
        for (j = 0; j < GRID_SIZE_Y; j++) {
            fileI >> arr[i][j];
            file << arr[i][j] << " ";
        }
        file << "\n";
    }
    file.close();
    fileI.close();
}




void initialize_setup() {

    reset_pacman_position();
    for (i = 0; i < 4; i++) {
        reset_ghost(i);
    }
    reset_stats();
    initialize_maze();





}

void dots_collision() {
    //normal dots
    if (arr[pacman_x][pacman_y] == '.') {
        if (!mute) {

            dot_eat_sound.play();
        }
        scores += 20;
        dots_eaten++;
        arr[pacman_x][pacman_y] = 'X';

    }//if

     //power pallet
    power_pallet();

    //level complete check
    level_handling();

}//dots
int ghost_collided() {
    for (i = 0; i < 4; i++) {
        if (pacman_x == ghost_x[i] && pacman_y == ghost_y[i]) {
            return i;
        }

        else if (pacman_x == ghost_old_x[i] && pacman_y == ghost_old_y[i] && ghost_x[i] == pacman_old_x && ghost_y[i] == pacman_old_y) {
            return i;
        }

    }
    return -1;
}

void ghost_collision() {

    int i = ghost_collided();//funuc returns index of ghost collided.returns -1 if no collision
    if (i != -1) {


        if (ghost_state[i] == false) {
            if (!mute) {

                dora_die_sound.play();
            }
            for (i = 0; i < 4; i++) {//all ghost reset position,dora ko "billi ka sahara" milla bhagne k liyay
                ghost_x[i] = reset_ghost_x[i];
                ghost_y[i] = reset_ghost_y[i];
                //state not reset takay scared ghost scared rahain
                ghostDirection[i] = reset_ghostDirection[i];
            }
            reset_pacman_position();
            lives--;
        }


        else {
            if (!mute) {

                oh_man_sound.play();
            }
            reset_ghost(i);

            scores += 200;
        }


    }//ghost check




 //level continue check
    level_handling();

}
void scores_save() {

    ifstream file;
    file.open("files/scores.txt");
    for (i = 0; i < 5; i++) {
        file >> scores_arr[i];
    }
    file.close();
    scores_arr[5] = scores;//adding latest scores
    //write back
    ofstream fileO;
    fileO.open("files/scores.txt", ios::trunc);
    for (i = 1; i < 6; i++) {
        fileO << scores_arr[i] << endl;
    }
    fileO.close();
    scores = 0;//becuase new game me zero scores,lose ho ya win ho ya start kro,scores zero hi honge.
}
void highscores_save() {

    ifstream file;
    file.open("files/highscores.txt");
    for (i = 0; i < 3; i++) {
        file >> highscores_arr[i];
    }

    file.close();
    if (scores > highscores_arr[2]) {//2 iss liyay kyunke file me to highest latest last me hoga na


        highscores_arr[3] = scores;//adding latest highscores

        //write back
        ofstream fileO;
        fileO.open("files/highscores.txt", ios::trunc);
        for (i = 1; i < 4; i++) {
            fileO << highscores_arr[i] << endl;
        }
        fileO.close();
    }
    scores = 0;
}

void scores_store() {
    //scores
    scores_save();

    //highscores
    highscores_save();

}
void level_handling() {
    if (dots_eaten == total_dots) {

        if (level != last_level) {
            cout << "level completed!";
            level++;
            lives = 3;
            dots_eaten = 0;
            if (level == 2)
                total_dots = total_dots2;
            else if (level == 3)
                total_dots = total_dots3;
            else if (level == 4)
                total_dots = total_dots4;
            else if (level == 5)
                total_dots = total_dots5;
            else if (level == 6)
                total_dots = total_dots6;
            else if (level == 7)
                total_dots = total_dots7;

            sound_playing1 = false;
            state = 'l';
            initialize_maze();//maze me if statement lga kr diff maze initialize hogi
        }//next level move

        else {
            cout << "YOU WON,Dora";
            scores_store();
            state = 'w';

        }



    }

    //lives check
    else if (lives == 0) {
        cout << "oops,YOU LOSE!";
        scores_store();
        initialize_setup();
        state = 'f';

    }

}//level

int scared(int level) {
    int a = 10;//frameratelimt
    switch (level) {
    case 1: {
        return 6 * a;
        break;
    }

    case 2: {
        return 6 * a;
        break;
    }
    case 3: {
        return 6 * a;
        break;
    }
    case 4: {
        return 5 * a;
        break;
    }
    case 5: {
        return 4 * a;
        break;
    }
    case 6: {
        return 3 * a;
        break;
    }
    case 7: {
        return 2 * a;
        break;
    }
    }

    return 60;
}
//-------------
void power_pallet() {
    if (arr[pacman_x][pacman_y] == '1' || arr[pacman_x][pacman_y] == '2' || arr[pacman_x][pacman_y] == '3' || arr[pacman_x][pacman_y] == '4') {
        if (!mute) {

            pellet_eat_sound.play();
        }
        scores += 200;
        dots_eaten++;
        arr[pacman_x][pacman_y] = 'X';
        for (i = 0; i < 4; i++)
            ghost_state[i] = true;//all ghost scared now
        scared_timer = scared(level);//scared functin switch timing for each  level
    } //if  
}//power pallet

//-----------
bool isghost(int ghostindex, int next_r, int next_c)
{
    for (int i = 0; i < 4; i++)
    {
        if (i == ghostindex)
            continue;
        if (next_r == ghost_x[i] && next_c == ghost_y[i]) {
            return true;
        }
        else return false;

    }
}


//---GHOST MOVEMENT
// Helper function to calculate Manhattan distance (used for Scared Mode)
int manhattanDistance(int r1, int c1, int r2, int c2) {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}


void getChaseTarget(int ghostIndex, int& target_r, int& target_c) {
    if (dots_eaten > 60 && dots_eaten < 70 || dots_eaten>150 && dots_eaten < 160)//scattering
    {
        target_r = SCATTER_R[ghostIndex];
        target_c = SCATTER_C[ghostIndex];

    }
    else {
        // Default: Target Pac-Man directly (Blinky behavior)
        target_r = pacman_x;
        target_c = pacman_y;
    }

    switch (ghostIndex) {
    case 0: // Blinky (Red) - Chases Pac-Man directly
        // target_r and target_c are already set to pacman_x/y
        break;

    case 1: // Pinky (Pink) - Targets 4 cells AHEAD of Pac-Man
    {
        if (dots_eaten > 60 && dots_eaten < 70 || dots_eaten>150 && dots_eaten < 160)//scattering
        {
            target_r = SCATTER_R[ghostIndex];
            target_c = SCATTER_C[ghostIndex];

        }
        else
        {
            int offset = 4;
            // Use pacNextDirec for better prediction, fall back to pacmanDirection
            char direction = pacNextDirec != 'N' ? pacNextDirec : pacmanDirection;

            if (direction == 'U') {
                target_r -= offset;
            }
            else if (direction == 'D') {
                target_r += offset;
            }
            else if (direction == 'L') {
                target_c -= offset;
            }
            else if (direction == 'R') {
                target_c += offset;
            }
            // Clamp target to ensure it stays within the main grid area (avoid 0/19 border)
            target_r = std::max(1, std::min(GRID_SIZE_X - 2, target_r));
            target_c = std::max(1, std::min(GRID_SIZE_Y - 2, target_c));
        }
        break;
    }

    case 2: // Inky (Cyan) - Simplified: Targets 2 cells ahead of Pac-Man
    {
        if (dots_eaten > 60 && dots_eaten < 70 || dots_eaten>150 && dots_eaten < 155)//scattermode
        {
            target_r = SCATTER_R[ghostIndex];
            target_c = SCATTER_C[ghostIndex];

        }
        else {
            // The true Inky AI is complex (uses Blinky's position), but this simpler
            // variation ensures unique paths.
            int offset = 2;
            char direction = pacNextDirec != 'N' ? pacNextDirec : pacmanDirection;

            if (direction == 'U') {
                target_r -= offset;
            }
            else if (direction == 'D') {
                target_r += offset;
            }
            else if (direction == 'L') {
                target_c -= offset;
            }
            else if (direction == 'R') {
                target_c += offset;
            }
            target_r = std::max(1, std::min(GRID_SIZE_X - 2, target_r));
            target_c = std::max(1, std::min(GRID_SIZE_Y - 2, target_c));
        }
        break;
    }

    case 3: // Clyde (Orange) - Targets Pac-Man when FAR, scatters when CLOSE (< 8 cells)
    {
        // Check if Clyde is CLOSE to Pac-Man (< 8 cells)
        if (manhattanDistance(pacman_x, pacman_y, ghost_x[ghostIndex], ghost_y[ghostIndex]) < 8) {

            // **SCATTER MODE (CLOSE):** Retreat to scatter corner (bottom-left)
            // target_r (Row) gets SCATTER_R[ghostIndex]
            // target_c (Column) gets SCATTER_C[ghostIndex]
            target_r = SCATTER_R[ghostIndex];
            target_c = SCATTER_C[ghostIndex];

        }
        else {
            target_r = pacman_x;
            target_c = pacman_y;
        }

        break;
    }
    }

}

char findNextStepBFS(int ghostIndex,int start_r, int start_c, int target_r, int target_c) {
    if (start_r == target_r && start_c == target_c) return 'N';

    // 1. Reset BFS structures
    queue_head = 0;
    queue_tail = 0;
    bool visited[GRID_SIZE_X][GRID_SIZE_Y] = { {false} }; // Visited array
    int parent_dir_index[GRID_SIZE_X][GRID_SIZE_Y];      // Stores the index of the direction taken to reach this cell

    // 2. Enqueue start position
    bfs_queue_r[queue_tail] = start_r;
    bfs_queue_c[queue_tail] = start_c;
    queue_tail++;
    visited[start_r][start_c] = true;

    // Use a special value (e.g., -1) for the starting cell's parent direction
    parent_dir_index[start_r][start_c] = -1;

    // The direction of the *first step* from the start cell. 
    // This is what we will ultimately return.
    char first_step_direction = 'N';

    // 3. BFS Loop (while queue is not empty)
    while (queue_head < queue_tail) {
        int current_r = bfs_queue_r[queue_head];
        int current_c = bfs_queue_c[queue_head];
        queue_head++;


        // Target found
        if (current_r == target_r && current_c == target_c) {

            // Backtrack to find the first step
            int curr_r = current_r;
            int curr_c = current_c;

            while (parent_dir_index[curr_r][curr_c] != -1) {
                int dir_idx = parent_dir_index[curr_r][curr_c];  // Use curr_r, curr_c directly


                // Get the parent's coordinates by reversing the move
                int parent_r = curr_r - DR[dir_idx];
                int parent_c = curr_c - DC[dir_idx];

                // If the parent is the starting cell, the current cell is the next step
                if (parent_r == start_r && parent_c == start_c) {
                    return DIR_CHARS[dir_idx]; // This is the direction of the first move
                }

                // Move to parent for the next iteration
                curr_r = parent_r;
                curr_c = parent_c;
            }
            return 'N'; // Should not be reached if target was found
        }

        // 4. Explore Neighbors
        for (int i = 0; i < 4; ++i) {
            int next_r = current_r + DR[i];
            int next_c = current_c + DC[i];

            if (!isWall(next_r, next_c) && !visited[next_r][next_c] && !isghost(ghostIndex, next_r, next_c)) {
                visited[next_r][next_c] = true;
                parent_dir_index[next_r][next_c] = i; // Store the direction index used to reach this cell
                bfs_queue_r[queue_tail] = next_r;
                bfs_queue_c[queue_tail] = next_c;
                queue_tail++;
            }
        }
    }

    return 'N'; // No path found
}


/**
 * Determines the next move for a ghost based on its state (Chase or Scared).
 */
void ghostBFSMovement(int ghostIndex) {
    char chosen_direction = 'N';
    int r = ghost_x[ghostIndex];
    int c = ghost_y[ghostIndex];
    int target_r = -1;
    int target_c = -1;

   

    if (ghost_state[ghostIndex] == false) {
        // --- CHASE MODE: Calculate unique target for this ghost ---
        getChaseTarget(ghostIndex, target_r, target_c);
        chosen_direction = findNextStepBFS(ghostIndex,r, c, target_r, target_c);

    }
    else if (ghost_state[ghostIndex]==true){
        // --- SCARED MODE: Find the direction that MAXIMIZES distance from Pac-Man ---
        int max_dist = -1;

        // Loop through all 4 directions
        for (int i = 0; i < 4; ++i) {
            int next_r = r + DR[i];
            int next_c = c + DC[i];
            char potential_dir = DIR_CHARS[i];

            // Check if the move is legal (not a wall)
            if (!isWall(next_r, next_c) && !isghost(ghostIndex, next_r, next_c)) {

                // Calculate new distance to Pac-Man (this is the standard scared behavior)
                int current_dist = manhattanDistance(next_r, next_c, pacman_x, pacman_y);

                // Prioritize maximum distance.
                if (current_dist > max_dist) {
                    max_dist = current_dist;
                    chosen_direction = potential_dir;
                }
            }
        }
       
    }

    // --- Apply Movement ---
    if (chosen_direction != 'N') {
        int dir_index = -1;
        if (chosen_direction == 'U') dir_index = 0;
        else if (chosen_direction == 'D') dir_index = 1;
        else if (chosen_direction == 'L') dir_index = 2;
        else if (chosen_direction == 'R') dir_index = 3;

        if (dir_index != -1) {
            ghost_x[ghostIndex] += DR[dir_index];
            ghost_y[ghostIndex] += DC[dir_index];
            ghostDirection[ghostIndex] = chosen_direction;
        }
    }


}
//---------------------------Ghost Movement-------------------------------------------------

//pacman reset
void reset_pacman_position() {
    pacman_x = 4;
    pacman_y = 13;
    pacmanDirection = 'U';

}

//ghost reset
void reset_ghost(int i) {
    ghost_x[i] = reset_ghost_x[i];
    ghost_y[i] = reset_ghost_y[i];
    ghost_state[i] = reset_ghost_state[i];
    ghostDirection[i] = reset_ghostDirection[i];
}



void draw_sfml_game() {


    sf::Texture boots, bag, map, cam, star, dora, swiper, doraS, startbutton, pausebutton, resumebutton, quitbutton, resetbutton;
    sf::Texture exitbutton, menubutton, mutebutton, aboutbutton, backbutton, settingbutton, scorebutton;
    sf::Texture color_purplebutton, color_greenbutton, color_yellowbutton, color_pinkbutton;

    boots.loadFromFile("assets/boots.png");
    map.loadFromFile("assets/map.png");
    swiper.loadFromFile("assets/swiper.png");
    cam.loadFromFile("assets/cam.png");
    bag.loadFromFile("assets/bag.png");
    star.loadFromFile("assets/star.png");
    dora.loadFromFile("assets/dora.png");
    doraS.loadFromFile("assets/doraS.png");
    startbutton.loadFromFile("assets/startbutton.png");
    exitbutton.loadFromFile("assets/exitbutton.png");
    menubutton.loadFromFile("assets/menubutton.png");
    mutebutton.loadFromFile("assets/mutebutton.png");
    resetbutton.loadFromFile("assets/resetbutton.png");
    quitbutton.loadFromFile("assets/quitbutton.png");
    aboutbutton.loadFromFile("assets/aboutbutton.png");
    pausebutton.loadFromFile("assets/pausebutton.png");
    resumebutton.loadFromFile("assets/resumebutton.png");
    backbutton.loadFromFile("assets/backbutton.png");
    scorebutton.loadFromFile("assets/scorebutton.png");
    settingbutton.loadFromFile("assets/settingbutton.png");
    color_purplebutton.loadFromFile("assets/color_purplebutton.png");
    color_pinkbutton.loadFromFile("assets/color_pinkbutton.png");
    color_greenbutton.loadFromFile("assets/color_greenbutton.png");
    color_yellowbutton.loadFromFile("assets/color_yellowbutton.png");



    //bandar

    sf::Sprite boots_sprite(boots);
    boots_sprite.setScale({ 0.015f, 0.015f });
    boots_sprite.setOrigin({ boots.getSize().x / 2.0f,boots.getSize().y / 2.0f });
    //bandar




      //map

    sf::Sprite map_sprite(map);
    map_sprite.setScale({ 0.015f, 0.010f });
    map_sprite.setOrigin({ map.getSize().x / 2.0f,map.getSize().y / 2.0f });
    //map

    //swiper

    sf::Sprite swiper_sprite(swiper);
    swiper_sprite.setScale({ 0.03f, 0.03f });

    //swiper



    //cam

    sf::Sprite cam_sprite(cam);
    cam_sprite.setScale({ 0.015f, 0.015f });
    cam_sprite.setOrigin({ cam.getSize().x / 2.0f,cam.getSize().y / 2.0f });
    //cam

    //bag

    sf::Sprite bag_sprite(bag);
    bag_sprite.setScale({ 0.015f, 0.015f });
    bag_sprite.setOrigin({ bag.getSize().x / 2.0f,bag.getSize().y / 2.0f });
    //bag

    //star

    sf::Sprite star_sprite(star);
    star_sprite.setScale({ 0.01f, 0.01f });
    star_sprite.setOrigin({ star.getSize().x / 2.0f,star.getSize().y / 2.0f });
    //star






    //dora

    sf::Sprite dora_sprite(dora);
    dora_sprite.setScale({ 0.025f, 0.025f });
    //dora

    //doraS

    sf::Sprite doraS_sprite(doraS);
    doraS_sprite.setScale({ 1.0f, 1.85f });
    //doraS

    //startbutton

    Sprite startbutton_sprite(startbutton);
    startbutton_sprite.setOrigin({ startbutton.getSize().x / 2.0f,startbutton.getSize().y / 2.0f });
    startbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,(GRID_SIZE_X / 2.0f) * CELL_SIZE });
    startbutton_sprite.setScale({ 0.4f,0.4f });

    //startbutton

    //exitbutton

    Sprite exitbutton_sprite(exitbutton);
    exitbutton_sprite.setOrigin({ exitbutton.getSize().x / 2.0f,exitbutton.getSize().y / 2.0f });

    exitbutton_sprite.setScale({ 0.1f,0.1f });

    //exitbutton

    //menubutton

    Sprite menubutton_sprite(menubutton);
    menubutton_sprite.setOrigin({ menubutton.getSize().x / 2.0f,menubutton.getSize().y / 2.0f });
    menubutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,(GRID_SIZE_X / 2.0f) * CELL_SIZE });
    menubutton_sprite.setScale({ 0.4f,0.4f });

    //menubutton

    //mutebutton

    Sprite mutebutton_sprite(mutebutton);
    mutebutton_sprite.setOrigin({ mutebutton.getSize().x / 2.0f,mutebutton.getSize().y / 2.0f });
    mutebutton_sprite.setScale({ 0.4f,0.4f });
    //mutebutton

    //color_pinkbutton

    Sprite color_pinkbutton_sprite(color_pinkbutton);
    color_pinkbutton_sprite.setOrigin({ color_pinkbutton.getSize().x / 2.0f,color_pinkbutton.getSize().y / 2.0f });
   //color_pinkbutton

    //color_greenbutton

    Sprite color_greenbutton_sprite(color_greenbutton);
    color_greenbutton_sprite.setOrigin({ color_greenbutton.getSize().x / 2.0f,color_greenbutton.getSize().y / 2.0f });
     //color_greenbutton

    //color_purplebutton

    Sprite color_purplebutton_sprite(color_purplebutton);
    color_purplebutton_sprite.setOrigin({ color_purplebutton.getSize().x / 2.0f,color_purplebutton.getSize().y / 2.0f });
   //color_purplebutton

    //color_yellowbutton

    Sprite color_yellowbutton_sprite(color_yellowbutton);
    color_yellowbutton_sprite.setOrigin({ color_yellowbutton.getSize().x / 2.0f,color_yellowbutton.getSize().y / 2.0f });
     //color_yellowbutton


    //settingbutton

    Sprite settingbutton_sprite(settingbutton);
    settingbutton_sprite.setOrigin({ settingbutton.getSize().x / 2.0f,settingbutton.getSize().y / 2.0f });
    settingbutton_sprite.setScale({ 0.4f,0.4f });
    //settingbutton

      //scorebutton

    Sprite scorebutton_sprite(scorebutton);
    scorebutton_sprite.setOrigin({ scorebutton.getSize().x / 2.0f,scorebutton.getSize().y / 2.0f });
    scorebutton_sprite.setScale({ 0.4f,0.4f });
    //scorebutton

    //aboutbutton

    Sprite aboutbutton_sprite(aboutbutton);
    aboutbutton_sprite.setOrigin({ aboutbutton.getSize().x / 2.0f,aboutbutton.getSize().y / 2.0f });
    aboutbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,((GRID_SIZE_X / 2.0f) * CELL_SIZE) + (2 * CELL_SIZE) });
    aboutbutton_sprite.setScale({ 0.4f,0.4f });

    //aboutbutton


    //pausebutton

    Sprite pausebutton_sprite(pausebutton);
    pausebutton_sprite.setScale({ 1.0f,1.0f });

    //pausebutton

    //resumebutton

    Sprite resumebutton_sprite(resumebutton);
    resumebutton_sprite.setScale({ 1.0f,1.0f });


    //resumebutton


    //backbutton

    Sprite backbutton_sprite(backbutton);
    backbutton_sprite.setScale({ 1.0f,1.0f });


    //backbutton


    //resetbutton

    Sprite resetbutton_sprite(resetbutton);
    resetbutton_sprite.setScale({ 1.0f,1.0f });

    //resetbutton

    //quitbutton

    Sprite quitbutton_sprite(quitbutton);
    quitbutton_sprite.setScale({ 0.3f,0.3f });

    //quitbutton

    ////font
    //

    sf::Font font("font.ttf");

    sf::Text text(font);




    // set the character size
    text.setCharacterSize(25); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);

    // set the text style
    text.setStyle(sf::Text::Bold);

    //setposition
    text.setPosition({ 1.0f * CELL_SIZE, (GRID_SIZE_X / 2.0f) * CELL_SIZE });

    // --- 2. SFML Drawing Loop (Replaces the cout loop) ---

    if (state == 'g' || state == 'p') {
        start.setVolume(10.0f);

        for (i = 0; i < GRID_SIZE_X; i++) {
            for (j = 0; j < GRID_SIZE_Y; j++) {

                // Create a temporary rectangle shape for the current cell
                RectangleShape cell(Vector2f({ CELL_SIZE, CELL_SIZE }));

                // Set position based on array indices (i=row, j=column)
                cell.setPosition({ j * CELL_SIZE, i * CELL_SIZE });

                // Define the color based on the character in the array

                if (arr[i][j] == '#') {
                    // Wall
                      // Current cell coordinates
                    float x = j * CELL_SIZE;
                    float y = i * CELL_SIZE;

                    // Check top neighbor
                    // Draw a top line if it's the edge or the neighbor above is not a wall.
                    if (i == 0 || arr[i - 1][j] != '#') {
                        RectangleShape topWall({ CELL_SIZE, wallThickness });
                        topWall.setPosition({ x, y });
                        if (color == 'm') {
                            topWall.setFillColor(Color::Magenta);
                        }
                        if (color == 'p') {
                            topWall.setFillColor(Color(128, 0, 128));
                        }
                        if (color == 'g') {
                            topWall.setFillColor(Color::Green);
                        }
                        if (color == 'y') {
                            topWall.setFillColor(Color::Yellow);
                        }
                        window.draw(topWall);
                    }

                    // Check bottom neighbor
                    // Draw a bottom line if it's the edge or the neighbor below is not a wall.
                    if (i == GRID_SIZE_X - 1 || arr[i + 1][j] != '#') {
                        RectangleShape bottomWall({ CELL_SIZE, wallThickness });
                        bottomWall.setPosition({ x, y + CELL_SIZE - wallThickness });
                        if (color == 'm') {
                            bottomWall.setFillColor(Color::Magenta);
                        }
                        if (color == 'p') {
                            bottomWall.setFillColor(Color(128, 0, 128));
                        }
                        if (color == 'g') {
                            bottomWall.setFillColor(Color::Green);
                        }
                        if (color == 'y') {
                            bottomWall.setFillColor(Color::Yellow);
                        }
                        window.draw(bottomWall);
                    }

                    // Check left neighbor
                    // Draw a left line if it's the edge or the neighbor to the left is not a wall.
                    if (j == 0 || arr[i][j - 1] != '#') {
                        RectangleShape leftWall({ wallThickness, CELL_SIZE });
                        leftWall.setPosition({ x, y });
                        if (color == 'm') {
                            leftWall.setFillColor(Color::Magenta);
                        }
                        if (color == 'p') {
                            leftWall.setFillColor(Color(128, 0, 128));
                        }
                        if (color == 'g') {
                            leftWall.setFillColor(Color::Green);
                        }
                        if (color == 'y') {
                            leftWall.setFillColor(Color::Yellow);
                        }
                        window.draw(leftWall);
                    }

                    // Check right neighbor
                    // Draw a right line if it's the edge or the neighbor to the right is not a wall.
                    if (j == GRID_SIZE_Y - 1 || arr[i][j + 1] != '#') {
                        RectangleShape rightWall({ wallThickness, CELL_SIZE });
                        rightWall.setPosition({ x + CELL_SIZE - wallThickness, y });
                        if (color == 'm') {
                            rightWall.setFillColor(Color::Magenta);
                        }
                        if (color == 'p') {
                            rightWall.setFillColor(Color(128, 0, 128));
                        }
                        if (color == 'g') {
                            rightWall.setFillColor(Color::Green);
                        }
                        if (color == 'y') {
                            rightWall.setFillColor(Color::Yellow);
                        }
                        window.draw(rightWall);
                    }



                }


                else if (arr[i][j] == '.') {


                    star_sprite.setPosition({ j * CELL_SIZE + CELL_SIZE / 2.0f, i * CELL_SIZE + CELL_SIZE / 2.0f });
                    window.draw(star_sprite);


                }
                //big dots-----------------


               //no 1
                else if (arr[i][j] == '1') {


                    boots_sprite.setPosition({ j * CELL_SIZE + CELL_SIZE / 2.0f, i * CELL_SIZE + CELL_SIZE / 2.0f });
                    window.draw(boots_sprite);
                }

                //no 2
                else if (arr[i][j] == '2') {

                    map_sprite.setPosition({ j * CELL_SIZE + CELL_SIZE / 2.0f, i * CELL_SIZE + CELL_SIZE / 2.0f });
                    window.draw(map_sprite);
                }

                //no 3
                else if (arr[i][j] == '3') {

                    cam_sprite.setPosition({ j * CELL_SIZE + CELL_SIZE / 2.0f, i * CELL_SIZE + CELL_SIZE / 2.0f });
                    window.draw(cam_sprite);
                }

                //no 4
                else if (arr[i][j] == '4') {

                    bag_sprite.setPosition({ j * CELL_SIZE + CELL_SIZE / 2.0f, i * CELL_SIZE + CELL_SIZE / 2.0f });
                    window.draw(bag_sprite);
                }


                pausebutton_sprite.setOrigin({ pausebutton.getSize().x / 2.0f,pausebutton.getSize().y / 2.0f });
                pausebutton_sprite.setPosition({ 0.75 * CELL_SIZE,0.75 * CELL_SIZE });
                window.draw(pausebutton_sprite);
                pauseButtonPos = pausebutton_sprite.getGlobalBounds();

            }
        }

        // Draw Pacman from separate variables
        dora_sprite.setPosition({ pacman_y * CELL_SIZE, pacman_x * CELL_SIZE });
        window.draw(dora_sprite);

        // Draw Ghosts from separate variables  
        for (int i = 0; i < 4; i++) {
            //not scared ghost ,state=false for not scared ghost
            if (ghost_state[i] == false) {
                swiper_sprite.setPosition({ (ghost_y[i] - 0.3f) * CELL_SIZE, ghost_x[i] * CELL_SIZE });
                swiper_sprite.setColor(sf::Color::White);
                window.draw(swiper_sprite);
            }

            else
            {
                //scared ghost ,state = true
                swiper_sprite.setPosition({ (ghost_y[i] - 0.3f) * CELL_SIZE, ghost_x[i] * CELL_SIZE });
                swiper_sprite.setColor({ 0, 255, 255, 255 });//this color change shows he is scared
                window.draw(swiper_sprite);
            }
        }


        text.setString("Scores " + std::to_string(scores));
        text.setCharacterSize(15); // in pixels, not points!
        text.setPosition({ CELL_SIZE ,CELL_SIZE });
        window.draw(text);

        text.setString("level " + std::to_string(level));
        text.setCharacterSize(15); // in pixels, not points!
        text.setPosition({ 8 * CELL_SIZE  ,CELL_SIZE });
        window.draw(text);

        text.setString("lives " + std::to_string(lives));
        text.setCharacterSize(15); // in pixels, not points!
        text.setPosition({ 26 * CELL_SIZE / 2.0f ,CELL_SIZE });
        window.draw(text);

        text.setString("starsCOLLECTED   " + std::to_string(dots_eaten));
        text.setCharacterSize(15); // in pixels, not points!
        text.setPosition({ 34 * CELL_SIZE / 2.0f ,CELL_SIZE });
        window.draw(text);

        if (scared_timer > 0) {
            text.setString(std::to_string(scared_timer));
            text.setFillColor(sf::Color::Red);
            text.setCharacterSize(15); // in pixels, not points!
            text.setPosition({ 53 * CELL_SIZE / 2.0f ,CELL_SIZE });
            window.draw(text);
            text.setFillColor(sf::Color::White);//baqi text k liyay wapis white hojayay
        }


        //pause button
        if (state == 'p') {




            // Draw semi-transparent overlay
            RectangleShape overlay(Vector2f(Window_width, Window_height));
            overlay.setFillColor(Color(0, 0, 0, 150)); // Semi-transparent black
            window.draw(overlay);

            // Create pause menu window (rectangle)
            float menuWidth = Window_width / 2.0f;
            float menuHeight = Window_height / 2.0f;
            float menuX = (Window_width - menuWidth) / 2.0f;
            float menuY = (Window_height - menuHeight) / 2.0f;

            RectangleShape pauseMenu(Vector2f(menuWidth, menuHeight));
            pauseMenu.setPosition({ menuX, menuY });
            pauseMenu.setFillColor(Color(50, 50, 100)); // Dark blue background
            pauseMenu.setOutlineThickness(3);
            pauseMenu.setOutlineColor(Color::White);
            window.draw(pauseMenu);

            sf::Text pausedText(font);
            pausedText.setString("PAUSED");
            pausedText.setCharacterSize(30);
            pausedText.setFillColor(sf::Color::White);
            pausedText.setPosition({ 2.0f * CELL_SIZE, 1.0f * CELL_SIZE });
            window.draw(pausedText);


            settingbutton_sprite.setOrigin({ settingbutton.getSize().x / 2.0f,settingbutton.getSize().y / 2.0f });
            settingbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,(GRID_SIZE_X / 2.0f - 2.0f) * CELL_SIZE });
            window.draw(settingbutton_sprite);


            resumebutton_sprite.setOrigin({ resumebutton.getSize().x / 2.0f,resumebutton.getSize().y / 2.0f });
            resumebutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,(GRID_SIZE_X / 2.0f) * CELL_SIZE });
            window.draw(resumebutton_sprite);

            resetbutton_sprite.setOrigin({ resetbutton.getSize().x / 2.0f,resetbutton.getSize().y / 2.0f });
            resetbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,(GRID_SIZE_X / 2.0f + 2.0f) * CELL_SIZE });
            window.draw(resetbutton_sprite);

            quitbutton_sprite.setOrigin({ quitbutton.getSize().x / 2.0f,quitbutton.getSize().y / 2.0f });
            quitbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,(GRID_SIZE_X / 2.0f + 4.0f) * CELL_SIZE });
            window.draw(quitbutton_sprite);



            // Store button positions for input handling
            resumeButtonPos = resumebutton_sprite.getGlobalBounds();
            resetButtonPos = resetbutton_sprite.getGlobalBounds();
            quitButtonPos = quitbutton_sprite.getGlobalBounds();
            settingButtonPos = settingbutton_sprite.getGlobalBounds();

        }
    }//if state

    else if (state == 's' || state == 'l' || state == 'w' || state == 'e' || state == 'f') {

        window.draw(doraS_sprite);
        if (state == 's') {
            start.setVolume(100.0f);
            if (!sound_playing) {

                start.setLooping(true);

                start.play();
                sound_playing = true;
            }

            window.draw(startbutton_sprite);
            startButtonPos = startbutton_sprite.getGlobalBounds();


            window.draw(aboutbutton_sprite);
            aboutButtonPos = aboutbutton_sprite.getGlobalBounds();

            resumebutton_sprite.setOrigin({ resumebutton.getSize().x / 2.0f,resumebutton.getSize().y / 2.0f });
            resumebutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,((GRID_SIZE_X / 2.0f) * CELL_SIZE) + (4 * CELL_SIZE) });
            window.draw(resumebutton_sprite);
            resumeButtonPos = resumebutton_sprite.getGlobalBounds();



            settingbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,((GRID_SIZE_X / 2.0f) * CELL_SIZE) + (6 * CELL_SIZE) });
            window.draw(settingbutton_sprite);
            settingButtonPos = settingbutton_sprite.getGlobalBounds();

            scorebutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,((GRID_SIZE_X / 2.0f) * CELL_SIZE) + (8 * CELL_SIZE) });
            window.draw(scorebutton_sprite);
            scoreButtonPos = scorebutton_sprite.getGlobalBounds();

            exitbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,((GRID_SIZE_X / 2.0f) * CELL_SIZE) + (10 * CELL_SIZE) });
            window.draw(exitbutton_sprite);
            exitButtonPos = exitbutton_sprite.getGlobalBounds();

        }

        else if (state == 'e') {

            start.setVolume(100.0f);
            if (!sound_playing) {

                start.setLooping(true);

                start.play();
                sound_playing = true;
            }


            window.draw(menubutton_sprite);
            menuButtonPos = menubutton_sprite.getGlobalBounds();

            exitbutton_sprite.setPosition({ (GRID_SIZE_Y / 2.0f) * CELL_SIZE,((GRID_SIZE_X / 2.0f) * CELL_SIZE) + (3 * CELL_SIZE) });
            window.draw(exitbutton_sprite);
            exitButtonPos = exitbutton_sprite.getGlobalBounds();
        }

        else if (state == 'l') {
            if (!mute) {
                if (!sound_playing1) {

                    win_sound.play();
                    sound_playing = true;
                }
            }
            text.setString("level completed! press enter to proceed");


            window.draw(text);


        }

        else if (state == 'f') {
            if (!mute) {
                if (!sound_playing2) {
                    dora_die_sound.play();
                    sound_playing2 = true;
                }
            }
            text.setString("Oops! you lose Presss enter to exit");


            window.draw(text);


        }

        else if (state == 'w') {
            if (!mute) {
                if (!sound_playing1) {
                    win_sound.play();
                    sound_playing = true;
                }
            }
            text.setString("YOU WON,DORA YOHOOOO! press enter to exit");


            window.draw(text);
        }
    }

    else if (state == 'a') {
        start.setVolume(10.0f);

        // Create about menu window (rectangle)
        float menuWidth = Window_width / 2.0f;
        float menuHeight = Window_height / 2.0f;
        float menuX = (Window_width - menuWidth) / 2.0f;
        float menuY = (Window_height - menuHeight) / 2.0f;

        RectangleShape pauseMenu(Vector2f(menuWidth, menuHeight));
        pauseMenu.setPosition({ menuX, menuY });
        pauseMenu.setFillColor(Color(50, 50, 100)); // Dark blue background
        pauseMenu.setOutlineThickness(3);
        pauseMenu.setOutlineColor(Color::White);
        window.draw(pauseMenu);

        sf::Text pausedText(font);
        pausedText.setString("ABOUT");
        pausedText.setCharacterSize(30);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 2.0f * CELL_SIZE, 1.0f * CELL_SIZE });
        window.draw(pausedText);


        pausedText.setString("made by ");
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ menuX + (2.0f * CELL_SIZE), menuY + (4.0f * CELL_SIZE) });
        window.draw(pausedText);

        pausedText.setString("Eman Fatima 3010 ");
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ menuX + (2.0f * CELL_SIZE), menuY + (7.0f * CELL_SIZE) });
        window.draw(pausedText);

        pausedText.setString("Jannat Fatima 3077");
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ menuX + (2.0f * CELL_SIZE), menuY + (10.0f * CELL_SIZE) });
        window.draw(pausedText);

        pausedText.setString("Haiba Naeem 3014 ");
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ menuX + (2.0f * CELL_SIZE), menuY + (13.0f * CELL_SIZE) });
        window.draw(pausedText);


        backbutton_sprite.setPosition({ menuX, menuY });
        window.draw(backbutton_sprite);
        backButtonPos = backbutton_sprite.getGlobalBounds();
    }

    else if (state == 'o') {
        start.setVolume(10.0f);


        // pause menu window
        float menuWidth = Window_width / 2.0f;
        float menuHeight = Window_height / 2.0f;
        float menuX = (Window_width - menuWidth) / 2.0f;
        float menuY = (Window_height - menuHeight) / 2.0f;

        RectangleShape pauseMenu(Vector2f(menuWidth, menuHeight));
        pauseMenu.setPosition({ menuX, menuY });
        pauseMenu.setFillColor(Color(50, 50, 100)); // Dark blue background
        pauseMenu.setOutlineThickness(3);
        pauseMenu.setOutlineColor(Color::White);
        window.draw(pauseMenu);

        sf::Text pausedText(font);
        pausedText.setString("SETTINGS");
        pausedText.setCharacterSize(30);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 2.0f * CELL_SIZE, 1.0f * CELL_SIZE });
        window.draw(pausedText);


        //
        backbutton_sprite.setPosition({ menuX, menuY });
        window.draw(backbutton_sprite);
        backButtonPos = backbutton_sprite.getGlobalBounds();

        mutebutton_sprite.setScale({ 1.0f,1.0f });
        mutebutton_sprite.setPosition({ menuX + 8 * CELL_SIZE,menuY + 13 * CELL_SIZE });
        window.draw(mutebutton_sprite);
        muteButtonPos = mutebutton_sprite.getGlobalBounds();

        color_yellowbutton_sprite.setScale({ 0.2f,0.2f });
        color_yellowbutton_sprite.setPosition({ menuX + 5 * CELL_SIZE, menuY + 10 * CELL_SIZE });
        window.draw(color_yellowbutton_sprite);
        color_yellowButtonPos = color_yellowbutton_sprite.getGlobalBounds();

        color_greenbutton_sprite.setScale({ 0.2f,0.2f });
        color_greenbutton_sprite.setPosition({ menuX + 10 * CELL_SIZE, menuY + 10 * CELL_SIZE });
        window.draw(color_greenbutton_sprite);
        color_greenButtonPos = color_greenbutton_sprite.getGlobalBounds();

        color_pinkbutton_sprite.setScale({ 0.2f,0.2f });
        color_pinkbutton_sprite.setPosition({ menuX + 5 * CELL_SIZE, menuY + 6 * CELL_SIZE });
        window.draw(color_pinkbutton_sprite);
        color_pinkButtonPos = color_pinkbutton_sprite.getGlobalBounds();

        color_purplebutton_sprite.setScale({ 0.2f,0.2f });
        color_purplebutton_sprite.setPosition({ menuX + 10 * CELL_SIZE, menuY + 6 * CELL_SIZE });
        window.draw(color_purplebutton_sprite);
        color_purpleButtonPos = color_purplebutton_sprite.getGlobalBounds();


    }

    else if (state == 'n') {
        start.setVolume(10.0f);
        backbutton_sprite.setPosition({ 20.0f * CELL_SIZE,1.0f * CELL_SIZE });
        window.draw(backbutton_sprite);
        backButtonPos = backbutton_sprite.getGlobalBounds();

        sf::Text pausedText(font);
        pausedText.setString("HIGHSCORES");
        pausedText.setCharacterSize(30);
        pausedText.setFillColor(sf::Color::Magenta);
        pausedText.setStyle(sf::Text::Bold);
        pausedText.setStyle(sf::Text::Underlined);
        pausedText.setPosition({ 2.0f * CELL_SIZE, 1.0f * CELL_SIZE });
        window.draw(pausedText);

        //from file
        ifstream file;
        file.open("files/highscores.txt");
        for (i = 0; i < 3; i++) {
            file >> highscores_arr[i];
        }
        file.close();
        //1
        pausedText.setString(std::to_string(highscores_arr[2]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 3.0f * CELL_SIZE });
        window.draw(pausedText);
        //2
        pausedText.setString(std::to_string(highscores_arr[1]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 6.0f * CELL_SIZE });
        window.draw(pausedText);
        //3
        pausedText.setString(std::to_string(highscores_arr[0]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 9.0f * CELL_SIZE });
        window.draw(pausedText);

        //scores

        //array not updating so phirse load kring

        file.open("files/scores.txt");
        for (i = 0; i < 5; i++) {
            file >> scores_arr[i];
        }
        file.close();

        pausedText.setString("SCORES");
        pausedText.setCharacterSize(30);
        pausedText.setFillColor(sf::Color::Magenta);
        pausedText.setStyle(sf::Text::Bold);
        pausedText.setStyle(sf::Text::Underlined);
        pausedText.setPosition({ 2.0f * CELL_SIZE, 12.0f * CELL_SIZE });

        window.draw(pausedText);

        //1
        pausedText.setString(std::to_string(scores_arr[4]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 16.0f * CELL_SIZE });
        window.draw(pausedText);
        //2
        pausedText.setString(std::to_string(scores_arr[3]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 19.0f * CELL_SIZE });
        window.draw(pausedText);
        //3
        pausedText.setString(std::to_string(scores_arr[2]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 22.0f * CELL_SIZE });
        window.draw(pausedText);
        //4
        pausedText.setString(std::to_string(scores_arr[1]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 25.0f * CELL_SIZE });
        window.draw(pausedText);
        //5
        pausedText.setString(std::to_string(scores_arr[0]));
        pausedText.setCharacterSize(20);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition({ 6.0f * CELL_SIZE, 28.0f * CELL_SIZE });
        window.draw(pausedText);
    }

}//draw function


void collision_handling() {
    dots_collision();
    ghost_collision();
}
bool isWall(int row, int col)//wall collision, also it will check the boundaries of grid
{
    if (row < 0 || row >= GRID_SIZE_X || col < 0 || col >= GRID_SIZE_Y)
    {
        return true;
    }
    return (arr[row][col] == '#');
}







void movement_Pac()
{
    //collision_handling();// to check dot and ghost collision
    if (pacNextDirec == 'U')
    {
        if (!isWall(pacman_x - 1, pacman_y))
        {
            pacman_x--;
            pacmanDirection = pacNextDirec;
        }
    }
    else if (pacNextDirec == 'D')
    {
        if (!isWall(pacman_x + 1, pacman_y))
        {
            pacman_x++;
            pacmanDirection = pacNextDirec;
        }
    }
    else if (pacNextDirec == 'R')
    {

        if (!isWall(pacman_x, pacman_y + 1))
        {
            pacman_y++;
            pacmanDirection = pacNextDirec;
        }
    }
    else if (pacNextDirec == 'L')
    {

        if (!isWall(pacman_x, pacman_y - 1))
        {

            pacman_y--;
            pacmanDirection = pacNextDirec;
        }
    }
    // moving through tunnels
    if (pacman_x == 16 && pacman_y == 0)
    {
        pacman_x = 16;
        pacman_y = 27;
        pacNextDirec = 'L';
    }
    else if (pacman_x == 16 && pacman_y == 27)
    {
        pacman_x = 16;
        pacman_y = 0;
        pacNextDirec = 'R';
    }


}



int main()
{
    initialize_setup();

    window.create(VideoMode({ Window_width,Window_height }), "PAC-DORA");

    //main game loop

    window.setFramerateLimit(10);








    //sounds
    button_click.loadFromFile("sounds/button_click.wav");
    win.loadFromFile("sounds/win.wav");
    start_song.loadFromFile("sounds/start.wav");
    pellet_eat.loadFromFile("sounds/pellet_eat.wav");
    oh_man.loadFromFile("sounds/oh_man.wav");
    dora_die.loadFromFile("sounds/dora_die.wav");
    dot_eat.loadFromFile("sounds/dot_eat.wav");

    //sounds 

    while (window.isOpen())
    {

        while (const optional<Event> event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) {
                previous_game_save();
                window.close();
            }
            // keyboard input handling and basic movement of pacman plus wall collision check
            else if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
            {
                // if escape pressed, window close 
                if (keyPressed->scancode == Keyboard::Scancode::Escape)
                {
                    pause = false;//agar player settings wali window me esc krdeta so for new game i dont want pause true
                    previous_game_save();
                    window.close();
                }

                else if (keyPressed->scancode == Keyboard::Scancode::Enter)
                {
                    if (state == 'l') {
                        state = 'g'; // Change to play state
                    }

                    else if (state == 'w') {
                        state = 'e';
                    }

                    else if (state == 'f') {
                        state = 'e';
                    }


                }

                if (state == 'g') {
                    // S key or down arrow key press for down
                    if (keyPressed->scancode == Keyboard::Scancode::S)
                    {
                        if (!isWall(pacman_x + 1, pacman_y))
                        {
                            pacNextDirec = 'D';
                        }
                    }

                    if (keyPressed->scancode == Keyboard::Scancode::Down)
                    {
                        if (!isWall(pacman_x + 1, pacman_y))
                        {
                            pacNextDirec = 'D';
                        }
                    }
                    // W key or up arrow key for up
                    if (keyPressed->scancode == Keyboard::Scancode::W)
                    {
                        if (!isWall(pacman_x - 1, pacman_y))
                        {
                            pacNextDirec = 'U';
                        }
                    }
                    if (keyPressed->scancode == Keyboard::Scancode::Up)
                    {
                        if (!isWall(pacman_x - 1, pacman_y))
                        {
                            pacNextDirec = 'U';
                        }
                    }
                    // D or right arrow key for right
                    if (keyPressed->scancode == Keyboard::Scancode::D)
                    {
                        if (!isWall(pacman_x, pacman_y + 1))
                        {
                            pacNextDirec = 'R';
                        }
                    }
                    if (keyPressed->scancode == Keyboard::Scancode::Right)
                    {
                        if (!isWall(pacman_x, pacman_y + 1))
                        {
                            pacNextDirec = 'R';
                        }
                    }
                    // A or left arrow key for left
                    if (keyPressed->scancode == Keyboard::Scancode::A)
                    {
                        if (!isWall(pacman_x, pacman_y - 1))
                        {
                            pacNextDirec = 'L';
                        }
                    }
                    if (keyPressed->scancode == Keyboard::Scancode::Left)
                    {
                        if (!isWall(pacman_x, pacman_y - 1))
                        {
                            pacNextDirec = 'L';
                        }
                    }
                    //

                }

            }




            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {

                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    if (!mute) {

                        button_click_sound.play();
                    }

                    //start state buttons
                    if (state == 's') {
                        // start button clicked
                        if (startButtonPos.contains(worldPos)) {

                            previous_game_initialize();
                            initialize_setup();
                            state = 'g'; // starts game

                        }

                        if (aboutButtonPos.contains(worldPos)) {
                            state = 'a'; // about window show
                        }

                        if (scoreButtonPos.contains(worldPos)) {
                            state = 'n'; // scores window show
                        }

                        if (settingButtonPos.contains(worldPos)) {
                            state = 'o'; // option window show
                        }
                        if (resumeButtonPos.contains(worldPos)) {

                            previous_game_load();
                            state = 'g'; // Return to game

                        }

                        // exit button clicked
                        if (exitButtonPos.contains(worldPos)) {
                            window.close();//state being stored in quit so no need to store here
                        }


                    }

                    //game state buttons
                    if (state == 'g') {
                        // pause button clicked
                        if (pauseButtonPos.contains(worldPos)) {
                            state = 'p'; // Return to game

                        }
                    }


                    // PAUSE STATE BUTTON HANDLING
                    if (state == 'p') {

                        if (settingButtonPos.contains(worldPos)) {
                            pause = true;//now pause true,backbutton pause pe ayay ga
                            state = 'o'; // option window show
                        }

                        // Resume button clicked
                        if (resumeButtonPos.contains(worldPos)) {
                            state = 'g'; // Return to game
                        }
                        // Reset button clicked  
                        else if (resetButtonPos.contains(worldPos)) {
                            initialize_setup();
                            state = 'g'; // Return to game
                        }
                        // Quit button clicked
                        else if (quitButtonPos.contains(worldPos)) {
                            previous_game_save();
                            state = 'e';
                        }
                    }

                    //game state buttons
                    if (state == 'e') {
                        // menu button clicked
                        if (menuButtonPos.contains(worldPos)) {
                            state = 's'; // Return to start window
                        }

                        // exit button clicked
                        if (exitButtonPos.contains(worldPos)) {
                            window.close();
                        }
                    }

                    if (state == 'o') {//options
                        if (backButtonPos.contains(worldPos)) {
                            if (pause) {
                                state = 'p';//return back to pause
                                pause = false;
                            }
                            else
                                state = 's'; // Return to start window
                        }
                    }



                    if (state == 'n' || state == 'a') {
                        if (backButtonPos.contains(worldPos)) {
                            state = 's';
                        }
                    }

                    if (state == 'o') {//settings tab
                        if (muteButtonPos.contains(worldPos)) {
                            if (mute == false) {
                                mute = true;
                                start.stop();
                                sound_playing = true;
                            }
                            else
                            {
                                mute = false;
                                start.setLooping(true);

                                start.play();
                                sound_playing = true;
                            }
                        }

                        if (color_pinkButtonPos.contains(worldPos)) {
                            color = 'm';
                        }

                        if (color_yellowButtonPos.contains(worldPos)) {
                            color = 'y';
                        }

                        if (color_greenButtonPos.contains(worldPos)) {
                            color = 'g';
                        }

                        if (color_purpleButtonPos.contains(worldPos)) {
                            color = 'p';
                        }
                    }
                }
            }
        }

        if (state == 'g') {

            if (scared_timer > 0) {
                scared_timer--;
                if (scared_timer == 0) {
                    for (i = 0; i < 4; i++) {
                        ghost_state[i] = false;
                    }
                }
            }
            //storing them for my swap collision handling
            for (i = 0; i < 4; i++) {
                ghost_old_x[i] = ghost_x[i];
                ghost_old_y[i] = ghost_y[i];

            }
            pacman_old_x = pacman_x;
            pacman_old_y = pacman_y;

            //storing done

            movement_Pac();
            ghostBFSMovement(0);
            ghostBFSMovement(1);
            ghostBFSMovement(2);
            ghostBFSMovement(3);
            collision_handling();


        }
        window.clear();

        draw_sfml_game();

        window.display();

    }


    return 0;
}





