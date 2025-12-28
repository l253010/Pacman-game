#pragma once

// Global Variables


// BFS Queue simulation: Max size is 28*31(uper ki 3 rows blank)
int bfs_queue_r[836];
int bfs_queue_c[836];
int queue_head = 0;
int queue_tail = 0;

// Direction arrays (U, D, L, R)
const int DR[] = { -1, 1, 0, 0 }; // Change in Row (r) for U, D, L, R
const int DC[]={ 0, 0, -1, 1 }; // Change in Column (c) for U, D, L, R
const char DIR_CHARS[] = { 'U', 'D', 'L', 'R' };
//const int SCATTER_R[4] = { 1, 1, 18, 18 };
//const int SCATTER_C[4] = { 18, 1, 18, 1 };
const int SCATTER_R[4] = { 4, 4, 32, 32 };
const int SCATTER_C[4] = { 26, 1, 26, 1 };

const int reset_ghost_x[4] = { 18,18,18,18 };
const int reset_ghost_y[4] = { 12,13,14,15 };
const bool reset_ghost_state[4] = { false,false,false,false };
const char reset_ghostDirection[4] = { 'U','D','L','R' };
int ghost_x[4];
int ghost_y[4];
bool ghost_state[4];
int scared_timer=0 ;
char ghostDirection[4] = { 'U','D','L','R' };//
char ghost_next_direction[4];//
int pacman_x = 4;
int pacman_y = 13;
char pacmanDirection = 'U';
char pacNextDirec = 'U';//
int i, j;
const int GRID_SIZE_X = 34;
const int GRID_SIZE_Y = 28;
char arr[GRID_SIZE_X][GRID_SIZE_Y];
const float CELL_SIZE = 20.0f;//
float wallThickness = 2.0f;//
const int Window_width = GRID_SIZE_Y * CELL_SIZE;//
const int Window_height = GRID_SIZE_X * CELL_SIZE;//

int ghost_old_x[4] = { 18,18,18,18 };
int ghost_old_y[4] = { 12,13,14,15 };
int pacman_old_x;
int pacman_old_y;


//level handling
int level=1 ;
int last_level=7 ;//agar last level me doteaten and totaldots equal howay to win,no level++
int total_dots;
const int total_dots1 = 326;
const int total_dots2 = 372;
const int total_dots3 = 354;
const int total_dots4 = 345;
const int total_dots5 = 390;
const int total_dots6 = 408;
const int total_dots7 = 386;



//reset stats
int dots_eaten = 0;
int lives = 3;
int scores = 0;




//states management 
char state = 's';
char color = 'm';//mazecolor pink
bool pause = false;

//sounds
bool mute;
bool sound_playing = false;//start ,win
bool sound_playing1 = false;//win,level window
bool sound_playing2 = false;//lose window

//file i/o
int scores_arr[6] = { 0,0,0,0,0,0 };
int highscores_arr[4] = { 0,0,0,0 };

// functions
void collision_handling();
void ghost_collision();
void dots_collision();
void power_pallet();

void level_handling();
void reset_pacman_position();
void reset_ghost(int i);
bool isWall(int, int);
void reset_stats();
void initialize_maze();
void previous_game_save();
void previous_game_load();
void previous_game_initialize(); 
void initialize_setup();
void dots_collision();
int ghost_collided();
void ghost_collision();
void scores_save();
void highscores_save();
void scores_store();
int scared(int level);
bool isghost(int ghostindex, int next_r, int next_c);
int manhattanDistance(int r1, int c1, int r2, int c2);
void getChaseTarget(int ghostIndex, int& target_r, int& target_c);
char findNextStepBFS(int start_r, int start_c, int target_r, int target_c);
void ghostBFSMovement(int ghostIndex);
void draw_sfml_game();
void movement_Pac();