#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    for (auto& row : board) {
        for (auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    write_board_csv(board, first);
}

void spawn_tile(vector<vector<int>>& board) {
    // Collect all empty positions
    vector<pair<int, int>> empty_positions;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (board[r][c] == 0) {
                empty_positions.push_back({r, c});
            }
        }
    }
    
    // If no empty positions, return
    if (empty_positions.empty()) return;
    
    // Random number generation
    static std::mt19937 rng(std::time(nullptr));
    std::uniform_int_distribution<int> pos_dist(0, empty_positions.size() - 1);
    std::uniform_int_distribution<int> value_dist(1, 10);
    
    // Select random empty position
    int random_index = pos_dist(rng);
    int row = empty_positions[random_index].first;
    int col = empty_positions[random_index].second;
    
    // Assign value: 90% chance for 2, 10% chance for 4
    int value = (value_dist(rng) <= 9) ? 2 : 4;
    board[row][col] = value;
}


std::vector<int> compress_row(const std::vector<int>& row) {
    vector<int> compressed;
    copy_if(row.begin(), row.end(), back_inserter(compressed), [](int x) { return x != 0; });
    compressed.resize(4, 0);
    return compressed;
}

std::vector<int> merge_row(std::vector<int> row) {
    for (auto it = row.begin(); it != row.end() - 1; ++it) {
        if (*it == *(it + 1) && *it != 0) {
            *it *= 2;
            *(it + 1) = 0;
            ++it;
        }
    }
    return compress_row(row);
}

bool move_left(vector<vector<int>>& board){
    bool moved = false;
    for (auto& row : board) {
        vector<int> original = row;  
        row = compress_row(row);
        row = merge_row(row);
        moved = moved || (row != original); 
    }
    return moved;
}

bool move_right(vector<vector<int>>& board){
    bool moved = false;
    for (auto& row : board) {
        vector<int> original = row;
        reverse(row.begin(), row.end());
        row = compress_row(row); 
        row = merge_row(row);      
        reverse(row.begin(), row.end());   
        moved = moved || (row != original);
    }
    return moved;
}
    
// TODO: use column traversal
bool move_up(vector<vector<int>>& board){
    bool moved = false;
    for (int c = 0; c < 4; c++) {
        vector<int> column(4, 0);
        for (int r = 0; r < 4; r++) {
            column[r] = board[r][c];
        }
        vector<int> original = column;
        column = compress_row(column);
        column = merge_row(column);
        moved = moved || (column != original);

        for (int r = 0; r < 4; r++) {
            board[r][c] = column[r];
        }
    }
    return moved;
}
// TODO: use column traversal with reverse
bool move_down(vector<vector<int>>& board){return false;}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board, first);
        first=false;
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: get the history and print the board and continue
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Store the previous state here!
            spawn_tile(board);
        }
    }
    return 0;
}
